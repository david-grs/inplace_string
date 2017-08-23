#define SMALL_STRING_SANITY_CHECKS
#include "inplace_string.h"

#include <boost/noncopyable.hpp>
#include <gtest/gtest.h>

#include <fstream>

TEST(inplace_string, constructor)
{
	{
		inplace_string s;
		ASSERT_TRUE(s.empty());
		ASSERT_EQ(0, s.size());
		EXPECT_EQ("", std::string(s.c_str()));
	}

	{
		inplace_string s(6, 'a');
		ASSERT_EQ(6, s.size());
		EXPECT_EQ(std::string(6, 'a'), std::string(s.c_str()));
	}

	{
		std::string str("ZZZfoobar");
		inplace_string s(str, 3);
		ASSERT_EQ(6, s.size());
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}

	{
		std::string str("ZZZfoobar");
		inplace_string s(str, 3, inplace_string::npos);
		ASSERT_EQ(6, s.size());
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}

	{
		std::string str("ZZZfoobar");
		inplace_string s(str, 3, 2);
		ASSERT_EQ(2, s.size());
		EXPECT_EQ("fo", std::string(s.c_str()));
	}

	{
		inplace_string str("ZZZfoobar");
		inplace_string s(str, 3);
		ASSERT_EQ(6, s.size());
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}

	{
		inplace_string str("ZZZfoobar");
		inplace_string s(str, 3, inplace_string::npos);
		ASSERT_EQ(6, s.size());
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}

	{
		inplace_string str("ZZZfoobar");
		inplace_string s(str, 3, 2);
		ASSERT_EQ(2, s.size());
		EXPECT_EQ("fo", std::string(s.c_str()));
	}

	{
		inplace_string s("foobarfoo", 6);
		ASSERT_EQ(6, s.size());
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}

	{
		inplace_string s("foobar", 6);
		ASSERT_EQ(6, s.size());
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}

	{
		std::string str("foobarfoo");
		inplace_string s(str.cbegin(), str.cbegin() + 6);
		ASSERT_EQ(6, s.size());
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}

	{
		std::string tmp = std::tmpnam(nullptr);
		std::ofstream ofs(tmp);
		ofs << "foobar";
		ofs.close();

		std::ifstream file(tmp);
		inplace_string s(
			std::istreambuf_iterator<char>(file),
			(std::istreambuf_iterator<char>())
		);
		ASSERT_EQ(6, s.size());
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}

	{
		std::string str("foobar");
		inplace_string s(str);
		ASSERT_EQ(6, s.size());
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}

	{
		inplace_string str("foobar");
		inplace_string s(str);
		ASSERT_EQ(6, s.size());
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}

	{
		inplace_string s({'f', 'o', 'o', 'b', 'a', 'r'});
		ASSERT_EQ(6, s.size());
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}

	{
		inplace_string s(std::experimental::string_view("foobarFOO", 6));
		ASSERT_EQ(6, s.size());
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}
}

TEST(inplace_string, at)
{
	inplace_string s("foobar");
	std::string str("foobar");

	for (std::size_t i = 0;i < s.size(); ++i)
		EXPECT_EQ(str.at(i), s.at(i));

	EXPECT_THROW(s.at(6), std::out_of_range);
}

TEST(inplace_string, sqbck)
{
	inplace_string s("foobar");
	std::string str("foobar");

	for (std::size_t i = 0;i < s.size(); ++i)
		EXPECT_EQ(str[i], s[i]);

	EXPECT_NO_THROW((void)s[6]);
}

TEST(inplace_string, front)
{
	inplace_string s("foobar");
	EXPECT_EQ('f', s.front());

	s[0] = 'z';
	EXPECT_EQ('z', s.front());
}

TEST(inplace_string, back)
{
	inplace_string s("foobar");
	EXPECT_EQ('r', s.back());

	s.push_back('z');
	EXPECT_EQ('z', s.back());

	s.pop_back();
	EXPECT_EQ('r', s.back());
}

