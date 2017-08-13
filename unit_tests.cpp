#define SMALL_STRING_SANITY_CHECKS
#include "small_string.h"

#include <boost/noncopyable.hpp>
#include <gtest/gtest.h>

#include <fstream>

TEST(small_string, constructor)
{
	{
		small_string s;
		ASSERT_TRUE(s.empty());
		ASSERT_EQ(0, s.size());
		EXPECT_EQ("", std::string(s.c_str()));
	}

	{
		small_string s(6, 'a');
		ASSERT_EQ(6, s.size());
		EXPECT_EQ(std::string(6, 'a'), std::string(s.c_str()));
	}

	{
		std::string str("ZZZfoobar");
		small_string s(str, 3);
		ASSERT_EQ(6, s.size());
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}

	{
		std::string str("ZZZfoobar");
		small_string s(str, 3, small_string::npos);
		ASSERT_EQ(6, s.size());
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}

	{
		std::string str("ZZZfoobar");
		small_string s(str, 3, 2);
		ASSERT_EQ(2, s.size());
		EXPECT_EQ("fo", std::string(s.c_str()));
	}

	{
		small_string str("ZZZfoobar");
		small_string s(str, 3);
		ASSERT_EQ(6, s.size());
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}

	{
		small_string str("ZZZfoobar");
		small_string s(str, 3, small_string::npos);
		ASSERT_EQ(6, s.size());
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}

	{
		small_string str("ZZZfoobar");
		small_string s(str, 3, 2);
		ASSERT_EQ(2, s.size());
		EXPECT_EQ("fo", std::string(s.c_str()));
	}

	{
		small_string s("foobarfoo", 6);
		ASSERT_EQ(6, s.size());
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}

	{
		small_string s("foobar", 6);
		ASSERT_EQ(6, s.size());
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}

	{
		std::string str("foobarfoo");
		small_string s(str.cbegin(), str.cbegin() + 6);
		ASSERT_EQ(6, s.size());
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}

	{
		std::string tmp = std::tmpnam(nullptr);
		std::ofstream ofs(tmp);
		ofs << "foobar";
		ofs.close();

		std::ifstream file(tmp);
		small_string s(
			std::istreambuf_iterator<char>(file),
			(std::istreambuf_iterator<char>())
		);
		ASSERT_EQ(6, s.size());
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}

	{
		std::string str("foobar");
		small_string s(str);
		ASSERT_EQ(6, s.size());
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}

	{
		small_string str("foobar");
		small_string s(str);
		ASSERT_EQ(6, s.size());
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}

	{
		small_string s({'f', 'o', 'o', 'b', 'a', 'r'});
		ASSERT_EQ(6, s.size());
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}

	{
		small_string s(std::experimental::string_view("foobarFOO", 6));
		ASSERT_EQ(6, s.size());
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}
}

