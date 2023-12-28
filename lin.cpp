//
// Created by Bartek on 23.12.2023.
//

#include <iostream>
#include <set>
#include <tuple>
#include <functional>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>

int treeSize;
int number_of_intervals, max_distance;

using namespace std;

using tup = std::tuple<int, int, int>;


bool cmp(tup *t1, tup *t2) {
    return *t1 <= *t2;
}

struct Node {
    int min, max;
//    vector<tup *> local;
//    vector<tup *> below;
//    vector<tup *> above_max_path;
//    vector<tup *> above_min_path;
    int local;
    int below;
    int above_max_path;
    int above_min_path;

    Node() : min(INT_MIN), max(INT_MAX), local(0), below(0), above_max_path(0), above_min_path(0) {};
};

int parent(int x) {
    return x / 2;
}

int leftSon(int x) {
    return 2 * x;
}

int rightSon(int x) {
    return 2 * x + 1;
}

int neighbour(int x) {
    return 2 * (x / 2) + (1 - (x % 2));
}

bool isLeftSon(int x) {
    return x == leftSon(parent(x));
}

bool isRightSon(int x) {
    return x == rightSon(parent(x));
}

int smallestPow(size_t n) {
    for (int i = 0; true; i++) {
        if ((1 << i) >= n) {
            return i;
        }
    }
}

void nextIndex(int &current, Node *tree, tup *element, bool left) {
    if (leftSon(current) >= treeSize) {
        current = leftSon(current);
        return;
    }
    if (left) {
        if (get<1>(*element) <= tree[leftSon(current)].max) {
            current = leftSon(current);
        } else {
            current = rightSon(current);
        }
    } else {
        if (get<2>(*element) <= tree[leftSon(current)].max) {
            current = leftSon(current);
        } else {
            current = rightSon(current);
        }
    }

}

int findInTree(Node* tree, tup* element){
    int current_L = 1;// korzen w jedynce?
    int current_R = 1;
    nextIndex(current_L, tree, element, true);
    nextIndex(current_R, tree, element, false);
    while (current_L == current_R && current_L < treeSize) {
        nextIndex(current_R, tree, element, false);
        nextIndex(current_L, tree, element, true);
    }
    if (current_L == current_R){
        return current_L;
    } else {
        return parent(current_L);
    }
}

void insertToTree(Node *tree, tup *element) {
    int current_L = 1;// korzen w jedynce?
    int current_R = 1;
    nextIndex(current_L, tree, element, true);
    nextIndex(current_R, tree, element, false);
    while (current_L == current_R && current_L < treeSize) {
        tree[parent(current_L)].below++;// z posortowanej push back dziala
        nextIndex(current_R, tree, element, false);
        nextIndex(current_L, tree, element, true);
    }

    tree[parent(current_L)].local++;
    tree[parent(current_L)].below++;
    if (current_L == current_R) {
        return;
    }
    while (current_R < treeSize) {
        tree[current_L].above_min_path++;
        tree[current_R].above_max_path++;
        nextIndex(current_R, tree, element, false);
        nextIndex(current_L, tree, element, true);
    }
}

void removeFromTree(Node *tree, tup *element) {
    int current_L = 1;// korzen w jedynce?
    int current_R = 1;
    nextIndex(current_L, tree, element, true);
    nextIndex(current_R, tree, element, false);
    while (current_L == current_R && current_L < treeSize) {
        tree[parent(current_L)].below--;// z posortowanej push back dziala
        nextIndex(current_R, tree, element, false);
        nextIndex(current_L, tree, element, true);
    }

    tree[parent(current_L)].local--;
    tree[parent(current_L)].below--;
    if (current_L == current_R) {
        return;
    }
    while (current_R < treeSize) {
        tree[current_L].above_min_path--;
        tree[current_R].above_max_path--;
        nextIndex(current_R, tree, element, false);
        nextIndex(current_L, tree, element, true);
    }
}

