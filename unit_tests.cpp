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

TEST(small_string, init_basic_string)
{
	std::string str = "foo";
	small_string ss(str);

	ASSERT_FALSE(ss.empty());
	ASSERT_EQ(ss.size(), 3);
	ASSERT_TRUE(ss == "foo");
}

TEST(small_string, substr)
{
	small_string s("123456");
	small_string ss = s.substr();

	EXPECT_EQ(s, ss);

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

TEST(small_string, compare_eq)
{
	small_string s1("123456");
	small_string s2("123456");
	EXPECT_TRUE(s1 == s2);
	EXPECT_EQ(s1.compare(s2), 0);
	EXPECT_TRUE(s1 == "123456");
	EXPECT_TRUE("123456" == s1);
}

TEST(small_string, compare_ne)
{
	small_string s1("123456");
	small_string s2("123356");
	EXPECT_TRUE(s1 != s2);
	EXPECT_NE(s1.compare(s2), 0);
}

TEST(small_string, compare_lt)
{
	small_string s1("123456");
	small_string s2("123456789");
	EXPECT_TRUE(s1 < s2);
	EXPECT_LT(s1.compare(s2), 0);

	s1 = "123456";
	s2 = "123466";
	EXPECT_TRUE(s1 < s2);
	EXPECT_LT(s1.compare(s2), 0);
}

TEST(small_string, compare_lte)
{
	small_string s1("123456");
	small_string s2("123456789");
	EXPECT_TRUE(s1 <= s2);
	EXPECT_LE(s1.compare(s2), 0);

	s1 = "123456";
	s2 = "123456";
	EXPECT_TRUE(s1 <= s2);
	EXPECT_LE(s1.compare(s2), 0);
}

TEST(small_string, compare_gt)
{
	small_string s1("123356789");
	small_string s2("123356");
	EXPECT_TRUE(s1 > s2);
	EXPECT_GT(s1.compare(s2), 0);

	s1 = "123466";
	s2 = "123456";
	EXPECT_TRUE(s1 > s2);
	EXPECT_GT(s1.compare(s2), 0);
}

TEST(small_string, compare_gte)
{
	small_string s1("123356789");
	small_string s2("123356");
	EXPECT_TRUE(s1 >= s2);
	EXPECT_GE(s1.compare(s2), 0);

	s1 = "123456";
	s2 = "123456";
	EXPECT_TRUE(s1 >= s2);
	EXPECT_GE(s1.compare(s2), 0);
}

struct A : std::experimental::string_view
{
	A() : std::experimental::string_view("foo") { }
};

TEST(small_string, append)
{
	small_string s("foo");
	s.append("bar");
	EXPECT_EQ("foobar", s);

	s.append(std::string("baz"));
	EXPECT_EQ("foobarbaz", s);

	s.append("burbur", 3);
	EXPECT_EQ("foobarbazbur", s);

	s.append(std::experimental::string_view("buz"));
	EXPECT_EQ("foobarbazburbuz", s);

	s.append(3, 'z');
	EXPECT_EQ("foobarbazburbuzzzz", s);

	A a;
	s.append(a, 0);
	EXPECT_EQ("foobarbazburbuzzzzfoo", s);
}


TEST(small_string, resize)
{
	small_string s("foo");
	s.resize(3, 'z');
	EXPECT_EQ("foo", s);

	s.resize(6, 'z');
	EXPECT_EQ("foozzz", s);

	s.resize(2);
	EXPECT_EQ("fo", s);

	s.resize(6, 'z');
	EXPECT_EQ("fozzzz", s);

	s.resize(6, 'o');
	EXPECT_EQ("fozzzz", s);
}
