#pragma once
#include <string>
#include <queue>
#include "stack.h"
#include <iostream>
using namespace std;
// объ€вление функций и классов дл€ вычислени€ арифметических выражений

class arithmetic {
public:
	struct operation {
		string def;
		int priority;
		int count_arg;
		double(*f)(double, double);
		operation(string d = "", int p = 0, int c = 0, double(*g)(double, double) = nullptr) {
			set(d, p, c, g);
		}
		void set(string d, int p, int c, double(*g)(double, double));
	};
	struct variable {
		string def;
		double value;
	};
	my_vector<operation> op;
	my_vector<variable> val;
	arithmetic();
	bool is_prefix(string& part, string& whole);
	bool name_allowed(string& str);
	double solve(string input);
private:
	bool check_name();
	string parse(string& input);
	double parseToDouble(string& str);
	double getAns(string& input);
	string prepare_for_parse(string input);
};