int countElements(vector<tup *> v, int t) {
    tup upper_b = tuple(t + max_distance, INT_MAX, INT_MAX);
    tup lower_b = tuple(t - max_distance, INT_MIN, INT_MIN);
    if (v.empty()) {
        return 0;
    }
    return distance(
            lower_bound(v.begin(), v.end(), &lower_b, cmp),
            upper_bound(v.begin(), v.end(), &upper_b, cmp)
    );
}

int countNode(int &index, int &undecided, Node *tree, bool left, int t, size_t &undecided_count) {
    int count = 0;
    int undecided_counter = left ? tree[index].above_max_path : tree[index].above_min_path;
    if ((isLeftSon(index) && left) || (isRightSon(index) && !left)) {
        count += tree[parent(index)].local;
        count += tree[neighbour(index)].below;
        int temp = undecided_counter;
        undecided_count += undecided - temp;
        undecided = temp;
    } else if ((isRightSon(index) && left) || (isLeftSon(index) && !left)) {
        undecided = undecided_counter;
    }
    return count;
}

size_t get_count(Node *tree, tup *element, int pos, size_t &undecided_counter) {
    size_t result = 0;
    int index_left = pos;
    int index_right = pos;
    int left_undecided = 0;
    int right_undecided = 0;
    int t = get<0>(*element);
    size_t count = 0;
    undecided_counter += tree[pos].local - 1;

    if (rightSon(index_right) < treeSize) {
        nextIndex(index_left, tree, element, true);
        nextIndex(index_right, tree, element, false);
        left_undecided = tree[index_left].above_max_path;
        right_undecided = tree[index_right].above_min_path;
    }

    while (rightSon(index_left) < treeSize) {
        nextIndex(index_left, tree, element, true);
        nextIndex(index_right, tree, element, false);
        count += countNode(index_left, left_undecided, tree, true, t, undecided_counter);
        count += countNode(index_right, right_undecided, tree, false, t, undecided_counter);
    }

    undecided_counter += left_undecided + right_undecided;
    if (index_left != index_right) {
        undecided_counter += tree[index_left].local + tree[index_right].local;
    } else if (index_left == index_right) {
        undecided_counter += tree[index_left].above_max_path + tree[index_right].above_min_path;
    }

    result += count;
    return result;
}

int main(void) {
    size_t result = 0;
    size_t undecided_counter = 0;
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    set<int> allInts;
    cin >> number_of_intervals;
    tup elements[number_of_intervals];
    cin >> max_distance;
    for (int i = 0; i < number_of_intervals; i++) {
        int a, b, t;
        cin >> a;
        cin >> b;
        cin >> t;
        elements[i] = std::tie(t, a, b);
        allInts.insert(a);
        allInts.insert(b);
    }
    sort(elements, elements + number_of_intervals);
    int depth = smallestPow(allInts.size()) + 1; // moze bez +1
    treeSize = (1 << (smallestPow(allInts.size()) + 1));// korzen w 1
    Node *tree = new Node[treeSize];
    for (int i = 0; i < depth; ++i) {
        for (int j = treeSize / (1 << (i + 1)); j < treeSize / (1 << (i)); j++) {
            if (i == 0 && !allInts.empty()) {
                tree[j].min = *allInts.begin();
                tree[j].max = allInts.empty() ? INT_MAX : *allInts.begin();
                allInts.erase(allInts.begin());
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
    int first = 0;
    int last = 0;
    for (int i = 0; i < number_of_intervals; i++) {
        while (last < number_of_intervals && abs(get<0>(elements[last]) - get<0>(elements[i])) <= max_distance){
            insertToTree(tree, &elements[last]);
            last++;
        }
        while (abs(get<0>(elements[first]) - get<0>(elements[i])) > max_distance) {
            removeFromTree(tree, &elements[first]);
            first++;
        }
        result += get_count(tree, &elements[i], findInTree(tree, &elements[i]), undecided_counter);
    }
    assert(undecided_counter % 2 == 0);
    result += undecided_counter / 2;
    cout << result << endl;
    return 0;
}