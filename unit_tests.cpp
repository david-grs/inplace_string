#define INPLACE_STRING_SANITY_CHECKS
#include "inplace_string.h"

#include <gtest/gtest.h>

#include <fstream>

using my_string = inplace_string<31>;

TEST(inplace_string, constructor)
{
	{
		my_string s;
		ASSERT_TRUE(s.empty());
		ASSERT_EQ(0, s.size());
		EXPECT_EQ("", std::string(s.c_str()));
	}

	{
		my_string s(6, 'a');
		ASSERT_EQ(6, s.size());
		EXPECT_EQ(std::string(6, 'a'), std::string(s.c_str()));
	}

	{
		std::string str("ZZZfoobar");
		my_string s(str, 3);
		ASSERT_EQ(6, s.size());
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}

	{
		std::string str("ZZZfoobar");
		my_string s(str, 3, my_string::npos);
		ASSERT_EQ(6, s.size());
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}

	{
		std::string str("ZZZfoobar");
		my_string s(str, 3, 2);
		ASSERT_EQ(2, s.size());
		EXPECT_EQ("fo", std::string(s.c_str()));
	}

	{
		my_string str("ZZZfoobar");
		my_string s(str, 3);
		ASSERT_EQ(6, s.size());
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}

	{
		my_string str("ZZZfoobar");
		my_string s(str, 3, my_string::npos);
		ASSERT_EQ(6, s.size());
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}

	{
		my_string str("ZZZfoobar");
		my_string s(str, 3, 2);
		ASSERT_EQ(2, s.size());
		EXPECT_EQ("fo", std::string(s.c_str()));
	}

	{
		my_string s("foobarfoo", 6);
		ASSERT_EQ(6, s.size());
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}

	{
		my_string s("foobar", 6);
		ASSERT_EQ(6, s.size());
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}

	{
		std::string str("foobarfoo");
		my_string s(str.cbegin(), str.cbegin() + 6);
		ASSERT_EQ(6, s.size());
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}

	{
		std::string tmp = std::tmpnam(nullptr);
		std::ofstream ofs(tmp);
		ofs << "foobar";
		ofs.close();

		std::ifstream file(tmp);
		my_string s(
			std::istreambuf_iterator<char>(file),
			(std::istreambuf_iterator<char>())
		);
		ASSERT_EQ(6, s.size());
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}

	{
		std::string str("foobar");
		my_string s(str);
		ASSERT_EQ(6, s.size());
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}

	{
		my_string str("foobar");
		my_string s(str);
		ASSERT_EQ(6, s.size());
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}

	{
		my_string s({'f', 'o', 'o', 'b', 'a', 'r'});
		ASSERT_EQ(6, s.size());
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}

	{
		my_string s(std::experimental::string_view("foobarFOO", 6));
		ASSERT_EQ(6, s.size());
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}
}

TEST(inplace_string, at)
{
	my_string s("foobar");
	std::string str("foobar");

	for (std::size_t i = 0;i < s.size(); ++i)
		EXPECT_EQ(str.at(i), s.at(i));

	EXPECT_THROW(s.at(6), std::out_of_range);
}

TEST(inplace_string, sqbck)
{
	my_string s("foobar");
	std::string str("foobar");

	for (std::size_t i = 0;i < s.size(); ++i)
		EXPECT_EQ(str[i], s[i]);
}

TEST(inplace_string, front)
{
	my_string s("foobar");
	EXPECT_EQ('f', s.front());

	s[0] = 'z';
	EXPECT_EQ('z', s.front());
}

TEST(inplace_string, back)
{
	my_string s("foobar");
	EXPECT_EQ('r', s.back());

	s.push_back('z');
	EXPECT_EQ('z', s.back());

	s.pop_back();
	EXPECT_EQ('r', s.back());
}

TEST(inplace_string, c_str)
{
	my_string s("foobar");
	EXPECT_EQ("foobar", std::string(s.c_str()));

	s.clear();
	EXPECT_EQ("", std::string(s.c_str()));
}

TEST(inplace_string, data)
{
	my_string s("foobar");
	EXPECT_EQ("foobar", std::string(s.data()));

	s.clear();
	EXPECT_EQ("", std::string(s.data()));
}

