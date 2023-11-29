#include <iostream>
#include <memory>

using namespace std;

enum order
{
	BLACK,
	WHITE,
	NONE
};

struct node
{
	order Order;
	int counter;
	int level;
};

#define MAX 1000000000

using Node = node*;

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

int addInterval(int a, int b, bool isWhite, Node *tree, int treeSize, int depth)
{
	int temp = treeSize / 2;
	int change = 0;
	int temp_a = a - 1;
	int temp_b = b - 1;
	int factor = isWhite ? 1 : 0;
	Node a_active_order = nullptr;
	Node b_active_order = nullptr;

	for (int i = 0; i < depth; i++)
	{
		if (a_active_order == nullptr || tree[temp + temp_a]->Order != NONE)
		{
			a_active_order = tree[temp + temp_a];
		}

		if (b_active_order == nullptr || tree[temp + temp_b]->Order != NONE)
		{
			b_active_order = tree[temp + temp_b];
		}
		temp = temp / 2;
		temp_a = temp_a / 2;
		temp_b = temp_b / 2;
	}

	int i;
	temp = treeSize / 2;
	a--;
	b--;
	for (i = 1; i < depth; i++) // dalej nie dziala bo musimy zobaczc co sie dzieje jak wbijamy wyzej
	{
		if (a_active_order != NULL)
		{
			if (a % 2 == 0 && a + 1 != b && a != b)
			{
				tree[temp + a + 1]->counter = (isWhite ? 1 : 0) * (1 << (tree[temp + a + 1]->level));
				tree[temp + a + 1]->Order = isWhite ? WHITE : BLACK;
			}
			else if (a % 2 == 1 && tree[temp + a] != a_active_order)
			{
				tree[temp + a - 1]->counter = (a_active_order->Order == WHITE ? 1 : 0) * (1 << (tree[temp + a - 1]->level));
				tree[temp + a - 1]->Order = a_active_order->Order;
			}

			if (temp != treeSize / 2)
			{
				tree[temp + a]->counter = tree[(2 * temp + 1) + 2 * a]->counter + tree[(2 * temp + 1) + 2 * a + 1]->counter;
				tree[temp + a]->Order = NONE;
			}
			else
			{
				tree[temp + a]->counter = isWhite ? 1 : 0;
				tree[temp + a]->Order = isWhite ? WHITE : BLACK;
			}

			if (tree[temp + a] == a_active_order)
			{
				a_active_order = NULL;
			}
		}
		else
		{ // bez ordera ale sie nie spotkaly
			tree[temp + a]->Order = NONE;
			tree[temp + a]->counter = tree[(2 * temp + 1) + 2 * a]->counter + tree[(2 * temp + 1) + 2 * a + 1]->counter;
			if (a % 2 == 0 && a + 1 != b && a != b)
			{
				tree[temp + a + 1]->counter = (isWhite ? 1 : 0) * (1 << (tree[temp + a + 1]->level));
				tree[temp + a + 1]->Order = isWhite ? WHITE : BLACK;
			}
		}

		if (b_active_order != NULL)
		{
			

			if (b % 2 == 1 && b - 1 != a && a != b)
			{
				tree[temp + b - 1]->counter = (isWhite ? 1 : 0) * (1 << (tree[temp + b - 1]->level));
				tree[temp + b - 1]->Order = isWhite ? WHITE : BLACK;
			}
			else if (b % 2 == 0 && tree[temp + b] != b_active_order)
			{
				tree[temp + b + 1]->counter = (b_active_order->Order == WHITE ? 1 : 0) * (1 << (tree[temp + b + 1]->level));
				tree[temp + b + 1]->Order = b_active_order->Order;
			} // scizezka

			if (temp != treeSize / 2)
			{
				tree[temp + b]->counter = tree[(2 * temp + 1) + 2 * b]->counter + tree[(2 * temp + 1) + 2 * b + 1]->counter; // sciezka updatuje po dzieciach
				tree[temp + b]->Order = NONE;
			}
			else
			{
				tree[temp + b]->counter = isWhite ? 1 : 0;
				tree[temp + b]->Order = isWhite ? WHITE : BLACK;
			}

			if (tree[temp + b] == b_active_order)
			{
				b_active_order = NULL;
			}
		}
		else
		{ // bez ordera ale sie nie spotkaly
			tree[temp + b]->Order = NONE;
			tree[temp + b]->counter = tree[(2 * temp + 1) + 2 * b]->counter + tree[(2 * temp + 1) + 2 * b + 1]->counter;
			if (b % 2 == 1 && b - 1 != a && a != b)
			{
				tree[temp + b - 1]->counter = (isWhite ? 1 : 0) * (1 << (tree[temp + b - 1]->level));
				tree[temp + b - 1]->Order = isWhite ? WHITE : BLACK;
			}
		}
		temp = temp / 2;
		a = a / 2;
		b = b / 2;
	}
	tree[0]->counter = tree[1]->counter + tree[2]->counter;
	tree[0]->Order = NONE;

	return change;
}

int main(void)
{
	int n, m;
	cin >> n;
	cin >> m;
	int depth = smallestPow(n) + 1;
	int treeSize = (1 << (smallestPow(n) + 1)) - 1;
	int count = 0;
	Node *tree = new Node[treeSize];
	for (int i = 0; i < treeSize; i++)
	{
		tree[i] = new node;
		tree[i]->counter = 0;
		tree[i]->Order = NONE;
		tree[i]->level = depth - smallestPow(i + 2);
	}
	int prev = 0;
	tree[0]->Order = BLACK;
	for (size_t i = 0; i < m; i++)
	{
		int a, b;
		char c;
		cin >> a;
		cin >> b;
		cin >> c;
		addInterval(a, b, c == 'B', tree, treeSize, depth);
		count += (tree[0]->counter - prev);
		prev = tree[0]->counter;
		cout << count << endl;
	}
	
	for (int i = 0; i < treeSize; i++)
	{
		delete tree[i];
	}
	delete[] tree;
	return 0;
}