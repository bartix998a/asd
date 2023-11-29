#include <iostream>

using namespace std;

int main(void){
	int n,m;
	int a,b, count;
	char c;
	cin >> n;
	cin >> m;
	bool biale[n];
	for (size_t i = 0; i < n; i++)
	{
		biale[i] = false;
	}
	
	for (size_t i = 0; i < m; i++)
	{
		count = 0;
		cin >> a;
		cin >> b;
		cin >> c;
		for (size_t j = a - 1; j <= b - 1; j++)
		{
			biale[j] = c == 'B' ? true : false;
		}
		
		for(int j = 0; j < n; j++){
			count += biale[j] ? 1 : 0;
		}

		cout << count << endl;
	}
	
}