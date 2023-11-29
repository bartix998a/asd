#include<iostream>
#include<algorithm>
#include <string>
#include<fstream>
#include<list>

using namespace std;

int factorial(int n){
	int result = 1;
	for (int i = 1; i <= n; i++)
	{
		result *= i;
	}
	return result;
}

void printToFile(int index, int *array, int len){
	ofstream file("t" + to_string(index));
	file << len << endl;
	for (int i = 0; i < len - 1; i++)
	{
		file << array[i] << " ";
	}
	file << array[len - 1] << endl;
}

bool same(int* a, int* b, int len){
	for (int i = 0; i < len; i++)
	{
		if (a[i] != b[i])
		{
			return false;
		}
		
	}
	return true;
}

int* step(int len, int* arr){
	list<int> result;
	int* resultProper = new int[len];
	int last = arr[0];
	result.push_front(arr[0]);
	for (int i = 1; i < len; i++)
	{
		if (last > arr[i])
		{
			result.push_front(arr[i]);
		} else {
			result.push_back(arr[i]);
		}
		last = arr[i];
	}
	copy(result.begin(), result.end(), resultProper);
	return resultProper;
}

int main(int argc, char** argv){
	int len;
	int sum = 0;
	cin >> len;
	int* seq = new int[len];
	int* permutation = new int[len];
	for (int i = 0; i < len; i++)
	{
		cin >> seq[i];
		permutation[i] = i + 1;
	}
	

	for (int i = 0;i < factorial(len); i++)
	{
		if (same(step(len, permutation), seq, len))
		{
			sum++;
		}
		next_permutation(permutation, permutation + len);
	}
	cout << sum << endl;
	
}