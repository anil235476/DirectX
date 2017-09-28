#include "gtest/gtest.h"

#include <iostream>
#include "utility/array_helper.h"
#include <string>
#include <algorithm>

using namespace std;

template<typename T>
T add(const T& a, const T& b) {
	return a + b;
}

TEST(test_add, addition) {
	EXPECT_EQ(6, add(3, 3));
}


TEST(test_array, experiment_test_construction) {
	using namespace experiment;
	{
		const string one{ "anil" };
		const string two{ "kumar" };
		const string three{ "singh" };

		const auto ret = make_array(one, two, three);
		const std::array<string, 3> expected{ one, two, three };
		EXPECT_EQ(ret.size(), expected.size());
		EXPECT_TRUE(std::equal(expected.begin(), expected.end(),
			ret.begin()));
	
	}

	{
		const int one{ 34 };
		const int two{ 36 };
		const int three{ 54 };
		const auto ret = make_array(one, two, three);
		const std::array<int, 3> expected{one, two, three};
		EXPECT_EQ(ret.size(), expected.size());
		EXPECT_TRUE(std::equal(expected.begin(), expected.end(),
			ret.begin()));
	}
}