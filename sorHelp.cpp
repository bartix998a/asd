#include<iostream>
#include<algorithm>
#include<fstream>

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

int main(void){
	int len;
	int index = 1;
	cin >> len;
	int* seq = new int[len];
	for (int i = 0; i < len; i++)
	{
		seq[i] = i + 1;
	}
	

	for (int i = 0;i < factorial(len); i++)
	{
		printToFile(index, seq, len);
		index++;
		next_permutation(seq, seq + len);
	}
	
}