TEST(small_string, empty)
{
	small_string s;
	EXPECT_TRUE(s.empty());

	s.push_back(1);
	EXPECT_FALSE(s.empty());

	s.pop_back();
	EXPECT_TRUE(s.empty());

	s = "foo";
	EXPECT_FALSE(s.empty());

	s.clear();
	EXPECT_TRUE(s.empty());

	s = small_string{};
	EXPECT_TRUE(s.empty());
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

TEST(small_string, append)
{
	small_string s("foo");
	s.append("bar");
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("foobar", s);
	EXPECT_EQ("foobar", std::string(s.c_str()));

	s = "foo";
	s.append(std::string("baz"));
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("foobaz", s);

	s = "foo";
	s.append("burbar", 3);
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("foobur", s);

	s = "foo";
	s.append(std::experimental::string_view("buz"));
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("foobuz", s);

	s = "foo";
	s.append(3, 'z');
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("foozzz", s);

	s = "foo";

	struct A : std::experimental::string_view
	{
		A() : std::experimental::string_view("bar") { }
	};
	A a;
	s.append(a, 0);
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("foobar", s);
}

TEST(small_string, append_operator)
{
	small_string s("foo");
	s += "bar";
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("foobar", s);
	EXPECT_EQ("foobar", std::string(s.c_str()));

	s = "foo";
	s += std::string("baz");
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("foobaz", s);

	s = "foo";
	s += 'b';
	EXPECT_EQ(4, s.size());
	EXPECT_EQ("foob", s);

	s = "foo";
	s += std::experimental::string_view("buz");
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("foobuz", s);

	s = "foo";
	s += {'b', 'a', 'r'};
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("foobar", s);
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

TEST(small_string, replace_basic)
{
	small_string s("fooFOO");

	s.replace(3, 3, "bar");
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("foobar", std::string(s.c_str()));

	s.replace(0, 3, "FOO");
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("FOObar", std::string(s.c_str()));

	s.replace(3, 3, "BARFOO");
	EXPECT_EQ(9, s.size());
	EXPECT_EQ("FOOBARFOO", std::string(s.c_str()));

	s.replace(3, 6, "BUZ");
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("FOOBUZ", std::string(s.c_str()));

	s.replace(0, 3, "foobar");
	EXPECT_EQ(9, s.size());
	EXPECT_EQ("foobarBUZ", std::string(s.c_str()));

	s.replace(0, 6, "FOO");
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("FOOBUZ", std::string(s.c_str()));

	s.replace(0, 6, "foo");
	EXPECT_EQ(3, s.size());
	EXPECT_EQ("foo", std::string(s.c_str()));

	s.replace(0, 3, "foobarbuzbaz");
	EXPECT_EQ(12, s.size());
	EXPECT_EQ("foobarbuzbaz", std::string(s.c_str()));

	s.replace(0, 1, "FOOBARf");
	EXPECT_EQ(18, s.size());
	EXPECT_EQ("FOOBARfoobarbuzbaz", std::string(s.c_str()));

	s.replace(0, 7, "f");
	EXPECT_EQ(12, s.size());
	EXPECT_EQ("foobarbuzbaz", std::string(s.c_str()));
}

TEST(small_string, replace)
{
	small_string s("fooFOOBAR");

	s.replace(3, 6, small_string("bar"));
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("foobar", std::string(s.c_str()));

	s.replace(0, 6, std::string("FOObar"));
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("FOObar", std::string(s.c_str()));

	s.replace(s.begin(), s.begin() + 3, std::string("FOOBAR"));
	EXPECT_EQ(9, s.size());
	EXPECT_EQ("FOOBARbar", std::string(s.c_str()));

	s.replace(s.begin(), s.begin() + 6, small_string("foo"));
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("foobar", std::string(s.c_str()));

	s.replace(3, 3, std::string("BARFOO"), 0, 3);
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("fooBAR", std::string(s.c_str()));

	s.replace(0, 3, small_string("BARFOO"), 3, 3);
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("FOOBAR", std::string(s.c_str()));

	std::string str("foobarfoo");
	s.replace(s.begin(), s.end(), str.begin(), str.end());
	EXPECT_EQ(9, s.size());
	EXPECT_EQ("foobarfoo", std::string(s.c_str()));

	s.replace(3, 6, "foobar", 3);
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("foofoo", std::string(s.c_str()));

	s.replace(s.begin() + 3, s.end(), "FOOBARfoo", 6);
	EXPECT_EQ(9, s.size());
	EXPECT_EQ("fooFOOBAR", std::string(s.c_str()));

	s = small_string(31, 'a');
	s.replace(s.begin(), s.end(), "foo");
	EXPECT_EQ(3, s.size());
	EXPECT_EQ("foo", std::string(s.c_str()));

	s.replace(1, 2, "OOBAR");
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("fOOBAR", std::string(s.c_str()));

	s.replace(3, 3, s.max_size() - 3, 'z');
	EXPECT_EQ(s.max_size(), s.size());
	EXPECT_EQ("fOO" + std::string(s.max_size() - 3, 'z'), std::string(s.c_str()));

	s.replace(s.begin(), s.begin() + 3, 3, 'a');
	EXPECT_EQ(s.max_size(), s.size());
	EXPECT_EQ("aaa" + std::string(s.max_size() - 3, 'z'), std::string(s.c_str()));

	s = "foobar";
	s.replace(s.begin() + 3, s.begin() + 6, {'f', 'o', 'o', 'b', 'a', 'r'});
	EXPECT_EQ(9, s.size());
	EXPECT_EQ("foofoobar", std::string(s.c_str()));

	s.replace(0, 3, std::experimental::string_view("FOOBARBUZ", 6));
	EXPECT_EQ(12, s.size());
	EXPECT_EQ("FOOBARfoobar", std::string(s.c_str()));

	s.replace(s.begin(), s.end() - 3, std::experimental::string_view("foobuz", 3));
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("foobar", std::string(s.c_str()));
}


TEST(small_string, copy)
{
	small_string s("foobar");

	char str[10];
	small_string::size_type bytes = s.copy(str, small_string::npos, 0);
	EXPECT_EQ(6, bytes);
	EXPECT_EQ("foobar", std::string(str));

	bytes = s.copy(str, 10, 3);
	EXPECT_EQ(3, bytes);
	EXPECT_EQ("barbar", std::string(str));
}
