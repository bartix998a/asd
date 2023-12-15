#include <iostream>
#include <stdexcept>
#include <queue>
#include <iostream>
#include <ranges>
#include <list>
#include <algorithm>
#include <random>
#include <stack>
#include <cassert>

class Node{
public:
    /*
     * leftSon->val < val < righjtSon-val
     */
    Node* leftSon;
    Node* rightSon;
    int64_t start, len;
    int64_t offset;
    int x;
    int height;
    Node(size_t start, size_t len, int x) : leftSon(nullptr), rightSon(nullptr), start(start), len(len), offset(0), x(x), height(1){};

    void calculateHeight(){
        height = 1 + std::max(rightSon == nullptr ? 0 : rightSon->height, leftSon == nullptr ? 0 : leftSon->height);
    }

    int bFactor(){
        return (leftSon == nullptr ? 0 : leftSon->height) - (rightSon == nullptr ? 0 : rightSon->height);
    }

    void pushOffsetToChildren(size_t extraOffset = 0){
        if (rightSon != nullptr){
            rightSon->offset += offset + extraOffset;
        }
        if (leftSon != nullptr){
            leftSon->offset += offset + extraOffset;
        }
        start += offset + extraOffset;
        offset = 0;
    }

    Node* leftRotation(){ // ok
        Node* temp1 = rightSon->leftSon;
        Node* newParent = rightSon;

        pushOffsetToChildren();
        newParent->pushOffsetToChildren(offset);

        rightSon->leftSon = this;
        rightSon = temp1;
        this->calculateHeight();
        newParent->calculateHeight();
        return newParent;
    }

    Node* rightRotation(){// ok
        Node* temp1 = leftSon->rightSon;
        Node* newParent = leftSon;

        pushOffsetToChildren();
        newParent->pushOffsetToChildren(offset);

        leftSon->rightSon = this;
        leftSon = temp1;
        this->calculateHeight();
        newParent->calculateHeight();
        return newParent;
    }

    Node* doubleRightRotation(){
        leftSon = leftSon->leftRotation();
        return this->rightRotation();
    }

    Node* doubleLeftRotation(){ //might be a problem with nulls
        rightSon = rightSon->rightRotation();
        return this->leftRotation();
    }

    Node* balanceTree(){
        if (bFactor() == -2 && rightSon->bFactor() <= 0){
            return leftRotation();
        } else if (bFactor() == -2 && rightSon->bFactor() > 0){
            return  doubleLeftRotation();
        } else if (bFactor() == 2 && leftSon->bFactor() >= 0){
            return rightRotation();
        } else if (bFactor() == 2 && leftSon->bFactor() < 0){
            return doubleRightRotation();
        } else return this;
    }

    Node* insert_a_thing_prep(size_t start, size_t len, int x){// this is the hard insert
        std::stack<Node*> path;
        Node* current = this;
        size_t totalOffset = 0;
        size_t pos = start;
        size_t temp;
        while (!(current->start + current->offset + totalOffset <= pos && pos < current->start + current->offset + totalOffset + current->len)){
            totalOffset += current->offset;
            path.push(current);
            if (pos < current->start + totalOffset){
                if (current->rightSon != nullptr){
                    current->rightSon->offset += len;
                }
                current->start += len;
                current = current->leftSon;
            } else if (pos >= current->start + totalOffset + current->len) {
                current = current->rightSon;
            }
        }
        temp = current->len;
        if (current->rightSon != nullptr){
            current->rightSon->offset += len;
        }
        if (pos - (current->start + current->offset + totalOffset) != 0){
            current->len = pos - (current->start + current->offset + totalOffset);
        } else {
            current->start += len;
        }
        if (temp - current->len > 0){
            return insert(current->start + current->offset + totalOffset + current->len + len, temp - current->len, current->x);
        } else {
            return this;
        }

    }

    Node* insert(size_t start, size_t len, int x){// this is the hard insert
        return insert(new Node(start, len, x));
    }

