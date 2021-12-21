// тесты для стека

#include "stack.h"
#include <gtest.h>
TEST(my_stack, can_create_stack) {
	ASSERT_NO_THROW(my_stack<int> st);
}
TEST(my_stack, new_stack_is_empty) {
	my_stack<int> st;
	EXPECT_EQ(st.empty(), true);
}
TEST(my_stack, new_stack_size_equal_zero) {
	my_stack<int> st;
	EXPECT_EQ(st.size(), 0);
}
TEST(my_stack, can_push_element) {
	my_stack<int> st;
	ASSERT_NO_THROW(st.push(5));
}
TEST(my_stack, stack_that_has_element_is_not_empty) {
	my_stack<int> st;
	st.push(5);
	EXPECT_EQ(st.empty(), false);
}
TEST(my_stack, can_get_back_element) {
	my_stack<int> st;
	st.push(5);
	EXPECT_EQ(st.back(), 5);
}
TEST(my_stack, throw_exception_when_get_back_element_from_empty_stack) {
	my_stack<int> st;
	ASSERT_ANY_THROW(st.back());
}
TEST(my_stack, can_change_back_element) {
	my_stack<int> st;
	st.push(5);
	st.back() = 3;
	EXPECT_EQ(st.back(), 3);
}
TEST(my_stack, can_pop_element_from_no_empty_stack) {
	my_stack<int> st;
	st.push(5);
	ASSERT_NO_THROW(st.pop());
	EXPECT_EQ(st.empty(), true);
}
TEST(my_stack, no_throw_when_pop_element_from_empty_stack) {
	my_stack<int> st;
	ASSERT_NO_THROW(st.pop());
}
TEST(my_stack, can_clear_stack) {
	my_stack<int> st;
	st.push(2);
	st.push(3);
	st.push(5);
	ASSERT_NO_THROW(st.clear());
	EXPECT_EQ(st.empty(), true);
}
TEST(my_stack, stack_store_elements_correctly) {
	int n = 5;
	my_stack<int> st;
	for (int i = 0; i < n; i++) {
		st.push(i);
	}
	for (int i = n - 1; i >= 0; i--) {
		EXPECT_EQ(st.back(), i);
		st.pop();
	}
}

TEST(my_vector, can_create_vector_with_positive_length) {
	EXPECT_NO_THROW(my_vector<int> v(5));
}
TEST(my_vector, new_vector_is_empty) {
	my_vector<int> v;
	EXPECT_TRUE(v.empty());
}
TEST(my_vector, size_vector_change) {
	my_vector<int> v(5);
	EXPECT_EQ(v.size(), 5);
}
TEST(my_vector, resize_can_change_size) {
	my_vector<int> v(5);
	v.resize(7);
	EXPECT_EQ(v.size(), 7);
}
TEST(my_vector, push_back_can_change_size) {
	my_vector<int> v(5);
	v.push_back(1);
	EXPECT_EQ(v.size(), 6);
}
TEST(my_vector, pop_back_can_change_size) {
	my_vector<int> v(5);
	v.pop_back();
	EXPECT_EQ(v.size(), 4);
}
TEST(my_vector, the_order_is_correct) {
	my_vector<int> v;
	for (int i = 0; i < 5; i++) {
		v.push_back(i);
	}
	for (int i = 4; i >= 0; i--) {
		EXPECT_EQ(v.back(), i);
		v.pop_back();
	}
}