TEST(inplace_string, string_view)
{
	my_string s("foobar");
	std::experimental::string_view sv = s;
	EXPECT_EQ("foobar", std::string(sv));
}

TEST(inplace_string, criterator)
{
	my_string s("foobar");
	std::string str("foobar");

	auto sit = str.rbegin();
	my_string::size_type count = 0;
	for (auto it = s.crbegin(); it != s.crend(); ++it, ++sit, ++count)
		EXPECT_EQ(*sit, *it);

	EXPECT_EQ(s.size(), count);
}

TEST(inplace_string, riterator)
{
	my_string s("foobar");
	std::string str("foobar");

	auto sit = str.rbegin();
	my_string::size_type count = 0;
	for (auto it = s.rbegin(); it != s.rend(); ++it, ++sit, ++count)
		EXPECT_EQ(*sit, *it);

	EXPECT_EQ(s.size(), count);
}

TEST(inplace_string, iterator)
{
	my_string s("foobar");
	std::string str("foobar");

	auto sit = str.begin();
	my_string::size_type count = 0;
	for (auto it = s.begin(); it != s.end(); ++it, ++sit, ++count)
		EXPECT_EQ(*sit, *it);

	EXPECT_EQ(s.size(), count);
}

TEST(inplace_string, citerator)
{
	my_string s("foobar");
	std::string str("foobar");

	auto sit = str.begin();
	my_string::size_type count = 0;
	for (auto it = s.cbegin(); it != s.cend(); ++it, ++sit, ++count)
		EXPECT_EQ(*sit, *it);

	EXPECT_EQ(s.size(), count);
}

TEST(inplace_string, empty)
{
	my_string s;
	EXPECT_TRUE(s.empty());

	s.push_back(1);
	EXPECT_FALSE(s.empty());

	s.pop_back();
	EXPECT_TRUE(s.empty());

	s = "foo";
	EXPECT_FALSE(s.empty());

	s.clear();
	EXPECT_TRUE(s.empty());

	s = my_string{};
	EXPECT_TRUE(s.empty());
}

TEST(inplace_string, length)
{
	my_string s;
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
	my_string s;
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
	my_string s;
	EXPECT_EQ(31, s.max_size());

	inplace_string<41> ss;
	EXPECT_EQ(41, ss.max_size());
}

TEST(inplace_string, capacity)
{
	my_string s;
	EXPECT_EQ(31, s.capacity());

	inplace_string<41> ss;
	EXPECT_EQ(41, ss.capacity());
}

TEST(inplace_string, shrink_to_fit)
{
	my_string s("foobar");
	s.shrink_to_fit();
	EXPECT_EQ("foobar", std::string(s.c_str()));
}

TEST(inplace_string, clear)
{
	my_string s("foobar");
	s.clear();

	EXPECT_EQ(0, s.size());
	EXPECT_EQ(0, *s.c_str());
	EXPECT_EQ("", std::string(s.c_str()));
	EXPECT_TRUE(s.empty());
}

TEST(inplace_string, compare)
{
	{
		my_string s("foobar");
		EXPECT_EQ(s.compare("foobar"), 0);
	}
	{
		my_string s("foobar");
		EXPECT_LT(s.compare("foobas"), 0);
	}
	{
		my_string s("foobar");
		EXPECT_GT(s.compare("eoobar"), 0);
	}
	{
		my_string s("foobar");
		EXPECT_EQ(s.compare(0, 3, "foo"), 0);
	}
	{
		my_string s("FOOfoobar");
		EXPECT_EQ(s.compare(3, 3, "foo"), 0);
	}
	{
		my_string s("FOOfoobar");
		EXPECT_EQ(s.compare(3, 6, "FOOfoobarFOO", 3, 6), 0);
	}
	{
		my_string s("foobar");
		EXPECT_EQ(s.compare(std::string("foobar")), 0);
		EXPECT_TRUE(s == std::string("foobar"));
		EXPECT_TRUE(std::string("foobar") == s);
	}
	{
		my_string s("foobar");
		EXPECT_EQ(s.compare(std::experimental::string_view("foobar")), 0);
	}
	{
		struct A : std::experimental::string_view
		{
			A() : std::experimental::string_view("foobar") { }
		};

		A a;
		my_string s("foobar");
		EXPECT_EQ(s.compare(a), 0);
	}
	{
		my_string s("FOOfoobar");
		EXPECT_EQ(s.compare(3, 6, std::string("foobar")), 0);
	}
	{
		my_string s("FOOfoobar");
		EXPECT_EQ(s.compare(3, 6, std::string("FOOfoobar"), 3), 0);
	}
	{
		my_string s("FOOfoobar");
		EXPECT_EQ(s.compare(3, 6, std::string("FOOfoobarFOO"), 3, 6), 0);
	}
}

