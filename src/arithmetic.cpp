// реализация функций и классов для вычисления арифметических выражений
#include <string>
#include "..\include\arithmetic.h"
using namespace std;
typedef unsigned int uint;

void arithmetic::operation::set(string d, int p, int c, double(*g)(double, double)) {
	def = d;
	priority = p;
	count_arg = c;
	f = g;
}

arithmetic::arithmetic() {
	op.push_back(operation("~", -1, 0, [](double x, double y) {return 0.0; })); //конец строки
	op.push_back(operation("+", 1, 2, [](double x, double y) {return x + y; }));
	op.push_back(operation("-", 1, 2, [](double x, double y) {return x - y; }));
	op.push_back(operation("*", 3, 2, [](double x, double y) {return x * y; }));
	op.push_back(operation("/", 3, 2, [](double x, double y) {return x / y; }));
	op.push_back(operation("^", 4, 2, [](double x, double y) {return pow(x, y); }));
	op.push_back(operation("sin", 5, 1, [](double x, double y) {return sin(x); }));
	op.push_back(operation("cos", 5, 1, [](double x, double y) {return cos(x); }));
	op.push_back(operation("abs", 5, 1, [](double x, double y) {if (x >= 0)return x; else return -x; }));
	op.push_back(operation("`", 6, 1, [](double x, double y) {return -x; })); //унарный минус
}

bool arithmetic::is_prefix(string& part, string& whole) {
	if (part.size() > whole.size())return false;
	for (int i = 0; i < part.size(); i++) {
		if (part[i] != whole[i])return false;
	}
	return true;
}

bool arithmetic::name_allowed(string& str) {
	for (auto& it : str) {
		if (('0' <= it && it <= '9') || it == '.')return false;
	}

	for (int i = 0; i < op.size(); i++) {
		if (is_prefix(str, op[i].def)) {
			return false;
		}
	}
	for (int i = 0; i < val.size(); i++) {
		if (is_prefix(str, val[i].def)) {
			return false;
		}
	}
	return true;
}

double arithmetic::solve(string &input) {
	if (!check_name())throw string("wrong variables");
	input = prepare_for_parse(input);
	string in = input;
	in = parse(in);
	return getAns(in);
}

bool arithmetic::check_name() {
	for (int i = 0; i < val.size(); i++) {
		for (int j = 0; j < val.size(); j++) {
			if (i != j && is_prefix(val[i].def, val[j].def)) {
				return false;
			}
		}
		for (int k = 0; k < op.size(); k++) {
			if (is_prefix(val[i].def, op[k].def)) {
				return false;
			}
		}
	}
	return true;
}

//delete extra minus
string arithmetic::prepare_for_parse(string input) {
	string output = "";
	char last = 0;
	string temp = "";
	if (input.size() != 0) {
		for (int i = 0; i < input.size(); i++) {
			char cur = input[i];
			if (cur == '-' || (last == '-' && cur == ' ')) {
				if (cur == '-') {
					temp += cur;
				}
			}
			else {
				if (temp.size() != 0) {
					if (temp.size() % 2 == 0) {
						temp = "+";
					}
					else {
						temp = "-";
					}
					output += temp;
					temp = "";
				};
				output += cur;
			}
			if (cur != ' ')last = cur;
		}
		if (temp.size() != 0) {
			if (temp.size() % 2 == 0) {
				temp = "+";
			}
			else {
				temp = "-";
			}
			output += temp;
			temp = "";
		};
	}
	return output;
};

string parseToString(uint number) {
	string res = "";
	do {
		res += ((number % 10) + '0');
		number /= 10;
	} while (number > 0);
	return res;
}

struct info {
	string def;
	int pr;
	int index;
	info() = default;
	info(string d, int p, int i) {
		def = d;
		pr = p;
		index = i;
	}
};

void push_operation(string& output, my_stack<info>& stack, int priority) {
	while (!stack.empty() && stack.back().pr >= priority) {
		output += " " + parseToString((uint)stack.back().index) + "#" + stack.back().def;
		stack.pop();
	}
}

unsigned int parseToUInt(const string& str) {
	uint res = 0;
	for (int i = 0; i < str.size(); i++) {
		if (!('0' <= str[i] && str[i] <= '9'))throw string("error: ParseToUInt()");
		res *= 10;
		res += (str[i] - '0');
	}
	return res;
}

void operand(bool& was_operand, bool able, int index) {
	if (was_operand && able) throw error("lost_operation", index - 1);
	was_operand = able;
}