TEST(inplace_string, c_str)
{
	inplace_string s("foobar");
	EXPECT_EQ("foobar", std::string(s.c_str()));

	s.clear();
	EXPECT_EQ("", std::string(s.c_str()));
}

TEST(inplace_string, data)
{
	inplace_string s("foobar");
	EXPECT_EQ("foobar", std::string(s.data()));

	s.clear();
	EXPECT_EQ("", std::string(s.data()));
}

TEST(inplace_string, string_view)
{
	inplace_string s("foobar");
	std::experimental::string_view sv = s;
	EXPECT_EQ("foobar", std::string(sv));
}

TEST(inplace_string, criterator)
{
	inplace_string s("foobar");
	std::string str("foobar");

	auto sit = str.rbegin();
	inplace_string::size_type count = 0;
	for (auto it = s.crbegin(); it != s.crend(); ++it, ++sit, ++count)
		EXPECT_EQ(*sit, *it);

	EXPECT_EQ(s.size(), count);
}

TEST(inplace_string, riterator)
{
	inplace_string s("foobar");
	std::string str("foobar");

	auto sit = str.rbegin();
	inplace_string::size_type count = 0;
	for (auto it = s.rbegin(); it != s.rend(); ++it, ++sit, ++count)
		EXPECT_EQ(*sit, *it);

	EXPECT_EQ(s.size(), count);
}

TEST(inplace_string, iterator)
{
	inplace_string s("foobar");
	std::string str("foobar");

	auto sit = str.begin();
	inplace_string::size_type count = 0;
	for (auto it = s.begin(); it != s.end(); ++it, ++sit, ++count)
		EXPECT_EQ(*sit, *it);

	EXPECT_EQ(s.size(), count);
}

TEST(inplace_string, citerator)
{
	inplace_string s("foobar");
	std::string str("foobar");

	auto sit = str.begin();
	inplace_string::size_type count = 0;
	for (auto it = s.cbegin(); it != s.cend(); ++it, ++sit, ++count)
		EXPECT_EQ(*sit, *it);

	EXPECT_EQ(s.size(), count);
}

TEST(inplace_string, empty)
{
	inplace_string s;
	EXPECT_TRUE(s.empty());

	s.push_back(1);
	EXPECT_FALSE(s.empty());

	s.pop_back();
	EXPECT_TRUE(s.empty());

	s = "foo";
	EXPECT_FALSE(s.empty());

	s.clear();
	EXPECT_TRUE(s.empty());

	s = inplace_string{};
	EXPECT_TRUE(s.empty());
}

TEST(inplace_string, length)
{
	inplace_string s;
	EXPECT_EQ(0, s.length());

	s.push_back('f');
	EXPECT_EQ(1, s.length());

	s.pop_back();
	EXPECT_EQ(0, s.length());

	s.append("foobar");
	EXPECT_EQ(6, s.length());

	s.erase(4, 1);
	EXPECT_EQ(5, s.length());

	s.erase();
	EXPECT_EQ(0, s.length());

	s.insert(std::size_t(0), 3, 'f');
	EXPECT_EQ(3, s.length());

	s.clear();
	EXPECT_EQ(0, s.length());
}

TEST(inplace_string, size)
{
	inplace_string s;
	EXPECT_EQ(0, s.size());

	s.push_back('f');
	EXPECT_EQ(1, s.size());

	s.pop_back();
	EXPECT_EQ(0, s.size());

	s.append("foobar");
	EXPECT_EQ(6, s.size());

	s.erase(4, 1);
	EXPECT_EQ(5, s.size());

	s.erase();
	EXPECT_EQ(0, s.size());

	s.insert(std::size_t(0), 3, 'f');
	EXPECT_EQ(3, s.size());

	s.clear();
	EXPECT_EQ(0, s.size());
}

TEST(inplace_string, max_size)
{
	inplace_string s;
	EXPECT_EQ(31, s.max_size());

	inplace_string_t<42> ss;
	EXPECT_EQ(41, ss.max_size());
}

TEST(inplace_string, capacity)
{
	inplace_string s;
	EXPECT_EQ(32, s.capacity());

	inplace_string_t<42> ss;
	EXPECT_EQ(42, ss.capacity());
}

