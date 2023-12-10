//
// Created by Bartek on 10.12.2023.
//
#include <stdexcept>

class Node{
public:
    /*
     * leftSon->val < val < righjtSon-val
     */
    Node* leftSon;
    Node* rightSon;
    size_t val;
    int height;
    Node(size_t minIndex, unsigned int x, unsigned int count) : leftSon(nullptr), rightSon(nullptr), height(0){};

    void calculateHeight(){
        height = 1 + std::max(rightSon == nullptr ? 0 : rightSon->height, leftSon == nullptr ? 0 : leftSon->height);
    }

    int bFactor(){
        return leftSon->height - rightSon->height;
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

    Node* insert(Node* node){
        if (node->val == this->val) {
            throw std::invalid_argument("Wartosc istnieje");
        }
        if (node->val < this->val){
            if (leftSon == nullptr){
                leftSon = node;
            } else {
                leftSon = leftSon->insert(node);
            }
            if (bFactor() == 2 && leftSon->bFactor() > 0){
                return leftRotation();
            } else if (bFactor() == 2 && leftSon->bFactor() < 0){
                return doubleRightRotation();
            }
        } else {
            if(rightSon == nullptr){
                rightSon = node;
            } else {
                rightSon = rightSon->insert(node);
            }
            if (bFactor() == -2 && rightSon->bFactor() > 0){
                return rightRotation();
            } else if (bFactor() == -2 && rightSon->bFactor() < 0){
                return  doubleLeftRotation();
            }
        }
        return this;
    };

    // Assuming the tree contains pos
    Node* remove(size_t pos){
        if (pos < val){
            leftSon = leftSon->remove(pos);
            if (bFactor() == -2 && rightSon->bFactor() <= 0){
                return leftRotation();
            } else if (bFactor() == -2 && rightSon->bFactor() == 1){
                return doubleLeftRotation();
            }
        } if (pos > val) {
            rightSon = rightSon->remove(pos);
            if (bFactor() == 2 && leftSon->bFactor() >= 0){
                return rightRotation();
            } else if (bFactor() == 2 && rightSon->bFactor() == -1){
                return doubleRightRotation();
            }
        } else {
            if (rightSon == nullptr) {
                return leftSon;
            } else if (leftSon == nullptr) {
                return rightSon;
            }
            Node* temp;
            if (bFactor() == 1) {
                Node* temp = leftRotation();
                temp->leftSon = remove(pos);
            } else {
                Node* temp = rightRotation();
                temp->leftSon = remove(pos);
            }

            delete this;
            return temp;
        }
        return this;
    }

    unsigned int get(size_t pos){
        // pos should always be in the tree
        if (pos < val){
            if (leftSon != nullptr){
                return leftSon->get(pos);
            }
            throw std::invalid_argument("Nie ma w drzewie");
        } else if (pos > val) {
            if (rightSon != nullptr){
                return rightSon->get(pos);
            }
            throw std::invalid_argument("Nie ma w drzewie");
        } else {
            return val;
        }
    }
};


int main(void){
    return 0;
}