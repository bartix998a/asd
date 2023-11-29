#include <iostream>
#include <memory>

using namespace std;

#define MAX 1000000000

void swap(int**a, int** b){
	int* temp = *a;
	*a = *b;
	*b = temp;
}

int smallestPow(int n){
	for (int i = 0; true; i++)
	{
		if ((1 << i) >= n)
		{
			return i;
		}	
	}
}

void add(int* tree, int treeSize, int n, int weight = 1){
	int temp = treeSize/2;
	n--;
	for (int i = 0; i < tree[treeSize]; i++)
	{
		tree[temp + n] = (tree[temp + n] + weight) % MAX;
		temp = temp/2;
		n = n/2;
	}
	
}

int sum(int *a, int len)
{
	int temp = 0;
	for (int i = 0; i < len; i++)
	{
		temp = (temp + a[i]) % MAX;
	}
	return temp;
}


int elementsInInterval(int a, int b, int* tree, int treeSize){
	int temp = treeSize/2;
	int sum;
	a--;
	b--;
	sum = tree[temp + a] + tree[temp + b];
	for (int i = 0; i < tree[treeSize] && (a/2 != b/2); i++)
	{
		if (a % 2 == 0)
		{
			sum  = (sum + tree[temp + a + 1]) % MAX;
		}
		if (b%2 == 1)
		{
			sum = (sum + tree[temp + b - 1]) % MAX;
		}
		temp = temp/2;
		a = a/2;
		b = b/2;
	}
	return sum;
}

int main(void){
	int n, k;
	cin >> n;
	cin >> k;
	int* results = new int[n];
	int* buffer = new int[n];
	int permutation[n];
	int treeSize = (1 << (smallestPow(n) + 1)) - 1;
	
	
	for (int i = 0; i < n; i++)
	{
		cin >> permutation[i];
		results[i] = 1;
		buffer[i] = 0;
	}
	
	for (int i = 1; i < k; i++){
		int* tree = new int[treeSize + 1];
		tree[treeSize] = (smallestPow(n) + 1);
		for (int j = 0; j < treeSize; j++)
		{
			tree[j] = 0;
		}
		
		for (int j = 0; j < n; j++)
		{
			buffer[j] = 0;
		}
		

		swap(&results, &buffer);
		for (int j = 1; j < n; j++)
		{
			add(tree, treeSize, permutation[j - 1], buffer[j - 1]);
			results[j] = elementsInInterval(permutation[j], n, tree, treeSize);
		}
		delete tree;
	}

	cout << sum(results, n) << endl;

	return 0;
}