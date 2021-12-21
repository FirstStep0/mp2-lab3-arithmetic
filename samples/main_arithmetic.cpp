// реализация пользовательского приложения
#include <iostream>
#include <string>
#include "arithmetic.h"
using namespace std;

arithmetic ar;

void change_name_of_variable() {
	int n = -1;
	ar.val.resize(0);
	cout << "Enter the count of variables\n";
	while (cin.fail() || (n < 0)) {
		cout << ">";
		cin >> n;
		cin.clear();
		cin.ignore(10000, '\n');
	}
	cout << "Enter the variable name\n";
	for (int i = 0; i < n; i++) {
		bool again = true;
		while (again) {
			string name;
			cin >> name;
			//cin.clear();
			//cin.ignore(10000, '\n');
			if (ar.name_allowed(name)) {
				arithmetic::variable v;
				v.def = name;
				v.value = 0.0;
				ar.val.push_back(v);
				again = false;
			}
			else {
				cout << "incorrect_name. Try again\n";
			}
		}
	}
	cout << "variable names are setted\n";
	
};

void show_variable() {
	for (int i = 0; i < ar.val.size(); i++) {
		auto& it = ar.val[i];
		cout << i << ": " << it.def << " = " << it.value << "\n";
	}
}

void change_value() {
	show_variable();
	int index = -1;
	double value = 0.0;
	if (ar.val.size() > 0) {
		cout << "Choose number of variable\n";
		while (!(0 <= index && index < ar.val.size())) {
			cout << ">";
			cin >> index;
			cin.clear();
			cin.ignore(10000, '\n');
		}
		cout << "Enter the value:\n";
		cout << ">";
		cin >> value;
		cin.clear();
		cin.ignore(10000, '\n');
		ar.val[index].value = value;
		cout << "variable changed\n";
	}
	else {
		cout << "variable not exist\n";
	}
}

int Exit = 10;
void Initialize() {
	ar.val.resize(0);
	arithmetic::variable v;
	v.def = "x";
	v.value = 1.0;
	ar.val.push_back(v);
	v.def = "y";
	v.value = 2.0;
	ar.val.push_back(v);
	v.def = "z";
	v.value = 3.0;
	ar.val.push_back(v);
}

int main()
{
	Initialize();
	cout.precision(10);
	int code = 0;
	while (code != Exit) {
		system("cls");
		cout << "+-----------------------------------------------+\n";
		cout << "|program for working with arithmetic expressions|\n";
		cout << "+-----------------------------------------------+\n";
		cout << "operation: '+' '-' '*' '/' '^' 'sin' 'cos' 'abs'\n";
		cout << "[command for variables]\n";
		cout << "1. Setup variable\n";
		cout << "2. Set value of variable\n";
		cout << "3. Show the variable\n";
		cout << "\n";
		cout << "[command for expressions]\n";
		cout << "4. Solve\n";
		cout << "\n";
		cout << "10. Exit\n";
		cout << ">";
		cin >> code;
		cin.clear();
		cin.ignore(10000, '\n');
		switch (code) {
		case 1: {
			change_name_of_variable();
			break;
		}
		case 2: {
			change_value();
			break;
		}
		case 3: {
			show_variable();
			break;
		}
		case 4: {
			string input;
			getline(cin, input);
			double res;
			try {
				res = ar.solve(input);
				//cout << "expr: " << 
				cout << "Answer: " << res << "\n";
			}
			catch (string e) {
				cout << e << "\n";
			}
			catch (...) {
				cout << "undefined error\n";
			};
			break;
		}
		case 10: {
			break;
		}
		default: {
			cout << "command not found\n";
		}
		}
		system("pause");
	}

}