TEST(inplace_string, shrink_to_fit)
{
	inplace_string s("foobar");
	s.shrink_to_fit();
	EXPECT_EQ("foobar", std::string(s.c_str()));
}

TEST(inplace_string, clear)
{
	inplace_string s("foobar");
	s.clear();

	EXPECT_EQ(0, s.size());
	EXPECT_EQ(0, *s.c_str());
	EXPECT_EQ("", std::string(s.c_str()));
	EXPECT_TRUE(s.empty());
}

TEST(inplace_string, substr)
{
	inplace_string s("123456");
	inplace_string ss = s.substr();

	EXPECT_EQ(s, ss);

	ss = s.substr(4);
	EXPECT_EQ(2, int(ss.size()));
	EXPECT_EQ('5', ss.at(0));
	EXPECT_EQ('6', ss.at(1));

	ss = s.substr(4, 1);
	EXPECT_EQ(1, int(ss.size()));
	EXPECT_EQ('5', ss.at(0));
}

TEST(inplace_string, compare_eq)
{
	inplace_string s1("123456");
	inplace_string s2("123456");
	EXPECT_TRUE(s1 == s2);
	EXPECT_EQ(s1.compare(s2), 0);
	EXPECT_TRUE(s1 == "123456");
	EXPECT_TRUE("123456" == s1);
}

TEST(inplace_string, compare_ne)
{
	inplace_string s1("123456");
	inplace_string s2("123356");
	EXPECT_TRUE(s1 != s2);
	EXPECT_NE(s1.compare(s2), 0);
}

TEST(inplace_string, compare_lt)
{
	inplace_string s1("123456");
	inplace_string s2("123456789");
	EXPECT_TRUE(s1 < s2);
	EXPECT_LT(s1.compare(s2), 0);

	s1 = "123456";
	s2 = "123466";
	EXPECT_TRUE(s1 < s2);
	EXPECT_LT(s1.compare(s2), 0);
}

TEST(inplace_string, compare_lte)
{
	inplace_string s1("123456");
	inplace_string s2("123456789");
	EXPECT_TRUE(s1 <= s2);
	EXPECT_LE(s1.compare(s2), 0);

	s1 = "123456";
	s2 = "123456";
	EXPECT_TRUE(s1 <= s2);
	EXPECT_LE(s1.compare(s2), 0);
}

TEST(inplace_string, compare_gt)
{
	inplace_string s1("123356789");
	inplace_string s2("123356");
	EXPECT_TRUE(s1 > s2);
	EXPECT_GT(s1.compare(s2), 0);

	s1 = "123466";
	s2 = "123456";
	EXPECT_TRUE(s1 > s2);
	EXPECT_GT(s1.compare(s2), 0);
}

TEST(inplace_string, compare_gte)
{
	inplace_string s1("123356789");
	inplace_string s2("123356");
	EXPECT_TRUE(s1 >= s2);
	EXPECT_GE(s1.compare(s2), 0);

	s1 = "123456";
	s2 = "123456";
	EXPECT_TRUE(s1 >= s2);
	EXPECT_GE(s1.compare(s2), 0);
}

