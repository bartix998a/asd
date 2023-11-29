#include <stdio.h>
#include <string.h>
#include <limits.h>

char wall[1000001];

int min(int a, int b){
	if (a <= b)
	{
		return a;
	}
	return b;
}

int main(void){
	char last_color = 0;
	int smallest_gap; // -1 to niezadeklarowana
	int current_gap = 0;
	int length;

	scanf("%s", &wall);
	length = strlen(wall);
	smallest_gap = length - 1;

	for (int i = 0; i < length; i++)
	{
		if (wall[i] != '*')
		{
			if (last_color == 0)
			{
				last_color = wall[i];
				current_gap = 0;
			} else if (last_color != wall[i])
			{
				smallest_gap = min(smallest_gap, current_gap);
				current_gap = 0;
				last_color = wall[i];
			} else if (last_color == wall[i])
			{
				current_gap = 0;
			}
		} else {
			current_gap++;
		}
	}
	printf("%d\n", length - smallest_gap);
	
}