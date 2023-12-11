//
// Created by Bartek on 10.12.2023.
//
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
    size_t val;
    int height;
    Node(size_t val) : leftSon(nullptr), rightSon(nullptr), val(val), height(1){};

    void calculateHeight(){
        height = 1 + std::max(rightSon == nullptr ? 0 : rightSon->height, leftSon == nullptr ? 0 : leftSon->height);
    }

    int bFactor(){
        return (leftSon == nullptr ? 0 : leftSon->height) - (rightSon == nullptr ? 0 : rightSon->height);
    }

    Node* leftRotation(){
        Node* temp1 = rightSon->leftSon;
        Node* newParent = rightSon;
        rightSon->leftSon = this;
        rightSon = temp1;
        this->calculateHeight();
        newParent->calculateHeight();
        return newParent;
    }

    Node* rightRotation(){
        Node* temp1 = leftSon->rightSon;
        Node* newParent = leftSon;
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

    size_t max(){
        if (this->rightSon != nullptr){
            return rightSon->max();
        }
        return val;
    }

    size_t min(){
        if (this->leftSon != nullptr){
            return leftSon->min();
        }
        return val;
    }

    Node* insert(size_t pos){
        return insert(new Node(pos));
    }

    Node* insert(Node* node){
        if (node->val == this->val) {
            throw std::invalid_argument("Wartosc istnieje");
        }
        if (node->val < this->val){
            if (leftSon == nullptr){
                leftSon = node;
                calculateHeight();
                return this;
            } else {
                leftSon = leftSon->insert(node);
            }
            calculateHeight();
            return balanceTree();
        } else {
            if(rightSon == nullptr){
                rightSon = node;
                calculateHeight();
                return this;
            } else {
                rightSon = rightSon->insert(node);
            }
            calculateHeight();
            return balanceTree();
        }
        calculateHeight();
        return this;
    };

    template<typename K>
    Node* insert_range(K v){
        Node* temp = this;
        for (int i : v){
            temp = temp->insert(i);
        }
        return temp;
    }

    // Assuming the tree contains pos
    Node* remove(size_t pos){
        std::stack<Node*> path;
        Node* current = this;
        while (current->val != pos){
            path.push(current);
            if (pos < current->val){
                current = current->leftSon;
            } else if (pos > current->val) {
                current = current->rightSon;
            }
        }

        Node* temp = current;


            if (current->rightSon != nullptr){
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

                temp->val = current->val;
                delete current;
            } else if (current->leftSon != nullptr){
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
                temp->val = current->val;
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

    Node* get_node(size_t pos){
        if (pos < val){
            if (leftSon != nullptr){
                return leftSon->get_node(pos);
            }
            throw std::invalid_argument("Nie ma w drzewie");
        } else if (pos > val) {
            if (rightSon != nullptr){
                return rightSon->get_node(pos);
            }
            throw std::invalid_argument("Nie ma w drzewie");
        } else {
            return this;
        }
    }

    unsigned int get(size_t pos){
        // pos should always be in the tree
        return get_node(pos)->val;
    }

    void print_infix(){
        if (leftSon != nullptr){
            leftSon->print_infix();
        }
        std::cout << val << std::endl;
        if (rightSon != nullptr){
            rightSon->print_infix();
        }
    }

    void update_heights(){
        if (leftSon != nullptr){
            leftSon->calculateHeight();
        }
        if (rightSon != nullptr){
            rightSon->calculateHeight();
        }
        calculateHeight();
    }

    void check_avl(){
        if (leftSon != nullptr){
            leftSon->check_avl();
        }
        if (rightSon != nullptr){
            rightSon->check_avl();
        }
        assert(bFactor() >= -1 && bFactor() <= 1);
    }
};

int main(void){
    size_t test = 200;
    //std::cin >> test;
    std::vector<size_t> a;
    for(int i = 1; i < test; i++){
        a.push_back(i);
    }
    std::shuffle(a.begin(), a.end(), std::default_random_engine(4577645674));
    Node* n = new Node(0);
    for (auto val : a){
        n = n->insert(val);
    }
    std::shuffle(a.begin(), a.end(), std::default_random_engine(46776765));
    for (auto val : a){
        n = n->remove(val);
        n->update_heights();
        n->check_avl();
        assert(n->height <= 20);
    }
    return 0;
}