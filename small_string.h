#pragma once

#include <array>
#include <cassert>
#include <algorithm>

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

template <
	std::size_t N,
	typename CharT = char,
	typename Traits = std::char_traits<CharT>>
struct basic_small_string_t
{
	using traits_type = Traits;
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

	static const size_type npos = std::basic_string<value_type, traits_type>::npos;

/*assign - assigns a character
eq lt - compares two characters
move - moves one character sequence onto another
copy - copies a character sequence
compare - lexicographically compares two character sequences
length - returns the length of a character sequence
find - finds a character in a character sequence
to_char_type - converts int_type to equivalent char_type
to_int_type - converts char_type to equivalent int_type
eq_int_type - compares two int_type values
eof - returns an eof value
not_eof - checks whether a character is eof value
*/
	static_assert(std::is_pod<value_type>::value, "CharT type of basic_small_string_t must be a POD");
	static_assert(std::is_same<value_type, typename traits_type::char_type>::value, "ChartT type must be the same type as Traits::char_type");

	basic_small_string_t()
	{
		zero();
	}

	basic_small_string_t(std::size_t count, value_type ch)
	{
		init(ch, count);
	}

	template <typename StringT,
			  typename = typename std::enable_if_t<std::is_same<StringT, basic_small_string_t>::value || std::is_same<StringT, std::string>::value>>
	basic_small_string_t(const StringT& other, std::size_t pos)
	{
		init(other.data() + pos, other.size() - pos);
	}

	template <typename StringT,
			  typename = typename std::enable_if_t<std::is_same<StringT, basic_small_string_t>::value || std::is_same<StringT, std::string>::value>>
	basic_small_string_t(const StringT& other, std::size_t pos, std::size_t count)
	{
		init(other.data() + pos, count);
	}

	basic_small_string_t(const value_type* str, std::size_t count)
	{
		init(str, count);
	}

	basic_small_string_t(const value_type* str)
	{
		init(str, traits_type::length(str));
	}

	template <typename InputIt>
	basic_small_string_t(InputIt first, InputIt last)
	{
		init(first, last);
	}

	reference   at(std::size_t i)       { return _data.at(i); }
	value_type  at(std::size_t i) const { return _data.at(i); }

	reference operator[](std::size_t i)         { return assert(i < size()); _data[i]; }
	value_type  operator[](std::size_t i) const { return assert(i < size()); _data[i]; }

	value_type front() const { return assert(!empty()); _data[0]; }
	reference  front()       { return assert(!empty()); _data[0]; }
	value_type back() const  { return assert(!empty()); _data[size() - 1]; }
	reference  back()        { return assert(!empty()); _data[size() - 1]; }

	const value_type* data() const { return _data.data(); }
	char* data() { return _data.data(); }

	const value_type* c_str() const { return _data.data(); }

	operator std::experimental::string_view() const noexcept { return {_data.data(), size()}; }

	const_iterator begin() const  { return std::begin(_data); }
	const_iterator end() const    { return std::begin(_data) + size() + 1; }
	iterator begin()              { return std::begin(_data); }
	iterator end()                { return std::begin(_data) + size() + 1; }
	const_iterator cbegin() const { return begin(); }
	const_iterator cend() const   { return end(); }

	bool      empty() const    { return size() == 0; }
	size_type length() const   { return size(); }
	size_type size() const     { return get_size(); }
	size_type max_size() const { return N; }
	size_type capacity() const { return N; }
	void      shrink_to_fit()  {}

	void clear() { zero(); }

	basic_small_string_t& insert(size_type index, size_type count, value_type ch)
	{
		_insert(index, count, ch);
		return *this;
	}

	basic_small_string_t& insert(size_type index, const value_type* str)
	{
		_insert(index, str, traits_type::length(str));
		return *this;
	}

	basic_small_string_t& insert(size_type index, const value_type* str, size_type count)
	{
		_insert(index, str, count);
		return *this;
	}

	basic_small_string_t& insert(size_type index, const basic_small_string_t& str)
	{
		_insert(index, str.data(), str.size());
		return *this;
	}

