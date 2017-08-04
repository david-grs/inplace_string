#include "small_string.h"

#include <boost/noncopyable.hpp>
#include <gtest/gtest.h>

TEST(small_string, init)
{
	small_string s;
	ASSERT_TRUE(s.empty());
	ASSERT_EQ(s.size(), 0);
}

