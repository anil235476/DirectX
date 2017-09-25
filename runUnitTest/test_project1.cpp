#include "gtest/gtest.h"

#include <iostream>

template<typename T>
T add(const T& a, const T& b) {
	return a + b;
}

TEST(test_add, addition) {
	EXPECT_TRUE(6, add(3, 3));
}
