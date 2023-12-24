//
// Created by Bartek on 20.12.2023.
//

#include <iostream>
#include <tuple>
#include <set>
#include <array>
#include <algorithm>
#include <list>

int cmp_flag = 0;

class Node{
public:
    std::pair<int,int> coordinates;
    std::array<Node*, 4> neighbours;
    size_t shortest_path;
    Node() : coordinates({0,0}), neighbours({nullptr, nullptr, nullptr, nullptr}), shortest_path(SIZE_MAX) {}
    Node(int x, int y) : coordinates({x,y}), neighbours({nullptr, nullptr, nullptr, nullptr}), shortest_path(SIZE_MAX) {}
    auto operator==(Node& other){
        return this->coordinates == other.coordinates;
    }
    auto operator!=(Node& other){
        return this->coordinates != other.coordinates;
    }

};

int weight(Node* n1, Node* n2){
    return std::min(std::abs(n1->coordinates.first - n2->coordinates.first), std::abs(n1->coordinates.second - n2->coordinates.second));
}

bool cmpx(Node* n1, Node* n2){
    return n1->coordinates.first < n2->coordinates.first;
}

bool cmpy(Node* n1, Node* n2){
    return n1->coordinates.second < n2->coordinates.second;
}


int main(void) {
    int n;
    std::cin >> n;
    Node nodes[n];
    Node* node_ptrs[n];
    for (int i = 0; i < n; i++) {
        int first, second;
        std::cin >> first;
        std::cin >> second;
        nodes[i] = Node(first, second); // undef behaviour? chyba nie ale mozna sprawdzic albo napisac assignment operator
        node_ptrs[i] = &(nodes[i]);
    }

    nodes[0].shortest_path = 0;

    std::sort(node_ptrs, node_ptrs + n, cmpx);

    for (int i = 0; i < n; ++i) {
        if (i != 0){
            node_ptrs[i]->neighbours[0] = node_ptrs[i-1];
        } else {
            node_ptrs[i]->neighbours[0] = nullptr;
        }
        if (i != n - 1){
            node_ptrs[i]->neighbours[1] = (node_ptrs[i + 1]);
        } else {
            node_ptrs[i]->neighbours[1] = nullptr;
        }
    }

    std::sort(node_ptrs, node_ptrs + n, cmpy);

    for (int i = 0; i < n; ++i) {
        if (i != 0){
            node_ptrs[i]->neighbours[2] = node_ptrs[i-1];
        } else {
            node_ptrs[i]->neighbours[2] = nullptr;
        }
        if (i != n - 1){
            node_ptrs[i]->neighbours[3] = (node_ptrs[i + 1]);
        } else {
            node_ptrs[i]->neighbours[3] = nullptr;
        }
    }
    auto spcmp =[](Node* n1, Node* n2){
        return std::tie(n1->shortest_path, n1->coordinates) < std::tie(n2->shortest_path, n2->coordinates);
    };

    std::set<Node*, decltype(spcmp)> stack(spcmp);

    for (int i = 0; i < n; ++i) {
        stack.insert(node_ptrs[i]);
    }



    while (true) {
        Node* current = *(stack.begin());
        stack.erase(stack.begin());
        for (int i = 0; i < 4; i++){
            if (current->neighbours[i] != nullptr){
                if (weight(current, current->neighbours[i])+ current->shortest_path < current->neighbours[i]->shortest_path){
                    stack.erase(current->neighbours[i]);
                    current->neighbours[i]->shortest_path = std::min(
                            weight(current, current->neighbours[i])+ current->shortest_path,
                            current->neighbours[i]->shortest_path
                    );
                    stack.insert(current->neighbours[i]);
                }
            }
        }
        if (current == &nodes[n - 1]){
            std::cout << current->shortest_path << std::endl;
            return 0;
        }
    }
}