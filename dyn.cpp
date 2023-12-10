#include <iostream>
#include <stdexcept>

class Node{
public:
    Node* leftSon;
    Node* rightSon;
    size_t minIndex;
    size_t indexOverride;
    unsigned int x;
    unsigned int count;
    size_t height;
    Node(size_t minIndex, unsigned int x, unsigned int count) : leftSon(nullptr), rightSon(nullptr), minIndex(minIndex),
    x(x), count(count), height(0), indexOverride(0) {};

    void calculateHeight(){
        height = 1 + std::max(rightSon->height, leftSon->height);
    }

    size_t bFactor(){
        return rightSon->height - leftSon->height;
    }

    void fixIndexingInRotation(Node* newParent){
        if (newParent->leftSon != nullptr){
            newParent->leftSon->indexOverride += newParent->indexOverride;
        }

        if (newParent->rightSon != nullptr){
            newParent->rightSon->indexOverride += newParent->indexOverride;
        }

        newParent->minIndex += rightSon->indexOverride;
        newParent->indexOverride = 0;
    }

    Node* leftRotation(){
        Node* temp1 = rightSon->leftSon;
        Node* newParent = rightSon;
        fixIndexingInRotation(newParent);
        rightSon->leftSon = this;
        rightSon = temp1;
        this->calculateHeight();
        newParent->calculateHeight();
        return newParent;
    }

    Node* rightRotation(){
        Node* temp1 = leftSon->rightSon;
        Node* newParent = leftSon;
        fixIndexingInRotation(newParent);
        leftSon->rightSon = this;
        leftSon = temp1;
        this->calculateHeight();
        newParent->calculateHeight();
        return newParent;
    }

    size_t maxPos(size_t override = 0){
        if (this->leftSon != nullptr){
            return leftSon->maxPos(override + indexOverride);
        }
        return minIndex + count + indexOverride;
    }

    Node* remove(size_t pos){
        /* algorithm for removing*/
    }

    void insert(Node* node){
        /*
         * 1. Find the interval containing the index
         * 2. Extend the interval to the point that it can contain its og self and inserted interval
         * 3. Iterate back to the root adding the difference of the interval size to nodes to the right of the node
         * 4. Remove the node
         * 5. Split the node and insert all three parts with the standard algorithm
         */
    };



    unsigned int get(size_t pos, size_t override = 0){
        // pos should always be in the tree
        if (pos < minIndex + override){
            return leftSon->get(pos, override + this->indexOverride);
        } else if (pos >= minIndex + override + count) {
            return rightSon->get(pos, override + this->indexOverride);
        } else {
            return x;
        }
    }
};

int main(void){
    return 0;
}