TEST(inplace_string, compare_eq)
{
	my_string s1("123456");
	my_string s2("123456");
	EXPECT_TRUE(s1 == s2);
	EXPECT_EQ(s1.compare(s2), 0);
	EXPECT_TRUE(s1 == "123456");
	EXPECT_TRUE("123456" == s1);
}

TEST(inplace_string, compare_ne)
{
	my_string s1("123456");
	my_string s2("123356");
	EXPECT_TRUE(s1 != s2);
	EXPECT_NE(s1.compare(s2), 0);
}

TEST(inplace_string, compare_lt)
{
	my_string s1("123456");
	my_string s2("123456789");
	EXPECT_TRUE(s1 < s2);
	EXPECT_LT(s1.compare(s2), 0);

	s1 = "123456";
	s2 = "123466";
	EXPECT_TRUE(s1 < s2);
	EXPECT_LT(s1.compare(s2), 0);
}

TEST(inplace_string, compare_lte)
{
	my_string s1("123456");
	my_string s2("123456789");
	EXPECT_TRUE(s1 <= s2);
	EXPECT_LE(s1.compare(s2), 0);

	s1 = "123456";
	s2 = "123456";
	EXPECT_TRUE(s1 <= s2);
	EXPECT_LE(s1.compare(s2), 0);
}

TEST(inplace_string, compare_gt)
{
	my_string s1("123356789");
	my_string s2("123356");
	EXPECT_TRUE(s1 > s2);
	EXPECT_GT(s1.compare(s2), 0);

	s1 = "123466";
	s2 = "123456";
	EXPECT_TRUE(s1 > s2);
	EXPECT_GT(s1.compare(s2), 0);
}

TEST(inplace_string, compare_gte)
{
	my_string s1("123356789");
	my_string s2("123356");
	EXPECT_TRUE(s1 >= s2);
	EXPECT_GE(s1.compare(s2), 0);

	s1 = "123456";
	s2 = "123456";
	EXPECT_TRUE(s1 >= s2);
	EXPECT_GE(s1.compare(s2), 0);
}

TEST(inplace_string, append)
{
	{
		my_string s("foo");
		s.append("bar");
		EXPECT_EQ(6, s.size());
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}

	{
		my_string s("foo");
		s.append(std::string("bar"));
		EXPECT_EQ(6, s.size());
		EXPECT_EQ("foobar", s);
	}

	{
		my_string s("foo");
		s.append(std::string("burbar"), 3);
		EXPECT_EQ(6, s.size());
		EXPECT_EQ("foobar", s);
	}

	{
		my_string s("foo");
		s.append(std::string("burbarFOOBAR"), 3, 3);
		EXPECT_EQ(6, s.size());
		EXPECT_EQ("foobar", s);
	}

	{
		my_string s("foo");
		s.append("barbur", 3);
		EXPECT_EQ(6, s.size());
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}

	{
		my_string s(s.max_size(), 'z');
		EXPECT_THROW(s.append("a"), std::length_error);
		EXPECT_EQ(s.max_size(), s.size());
	}

	{
		my_string s("foo");
		s.append(std::experimental::string_view("bar"));
		EXPECT_EQ(6, s.size());
		EXPECT_EQ("foobar", s);
	}

	{
		my_string s("foo");
		s.append(3, 'z');
		EXPECT_EQ(6, s.size());
		EXPECT_EQ("foozzz", s);
	}

	{
		my_string s("foo");
		s.append(s.max_size() - s.size(), 'z');
		EXPECT_EQ(s.max_size(), s.size());
	}

	{
		my_string s("foo");
		EXPECT_THROW(s.append(s.max_size() - s.size() + 1, 'z'), std::length_error);
		EXPECT_EQ(3, s.size());
	}

	{
		my_string s("foo");

		struct A : std::experimental::string_view
		{
			A() : std::experimental::string_view("bar") { }
		};
		A a;
		s.append(a, 0);
		EXPECT_EQ(6, s.size());
		EXPECT_EQ("foobar", s);
	}
}

