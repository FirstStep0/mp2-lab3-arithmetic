// тесты для вычисления арифметических выражений
#include <gtest.h>
#include "arithmetic.h"
using namespace std;

class arithmetic_test : public testing::Test {
public:
	arithmetic ar;
	string exp;
	double res;
	double eps = 10e-9;
	virtual void SetUp() {}
	virtual void TearDown() {
		EXPECT_TRUE(abs(ar.solve(exp) - res) < eps);
	}
};

class arithmetic_error : public testing::Test {
public:
	arithmetic ar;
	string exp;
	virtual void SetUp() {}
	virtual void TearDown() {
		EXPECT_ANY_THROW(ar.solve(exp));
	}
};

//random_test
TEST_F(arithmetic_test, test1) {
	exp = "cos(1) - 23.5/3 + 0.3^2";
	res = -7.2030310274653333;
}
TEST_F(arithmetic_test, test2) {
	exp = "2^3+3-1-2*(cos(2)*sin(3))^3";
	res = 10.0004050751170476;
}
TEST_F(arithmetic_test, test3) {
	exp = "-3^-1-2--5*-1";
	res = -7.3333333333333333;
}
TEST_F(arithmetic_test, test4) {
	exp = "sin(2*(-(1^-5))^2)-7*8"; 
	res = -55.090702573174;
}
TEST_F(arithmetic_test, test5) {
	exp = "-1-2-3*-1/2*5^12";
	res = 366210934.5;
}
TEST_F(arithmetic_test, test6) {
	exp = "-2+ 3+ -1- -2-3-  -  -4-1- -2+ 2 +3";
	res = 1;
}
TEST_F(arithmetic_test, test7) {
	exp = ".5+.34-.12*.4";
	res = 0.792;
}
TEST_F(arithmetic_test, test8) {
	exp = ".5+.34-.12*.4";
	res = 0.792;
}
TEST_F(arithmetic_test, test9) {
	exp = "1/2*3/4*5/6*7/8";
	res = 0.2734375;
}
TEST_F(arithmetic_test, test10) {
	exp = "2*56^3-12*6/(7+121-(-32-7))";
	res = 351231.5688622754491018;
}

//specific tests
TEST_F(arithmetic_test, work_with_non_assoc_operations_is_correct) {
	exp = "1-2-3/4-5";
	res = -6.75;
}
TEST_F(arithmetic_test, the_order_of_operator_is_correct) {
	exp = "(1+2)*3*4/5+6-7-8-(-9)+10^sin(11.12)";
	res = 7.3017960431736667;
}
TEST_F(arithmetic_test, answer_no_depend_on_the_spaces_if_it_is_not_number) {
	exp = "1 +  2- 3 - - - 4 *  5  /1-(  12^ ( 4  ))";
	res = -20756;
}

//errors
TEST_F(arithmetic_error, throw_when_operand_lost) {
	exp = "2+3+ +5";
}
TEST_F(arithmetic_error, throw_when_operator_lost) {
	exp = "3-4+5 6-7";
}
TEST_F(arithmetic_error, throw_when_many_point_in_number) {
	exp = "345.67.89+12";
}
TEST_F(arithmetic_error, throw_when_many_bracket) {
	exp = "())";
}
TEST_F(arithmetic_error, throw_when_few_bracket) {
	exp = "(()";
}
TEST_F(arithmetic_error, throw_when_enter_incorrect_command) {
	exp = "tg(12)";// "tg" is absent
}

TEST(arithmetic, throw_when_enter_incorrect_variable_name) {
	arithmetic ar;
	arithmetic::variable v;
	v.def = "s"; //"s" is part of "sin"
	v.value = 0.0;
	ar.val.push_back(v);
	string exp = "12+34";
	EXPECT_ANY_THROW(ar.solve(exp));
}
TEST(arithmetic, added_command_can_be_used) {
	arithmetic ar;
	string exp = "sqr(3)";
	EXPECT_ANY_THROW(ar.solve(exp));// "sqr" not found
	arithmetic::operation v;
	//add "sqr"
	v.set("sqr", 5, 1, [](double x, double y) { return x * x; });
	ar.op.push_back(v);

	EXPECT_EQ(ar.solve(exp), 9.0);
}
TEST(arithmetic, added_variable_can_be_used) {
	arithmetic ar;
	string exp = "x+y";
	EXPECT_ANY_THROW(ar.solve(exp));// "x" and "y" not found
	arithmetic::variable v;

	v.def = "x";
	v.value = 2.0;
	ar.val.push_back(v);

	v.def = "y";
	v.value = 3.0;
	ar.val.push_back(v);

	EXPECT_EQ(ar.solve(exp), 5.0);
}
