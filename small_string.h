#pragma once

#include <array>
#include <cassert>
#include <algorithm>
#include <cstring>

#if defined _NO_EXCEPTIONS
#include <iostream>
#endif

#include <experimental/string_view>

namespace cxx { namespace detail {

namespace
{

template <typename T>
inline void throw_helper(const std::string& msg)
{
#ifndef _NO_EXCEPTIONS
	throw T(msg);
#else
	std::cerr << msg.c_str() << "\n";
	std::abort();
#endif
}

}

template <std::size_t N, typename CharT = char>
struct basic_small_string_t
{
	using value_type = CharT;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using size_type = std::size_t;
	using difference_type = std::size_t;
	using iterator = pointer;
	using const_iterator = const_pointer;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	static_assert(std::is_pod<value_type>::value, "CharT type of basic_small_string_t must be a POD");

	basic_small_string_t() :
		_size(0)
	{}

	basic_small_string_t(std::size_t count, value_type ch)
	{
		if (count > N)
			throw_helper<std::out_of_range>("basic_small_string_t::basic_small_string_t: out of range");

		std::fill_n(std::begin(_data), count, ch);
		_size = count;
	}

	// T can be another basic_small_string_t, std::string, std::vector, std::array, ...
	template <typename T>
	basic_small_string_t(const T& other, std::size_t pos)
	{
		init(other.data() + pos, other.size() - pos);
	}

	template <typename T>
	basic_small_string_t(const T& other, std::size_t pos, std::size_t count)
	{
		init(other.data() + pos, count);
	}

	basic_small_string_t(const value_type* str, std::size_t count)
	{
		init(str, count);
	}

	basic_small_string_t(const value_type* str)
	{
		init(str, std::strlen(str));
	}

	template <typename InputIt>
	basic_small_string_t(InputIt first, InputIt last)
	{
		init(first, last);
	}

	reference at(std::size_t i)         { return _data.at(i); }
	value_type  at(std::size_t i) const { return _data.at(i); }

	reference operator[](std::size_t i)         { return assert(i < _size); _data[i]; }
	value_type  operator[](std::size_t i) const { return assert(i < _size); _data[i]; }

	value_type front() const { return assert(!empty()); _data[0]; }
	reference  front()       { return assert(!empty()); _data[0]; }
	value_type back() const  { return assert(!empty()); _data[_size]; }
	reference  back()        { return assert(!empty()); _data[_size]; }

	const value_type* data() const { return _data.data(); }
	char* data() { return _data.data(); }

	const value_type* c_str() const { return _data.data(); }

	operator std::experimental::string_view() const noexcept { return {_data.data(), _size}; }

	bool empty() const           { return size() == 0; }
	std::size_t length() const   { return size(); }
	std::size_t size() const     { return _size; }
	std::size_t max_size() const { return N; }
	std::size_t capacity() const { return N; }
	void shrink_to_fit() {}

private:
	void init(const value_type* str, std::size_t count)
	{
		if (count > N)
			throw_helper<std::out_of_range>("basic_small_string_t::init: out of range");

		std::copy(str, str + count, std::begin(_data));
		_size = count;
	}

	template <typename InputIt>
	void init(InputIt first, InputIt last)
	{
		const std::size_t count = std::distance(first, last);
		if (count > N)
			throw_helper<std::out_of_range>("basic_small_string_t::init: out of range");

		std::copy(first, last, std::begin(_data));
		_size = count;
	}

	// TODO compressed pair, alignement
	std::array<char, N> _data;
	char _size;
};

} }

template <std::size_t N> using small_string_t = cxx::detail::basic_small_string_t<N, char>;
template <std::size_t N> using small_wstring_t = cxx::detail::basic_small_string_t<N, wchar_t>;
template <std::size_t N> using small_u16string_t = cxx::detail::basic_small_string_t<N, char16_t>;
template <std::size_t N> using small_u32string_t = cxx::detail::basic_small_string_t<N, char32_t>;

using small_string = small_string_t<31>;
using small_wstring = small_string_t<31>;
using small_u16string = small_string_t<31>;
using small_u32string = small_string_t<31>;