TEST(inplace_string, append_operator)
{
	{
		my_string s("foo");
		s += "bar";
		EXPECT_EQ(6, s.size());
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}

	{
		my_string s("foo");
		s += std::string("bar");
		EXPECT_EQ(6, s.size());
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}

	{
		my_string s("foo");
		s += 'b';
		EXPECT_EQ(4, s.size());
		EXPECT_EQ("foob", std::string(s.c_str()));
	}

	{
		my_string s("foo");
		s += std::experimental::string_view("bar");
		EXPECT_EQ(6, s.size());
		EXPECT_EQ("foobar", s);
	}

	{
		my_string s("foo");
		s += {'b', 'a', 'r'};
		EXPECT_EQ(6, s.size());
		EXPECT_EQ("foobar", s);
	}

	{
		my_string s("foo");
		s += std::string(s.max_size() - s.size(), 'z');
		EXPECT_EQ(s.max_size(), s.size());
	}

	{
		my_string s("foo");
		EXPECT_THROW(s += std::string(s.max_size() - s.size() + 1, 'z'), std::length_error);
		EXPECT_EQ(3, s.size());
	}
}

TEST(inplace_string, resize)
{
	my_string s("foo");
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

TEST(inplace_string, erase)
{
	{
		my_string s("foobar");
		s.erase(0, 3);
		EXPECT_EQ(3, s.size());
		EXPECT_EQ("bar", std::string(s.c_str()));
	}
	{
		my_string s("foobar");
		s.erase(5);
		EXPECT_EQ(5, s.size());
		EXPECT_EQ("fooba", std::string(s.c_str()));
	}
	{
		my_string s("foobar");
		s.erase(6);
		EXPECT_EQ(6, s.size());
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}
	{
		my_string s("foobar");
		EXPECT_THROW(s.erase(7), std::out_of_range);
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}
	{
		my_string s("foobar");
		s.erase(2);
		EXPECT_EQ(2, s.size());
		EXPECT_EQ("fo", std::string(s.c_str()));
	}
	{
		my_string s("foobar");
		s.erase(3, 1);
		EXPECT_EQ(5, s.size());
		EXPECT_EQ("fooar", std::string(s.c_str()));
	}
	{
		my_string s("foobar");
		s.erase(5);
		EXPECT_EQ(5, s.size());
		EXPECT_EQ("fooba", std::string(s.c_str()));
	}
	{
		my_string s("foobar");
		auto it = s.erase(s.cbegin());
		EXPECT_EQ('o', *it);
		EXPECT_EQ(5, s.size());
		EXPECT_EQ("oobar", std::string(s.c_str()));
	}
	{
		my_string s("foobar");
		auto it = s.erase(s.cend() - 1);
		EXPECT_EQ('\0', *it);
		EXPECT_EQ(5, s.size());
		EXPECT_EQ("fooba", std::string(s.c_str()));
	}
	{
		my_string s("foobar");
		s.erase(s.cbegin() + 3, s.cend());
		EXPECT_EQ(3, s.size());
		EXPECT_EQ("foo", std::string(s.c_str()));
	}
}

TEST(inplace_string, push_back)
{
	{
		my_string s("foobar");
		s.push_back('z');
		EXPECT_EQ(7, s.size());
		EXPECT_EQ("foobarz", std::string(s.c_str()));
	}
	{
		my_string s(my_string::max_size(), 'z');
		EXPECT_EQ(s.max_size(), s.size());
		EXPECT_THROW(s.push_back('a'), std::length_error);
		EXPECT_EQ(s.max_size(), s.size());
	}
}

TEST(inplace_string, pop_back)
{
	{
		my_string s("foobar");
		s.pop_back();
		EXPECT_EQ(5, s.size());
		EXPECT_EQ("fooba", std::string(s.c_str()));
	}
}

TEST(inplace_string, insert)
{
	{
		std::experimental::string_view view("FOOBAR", 6);
		my_string s = "foobarfoobar";
		s.insert(3, view);
		EXPECT_EQ(18, s.size());
		EXPECT_EQ("fooFOOBARbarfoobar", std::string(s.c_str()));
	}

	{
		std::experimental::string_view view("FOOBAR", 6);
		my_string s = "foobarfoobar";
		EXPECT_THROW(s.insert(13, view), std::out_of_range);
		s.insert(3, view);
		EXPECT_EQ(18, s.size());
		EXPECT_EQ("fooFOOBARbarfoobar", std::string(s.c_str()));
	}

	{
		my_string s = "foobarfoobar";
		std::string ss(my_string::max_size() - s.size(), 'a');
		EXPECT_NO_THROW(s.insert(3, std::experimental::string_view(ss.data(), ss.size())));
	}

	{
		my_string s = "foobarfoobar";
		std::string ss(my_string::max_size() - s.size() + 1, 'a');
		EXPECT_THROW(s.insert(3, std::experimental::string_view(ss.data(), ss.size())), std::length_error);
	}

	{
		my_string s;
		EXPECT_NO_THROW(s.insert(std::size_t(0), my_string::capacity(), 'a'));
	}

	{
		my_string s;
		EXPECT_THROW(s.insert(std::size_t(0), my_string::capacity() + 1, 'a'), std::length_error);
	}

	{
		my_string s = "foobarfoobar";
		s.insert(3, 6, 'z');
		EXPECT_EQ(18, s.size());
		EXPECT_EQ("foozzzzzzbarfoobar", std::string(s.c_str()));
	}

	{
		my_string s = "foobarfoobar";
		s.insert(3, "FOOBAR");
		EXPECT_EQ(18, s.size());
		EXPECT_EQ("fooFOOBARbarfoobar", std::string(s.c_str()));
	}

	{
		my_string s = "foobarfoobar";
		s.insert(3, "FOOBARBAZ", 6);
		EXPECT_EQ(18, s.size());
		EXPECT_EQ("fooFOOBARbarfoobar", std::string(s.c_str()));
	}

	{
		my_string s = "foobarfoobar";
		std::string ss = "FOOBAR";
		s.insert(3, ss);
		EXPECT_EQ(18, s.size());
		EXPECT_EQ("fooFOOBARbarfoobar", std::string(s.c_str()));
	}

	{
		my_string s = "foobarfoobar";
		std::string ss = "ZZZFOOBARZZZ";
		s.insert(3, ss, 3, 6);
		EXPECT_EQ(18, s.size());
		EXPECT_EQ("fooFOOBARbarfoobar", std::string(s.c_str()));
	}

	{
		my_string s = "foobarfoobar";
		std::string ss = "ZZZFOOBAR";
		s.insert(3, ss, 3);
		EXPECT_EQ(18, s.size());
		EXPECT_EQ("fooFOOBARbarfoobar", std::string(s.c_str()));
	}

	{
		my_string s = "foobarfoobar";
		auto it = s.insert(s.cbegin() + 3, 'z');
		EXPECT_EQ(13, s.size());
		EXPECT_EQ(s.cbegin() + 3, it);
		EXPECT_EQ("foozbarfoobar", std::string(s.c_str()));
	}

	{
		my_string s = "foobarfoobar";
		auto it = s.insert(s.cbegin() + 3, 6, 'z');
		EXPECT_EQ(18, s.size());
		EXPECT_EQ(s.cbegin() + 3, it);
		EXPECT_EQ("foozzzzzzbarfoobar", std::string(s.c_str()));
	}

	{
		my_string s = "foobarfoobar";
		auto it = s.insert(s.begin() + 3, {'F', 'O', 'O', 'B', 'A', 'R'});
		EXPECT_EQ(18, s.size());
		EXPECT_EQ(s.cbegin() + 3, it);
		EXPECT_EQ("fooFOOBARbarfoobar", std::string(s.c_str()));
	}

	{
		std::string str("FOOBAR");
		my_string s = "foobarfoobar";
		auto it = s.insert(s.cbegin() + 3, str.cbegin(), str.cend());
		EXPECT_EQ(18, s.size());
		EXPECT_EQ(s.cbegin() + 3, it);
		EXPECT_EQ("fooFOOBARbarfoobar", std::string(s.c_str()));
	}

	{
		std::string tmp = std::tmpnam(nullptr);
		std::ofstream ofs(tmp);
		ofs << "FOOBAR";
		ofs.close();

		std::ifstream file(tmp);
		my_string s = "foobarfoobar";
		auto it = s.insert(s.cbegin() + 3,
			std::istreambuf_iterator<char>(file),
			(std::istreambuf_iterator<char>())
		);
		EXPECT_EQ(18, s.size());
		EXPECT_EQ(s.cbegin() + 3, it);
		EXPECT_EQ("fooFOOBARbarfoobar", std::string(s.c_str()));
	}
}

TEST(inplace_string, replace)
{
	{
		my_string s = "foobar";
		s.replace(0, 3, std::string("FOOBAR"));
		EXPECT_EQ("FOOBARbar", std::string(s.c_str()));
	}
	{
		my_string s = "foobar";
		s.replace(s.cbegin(), s.cbegin() + 3, std::string("FOOBAR"));
		EXPECT_EQ("FOOBARbar", std::string(s.c_str()));
	}
	{
		my_string s = "foobar";
		s.replace(0, 3, std::string("FOOBAR"), 0);
		EXPECT_EQ("FOOBARbar", std::string(s.c_str()));
	}
	{
		my_string s = "foobar";
		s.replace(std::size_t(0), 3, std::string("FOOBAR"), 0, 6);
		EXPECT_EQ("FOOBARbar", std::string(s.c_str()));
	}
	{
		my_string s = "foobar";
		s.replace(std::size_t(0), 3, std::string("FOOBARBAZ"), 0, 6);
		EXPECT_EQ("FOOBARbar", std::string(s.c_str()));
	}
	{
		my_string s = "foobar";
		EXPECT_NO_THROW(s.replace(std::size_t(0), 3, std::string(s.max_size() - s.size() + 3, 'z')));
		EXPECT_EQ(s.max_size(), s.size());
	}
	{
		my_string s = "foobar";
		EXPECT_THROW(s.replace(std::size_t(0), 3, std::string(s.max_size() - s.size() + 4, 'z')), std::length_error);
		EXPECT_EQ(6, s.size());
	}
	{
		my_string s = "foobar";
		EXPECT_THROW(s.replace(std::size_t(0), 3, std::string("FOOBAR"), 7, 1), std::out_of_range);
		EXPECT_EQ(6, s.size());
	}
	{
		my_string s = "foobar";
		EXPECT_NO_THROW(s.replace(std::size_t(0), 6, std::string("FOOBAR")));
		EXPECT_EQ("FOOBAR", std::string(s.c_str()));
	}
	{
		my_string s = "foobar";
		EXPECT_NO_THROW(s.replace(std::size_t(6), 6, std::string("FOOBAR")));
		EXPECT_EQ("foobar", std::string(s.c_str()));
	}
	{
		my_string s = "foobar";
		EXPECT_THROW(s.replace(std::size_t(7), 1, std::string("FOOBAR")), std::out_of_range);
		EXPECT_EQ(6, s.size());
	}
	{
		std::string str("FOOBARBAZ");
		my_string s = "foobar";
		s.replace(s.cbegin(), s.cbegin() + 3, str.cbegin(), str.cbegin() + 6);
		EXPECT_EQ(9, s.size());
		EXPECT_EQ("FOOBARbar", std::string(s.c_str()));
	}
	{
		std::string tmp = std::tmpnam(nullptr);
		std::ofstream ofs(tmp);
		ofs << "FOOBARBAZ";
		ofs.close();

		std::ifstream file(tmp);
		my_string s = "foobar";
		s.replace(s.cbegin(), s.cbegin() + 3,
			std::istreambuf_iterator<char>(file),
			(std::istreambuf_iterator<char>())
		);
		EXPECT_EQ(12, s.size());
		EXPECT_EQ("FOOBARBAZbar", std::string(s.c_str()));
	}
	{
		my_string s = "foobar";
		s.replace(0, 3, "FOOBAR");
		EXPECT_EQ("FOOBARbar", std::string(s.c_str()));
	}
	{
		my_string s = "foobar";
		s.replace(0, 3, "FOOBAR", 0);
		EXPECT_EQ("bar", std::string(s.c_str()));
	}
	{
		my_string s = "foobar";
		s.replace(0, 3, "FOOBAR", 6);
		EXPECT_EQ("FOOBARbar", std::string(s.c_str()));
	}
	{
		my_string s = "foobar";
		s.replace(s.cbegin(), s.cbegin() + 3, "FOOBAR", 6);
		EXPECT_EQ("FOOBARbar", std::string(s.c_str()));
	}
	{
		my_string s = "foobar";
		s.replace(0, 3, 6, 'z');
		EXPECT_EQ("zzzzzzbar", std::string(s.c_str()));
	}
	{
		my_string s = "foobar";
		s.replace(s.cbegin(), s.cbegin() + 3, 6, 'z');
		EXPECT_EQ("zzzzzzbar", std::string(s.c_str()));
	}
	{
		my_string s = "foobar";
		s.replace(s.cbegin(), s.cbegin() + 3, {'F', 'O', 'O', 'B', 'A', 'R'});
		EXPECT_EQ("FOOBARbar", std::string(s.c_str()));
	}
	{
		my_string s = "foobar";
		s.replace(0, 3, std::experimental::string_view("FOOBAR"));
		EXPECT_EQ("FOOBARbar", std::string(s.c_str()));
	}
	{
		my_string s = "foobar";
		s.replace(s.cbegin(), s.cbegin() + 3, std::experimental::string_view("FOOBAR"));
		EXPECT_EQ("FOOBARbar", std::string(s.c_str()));
	}
}

TEST(inplace_string, substr)
{
	{
		my_string s("123456");
		my_string ss = s.substr(4);
		EXPECT_EQ(2, int(ss.size()));
		EXPECT_EQ('5', ss.at(0));
		EXPECT_EQ('6', ss.at(1));
	}

	{
		my_string s("123456");
		my_string ss = s.substr(4, 1);
		EXPECT_EQ(1, int(ss.size()));
		EXPECT_EQ('5', ss.at(0));
	}

	{
		my_string s("123456");
		EXPECT_THROW(s.substr(7, 1), std::out_of_range);
	}
}


TEST(inplace_string, copy)
{
	my_string s("foobar");

	char str[10];
	my_string::size_type bytes = s.copy(str, my_string::npos, 0);
	EXPECT_EQ(6, bytes);
	EXPECT_EQ("foobar", std::string(str));

	bytes = s.copy(str, 10, 3);
	EXPECT_EQ(3, bytes);
	EXPECT_EQ("barbar", std::string(str));
}

TEST(inplace_string, swap)
{
	my_string s("foobar");
	my_string ss("FOOBAR");

	s.swap(ss);
	EXPECT_EQ("foobar", std::string(ss.c_str()));
	EXPECT_EQ("FOOBAR", std::string(s.c_str()));
}


TEST(inplace_string, find)
{
	my_string s("foobar");

	EXPECT_EQ(0, s.find("foo"));
	EXPECT_EQ(1, s.find("o"));
	EXPECT_EQ(3, s.find("bar"));
	EXPECT_EQ(5, s.find("r"));

	const my_string::size_type npos = my_string::npos;
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

	EXPECT_EQ(3, s.find(my_string("bar")));
	EXPECT_EQ(3, s.find(my_string("bar"), 2));
	EXPECT_EQ(3, s.find(my_string("bar"), 3));
	EXPECT_EQ(npos, s.find(my_string("barb"), 3));
	EXPECT_EQ(npos, s.find(my_string("bar"), 4));

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

