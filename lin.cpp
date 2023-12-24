//
// Created by Bartek on 23.12.2023.
//

#include <iostream>
#include <set>
#include <tuple>
#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

using tup = std::tuple<int, int, int>;

struct cmp {
    bool operator() (tup *t1, tup* t2) const {
        return *t1 < *t2;
    }
};

struct Node {
    int min, max;
    vector<tup*> local;
    vector<tup*> below;
    vector<tup*> above;
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

int smallestPow(int n)
{
    for (int i = 0; true; i++)
    {
        if ((1 << i) >= n)
        {
            return i;
        }
    }
}

int main(void){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int n,k;
    set<int> allInts;
    cin >> n;
    tup elements[n];
    cin >> k;
    for (int i = 0; i < n; i++) {
        int a,b,t;
        cin >> a;
        cin >> b;
        cin >> t;
        elements[i] = std::tie(t, a, b);
        allInts.insert(a);
        allInts.insert(b);
    }
    sort(elements, elements + n);
    int depth = smallestPow(allInts.size()) + 1; // moze bez +1
    int treeSize = (1 << (smallestPow(allInts.size()) + 1)) - 1;
    int lastIndex = 0;
    int last;
    Node tree[treeSize];
    for (int i = 0; i < depth; ++i) {
        for (int j = treeSize /(1<< (i + 1)); j < treeSize / (1 << (i)); i++){
            if (i == 0 && !allInts.empty()){
                tree[j].min = *allInts.begin();
                allInts.erase(allInts.begin());
                tree[j].max = allInts.empty() ? INT_MAX : *allInts.begin();
            } else if (i == 0 && allInts.empty()) {
                tree[j].min = INT_MAX;
                tree[j].max = INT_MAX;
            } else if (i != 0) {
                tree[j].min = tree[leftSon(j)].min;
                tree[j].max = tree[rightSon(j)].max;
            }
        }
    }
}