	basic_small_string_t& insert(size_type index, const basic_small_string_t& str, size_type index_str, size_type count = npos)
	{
		basic_small_string_t subs = str.substr(index_str, count);
		_insert(index, subs.data(), subs.size());
		return *this;
	}

	iterator insert(const_iterator pos, value_type ch)
	{
		_insert(pos - begin(), 1, ch);
		return pos - begin();
	}

	iterator insert(const_iterator pos, size_type count, value_type ch)
	{
		_insert(pos - begin(), count, ch);
		return pos - begin();
	}

	template <typename InputIt>
	iterator insert(const_iterator pos, InputIt first, InputIt last)
	{
		insert(pos, first, last);
		return pos;
	}

	iterator insert( const_iterator pos, std::initializer_list<CharT> ilist);
	basic_small_string_t& insert(size_type pos, std::experimental::basic_string_view<value_type, traits_type> sv)
	{
		insert(pos, sv.data(), sv.size());
		return *this;
	}

	template <typename T>
	basic_small_string_t& insert(size_type index, const T& t, size_type index_str, size_type count = npos)
	{
		// TODO
		return *this;
	}

	void push_back(value_type ch)
	{
		insert(end(), ch);
	}

	void pop_back()
	{
		erase(size() - 1, 1);
	}

	basic_small_string_t substr(size_type pos = 0, size_type count = npos)
	{
		size_type sz = count == npos ? size() - pos : count;
		return basic_small_string_t(data() + pos, sz);
	}

	int compare(const basic_small_string_t& str) const
	{
		return _compare(0, size(), str.data(), str.size());
	}

	int compare(size_type pos1, size_type count1, const basic_small_string_t& str) const
	{
		return _compare(pos1, count1, str.data(), str.size());
	}

	int compare(size_type pos1, size_type count1, const basic_small_string_t& str, size_type pos2, size_type count2) const // TODO adding npos
	{
		return _compare(pos1, count1, str.data() + pos2, count2 - pos2);
	}

	int compare(const value_type* str) const
	{
		return _compare(0, size(), str, traits_type::length(str));
	}

	int compare(size_type pos1, size_type count1, const value_type* str) const
	{
		return _compare(pos1, count1, str, traits_type::length(str));
	}

	int compare(size_type pos1, size_type count1, const value_type* str, size_type count2) const
	{
		return _compare(pos1, count1, str, count2);
	}

	int compare(std::experimental::basic_string_view<CharT, Traits> sv) const
	{
		return _compare(0, size(), sv.data(), sv.size());
	}

	int compare(size_type pos1, size_type count1, std::experimental::basic_string_view<CharT, Traits> sv) const
	{
		return _compare(pos1, count1, sv.data(), sv.size());
	}

	// adding template < class T >
	//int compare( size_type pos1, size_type count1,
	  //           const T& t,
		//         size_type pos2, size_type count2 = npos) const;

private:
	int _compare(size_type pos1, size_type count1, const value_type* str, size_type count2) const
	{
		size_type sz = std::min(count1, count2);
		const int cmp = traits_type::compare(data() + pos1, str, sz);
		return cmp != 0 ? cmp : count1 - pos1 - count2;
	}

public:

private:
	void init(value_type ch, std::size_t count)
	{
		if (count > N)
			throw_helper<std::out_of_range>("basic_small_string_t::init: out of range");

		traits_type::assign(std::begin(_data), count, ch);
		set_size(count);
	}

	void init(const value_type* str, std::size_t count)
	{
		assert(str != nullptr);

		if (count > N)
			throw_helper<std::out_of_range>("basic_small_string_t::init: out of range");

		traits_type::copy(std::begin(_data), str, count);
		_data[count] = value_type{};

		set_size(count);
	}

	template <typename InputIt>
	void init(InputIt first, InputIt last)
	{
		const std::size_t count = std::distance(first, last);
		if (count > N)
			throw_helper<std::out_of_range>("basic_small_string_t::init: out of range");

		pointer p = _data.data();
		for (auto it = first; it != last; ++it, ++p)
			traits_type::assign(*p, *it);
		traits_type::assign(*p, value_type{});

		set_size(count);
	}

