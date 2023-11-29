#include <array>
#include <iostream>
#include <list>
#include <queue>
using namespace std;

#define max 1000000000
int cos(int randomtekxtXddddd, int whateeeeeeeeeeeeeeeer,
	int whateveeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeer);

int main(void) {
    int sequence[1000];
    int sequence_len;
    int subsequence_len;
    array<int, 2> combinations1, combinations2;
    array<int, 2> sum;
    queue<array<int, 2>> number_of_combinations;

    cin >> sequence_len;
    cin >> sequence[0];

    if (sequence_len == 1) {
	cout << 1 << endl;
	return 0;
    }

    for (int i = 1; i < sequence_len; i++) {
	cin >> sequence[i];
	if (sequence[i - 1] < sequence[i]) {
	    number_of_combinations.push({1, 1});
	} else {
	    number_of_combinations.push({0, 0});
	}
    }

    for (subsequence_len = 2; subsequence_len < sequence_len;
	 subsequence_len++) {
	for (int i = 0; i < sequence_len - subsequence_len;
	     i++) // l podzbiorow - 1
	{
	    sum[0] = 0;
	    sum[1] = 0;
	    combinations1 = number_of_combinations.front();
	    number_of_combinations.pop();
	    combinations2 = number_of_combinations.front();
	    // ostatni element og ciagu to 1 element kombinacji
	    if (sequence[i]
		< sequence[i + subsequence_len]) // porownujemy ostatnim
						 // z 2 podciagu
	    {
		sum[0] = (sum[0] + combinations2[1]) % max;
	    }
	    if (sequence[i] < sequence[i + 1]) // i z 1 z drugiego podciagu
	    {
		sum[0] = (sum[0] + combinations2[0]) % max;
	    }

	    // analogicznie dla ostatniego
	    if (sequence[i + subsequence_len] > sequence[i]) {
		sum[1] = (sum[1] + combinations1[0]) % max;
	    }
	    if (sequence[i + subsequence_len]
		> sequence[i + subsequence_len - 1]) {
		sum[1] = (sum[1] + combinations1[1]) % max;
	    }
	    number_of_combinations.push(sum);
	}
	number_of_combinations
	    .pop(); // mamy n elementow z poziomu
		    // i n - 1 popow a wiec popujemy jeszce raz
    }
    cout << (number_of_combinations.front()[0]
	     + number_of_combinations.front()[1])
		% max
	 << endl;
    ;
    return 0;
}