    Node* insert(Node* node, size_t prev_offset = 0){
        // potencjalnie dodac cos z wyjatkami
        if (node->start < start + offset + prev_offset){
            if (leftSon == nullptr){
                leftSon = node;
                node->start -= offset + prev_offset;
                calculateHeight();
                return this;
            } else {
                leftSon = leftSon->insert(node, offset + prev_offset);
            }
            calculateHeight();
            return balanceTree();
        } else {
            if(rightSon == nullptr){
                rightSon = node;
                node->start -= offset + prev_offset;
                calculateHeight();
                return this;
            } else {
                rightSon = rightSon->insert(node, offset + prev_offset);
            }
            calculateHeight();
            return balanceTree();
        }
        calculateHeight();
        return this;
    };

    // Assuming the tree contains pos
    Node* remove(size_t pos){
        std::stack<Node*> path;
        Node* current = this;
        size_t totalOffset = 0;
        while (!(current->start + current->offset + totalOffset <= pos && pos < current->start + current->offset + totalOffset + len)){
            totalOffset += current->offset;
            path.push(current);
            if (pos < current->start + current->offset + totalOffset){
                current = current->leftSon;
            } else if (pos > current->start + current->offset + totalOffset + len) {
                current = current->rightSon;
            }
        }

        Node* temp = current;
        size_t tempOffset = totalOffset;

        if (current->rightSon != nullptr){
            totalOffset += current->offset;
            path.push(current);
            current = current->rightSon;
            while (current->leftSon != nullptr) {
                path.push(current);
                current = current->leftSon;
            }
            if (path.top()->rightSon != current){
                path.top()->leftSon = current->rightSon;
            } else {
                path.top()->rightSon = current->rightSon;
            }

            temp->start = current->start + totalOffset - tempOffset;
            temp->len = current->len;
            temp->x = current->x;
            delete current;
        } else if (current->leftSon != nullptr){
            totalOffset += current->offset;
            path.push(current);
            current = current->leftSon;
            while (current->rightSon != nullptr) {
                path.push(current);
                current = current->rightSon;
            }
            if (path.top()->leftSon != current){
                path.top()->rightSon = current->leftSon;
            } else {
                path.top()->leftSon = current->leftSon;
            }
            temp->start = current->start + totalOffset - tempOffset;
            temp->len = current->len;
            temp->x = current->x;
            delete current;
        }



        while (path.size() != 1){
            current = path.top();
            current->calculateHeight();
            path.pop();
            if (path.top()->leftSon == current){
                path.top()->leftSon = current->balanceTree();
            } else if (path.top()->rightSon == current){
                path.top()->rightSon = current->balanceTree();
            }
        }
        return balanceTree();
    }

    Node* get_node(size_t pos, size_t prev_offset = 0){// ok
        if (pos < prev_offset + offset + start){
            if (leftSon != nullptr){
                return leftSon->get_node(pos, prev_offset + offset);
            }
            throw std::invalid_argument("Nie ma w drzewie");
        } else if (pos > prev_offset + offset + start + len - 1) {
            if (rightSon != nullptr){
                return rightSon->get_node(pos, prev_offset + offset);
            }
            throw std::invalid_argument("Nie ma w drzewie");
        } else {
            return this;
        }
    }

    int get(size_t pos){// ok
         // pos should always be in the tree
        return get_node(pos)->x;
    }

    void prt(bool first = true){
        if (leftSon != nullptr){
            leftSon->prt(false);
        }
        for (int i = 0; i < len; i++) {
            std::cout << x << " " << len << "; ";
        }
        if(rightSon != nullptr){
            rightSon->prt(false);
        }
        if (first){
            std::cout << std::endl;
        }
    }

};

int main(void){
    int m;
    size_t last_pos = 0;
    size_t size = 0;
    std::cin >> m;
    Node* n = nullptr;
    for (int i = 0; i < m; i++) {
        char a;
        std::cin >> a;
        switch (a) {
            case 'i':
                size_t j, k;
                int x;
                std::cin >> j;
                std::cin >> x;
                std::cin >> k;
                if (n == nullptr) {
                    n = new Node(0, k, x);
                } else if ((j + last_pos)%(size + 1) == size) {
                    n = n->insert(size, k, x);
                } else {
                    n = n->insert_a_thing_prep((j + last_pos)%(size + 1), k, x);
                    n = n->insert((j + last_pos)%(size + 1), k, x);
                }
                size += k;
                break;
            case 'g':
                size_t pos;
                std::cin >> pos;
                last_pos = n->get((pos + last_pos)%(size));
                std::cout << last_pos << std::endl;
                break;
        }
    }
}