	void zero()
	{
		_data[0] = value_type{};
		set_size(0);
	}

	void _insert(size_type index, size_type count, value_type ch)
	{
		traits_type::move(_data.data() + index + count, _data.data() + index, count);
		for (size_type i = 0; i != count; ++i)
			traits_type::assign(_data[index + i], ch);

		set_size(size() + count);
	}

	void _insert(size_type index, const value_type* str, size_type count)
	{
		traits_type::move(_data.data() + index + count, _data.data() + index, count);
		for (size_type i = 0; i != count; ++i)
			traits_type::assign(_data[index + i], str[i]);

		set_size(size() + count);
	}

	void set_size(size_type size)
	{
		assert(size <= N - 1);
		_data[N - 1] = N - 1 - size;
	}

	size_type get_size() const
	{
		return N - 1 - _data[N - 1];
	}

	std::array<value_type, N> _data;
};

template <std::size_t N, typename CharT, typename Traits>
inline std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const basic_small_string_t<N, CharT, Traits>& str)
{
	return os.write(str.data(), str.size());
}

template <std::size_t N, std::size_t M, typename CharT, typename Traits>
inline bool operator==(const basic_small_string_t<N, CharT, Traits>& lhs,
					   const basic_small_string_t<M, CharT, Traits>& rhs)
{
	return lhs.size() == rhs.size() && Traits::compare(lhs.data(), rhs.data(), lhs.size()) == 0;
}

template <std::size_t N, std::size_t M, typename CharT, typename Traits>
inline bool operator!=(const basic_small_string_t<N, CharT, Traits>& lhs,
					   const basic_small_string_t<M, CharT, Traits>& rhs)
{
	return !(lhs == rhs);
}

template <std::size_t N, std::size_t M, typename CharT, typename Traits>
inline bool operator<(const basic_small_string_t<N, CharT, Traits>& lhs,
					  const basic_small_string_t<M, CharT, Traits>& rhs)
{
	return lhs.compare(rhs) < 0;
}

template <std::size_t N, std::size_t M, typename CharT, typename Traits>
inline bool operator>(const basic_small_string_t<N, CharT, Traits>& lhs,
					  const basic_small_string_t<M, CharT, Traits>& rhs)
{
	return rhs < lhs;
}

template <std::size_t N, std::size_t M, typename CharT, typename Traits>
inline bool operator<=(const basic_small_string_t<N, CharT, Traits>& lhs,
					   const basic_small_string_t<M, CharT, Traits>& rhs)
{
	return !(rhs < lhs);
}

template <std::size_t N, std::size_t M, typename CharT, typename Traits>
inline bool operator>=(const basic_small_string_t<N, CharT, Traits>& lhs,
					   const basic_small_string_t<M, CharT, Traits>& rhs)
{
	return !(lhs < rhs);
}

} }

template <std::size_t N> using small_string_t = cxx::detail::basic_small_string_t<N, char>;
template <std::size_t N> using small_wstring_t = cxx::detail::basic_small_string_t<N, wchar_t>;
template <std::size_t N> using small_u16string_t = cxx::detail::basic_small_string_t<N, char16_t>;
template <std::size_t N> using small_u32string_t = cxx::detail::basic_small_string_t<N, char32_t>;

using small_string = small_string_t<32>;
using small_wstring = small_wstring_t<32>;
using small_u16string = small_u16string_t<32>;
using small_u32string = small_u32string_t<32>;

#define SMALL_STRING_HASH_DEF(x) \
	template <> \
	struct hash<x> \
	{ \
		size_t operator()(const x& str) const \
		{ \
			using view = typename std::experimental::basic_string_view<x::value_type>; \
			view v(str.data(), str.size()); \
			return std::hash<view>()(v); \
		} \
	};

namespace std
{

SMALL_STRING_HASH_DEF(small_string)
SMALL_STRING_HASH_DEF(small_wstring)
SMALL_STRING_HASH_DEF(small_u16string)
SMALL_STRING_HASH_DEF(small_u32string)

}

#undef SMALL_STRING_HASH_DEF

