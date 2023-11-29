#include<iostream>
#include <regex>
#include <string>

using namespace std;

int main(void){
	string s;
	regex license("[A-Z][A-Z0-9]{2,10}");// 
	regex hour("([0-9])|([0-1][0-9])|(2[0-4])");//liczby <= 24
	regex date("(([0-9])|([0-1][0-9])|(2[0-4])):[0-5][0-9]");// godzina:minuta
	regex expr("[A-Z][A-Z0-9]{2,10}([\\s]+(([8-9])|(0[8-9])|(1[0-9])):[0-5][0-9]){1,2}");
    regex end("[\\s]*...[\\s]*");
	while (!regex_match(s, end))
	{
		getline(cin ,s);
		cout << regex_match(s, expr) << endl;
	}
	return 0;	
}