#include <iostream>
#include <queue>
#include <array>
#include <algorithm>

using namespace std;
/*
int polishFlag(int beginIndex, int endIndex, int element, bool** array){

}
*/
int main(void)
{
	
	int n, m;
	int player, mid;
	cin >> n;
	cin >> m;
	size_t* teams = new size_t[n];// pod itym indeksem mamy zestaw m informacji w ktorym teamie byl zawodnik
	// true jesli w 1 false wpp
	//queue<array<int,3>> queue; // begin_index, end_index, level
	//queue.push({0, n-1, 0});

	for (size_t i = 0; i < n; i++)
	{
		teams[i] = 0;
	}
	
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cin >> player;
			teams[player - 1] <<= 1;
			teams[player - 1] += j/(n/2);// dodajemy 1 lub 0
		}
	}
	sort(teams, teams + n);
	for (size_t i = 0; i < n - 1; i++)
	{
		if (teams[i] == teams[i+1])
		{
			cout << "NIE" << endl;
			return 0;
		}
	}
	cout << "TAK" << endl;
	return 0;
	
	/*
	for (int i = 0; i < m; i++)
	{
		while ((queue.front())[2] == i)
		{
			mid = polishFlag(queue.front()[0], queue.front()[1], i, teams);
			queue.push({queue.front()[0], mid, i + 1});
			if (mid != queue.front()[1])
			{
				queue.push({mid, queue.front()[1], i + 1});
			}
			queue.pop();
		}
	}
	
	while (!queue.empty())
	{
		if (queue.front()[1] - queue.front()[0] > 1)
		{
			cout << "NIE" << endl;
			while (!queue.empty())
			{
				queue.pop();
			}
			return 0;
		}
	}
	cout << "TAK" << endl;
	return 0;
	*/	
}