TEST(inplace_string, append)
{
	inplace_string s("foo");
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

TEST(inplace_string, append_operator)
{
	inplace_string s("foo");
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

TEST(inplace_string, resize)
{
	inplace_string s("foo");
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

TEST(inplace_string, replace_basic)
{
	inplace_string s("fooFOO");

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

TEST(inplace_string, replace)
{
	inplace_string s("fooFOOBAR");

	s.replace(3, 6, inplace_string("bar"));
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("foobar", std::string(s.c_str()));

	s.replace(0, 6, std::string("FOObar"));
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("FOObar", std::string(s.c_str()));

	s.replace(s.begin(), s.begin() + 3, std::string("FOOBAR"));
	EXPECT_EQ(9, s.size());
	EXPECT_EQ("FOOBARbar", std::string(s.c_str()));

	s.replace(s.begin(), s.begin() + 6, inplace_string("foo"));
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("foobar", std::string(s.c_str()));

	s.replace(3, 3, std::string("BARFOO"), 0, 3);
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("fooBAR", std::string(s.c_str()));

	s.replace(0, 3, inplace_string("BARFOO"), 3, 3);
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

	s = inplace_string(31, 'a');
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


TEST(inplace_string, copy)
{
	inplace_string s("foobar");

	char str[10];
	inplace_string::size_type bytes = s.copy(str, inplace_string::npos, 0);
	EXPECT_EQ(6, bytes);
	EXPECT_EQ("foobar", std::string(str));

	bytes = s.copy(str, 10, 3);
	EXPECT_EQ(3, bytes);
	EXPECT_EQ("barbar", std::string(str));
}

TEST(inplace_string, swap)
{
	inplace_string s("foobar");
	inplace_string ss("FOOBAR");

	s.swap(ss);
	EXPECT_EQ("foobar", std::string(ss.c_str()));
	EXPECT_EQ("FOOBAR", std::string(s.c_str()));
}


TEST(inplace_string, find)
{
	inplace_string s("foobar");

	EXPECT_EQ(0, s.find("foo"));
	EXPECT_EQ(1, s.find("o"));
	EXPECT_EQ(3, s.find("bar"));
	EXPECT_EQ(5, s.find("r"));

	const inplace_string::size_type npos = inplace_string::npos;
	EXPECT_EQ(npos, s.find("baz"));
	EXPECT_EQ(npos, s.find("fooz"));
	EXPECT_EQ(npos, s.find("zar"));
	EXPECT_EQ(npos, s.find("foobarz"));
	EXPECT_EQ(npos, s.find("foofoofoofoo"));
	EXPECT_EQ(npos, s.find(""));

	EXPECT_EQ(3, s.find("bar", 1));
	EXPECT_EQ(3, s.find("bar", 3));
	EXPECT_EQ(npos, s.find("bar", 4));
	EXPECT_EQ(npos, s.find("foo", 1));

	EXPECT_EQ(npos, s.find("barbar", 3));
	EXPECT_EQ(3, s.find("barbar", 3, 2));
	EXPECT_EQ(3, s.find("barbar", 3, 3));
	EXPECT_EQ(npos, s.find("barbar", 3, 4));

	EXPECT_EQ(3, s.find(std::string("bar")));
	EXPECT_EQ(3, s.find(std::string("bar"), 2));
	EXPECT_EQ(3, s.find(std::string("bar"), 3));
	EXPECT_EQ(npos, s.find(std::string("barb"), 3));
	EXPECT_EQ(npos, s.find(std::string("bar"), 4));

	EXPECT_EQ(3, s.find(std::experimental::string_view("bar")));
	EXPECT_EQ(3, s.find(std::experimental::string_view("bar"), 2));
	EXPECT_EQ(3, s.find(std::experimental::string_view("bar"), 3));
	EXPECT_EQ(npos, s.find(std::experimental::string_view("barb"), 3));
	EXPECT_EQ(npos, s.find(std::experimental::string_view("bar"), 4));

	EXPECT_EQ(3, s.find(inplace_string("bar")));
	EXPECT_EQ(3, s.find(inplace_string("bar"), 2));
	EXPECT_EQ(3, s.find(inplace_string("bar"), 3));
	EXPECT_EQ(npos, s.find(inplace_string("barb"), 3));
	EXPECT_EQ(npos, s.find(inplace_string("bar"), 4));

	EXPECT_EQ(0, s.find('f'));
	EXPECT_EQ(1, s.find('o'));
	EXPECT_EQ(3, s.find('b'));
	EXPECT_EQ(5, s.find('r'));
	EXPECT_EQ(npos, s.find('z'));
	EXPECT_EQ(0, s.find('f', 0));
	EXPECT_EQ(npos, s.find('f', 1));
	EXPECT_EQ(3, s.find('b', 2));
	EXPECT_EQ(3, s.find('b', 3));
	EXPECT_EQ(npos, s.find('b', 4));
}

