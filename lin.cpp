//
// Created by Bartek on 23.12.2023.
//

#include <iostream>
#include <set>
#include <tuple>
#include <functional>
#include <vector>
#include <algorithm>

int treeSize;
int n,k;

using namespace std;

using tup = std::tuple<int, int, int>;


bool cmp(tup *t1, tup* t2) {
    return *t1 < *t2;
}

struct Node {
    int min, max;
    vector<tup*> local;
    vector<tup*> below;
    vector<tup*> above_max_path;
    vector<tup*> above_min_path;
    Node() : min(INT_MIN), max(INT_MAX) {};
};

int parent(int x){
    return x/2;
}

int leftSon(int x){
    return 2*x;
}

int rightSon(int x){
    return 2*x + 1;
}

int neighbour(int x){
    return 2*(x/2) + (1 - (x%2));
}

bool isLeftSon(int x){
    return x == leftSon(parent(x));
}

bool isRightSon(int x){
    return x == rightSon(parent(x));
}

int smallestPow(size_t n)
{
    for (int i = 0; true; i++)
    {
        if ((1 << i) >= n)
        {
            return i;
        }
    }
}

void nextIndex(int& current, Node* tree, tup* element, bool left){
    if (left){
        if (get<1>(*element) < tree[leftSon(current)].max){
            current = leftSon(current);
        } else {
            current = rightSon(current);
        }
    } else {
        if (get<2>(*element) <= tree[leftSon(current)].max){
            current = leftSon(current);
        } else {
            current = rightSon(current);
        }
    }

}

void insertToTree(Node* tree, tup* element){
    int current_L = 1;// korzen w jedynce?
    int current_R = 1;
    nextIndex(current_L, tree, element, true);
    nextIndex(current_R, tree, element, false);
    while (current_L == current_R && current_L < treeSize/2){
        tree[parent(current_L)].below.push_back(element);// z posortowanej push back dziala
        nextIndex(current_R, tree, element, false);
        nextIndex(current_L, tree, element, true);
    }
    if (current_L == current_R){
        nextIndex(current_R, tree, element, false);
        nextIndex(current_L, tree, element, true);
    }
    tree[parent(current_L)].local.push_back(element);
    if (current_L == current_R){
        return;
    }
    while (current_R < treeSize){
        tree[current_L].above_min_path.push_back(element);
        tree[current_R].above_max_path.push_back(element);
        nextIndex(current_R, tree, element, false);
        nextIndex(current_L, tree, element, true);
    }
}

int countElements(vector<tup*> v, int t){
    tup upper_b = tuple(t + k, INT_MAX, INT_MAX);
    tup lower_b = tuple(t - k, INT_MIN, INT_MIN);
    return distance(
            lower_bound(v.begin(), v.end(), &lower_b,cmp),
            upper_bound(v.begin(), v.end(), &upper_b,cmp)
    );
}

int countNode(int& index, int& undecided, Node* tree, bool left, int t){
    int count = 0;
    vector<tup*> undecided_counter = left ? tree[index].above_max_path : tree[index].above_min_path;
    if ((isLeftSon(index) && left) || (isRightSon(index) && !left)){
        count += countElements(tree[parent(index)].local, t);
        count += countElements(tree[neighbour(index)].below, t);
        int temp = countElements(undecided_counter, t);
        count += undecided - temp;
        undecided = temp;
    } else if ((isRightSon(index) && left) || (isLeftSon(index) && !left)) {
        undecided = countElements(undecided_counter, t);
    }
}

size_t get_count(Node* tree, int pos){
    size_t result;
    for(auto it = tree[pos].local.begin(); it !=  tree[pos].local.end(); it++){ // i hope it iterates normally from the begining
        int index_left = pos;
        int index_right = pos;
        int left_undecided = 0;
        int right_undecided = 0;
        int t = get<0>(**it);
        tup upper_b = tuple(get<0>(**it) + k, INT_MAX, INT_MAX);
        tup lower_b = tuple(get<0>(**it) - k, INT_MIN, INT_MIN);
        size_t count = std::distance(tree[pos].local.begin() ,upper_bound(it, tree[pos].local.end(), &upper_b, cmp));
        nextIndex(index_left, tree, *it, true);
        nextIndex(index_right, tree, *it, false);
        while(rightSon(index_left) < treeSize){
            nextIndex(index_left, tree, *it, true);
            nextIndex(index_right, tree, *it, false);
            count += countNode(index_left, left_undecided, tree, true, t);
            count += countNode(index_right, right_undecided, tree, false, t);
        }
        count += right_undecided + left_undecided;
        count += countElements(tree[index_left].local, t) + countElements(tree[index_right].local, t);
        result += count;
    }
    return result;
}

int main(void){
    size_t result = 0;
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    set<pair<int, int>> allInts;
    cin >> n;
    tup elements[n];
    cin >> k;
    for (int i = 0; i < n; i++) {
        int a,b,t;
        cin >> a;
        cin >> b;
        cin >> t;
        elements[i] = std::tie(t, a, b);
        allInts.insert({a, 1});
        allInts.insert({b, 2});
    }
    sort(elements, elements + n);
    int depth = smallestPow(allInts.size()) + 1; // moze bez +1
    treeSize = (1 << (smallestPow(allInts.size()) + 1));// korzen w 1
    Node* tree = new Node[treeSize];
    for (int i = 0; i < depth; ++i) {
        for (int j = treeSize /(1<< (i + 1)); j < treeSize / (1 << (i)); j++){
            if (i == 0 && !allInts.empty()){
                tree[j].min = allInts.begin()->first;
                allInts.erase(allInts.begin());
                tree[j].max = allInts.empty() ? INT_MAX : allInts.begin()->first;
            } else if (i == 0 && allInts.empty()) {
                tree[j].min = INT_MAX;
                tree[j].max = INT_MAX;
            } else if (i != 0) {
                tree[j].min = tree[leftSon(j)].min;
                tree[j].max = tree[rightSon(j)].max;
            }
        }
    }
    allInts.clear();
    for (int i = 0; i < n; ++i) {
        insertToTree(tree, &(elements[i]));
    }
    for (int i = 0; i < depth; ++i) {
        for (int j = treeSize / (1 << (i + 1)); j < treeSize / (1 << (i)); i++) {
            result += get_count(tree, j);
        }
    }
    cout << result << endl;
    return 0;
}