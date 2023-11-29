#include <array>
#include <iostream>
#include <list>
#include <map>
#include <regex>
#include <sstream>
#include <string>

using namespace std;

#define RETURN_ERROR(x)            \
    cerr << "ERROR " << x << endl; \
    return true;

#define RETURN_OK(x)            \
    cout << "OK " << x << endl; \
    return 1;

#define RETURN_YES(x)            \
    cout << "YES " << x << endl; \
    return 1;

#define RETURN_NO(x)            \
    cout << "NO " << x << endl; \
    return 0;

list<string> split(string input) {
    size_t i;
    list<string> result;
    while (input.length() != 0) {
	// Usuwamy białe znaki.
	for (i = 0; i < input.length(); i++) {
	    if (!isspace(input[i])) {
		if (i == 0) {
		    break;
		}

		input = input.substr(i, input.length() - i);
		i = 0;
		break;
	    }
	}

	if (i == input.length()) {
	    break;
	}

	// Usuwamy dopoki nie trafimy na biale znaki.
	for (i = 0; i < input.length(); i++) {
	    if (isspace(input[i])) {
		result.push_back(input.substr(0, i));
		input = input.substr(i, input.length() - i);
		i = 0;
		break;
	    }
	}

	if (i == input.length()) {
	    result.push_back(input);
	    break;
	}
    }
    return result;
}

// wczytuje string w postaci xx.xx lub x.xx gdzie x to dowolna liczba
//      i zwraca date jako int.
// dane sa zweryfikowane regexem.
int readDate(string input) {
    string hours, minutes;
    hours = input.substr(0, input.find('.'));
    minutes = input.substr(input.find('.') + 1, 2);
    return (stoi(hours) * 60) + stoi(minutes);
}

void update(list<pair<pair<int, int>, string>> *expirationDates, int input_hour,
	    int *current_day, int *current_hour) {
    if (input_hour <= *current_hour) {
	*current_day += 1;
    }
    *current_hour = input_hour;

    while (!(*expirationDates).empty()) {
	pair<int, int> front_date = (*expirationDates).front().first;

	if (front_date.first < *current_day
	    || (front_date.first == *current_day
		&& front_date.second <= *current_hour)) {
	    (*expirationDates).pop_front();
	} else {
	    return;
	}
    }

    return;
}

void write_new(list<pair<pair<int, int>, string>> *expirationDates,
	       int start_hour, int end_hour, int *current_hour,
	       int *current_day, string licenseNumber) {
    update(expirationDates, start_hour, current_day, current_hour);

    // zapisuje date, godz i numer do listy.
    pair<int, int> date;
    pair<pair<int, int>, string> dateNum;

    if (start_hour > end_hour) {
	date.first = *current_day + 1;
    } else {
	date.first = *current_day;
    }

    date.second = end_hour;

    dateNum.first = date;
    dateNum.second = licenseNumber;
    if (expirationDates->empty()) {
	expirationDates->push_front(dateNum);
	return;
    }

    // zapisuje w sposob uporzadkowany po datach konca.
    for (list<pair<pair<int, int>, string>>::iterator it
	 = (*expirationDates).end();
	 it != expirationDates->begin(); it--) {
	it--;
	if (((*it).first.first < date.first)
	    || ((*it).first.first == date.first
		&& (*it).first.second < date.second)) {
	    it++;
	    expirationDates->insert(it, dateNum);
	    return;
	} else {
	    it++;
	}
    }
    expirationDates->push_front(dateNum);
}

void check(list<pair<pair<int, int>, string>> *expirationDates, int input_hour,
	   int *current_hour, int *current_day, string licenseNumber,
	   int lineNumber) {
    update(expirationDates, input_hour, current_day, current_hour);

    for (auto ticket : (*expirationDates)) {
	if (ticket.second.compare(licenseNumber) == 0) {
	    cout << "YES " << lineNumber << endl;
	    return;
	}
    }
    cout << "NO " << lineNumber << endl;
}

// zwraca false gdy wczyta ...
bool readNewline(int lineNumber,
		 list<pair<pair<int, int>, string>> *expiriationDates,
		 int *currentDay, int *currentHour) {
    string input;
    list<string> dates;
    getline(cin, input);
    /*
	[\\s]* - zaczyna sie dowolna liczba bialych znakow
	[A-Z][A-Z0-9]{2,10} - pierwszy napis zaczyna sie wielka litera
	a potem ma wielkie litery i cyfry. Lacznie bedzie ich miedzy 3 a 11
	[\\s]+(([8-9])|(0[8-9])|(1[0-9]):[0-5][0-9] - warunki na przerwe z
       bialych znakow oraz liczby 8-19 dla 8 i 9 moze sie opcjonalnie pojawic 0
       z przodu. Na koncu generujemy dowolna liczbe z zakresu [0-59] tki blok
       powtarza sie 2 razy
    */

    regex correctInput(
	    "[\\s]*[A-Z][A-Z0-9]{2,10}([\\s]+(([8-9])|(0[8-9])|(1[0-"
	    "9])|(20.00)).[0-5][0-9]){1,2}[\\s]*");
    // jak wczytamy ... to sie kończy.

    dates = split(input);
    string licenseNumber;
    int hour, expirationHour;

    if (!input.compare("")) {
	return false;
    }

    // sprawdza czy wejscie jest poprawne za pomoca regexa.
    //  nie trzeba dalszej weryfikacji
    if (!regex_match(input, correctInput)) {
	RETURN_ERROR(lineNumber)
    }

    licenseNumber = dates.front();
    dates.pop_front();

    if (dates.size() == 1) {
	hour = readDate(dates.front());
	check(expiriationDates, hour, currentHour, currentDay, licenseNumber,
	      lineNumber);
    } else if (dates.size() == 2) {
	cout << "OK " << lineNumber << endl;
	hour = readDate(dates.front());
	expirationHour = readDate(dates.back());
	write_new(expiriationDates, hour, expirationHour, currentHour,
		  currentDay, licenseNumber);
    }
    return true;
}

int main(void) {
    list<pair<pair<int, int>, string>> expirationDates;
    int currentLine = 1;
    int day = 0;
    int current_hour = 8 * 60;	// zaczynamy o 8.00.
    while (readNewline(currentLine, &expirationDates, &day, &current_hour)) {
	currentLine++;
    }
    return 0;
}
