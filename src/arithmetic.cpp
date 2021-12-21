// ���������� ������� � ������� ��� ���������� �������������� ���������
#include <string>
#include "..\include\arithmetic.h"
using namespace std;

void arithmetic::operation::set(string d, int p, int c, double(*g)(double, double)) {
	def = d;
	priority = p;
	count_arg = c;
	f = g;
}

arithmetic::arithmetic() {
	op.push_back(operation("~", -1, 0, [](double x, double y) {return 0.0; })); //����� ������
	op.push_back(operation("+", 1, 2, [](double x, double y) {return x + y; }));
	op.push_back(operation("-", 1, 2, [](double x, double y) {return x - y; }));
	op.push_back(operation("*", 3, 2, [](double x, double y) {return x * y; }));
	op.push_back(operation("/", 3, 2, [](double x, double y) {return x / y; }));
	op.push_back(operation("^", 4, 2, [](double x, double y) {return pow(x, y); }));
	op.push_back(operation("sin", 5, 1, [](double x, double y) {return sin(x); }));
	op.push_back(operation("cos", 5, 1, [](double x, double y) {return cos(x); }));
	op.push_back(operation("abs", 5, 1, [](double x, double y) {if (x >= 0)return x; else return -x; }));
	op.push_back(operation("`", 6, 1, [](double x, double y) {return -x; })); //������� �����
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

double arithmetic::solve(string input) {
	if (!check_name())throw string("wrong variables");
	input = prepare_for_parse(input);
	//cout << input << "\n";
	input = parse(input);
	//cout << input << "\n";
	return getAns(input);
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
	}
	return output;
};

template<class T>
void push_operation(string& output, my_stack<T>& stack, int priority) {
	while (!stack.empty() && stack.back().second >= priority) {
		output += " ";
		output += stack.back().first;
		stack.pop();
	}
}

string arithmetic::parse(string& input) {
	my_stack<pair<string, int>> stack;
	string output = "";
	string temp = "";

	input += "~";			 //����� ������, �������� ������������� �����������

	bool proc = false;       //����� �� ������� temp?
	int extra_pr = 0;        //�������������� ��������� � ���������
	char last = 0;           //��������� ������
	bool was_point = false;  //���� �� ��� ����� ��� ��������� ����� double
	int count_bracket = 0;   //���-�� ������
	bool unary_minus = true; //����� �� ������� ����� ���� �������?

	for (int i = 0; i < input.size(); i++) {
		temp += input[i];
		proc = false;

		if (('0' <= temp[0] && temp[0] <= '9') || temp[0] == '.') {
			if (!(('0' <= last && last <= '9') || last == '.')) {
				output += " ";
				was_point = false;
			}
			if (temp[0] == '.') {
				if (was_point) {
					throw string("extra_point");
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
				count_bracket++;
				unary_minus = true;
			}
			else {
				extra_pr -= 100;
				count_bracket--;
				unary_minus = false;
			}

			if (count_bracket < 0) {
				throw string("incorrect_bracket");
			}
			proc = true;
		}
		else if (temp[0] == '-') {
			if (unary_minus) {
				push_operation(output, stack, 6 + extra_pr);
				stack.push({ "`",6 + extra_pr });
			}
			else {
				push_operation(output, stack, 1 + extra_pr);
				stack.push({ "-",1 + extra_pr });;
			}
			unary_minus = true;
			proc = true;
		}
		else if (temp[0] == ' ') {
			proc = true;
		}
		else {
			for (int j = 0; j < val.size(); j++) {
				if (val[j].def == temp) {
					output += " ";
					output += temp;
					proc = true;
					unary_minus = false;
					break;
				}
			}
			if (!proc) {
				for (int j = 0; j < op.size(); j++) {
					if (op[j].def == temp) {
						int cur_pr = op[j].priority + extra_pr;
						push_operation(output, stack, cur_pr);
						stack.push({ op[j].def, cur_pr });
						proc = true;
						unary_minus = true;
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
		throw string("wrong_command");
	}
	if (count_bracket > 0) {
		throw string("incorrect_bracket");
	}
	return output;
};

double arithmetic::parseToDouble(string& str) {
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

double arithmetic::getAns(string& input) {
	my_stack<double> stack;
	string temp = "";
	input += " ";
	for (int i = 0; i < input.size(); i++) {
		char& ch = input[i];
		if (ch == ' ') {
			if (temp.size() != 0) {
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
								throw string("few_arg");
								return false;
							}
							if (op[j].count_arg == 1) {
								double x = stack.back();
								stack.pop();
								double q = op[j].f(x, 0);
								stack.push(q);
							}
							else if (op[j].count_arg == 2) {
								double y = stack.back();
								stack.pop();
								double x = stack.back();
								stack.pop();

								double q = op[j].f(x, y);
								stack.push(q);
							}
							break;
						}
					}
				}
			}
			temp = "";
		}
		else {
			temp += ch;
		}
	}
	if (stack.size() > 1) {
		throw string("few_operands");
	}
	return stack.back();
}