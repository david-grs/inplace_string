#pragma once

#include <array>
#include <cassert>
#include <algorithm>
#include <cstring>

namespace cxx { namespace detail
{

template <std::size_t N>
struct basic_small_string
{
	basic_small_string() :
		_size(0)
	{}

	basic_small_string(std::size_t count, char ch)
	{
		if (count > N)
			throw std::out_of_range("basic_small_string::basic_small_string");

		std::fill_n(std::begin(_data), count, ch);
		_size = count;
	}

	// T can be another basic_small_string, std::string, std::vector, std::array, ...
	template <typename T>
	basic_small_string(const T& other, std::size_t pos)
	{
		init(other.data() + pos, other.size() - pos);
	}

	template <typename T>
	basic_small_string(const T& other, std::size_t pos, std::size_t count)
	{
		init(other.data() + pos, count);
	}

	basic_small_string(const char* str, std::size_t count)
	{
		init(str, count);
	}

	basic_small_string(const char* str)
	{
		init(str, std::strlen(str));
	}

	template <typename InputIt>
	basic_small_string(InputIt first, InputIt last)
	{
		init(first, last);
	}

	bool empty() const { return _size == 0; }
	std::size_t size() const { return _size; }

	char& at(std::size_t i)       { return _data.at(i); }
	char  at(std::size_t i) const { return _data.at(i); }

	char& operator[](std::size_t i)       { return assert(i < _size); _data[i]; }
	char  operator[](std::size_t i) const { return assert(i < _size); _data[i]; }

private:
	void init(const char* str, std::size_t count)
	{
		if (count > N)
			throw std::out_of_range("basic_small_string::init");

		std::copy(str, str + count, std::begin(_data));
		_size = count;
	}

	template <typename InputIt>
	void init(InputIt first, InputIt last)
	{
		const std::size_t count = std::distance(first, last);
		if (count > N)
			throw std::out_of_range("basic_small_string::init");

		std::copy(first, last, std::begin(_data));
		_size = count;
	}

	std::array<char, N> _data;
	char _size;
};

} }

template <std::size_t N>
using basic_small_string = cxx::detail::basic_small_string<N>;

using small_string = basic_small_string<31>;