string arithmetic::parse(string input) {
	my_stack<info> stack;
	string output = "";
	string temp = "";

	input += "~";			 //конец строки, обладает отрицательным приоритетом

	bool proc = false;       //нужно ли очищать temp?
	int extra_pr = 0;        //дополнительный приоритет к операциям
	char last = 0;           //последний символ
	bool was_point = false;  //была ли уже точка при обработке числа double
	int count_bracket = 0;   //кол-во скобок
	bool unary_minus = true; //может ли текущий минус быть унарным?
	int place_bracket = -1;
	bool was_operand = false;

	for (int i = 0; i < input.size(); i++) {
		temp += input[i];
		proc = false;

		if (('0' <= temp[0] && temp[0] <= '9') || temp[0] == '.') {
			if (!(('0' <= last && last <= '9') || last == '.')) {
				output += " " + parseToString(i) + "#";
				was_point = false;
				operand(was_operand, true, i);
			}
			if (temp[0] == '.') {
				if (was_point) {
					throw error("extra_point", i);
				}
				was_point = true;
			}
			output += temp[0];
			unary_minus = false;
			proc = true;
		}
		else if (temp[0] == '(' || temp[0] == ')') {
			if (temp[0] == '(') {
				extra_pr += 100;
				if (count_bracket == 0) {
					place_bracket = i;
				}
				count_bracket++;
				unary_minus = true;
			}
			else {
				extra_pr -= 100;
				count_bracket--;
				unary_minus = false;
			}

			if (count_bracket < 0) {
				throw error("many_bracket", i);
			}
			proc = true;
		}
		else if (temp[0] == '-') {
			if (unary_minus) {
				push_operation(output, stack, 6 + extra_pr);
				stack.push({ "`",6 + extra_pr ,i });
			}
			else {
				push_operation(output, stack, 1 + extra_pr);
				stack.push({ "-",1 + extra_pr,i });
			}
			unary_minus = true;
			operand(was_operand, false, i);
			proc = true;
		}
		else if (temp[0] == ' ') {
			proc = true;
		}
		else {
			for (int j = 0; j < val.size(); j++) {
				if (val[j].def == temp) {
					output += " " + parseToString(i) + "#" + temp;
					proc = true;
					unary_minus = false;
					operand(was_operand, true, i);
					break;
				}
			}
			if (!proc) {
				for (int j = 0; j < op.size(); j++) {
					if (op[j].def == temp) {
						int cur_pr = op[j].priority + extra_pr;
						push_operation(output, stack, cur_pr);
						stack.push({ op[j].def, cur_pr,i });
						proc = true;
						unary_minus = true;
						operand(was_operand, false, i);
						break;
					}
				}
			}
		}
		if (proc) {
			temp = "";
		}
		last = input[i];
	}
	if (temp.size() != 0) {
		throw error("wrong_command", input.size() - temp.size());
	}
	if (count_bracket > 0) {
		throw error("bracket_no_closed", place_bracket);
	}
	return output;
};

double arithmetic::parseToDouble(const string& str) {
	double res = 0.0;
	double pos = str.size();
	for (int i = 0; i < str.size(); i++) {
		if (str[i] == '.') {
			pos = i + 1;
		}
		else {
			res *= 10.0;
			res += (str[i] - '0');
		}
	}
	return res / pow(10.0, str.size() - pos);
}

int get_index(const string& str) {
	string temp = "";
	int i = 0;
	while (str[i] != '#') {
		temp += str[i];
		i++;
	}
	return parseToUInt(temp);
}

string get_command(const string& str) {
	string temp = "";
	int i = 0;
	while (i < str.size() && str[i] != '#') i++;
	i++;
	while (i < str.size()) {
		temp += str[i];
		i++;
	}
	return temp;
}

double arithmetic::getAns(string input) {
	my_stack<double> stack;
	string str = "";
	input += " ";
	for (int i = 0; i < input.size(); i++) {
		char& ch = input[i];
		if (ch == ' ') {
			if (str.size() != 0) {
				int index = get_index(str);
				string temp = get_command(str);
				if ((('0' <= temp[0]) && (temp[0] <= '9')) || temp[0] == '.') {
					stack.push(parseToDouble(temp));
				}
				bool flag = false;
				for (int j = 0; j < val.size(); j++) {
					if (temp == val[j].def) {
						stack.push(val[j].value);
						flag = true;
						break;
					}
				}
				if (!flag) {
					for (int j = 0; j < op.size(); j++) {
						if (temp == op[j].def) {
							if (stack.size() < op[j].count_arg) {
								throw error("few_operand", index + 1);
							}
							if (op[j].count_arg == 1) {
								double x = stack.pop();
								double result = op[j].f(x, 0);
								stack.push(result);
							}
							else if (op[j].count_arg == 2) {
								double y = stack.pop();
								double x = stack.pop();
								double result = op[j].f(x, y);
								stack.push(result);
							}
							break;
						}
					}
				}
			}
			str = "";
		}
		else {
			str += ch;
		}
	}
	//impossible code
	/*if (stack.size() > 1) {
		throw string("lost_operation!");
	}*/
	return stack.pop();
}