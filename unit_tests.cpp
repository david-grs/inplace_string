#include "small_string.h"

#include <boost/noncopyable.hpp>
#include <gtest/gtest.h>

TEST(small_string, init)
{
	small_string s;
	ASSERT_TRUE(s.empty());
	ASSERT_EQ(s.size(), 0);

	s = small_string(31, 'a');
	std::cout << sizeof(s) << std::endl;
	std::cout << s << std::endl;
	printf("%s\n", s.c_str());
}

TEST(small_string, substr)
{
	small_string s("123456");
	small_string ss = s.substr();

	EXPECT_TRUE(s == ss);

	ss = s.substr(4);
	EXPECT_EQ(2, int(ss.size()));
	EXPECT_EQ('5', ss.at(0));
	EXPECT_EQ('6', ss.at(1));

	ss = s.substr(4, 1);
	EXPECT_EQ(1, int(ss.size()));
	EXPECT_EQ('5', ss.at(0));
}

TEST(small_string, iterator)
{
	small_string s("123456");
	small_string::size_type sum = 0;

	for (const char ch : s)
		sum += ch;

	EXPECT_EQ(int('1') * 6 + 15, sum);
}

