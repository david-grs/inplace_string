#pragma once

#include <array>
#include <cassert>
#include <algorithm>
#include <type_traits>
#include <limits>

#if defined _NO_EXCEPTIONS
#include <iostream>
#endif

#include <experimental/string_view>

namespace detail
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

template <typename It, typename ItUp>
struct is_iterator_convertible_to :
		std::integral_constant<bool,
			std::is_convertible<typename std::iterator_traits<It>::iterator_category, ItUp>::value>
{};

template <typename It>
struct is_input_iterator :
	public is_iterator_convertible_to<It, std::input_iterator_tag>
{};

template <typename It>
struct is_forward_iterator :
	public is_iterator_convertible_to<It, std::forward_iterator_tag>
{};

template <typename It>
struct is_exactly_input_iterator :
	public std::integral_constant<bool,
		is_input_iterator<It>::value && !is_forward_iterator<It>::value>
{};

struct is_exactly_input_iterator_tag {};
struct is_input_iterator_tag {};

template <typename CharT, typename Traits>
const CharT* search_substring(const CharT* first1, const CharT* last1, const CharT* first2, const CharT* last2);

}

template <
	std::size_t N,
	typename CharT = char,
	typename Traits = std::char_traits<CharT>>
class basic_inplace_string
{
public:
	using __self = basic_inplace_string;

	using traits_type = Traits;
	using value_type = CharT;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using iterator = pointer;
	using const_iterator = const_pointer;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
	using static_size_type = std::make_unsigned_t<value_type>;

public:
	static constexpr const size_type npos = static_cast<size_type>(-1);

	static_assert(std::is_pod<value_type>::value, "CharT type of basic_inplace_string must be a POD");
	static_assert(std::is_same<value_type, typename traits_type::char_type>::value, "CharT type must be the same type as Traits::char_type");
	static_assert(N < std::numeric_limits<static_size_type>::max(), "N must be smaller than the maximum static_size possible with this CharT type");

	explicit basic_inplace_string() noexcept;

	basic_inplace_string(size_type count, value_type ch);
	basic_inplace_string(const std::basic_string<CharT, Traits>& other, size_type pos);
	basic_inplace_string(const basic_inplace_string& other, size_type pos);
	basic_inplace_string(const std::basic_string<CharT, Traits>& other, size_type pos, size_type count);
	basic_inplace_string(const basic_inplace_string& other, size_type pos, size_type count);
	basic_inplace_string(const value_type* str, size_type count);
	basic_inplace_string(const value_type* str);

	template <typename InputIt>
	basic_inplace_string(InputIt first, InputIt last);

	basic_inplace_string(const std::initializer_list<CharT>& ilist);

	explicit basic_inplace_string(const std::basic_string<CharT, Traits>& str);
	explicit basic_inplace_string(std::experimental::basic_string_view<CharT, Traits> sv);

	template <typename T,
			  typename X = typename std::enable_if<std::is_convertible<const T&, std::experimental::basic_string_view<CharT, Traits>>::value>::type>
	basic_inplace_string(const T& t, size_type pos, size_type n);

	reference       at(size_type i);
	const_reference at(size_type i) const;

	reference       operator[](size_type i)       { assert(i < size()); return _data[i]; }
	const_reference operator[](size_type i) const { assert(i < size()); return _data[i]; }

	reference       front()       { assert(!empty()); return _data[0]; }
	const_reference front() const { assert(!empty()); return _data[0]; }
	reference       back()        { assert(!empty()); return _data[size() - 1]; }
	const_reference back() const  { assert(!empty()); return _data[size() - 1]; }

	value_type*       data() noexcept        { return _data.data(); }
	const value_type* data() const noexcept  { return _data.data(); }
	const value_type* c_str() const noexcept { return _data.data(); }

	operator std::experimental::basic_string_view<CharT, Traits>() const noexcept { return {_data.data(), size()}; }

	iterator       begin() noexcept        { return std::begin(_data); }
	const_iterator begin() const noexcept  { return std::begin(_data); }
	const_iterator cbegin() const noexcept { return begin(); }
	iterator       end() noexcept          { return std::begin(_data) + size(); }
	const_iterator end() const noexcept    { return std::begin(_data) + size(); }
	const_iterator cend() const noexcept   { return end(); }

	reverse_iterator       rbegin() noexcept        { return reverse_iterator(end()); }
	const_reverse_iterator rbegin() const noexcept  { return const_reverse_iterator(cend()); }
	const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }

	reverse_iterator       rend() noexcept        { return reverse_iterator(begin()); }
	const_reverse_iterator rend() const noexcept  { return const_reverse_iterator(cbegin()); }
	const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }

	bool empty() const noexcept { return get_remaining_size() == max_size(); }

	size_type size() const noexcept { return N - _data[N]; }
	size_type length() const noexcept { return size(); }

	static constexpr size_type max_size() noexcept { return N; }
	static constexpr size_type capacity() noexcept { return N; }

	void shrink_to_fit() noexcept  {}

	void clear() noexcept { *this = __self{}; }

	basic_inplace_string& insert(size_type index, size_type count, value_type ch);
	basic_inplace_string& insert(size_type index, const value_type* str);
	basic_inplace_string& insert(size_type index, const value_type* str, size_type count);
	basic_inplace_string& insert(size_type index, const basic_inplace_string& str);
	basic_inplace_string& insert(size_type index, const basic_inplace_string& str, size_type index_str, size_type count = npos);
	iterator insert(const_iterator pos, value_type ch);
	iterator insert(const_iterator pos, size_type count, value_type ch);

	template <typename InputIt>
	iterator insert(const_iterator pos, InputIt first, InputIt last);

	iterator insert(const_iterator pos, std::initializer_list<CharT> ilist);
	basic_inplace_string& insert(size_type pos, std::experimental::basic_string_view<CharT, Traits> view);

	template <typename T,
			  typename X = typename std::enable_if<std::is_convertible<const T&, std::experimental::basic_string_view<CharT, Traits>>::value
												   && !std::is_convertible<const T&, const CharT*>::value>::type>
	basic_inplace_string& insert(size_type pos, const T& t, size_type index_str, size_type count = npos);

	basic_inplace_string& erase(size_type pos = 0, size_type count = npos);
	iterator erase(const_iterator pos);
	iterator erase(const_iterator first, const_iterator last);

	void push_back(value_type ch) { append(1, ch); }
	void pop_back()               { erase(size() - 1, 1); }

	basic_inplace_string& append(size_type count, value_type ch);
	basic_inplace_string& append(const std::basic_string<CharT, Traits>& str);
	basic_inplace_string& append(const std::basic_string<CharT, Traits>& str, size_type pos, size_type count = npos);
	basic_inplace_string& append(const value_type* str, size_type count);
	basic_inplace_string& append(const value_type* str);

	template <typename InputIt>
	basic_inplace_string& append(InputIt first, InputIt last);

	basic_inplace_string& append(std::initializer_list<value_type> ilist);
	basic_inplace_string& append(const std::experimental::basic_string_view<CharT, Traits>& view);

	template <typename T,
			  typename X = typename std::enable_if<std::is_convertible<const T&, std::experimental::basic_string_view<CharT, Traits>>::value
												   && !std::is_convertible<const T&, const CharT*>::value>::type>
	basic_inplace_string& append(const T& t, size_type pos, size_type count = npos);

	basic_inplace_string& operator+=(const std::basic_string<CharT, Traits>& str) { return append(str); }
	basic_inplace_string& operator+=(value_type ch) { push_back(ch); return *this; }
	basic_inplace_string& operator+=(const value_type* str) { return append(str); }
	basic_inplace_string& operator+=(std::initializer_list<value_type> ilist) {return append(ilist); }
	basic_inplace_string& operator+=(std::experimental::basic_string_view<CharT, Traits> view) { return append(view); }

	int compare(const basic_inplace_string& str) const noexcept;
	int compare(size_type pos1, size_type count1, const basic_inplace_string& str) const;
	int compare(size_type pos1, size_type count1, const basic_inplace_string& str, size_type pos2, size_type count2 = npos) const;
	int compare(const value_type* str) const;
	int compare(size_type pos1, size_type count1, const value_type* str) const;
	int compare(size_type pos1, size_type count1, const value_type* str, size_type count2) const;
	int compare(std::experimental::basic_string_view<CharT, Traits> sv) const noexcept;
	int compare(size_type pos1, size_type count1, std::experimental::basic_string_view<CharT, Traits> sv) const;

	template <typename T,
			  typename X = typename std::enable_if<std::is_convertible<const T&, std::experimental::basic_string_view<CharT, Traits>>::value
												   && !std::is_convertible<const T&, const CharT*>::value>::type>
	int compare(size_type pos1, size_type count1, const T& t, size_type pos2, size_type count2 = npos) const;

	basic_inplace_string& replace(size_type pos, size_type count, const basic_inplace_string& str);
	basic_inplace_string& replace(const_iterator first, const_iterator last, const basic_inplace_string& str);
	basic_inplace_string& replace(size_type pos, size_type count, const basic_inplace_string& str, size_type pos2, size_type count2 = npos);

	template <class InputIt>
	basic_inplace_string& replace(const_iterator first, const_iterator last, InputIt first2, InputIt last2);

	basic_inplace_string& replace(size_type pos, size_type count, const CharT* str, size_type count2);
	basic_inplace_string& replace(const_iterator first, const_iterator last, const CharT* str, size_type count2);
	basic_inplace_string& replace(size_type pos, size_type count, const CharT* str);
	basic_inplace_string& replace(const_iterator first, const_iterator last, const CharT* str);
	basic_inplace_string& replace(size_type pos, size_type count, size_type count2, value_type ch);
	basic_inplace_string& replace(const_iterator first, const_iterator last, size_type count2, value_type ch);
	basic_inplace_string& replace(const_iterator first, const_iterator last, std::initializer_list<value_type> ilist);
	basic_inplace_string& replace(size_type pos, size_type count, std::experimental::basic_string_view<CharT, Traits> sv);
	basic_inplace_string& replace(const_iterator first, const_iterator last, std::experimental::basic_string_view<CharT, Traits> sv);

	template <typename T,
			  typename X = typename std::enable_if<std::is_convertible<const T&, std::experimental::basic_string_view<CharT, Traits>>::value
												   && !std::is_convertible<const T&, const CharT*>::value>::type>
	basic_inplace_string& replace(size_type pos, size_type count, const T& t, size_type pos2, size_type count2 = npos);

	basic_inplace_string substr(size_type pos = 0, size_type count = npos) const;

	size_type copy(value_type* dest, size_type count, size_type pos = 0) const;

	void resize(size_type sz);
	void resize(size_type new_size, value_type ch);

	void swap(basic_inplace_string& other) noexcept;

	size_type find(const basic_inplace_string& other, size_type pos = 0) const noexcept;
	size_type find(const value_type* str, size_type pos, size_type count) const noexcept;
	size_type find(const value_type* str, size_type pos = 0) const noexcept;
	size_type find(value_type ch, size_type pos = 0) const noexcept;
	size_type find(std::experimental::basic_string_view<CharT, Traits> sv, size_type pos = 0) const noexcept;

private:
	template <typename InputIt>
	basic_inplace_string(InputIt first, InputIt last, detail::is_exactly_input_iterator_tag);

	template <typename InputIt>
	basic_inplace_string(InputIt first, InputIt last, detail::is_input_iterator_tag);

	template <typename InputIt>
	iterator insert(const_iterator pos, InputIt first, InputIt last, detail::is_exactly_input_iterator_tag);

	template <typename InputIt>
	iterator insert(const_iterator pos, InputIt first, InputIt last, detail::is_input_iterator_tag);

	template <typename InputIt>
	basic_inplace_string& replace(const_iterator first, const_iterator last, InputIt first2, InputIt last2, detail::is_exactly_input_iterator_tag);

	template <typename InputIt>
	basic_inplace_string& replace(const_iterator first, const_iterator last, InputIt first2, InputIt last2, detail::is_input_iterator_tag);

	void set_size(size_type sz)
	{
		assert(sz <= N + 1);
		_data[N] = static_cast<static_size_type>(N - sz);
	}

	size_type get_remaining_size() const
	{
		return _data[N];
	}

	std::array<value_type, N + 1> _data;
};

template <std::size_t N, typename CharT, typename Traits>
basic_inplace_string<N, CharT, Traits>::basic_inplace_string() noexcept
{
	_data[0] = value_type{};
	set_size(0);
}

template <std::size_t N, typename CharT, typename Traits>
basic_inplace_string<N, CharT, Traits>::basic_inplace_string(size_type count, value_type ch)
{
#ifdef INPLACE_STRING_SANITY_CHECKS
	for (size_type i = 0; i <= N; ++i)
		_data[i] = 'a';
#endif

	if (count > N)
		detail::throw_helper<std::out_of_range>("basic_inplace_string::init: out of range");

	traits_type::assign(std::begin(_data), count, ch);
	_data[count] = value_type{};

	set_size(count);
}

template <std::size_t N, typename CharT, typename Traits>
basic_inplace_string<N, CharT, Traits>::basic_inplace_string(const std::basic_string<CharT, Traits>& other, size_type pos) :
	basic_inplace_string(other.data() + pos, other.size() - pos)
{
}

template <std::size_t N, typename CharT, typename Traits>
basic_inplace_string<N, CharT, Traits>::basic_inplace_string(const basic_inplace_string& other, size_type pos) :
	basic_inplace_string(other.data() + pos, other.size() - pos)
{
}

template <std::size_t N, typename CharT, typename Traits>
basic_inplace_string<N, CharT, Traits>::basic_inplace_string(const std::basic_string<CharT, Traits>& other, size_type pos, size_type count) :
	basic_inplace_string(other.data() + pos, std::min(other.size() - pos, count))
{
}

template <std::size_t N, typename CharT, typename Traits>
basic_inplace_string<N, CharT, Traits>::basic_inplace_string(const basic_inplace_string& other, size_type pos, size_type count) :
	basic_inplace_string(other.data() + pos, std::min(other.size() - pos, count))
{
}

template <std::size_t N, typename CharT, typename Traits>
basic_inplace_string<N, CharT, Traits>::basic_inplace_string(const value_type* str, size_type count)
{
	assert(str != nullptr);

#ifdef INPLACE_STRING_SANITY_CHECKS
	for (size_type i = 0; i <= N; ++i)
		_data[i] = 'a';
#endif

	if (count > N)
		detail::throw_helper<std::out_of_range>("basic_inplace_string::init: out of range");

	traits_type::copy(std::begin(_data), str, count);
	_data[count] = value_type{};

	set_size(count);
}

template <std::size_t N, typename CharT, typename Traits>
basic_inplace_string<N, CharT, Traits>::basic_inplace_string(const value_type* str) :
	basic_inplace_string(str, traits_type::length(str))
{
}

template <std::size_t N, typename CharT, typename Traits>
basic_inplace_string<N, CharT, Traits>::basic_inplace_string(const std::basic_string<CharT, Traits>& str) :
	basic_inplace_string(str.data(), str.size())
{
}

template <std::size_t N, typename CharT, typename Traits>
basic_inplace_string<N, CharT, Traits>::basic_inplace_string(const std::initializer_list<CharT>& ilist) :
	basic_inplace_string(ilist.begin(), ilist.size())
{
}

template <std::size_t N, typename CharT, typename Traits>
basic_inplace_string<N, CharT, Traits>::basic_inplace_string(std::experimental::basic_string_view<CharT, Traits> sv) :
	basic_inplace_string(sv.data(), sv.size())
{
}

template <std::size_t N, typename CharT, typename Traits>
template <typename T, typename X>
basic_inplace_string<N, CharT, Traits>::basic_inplace_string(const T& t, size_type pos, size_type n)
{
	std::experimental::basic_string_view<CharT, Traits> sv = t;
	sv = sv.substr(pos, n);
	basic_inplace_string(sv.data(), sv.size());
}

template <std::size_t N, typename CharT, typename Traits>
template <typename InputIt>
basic_inplace_string<N, CharT, Traits>::basic_inplace_string(InputIt first, InputIt last) :
	basic_inplace_string(first,
						 last,
						 typename std::conditional<detail::is_exactly_input_iterator<InputIt>::value,
							 detail::is_exactly_input_iterator_tag,
							detail::is_input_iterator_tag>::type{})
{
}

template <std::size_t N, typename CharT, typename Traits>
template <typename InputIt>
basic_inplace_string<N, CharT, Traits>::basic_inplace_string(InputIt first, InputIt last, detail::is_exactly_input_iterator_tag)
{
#ifdef INPLACE_STRING_SANITY_CHECKS
	for (size_type i = 0; i <= N; ++i)
		_data[i] = 'a';
#endif

	pointer p = _data.data();
	size_type count = 0;
	for (auto it = first; it != last; ++it, ++p, ++count)
	{
		traits_type::assign(*p, *it);

		if (count >= N)
			detail::throw_helper<std::out_of_range>("basic_inplace_string::init: out of range");
	}
	traits_type::assign(*p, value_type{});

	set_size(count);
}

template <std::size_t N, typename CharT, typename Traits>
template <typename InputIt>
basic_inplace_string<N, CharT, Traits>::basic_inplace_string(InputIt first, InputIt last, detail::is_input_iterator_tag)
{
#ifdef INPLACE_STRING_SANITY_CHECKS
	for (size_type i = 0; i <= N; ++i)
		_data[i] = 'a';
#endif

	const size_type count = std::distance(first, last);
	if (count >= N)
		detail::throw_helper<std::out_of_range>("basic_inplace_string::init: out of range");

	pointer p = _data.data();
	for (auto it = first; it != last; ++it, ++p)
		traits_type::assign(*p, *it);
	traits_type::assign(*p, value_type{});

	set_size(count);
}

template <std::size_t N, typename CharT, typename Traits>
typename basic_inplace_string<N, CharT, Traits>::reference
basic_inplace_string<N, CharT, Traits>::at(size_type i)
{
	if (i >= size())
		detail::throw_helper<std::out_of_range>("basic_inplace_string::at: out of range");

	return _data[i];
}

template <std::size_t N, typename CharT, typename Traits>
typename basic_inplace_string<N, CharT, Traits>::const_reference
basic_inplace_string<N, CharT, Traits>::at(size_type i) const
{
	if (i >= size())
		detail::throw_helper<std::out_of_range>("basic_inplace_string::at: out of range");

	return _data[i];
}

template <std::size_t N, typename CharT, typename Traits>
basic_inplace_string<N, CharT, Traits>&
basic_inplace_string<N, CharT, Traits>::insert(size_type index, size_type count, value_type ch)
{
	const size_type sz = size();

	if (index > sz)
		detail::throw_helper<std::out_of_range>("basic_inplace_string::insert: out of range");

	if (sz + count > max_size())
		detail::throw_helper<std::length_error>("basic_inplace_string::insert: maximum capacity reached");

	traits_type::move(_data.data() + index + count, _data.data() + index, sz - index);
	for (size_type i = 0; i != count; ++i)
		traits_type::assign(_data[index + i], ch);

	const size_type new_size = sz + count;
	traits_type::assign(_data[new_size], value_type{});
	set_size(new_size);

	return *this;
}

template <std::size_t N, typename CharT, typename Traits>
basic_inplace_string<N, CharT, Traits>&
basic_inplace_string<N, CharT, Traits>::insert(size_type index, const value_type* str)
{
	return insert(index, str, traits_type::length(str));
}

template <std::size_t N, typename CharT, typename Traits>
basic_inplace_string<N, CharT, Traits>&
basic_inplace_string<N, CharT, Traits>::insert(size_type index, const value_type* str, size_type count)
{
	const size_type sz = size();

	if (index > sz)
		detail::throw_helper<std::out_of_range>("basic_inplace_string::insert: out of range");

	if (size() + count > max_size())
		detail::throw_helper<std::length_error>("basic_inplace_string::insert: maximum capacity reached");

	traits_type::move(_data.data() + index + count, _data.data() + index, sz - index);
	for (size_type i = 0; i != count; ++i)
		traits_type::assign(_data[index + i], str[i]);

	const size_type new_size = sz + count;
	traits_type::assign(_data[new_size], value_type{});
	set_size(new_size);

	return *this;
}

template <std::size_t N, typename CharT, typename Traits>
basic_inplace_string<N, CharT, Traits>&
basic_inplace_string<N, CharT, Traits>::insert(size_type index, const basic_inplace_string& str)
{
	return insert(index, str.data(), str.size());
}

template <std::size_t N, typename CharT, typename Traits>
basic_inplace_string<N, CharT, Traits>&
basic_inplace_string<N, CharT, Traits>::insert(size_type index, const basic_inplace_string& str, size_type index_str, size_type count)
{
	if (index_str > str.size())
		detail::throw_helper<std::out_of_range>("basic_inplace_string::insert: out of range");

	basic_inplace_string subs = str.substr(index_str, count);
	return insert(index, subs.data(), subs.size());
}

template <std::size_t N, typename CharT, typename Traits>
typename basic_inplace_string<N, CharT, Traits>::iterator
basic_inplace_string<N, CharT, Traits>::insert(const_iterator pos, value_type ch)
{
	const size_type index = pos - _data.data();
	insert(index, 1, ch);
	return _data.data() + index;
}

template <std::size_t N, typename CharT, typename Traits>
typename basic_inplace_string<N, CharT, Traits>::iterator
basic_inplace_string<N, CharT, Traits>::insert(const_iterator pos, size_type count, value_type ch)
{
	const size_type index = pos - _data.data();
	insert(index, count, ch);
	return _data.data() + index;
}

template <std::size_t N, typename CharT, typename Traits>
template <typename InputIt>
typename basic_inplace_string<N, CharT, Traits>::iterator
basic_inplace_string<N, CharT, Traits>::insert(const_iterator pos, InputIt first, InputIt last)
{
	return insert(pos, first, last, typename std::conditional<detail::is_exactly_input_iterator<InputIt>::value,
											detail::is_exactly_input_iterator_tag,
											detail::is_input_iterator_tag>::type{});
}

template <std::size_t N, typename CharT, typename Traits>
template <typename InputIt>
typename basic_inplace_string<N, CharT, Traits>::iterator
basic_inplace_string<N, CharT, Traits>::insert(const_iterator pos, InputIt first, InputIt last, detail::is_exactly_input_iterator_tag)
{
	assert(pos >= _data.data() && pos <= _data.data() + size());

	const size_type sz = size();
	const size_type index = pos - _data.data();
	size_type count = 0;
	for (; first != last; ++first, ++count)
	{
		traits_type::move(&_data[index + count + 1], &_data[index + count], sz - index + count);
		traits_type::assign(_data[index + count], *first);
	}

	const size_type new_size = sz + count;
	traits_type::assign(_data[new_size], value_type{});
	set_size(new_size);

	return _data.data() + index;
}

template <std::size_t N, typename CharT, typename Traits>
template <typename InputIt>
typename basic_inplace_string<N, CharT, Traits>::iterator
basic_inplace_string<N, CharT, Traits>::insert(const_iterator pos, InputIt first, InputIt last, detail::is_input_iterator_tag)
{
	assert(pos >= _data.data() && pos <= _data.data() + size());

	const size_type sz = size();
	const size_type index = pos - _data.data();
	const size_type count = std::distance(first, last);

	traits_type::move(&_data[index + count], &_data[index], sz - index);
	for (size_type i = 0; i < count; ++i, ++first)
		traits_type::assign(_data[index + i], *first);

	const size_type new_size = sz + count;
	traits_type::assign(_data[new_size], value_type{});
	set_size(new_size);

	return _data.data() + index;
}

template <std::size_t N, typename CharT, typename Traits>
typename basic_inplace_string<N, CharT, Traits>::iterator
basic_inplace_string<N, CharT, Traits>::insert(const_iterator pos, std::initializer_list<CharT> ilist)
{
	assert(pos >= _data.data() && pos <= _data.data() + size());

	const size_type index = pos - _data.data();
	insert(index, ilist.begin(), ilist.size());
	return _data.data() + index;
}

template <std::size_t N, typename CharT, typename Traits>
basic_inplace_string<N, CharT, Traits>&
basic_inplace_string<N, CharT, Traits>::insert(size_type pos, std::experimental::basic_string_view<CharT, Traits> view)
{
	return insert(pos, view.data(), view.size());
}

template <std::size_t N, typename CharT, typename Traits>
template <typename T, typename X>
basic_inplace_string<N, CharT, Traits>&
basic_inplace_string<N, CharT, Traits>::insert(size_type pos, const T& t, size_type index_str, size_type count)
{
	std::experimental::basic_string_view<CharT, Traits> view = t;

	if (index_str > view.size())
		detail::throw_helper<std::out_of_range>("basic_inplace_string::insert: out of range");

	return insert(pos, view.data(), index_str, std::min(count, view.size() - index_str));
}

template <std::size_t N, typename CharT, typename Traits>
basic_inplace_string<N, CharT, Traits>&
basic_inplace_string<N, CharT, Traits>::erase(size_type index, size_type count)
{
	size_type sz = size();
	count = std::min(sz - index, count);

	if (index > sz)
		detail::throw_helper<std::out_of_range>("basic_inplace_string::erase: out of range");

	traits_type::move(_data.data() + index, _data.data() + index + count, sz - index - count);

	sz -= count;
	traits_type::assign(_data[sz], value_type{});

	set_size(sz);
	return *this;
}

template <std::size_t N, typename CharT, typename Traits>
typename basic_inplace_string<N, CharT, Traits>::iterator
basic_inplace_string<N, CharT, Traits>::erase(const_iterator position)
{
	size_type index = static_cast<size_type>(position - _data.data());
	erase(index, 1);
	return iterator{_data.data() + index};
}

template <std::size_t N, typename CharT, typename Traits>
typename basic_inplace_string<N, CharT, Traits>::iterator
basic_inplace_string<N, CharT, Traits>::erase(const_iterator first, const_iterator last)
{
	size_type index = static_cast<size_type>(first - _data.data());
	erase(index, std::distance(first, last));
	return iterator{_data.data() + index};
}

template <std::size_t N, typename CharT, typename Traits>
basic_inplace_string<N, CharT, Traits>&
basic_inplace_string<N, CharT, Traits>::append(size_type count, value_type ch)
{
	const size_type sz = size();
	if (sz + count > max_size())
		detail::throw_helper<std::length_error>("basic_inplace_string::append: exceed maximum string length");

	traits_type::assign(_data.data() + sz, count, ch);

	const size_type new_size = sz + count;
	traits_type::assign(_data[new_size], value_type{});
	set_size(new_size);

	return *this;
}

template <std::size_t N, typename CharT, typename Traits>
basic_inplace_string<N, CharT, Traits>&
basic_inplace_string<N, CharT, Traits>::append(const std::basic_string<CharT, Traits>& str)
{
	return append(str.data(), str.size());
}

template <std::size_t N, typename CharT, typename Traits>
basic_inplace_string<N, CharT, Traits>&
basic_inplace_string<N, CharT, Traits>::append(const std::basic_string<CharT, Traits>& str, size_type pos, size_type count)
{
	return append(str.data() + pos, std::min(str.size() - pos, count));
}

template <std::size_t N, typename CharT, typename Traits>
basic_inplace_string<N, CharT, Traits>&
basic_inplace_string<N, CharT, Traits>::append(const value_type* str, size_type count)
{
	const size_type sz = size();
	if (sz + count > max_size())
		detail::throw_helper<std::length_error>("basic_inplace_string::append: exceed maximum string length");

	for (size_type i = 0; i != count; ++i)
		traits_type::assign(_data[sz + i], str[i]);

	const size_type new_size = sz + count;
	traits_type::assign(_data[new_size], value_type{});
	set_size(new_size);

	return *this;
}

template <std::size_t N, typename CharT, typename Traits>
basic_inplace_string<N, CharT, Traits>&
basic_inplace_string<N, CharT, Traits>::append(const value_type* str)
{
	size_type sz = traits_type::length(str);
	return append(str, sz);
}

template <std::size_t N, typename CharT, typename Traits>
template <typename InputIt>
basic_inplace_string<N, CharT, Traits>&
basic_inplace_string<N, CharT, Traits>::append(InputIt first, InputIt last)
{
	// TODO exact fwd it stuff
	const size_type sz = size();
	const size_type count = std::distance(first, last);

	if (sz + count > max_size())
		detail::throw_helper<std::length_error>("basic_inplace_string::append: exceed maximum string length");

	const pointer p = _data.data() + sz;

	for (auto it = first; it != last; ++it, ++p)
		traits_type::assign(*p, *it);
	traits_type::assign(*p, value_type{});

	set_size(sz + count);
	return *this;
}

template <std::size_t N, typename CharT, typename Traits>
basic_inplace_string<N, CharT, Traits>&
basic_inplace_string<N, CharT, Traits>::append(std::initializer_list<value_type> ilist)
{
	return append(ilist.begin(), ilist.size());
}

template <std::size_t N, typename CharT, typename Traits>
basic_inplace_string<N, CharT, Traits>&
basic_inplace_string<N, CharT, Traits>::append(const std::experimental::basic_string_view<CharT, Traits>& view)
{
	return append(view.data(), view.size());
}

template <std::size_t N, typename CharT, typename Traits>
template <typename T, typename X>
basic_inplace_string<N, CharT, Traits>&
basic_inplace_string<N, CharT, Traits>::append(const T& t, size_type pos, size_type count)
{
	std::experimental::basic_string_view<CharT, Traits> view = t;
	return append(view.data() + pos, std::min(view.size() - pos, count));
}

template <std::size_t N, typename CharT, typename Traits>
int basic_inplace_string<N, CharT, Traits>::compare(const basic_inplace_string& str) const noexcept
{
	return compare(0, size(), str.data(), str.size());
}

template <std::size_t N, typename CharT, typename Traits>
int basic_inplace_string<N, CharT, Traits>::compare(size_type pos1, size_type count1, const basic_inplace_string& str) const
{
	return compare(pos1, count1, str.data(), str.size());
}

template <std::size_t N, typename CharT, typename Traits>
int basic_inplace_string<N, CharT, Traits>::compare(size_type pos1, size_type count1, const basic_inplace_string& str, size_type pos2, size_type count2) const
{
	return compare(pos1, count1, str.data() + pos2, std::min(size() - pos2, count2));
}

template <std::size_t N, typename CharT, typename Traits>
int basic_inplace_string<N, CharT, Traits>::compare(const value_type* str) const
{
	return compare(0, size(), str, traits_type::length(str));
}

template <std::size_t N, typename CharT, typename Traits>
int basic_inplace_string<N, CharT, Traits>::compare(size_type pos1, size_type count1, const value_type* str) const
{
	return compare(pos1, count1, str, traits_type::length(str));
}

template <std::size_t N, typename CharT, typename Traits>
int basic_inplace_string<N, CharT, Traits>::compare(size_type pos1, size_type count1, const value_type* str, size_type count2) const
{
	const size_type sz = std::min(count1, count2);
	const int cmp = traits_type::compare(data() + pos1, str, sz);
	if (cmp != 0)
		return cmp;
	return count1 > count2 ? 1 : (count1 == count2 ? 0 : -1);
}

template <std::size_t N, typename CharT, typename Traits>
int basic_inplace_string<N, CharT, Traits>::compare(std::experimental::basic_string_view<CharT, Traits> sv) const noexcept
{
	return compare(0, size(), sv.data(), sv.size());
}

template <std::size_t N, typename CharT, typename Traits>
int basic_inplace_string<N, CharT, Traits>::compare(size_type pos1, size_type count1, std::experimental::basic_string_view<CharT, Traits> sv) const
{
	return compare(pos1, count1, sv.data(), sv.size());
}

template <std::size_t N, typename CharT, typename Traits>
template <typename T, typename X>
int basic_inplace_string<N, CharT, Traits>::compare(size_type pos1, size_type count1, const T& t, size_type pos2, size_type count2) const
{
	std::experimental::basic_string_view<CharT, Traits> view = t;

	if (pos2  > view.size())
		detail::throw_helper<std::out_of_range>("basic_inplace_string::replace: out of range");

	return compare(pos1, count1, view.data() + pos2, std::min(view.size() - pos2, count2));
}

template <std::size_t N, typename CharT, typename Traits>
basic_inplace_string<N, CharT, Traits>&
basic_inplace_string<N, CharT, Traits>::replace(size_type pos, size_type count, const basic_inplace_string& str)
{
	return replace(pos, count, str.c_str(), str.size());
}

template <std::size_t N, typename CharT, typename Traits>
basic_inplace_string<N, CharT, Traits>&
basic_inplace_string<N, CharT, Traits>::replace(const_iterator first, const_iterator last, const basic_inplace_string& str)
{
	return replace(first - _data.data(), std::distance(first, last), str.c_str(), str.size());
}

template <std::size_t N, typename CharT, typename Traits>
basic_inplace_string<N, CharT, Traits>&
basic_inplace_string<N, CharT, Traits>::replace(size_type pos, size_type count, const basic_inplace_string& str, size_type pos2, size_type count2)
{
	if (pos2  > str.size())
		detail::throw_helper<std::out_of_range>("basic_inplace_string::replace: out of range");

	return replace(pos, count, str.c_str() + pos2, std::min(str.size() - pos2, count2));
}

template <std::size_t N, typename CharT, typename Traits>
template <class InputIt>
basic_inplace_string<N, CharT, Traits>&
basic_inplace_string<N, CharT, Traits>::replace(const_iterator first, const_iterator last, InputIt first2, InputIt last2)
{
	return replace(first, last, first2, last2, typename std::conditional<detail::is_exactly_input_iterator<InputIt>::value,
													detail::is_exactly_input_iterator_tag,
													detail::is_input_iterator_tag>::type{});
}

template <std::size_t N, typename CharT, typename Traits>
template <class InputIt>
basic_inplace_string<N, CharT, Traits>&
basic_inplace_string<N, CharT, Traits>::replace(const_iterator first, const_iterator last, InputIt first2, InputIt last2, detail::is_exactly_input_iterator_tag)
{
	const size_type pos1 = first - _data.data();
	const size_type count1 =  std::distance(first, last);
	const size_type sz = size();

	if (pos1 > sz)
		detail::throw_helper<std::out_of_range>("basic_inplace_string::replace: out of range");

	size_type count2 = 0;
	for (; first2 != last2; ++first2, ++count2)
	{
		if (count2 >= count1)
			traits_type::move(_data.data() + pos1 + count2 + 1, _data.data() + pos1 + count2, sz - pos1 - count1);

		traits_type::assign(_data[pos1 + count2], *first2);
	}

	const difference_type new_bytes = count2 - count1;
	const size_type new_size = sz + new_bytes;

	traits_type::assign(_data[new_size], value_type{});
	set_size(new_size);

	return *this;
}

template <std::size_t N, typename CharT, typename Traits>
template <class InputIt>
basic_inplace_string<N, CharT, Traits>&
basic_inplace_string<N, CharT, Traits>::replace(const_iterator first, const_iterator last, InputIt first2, InputIt last2, detail::is_input_iterator_tag)
{
	const size_type pos1 = first - _data.data();
	const size_type count1 =  std::distance(first, last);

	const size_type count2 = std::distance(first2, last2);
	const difference_type new_bytes = count2 - count1;

	const size_type sz = size();
	const size_type new_size = sz + new_bytes;

	if (pos1 > sz)
		detail::throw_helper<std::out_of_range>("basic_inplace_string::replace: out of range");

	if (new_size > max_size())
		detail::throw_helper<std::length_error>("basic_inplace_string::replace: exceed maximum string length");

	traits_type::move(_data.data() + pos1 + count2, _data.data() + pos1 + count1, sz - pos1 - count1);

	pointer p = _data.data() + pos1;
	for (auto it = first2; it != last2; ++it, ++p)
		traits_type::assign(*p, *it);

	traits_type::assign(_data[new_size], value_type{});
	set_size(new_size);

	return *this;
}

template <std::size_t N, typename CharT, typename Traits>
basic_inplace_string<N, CharT, Traits>&
basic_inplace_string<N, CharT, Traits>::replace(size_type pos1, size_type count1, const CharT* str, size_type count2)
{
	const size_type sz = size();
	const difference_type new_bytes = count2 - count1;
	const size_type new_size = sz + new_bytes;

	if (pos1 > sz)
		detail::throw_helper<std::out_of_range>("basic_inplace_string::replace: out of range");

	if (new_size > max_size())
		detail::throw_helper<std::length_error>("basic_inplace_string::replace: exceed maximum string length");

	traits_type::move(_data.data() + pos1 + count2, _data.data() + pos1 + count1, sz - pos1);

	for (size_type i = 0; i != count2; ++i)
		traits_type::assign(_data[pos1 + i], str[i]);

	traits_type::assign(_data[new_size], value_type{});
	set_size(new_size);

	return *this;
}

template <std::size_t N, typename CharT, typename Traits>
basic_inplace_string<N, CharT, Traits>&
basic_inplace_string<N, CharT, Traits>::replace(const_iterator first, const_iterator last, const CharT* str, size_type count2)
{
	return replace(first - _data.data(), std::distance(first, last), str, count2);
}

template <std::size_t N, typename CharT, typename Traits>
basic_inplace_string<N, CharT, Traits>&
basic_inplace_string<N, CharT, Traits>::replace(size_type pos, size_type count, const CharT* str)
{
	return replace(pos, count, str, traits_type::length(str));
}

template <std::size_t N, typename CharT, typename Traits>
basic_inplace_string<N, CharT, Traits>&
basic_inplace_string<N, CharT, Traits>::replace(const_iterator first, const_iterator last, const CharT* str)
{
	return replace(first - _data.data(), std::distance(first, last), str, traits_type::length(str));
}

template <std::size_t N, typename CharT, typename Traits>
basic_inplace_string<N, CharT, Traits>&
basic_inplace_string<N, CharT, Traits>::replace(size_type pos1, size_type count1, size_type count2, value_type ch)
{
	const size_type sz = size();
	const difference_type new_bytes = count2 - count1;
	const size_type new_size = sz + new_bytes;

	if (pos1 > sz)
		detail::throw_helper<std::out_of_range>("basic_inplace_string::replace: out of range");

	if (new_size > max_size())
		detail::throw_helper<std::length_error>("basic_inplace_string::replace: exceed maximum string length");

	traits_type::move(_data.data() + pos1 + count2, _data.data() + pos1 + count1, size() - pos1 - count1);
	traits_type::assign(_data.data() + pos1, count2, ch);

	traits_type::assign(_data[new_size], value_type{});
	set_size(new_size);

	return *this;
}

template <std::size_t N, typename CharT, typename Traits>
basic_inplace_string<N, CharT, Traits>&
basic_inplace_string<N, CharT, Traits>::replace(const_iterator first, const_iterator last, size_type count2, value_type ch)
{
	return replace(first - _data.data(), std::distance(first, last), count2, ch);
}

template <std::size_t N, typename CharT, typename Traits>
basic_inplace_string<N, CharT, Traits>&
basic_inplace_string<N, CharT, Traits>::replace(const_iterator first, const_iterator last, std::initializer_list<value_type> ilist)
{
	return replace(first - _data.data(), std::distance(first, last), ilist.begin(), ilist.size());
}

template <std::size_t N, typename CharT, typename Traits>
basic_inplace_string<N, CharT, Traits>&
basic_inplace_string<N, CharT, Traits>::replace(size_type pos, size_type count, std::experimental::basic_string_view<CharT, Traits> sv)
{
	return replace(pos, count, sv.data(), sv.size());
}

template <std::size_t N, typename CharT, typename Traits>
basic_inplace_string<N, CharT, Traits>&
basic_inplace_string<N, CharT, Traits>::replace(const_iterator first, const_iterator last, std::experimental::basic_string_view<CharT, Traits> sv)
{
	return replace(first - _data.data(), std::distance(first, last), sv.data(), sv.size());
}

template <std::size_t N, typename CharT, typename Traits>
template <typename T, typename X>
basic_inplace_string<N, CharT, Traits>&
basic_inplace_string<N, CharT, Traits>::replace(size_type pos, size_type count, const T& t, size_type pos2, size_type count2)
{
	std::experimental::basic_string_view<CharT, Traits> view = t;

	if (pos2 > view.size())
		detail::throw_helper<std::out_of_range>("basic_inplace_string::replace: out of range");

	return replace(pos, count, view.data() + pos2, std::min(view.size() - pos2, count2));
}

template <std::size_t N, typename CharT, typename Traits>
basic_inplace_string<N, CharT, Traits>
basic_inplace_string<N, CharT, Traits>::substr(size_type pos, size_type count) const
{
	if (pos > size())
		detail::throw_helper<std::out_of_range>("basic_inplace_string::substr: out of range");

	return {data() + pos, std::min(count, size() - pos)};
}

template <std::size_t N, typename CharT, typename Traits>
typename basic_inplace_string<N, CharT, Traits>::size_type
basic_inplace_string<N, CharT, Traits>::find(const basic_inplace_string& other, size_type pos) const noexcept
{
	return find(other.data(), pos, other.size());
}

template <std::size_t N, typename CharT, typename Traits>
typename basic_inplace_string<N, CharT, Traits>::size_type
basic_inplace_string<N, CharT, Traits>::find(const value_type* str, size_type pos, size_type count) const noexcept
{
	if (pos >= size() || count == 0)
		return npos;

	const value_type* res = detail::search_substring<CharT, Traits>(cbegin() + pos, cend(), str, str + count);
	return res ? res - cbegin() : npos;
}

template <std::size_t N, typename CharT, typename Traits>
typename basic_inplace_string<N, CharT, Traits>::size_type
basic_inplace_string<N, CharT, Traits>::find(const value_type* str, size_type pos) const noexcept
{
	return find(str, pos, traits_type::length(str));
}

template <std::size_t N, typename CharT, typename Traits>
typename basic_inplace_string<N, CharT, Traits>::size_type
basic_inplace_string<N, CharT, Traits>::find(value_type ch, size_type pos) const noexcept
{
	const value_type* res = traits_type::find(data() + pos, size() - pos, ch);
	return res ? res - cbegin() : npos;
}

template <std::size_t N, typename CharT, typename Traits>
typename basic_inplace_string<N, CharT, Traits>::size_type
basic_inplace_string<N, CharT, Traits>::find(std::experimental::basic_string_view<CharT, Traits> sv, size_type pos) const noexcept
{
	return find(sv.data(), pos, sv.size());
}

template <std::size_t N, typename CharT, typename Traits>
typename basic_inplace_string<N, CharT, Traits>::size_type
basic_inplace_string<N, CharT, Traits>::copy(value_type* dest, size_type count, size_type pos) const
{
	if (pos > size())
		detail::throw_helper<std::out_of_range>("basic_inplace_string::copy: out of range");

	size_type i = pos;
	for (; i != pos + std::min(size() - pos, count); ++i, ++dest)
		traits_type::assign(*dest, _data[i]);

	return i - pos;
}

template <std::size_t N, typename CharT, typename Traits>
void basic_inplace_string<N, CharT, Traits>::resize(size_type sz)
{
	resize(sz, value_type{});
}

template <std::size_t N, typename CharT, typename Traits>
void basic_inplace_string<N, CharT, Traits>::resize(size_type new_size, value_type ch)
{
	if (new_size > max_size())
		detail::throw_helper<std::length_error>("basic_inplace_string::resize: exceed maximum string length");

	const size_type sz = size();
	const difference_type count = new_size - sz;
	if (count > 0)
		traits_type::assign(std::begin(_data) + size(), count, ch);

	set_size(new_size);
	_data[new_size] = value_type{};
}

template <std::size_t N, typename CharT, typename Traits>
void basic_inplace_string<N, CharT, Traits>::swap(basic_inplace_string& other) noexcept
{
	basic_inplace_string s(other);
	other = *this;
	*this = s;
}

template <std::size_t N, typename CharT, typename Traits>
inline std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const basic_inplace_string<N, CharT, Traits>& str)
{
	return os.write(str.data(), str.size());
}

template <std::size_t N, std::size_t M, typename CharT, typename Traits>
inline bool operator==(const basic_inplace_string<N, CharT, Traits>& lhs,
					   const basic_inplace_string<M, CharT, Traits>& rhs)
{
	return lhs.size() == rhs.size() && Traits::compare(lhs.data(), rhs.data(), lhs.size()) == 0;
}

template <std::size_t N, typename CharT, typename Traits>
inline bool operator==(const basic_inplace_string<N, CharT, Traits>& lhs,
					   const CharT* rhs)
{
	assert(rhs != nullptr);
	return lhs.size() == Traits::length(rhs) && Traits::compare(lhs.data(), rhs, lhs.size()) == 0;
}

template <std::size_t N, typename CharT, typename Traits>
inline bool operator==(const CharT* lhs,
					   const basic_inplace_string<N, CharT, Traits>& rhs)
{
	return rhs == lhs;
}

template <std::size_t N, typename CharT, typename Traits>
inline bool operator==(const basic_inplace_string<N, CharT, Traits>& lhs,
					   std::experimental::string_view rhs)
{
	return lhs.size() == rhs.size() && Traits::compare(lhs.data(), rhs.data(), lhs.size()) == 0;
}

template <std::size_t N, typename CharT, typename Traits>
inline bool operator==(std::experimental::string_view lhs,
					   const basic_inplace_string<N, CharT, Traits>& rhs)
{
	return rhs == lhs;
}

template <std::size_t N, std::size_t M, typename CharT, typename Traits>
inline bool operator!=(const basic_inplace_string<N, CharT, Traits>& lhs,
					   const basic_inplace_string<M, CharT, Traits>& rhs)
{
	return !(lhs == rhs);
}

template <std::size_t N, typename CharT, typename Traits>
inline bool operator!=(const basic_inplace_string<N, CharT, Traits>& lhs,
					   const CharT* rhs)
{
	assert(rhs != nullptr);
	return lhs.size() != Traits::length(rhs) || Traits::compare(lhs.data(), rhs, lhs.size()) != 0;
}

template <std::size_t N, typename CharT, typename Traits>
inline bool operator!=(const CharT* lhs,
					   const basic_inplace_string<N, CharT, Traits>& rhs)
{
	return rhs != lhs;
}

template <std::size_t N, typename CharT, typename Traits>
inline bool operator!=(const basic_inplace_string<N, CharT, Traits>& lhs,
					   std::experimental::string_view rhs)
{
	return !(lhs == rhs);
}

template <std::size_t N, typename CharT, typename Traits>
inline bool operator!=(std::experimental::string_view lhs,
					   const basic_inplace_string<N, CharT, Traits>& rhs)
{
	return rhs != lhs;
}

template <std::size_t N, std::size_t M, typename CharT, typename Traits>
inline bool operator<(const basic_inplace_string<N, CharT, Traits>& lhs,
					  const basic_inplace_string<M, CharT, Traits>& rhs)
{
	return lhs.compare(rhs) < 0;
}

template <std::size_t N, typename CharT, typename Traits>
inline bool operator<(const basic_inplace_string<N, CharT, Traits>& lhs,
					  const CharT* rhs)
{
	return lhs.compare(rhs) < 0;
}

template <std::size_t N, typename CharT, typename Traits>
inline bool operator<(const CharT* lhs,
					  const basic_inplace_string<N, CharT, Traits>& rhs)
{
	return rhs.compare(lhs) > 0;
}

template <std::size_t N, typename CharT, typename Traits>
inline bool operator<(const basic_inplace_string<N, CharT, Traits>& lhs,
					  std::experimental::string_view rhs)
{
	return lhs.compare(rhs) < 0;
}

template <std::size_t N, typename CharT, typename Traits>
inline bool operator<(std::experimental::string_view lhs,
					  const basic_inplace_string<N, CharT, Traits>& rhs)
{
	return rhs.compare(lhs) > 0;
}

template <std::size_t N, std::size_t M, typename CharT, typename Traits>
inline bool operator>(const basic_inplace_string<N, CharT, Traits>& lhs,
					  const basic_inplace_string<M, CharT, Traits>& rhs)
{
	return rhs < lhs;
}

template <std::size_t N, typename CharT, typename Traits>
inline bool operator>(const basic_inplace_string<N, CharT, Traits>& lhs,
					  const CharT* rhs)
{
	return rhs < lhs;
}

template <std::size_t N, typename CharT, typename Traits>
inline bool operator>(const CharT* lhs,
					  const basic_inplace_string<N, CharT, Traits>& rhs)
{
	return rhs < lhs;
}

template <std::size_t N, typename CharT, typename Traits>
inline bool operator>(const basic_inplace_string<N, CharT, Traits>& lhs,
					  std::experimental::string_view rhs)
{
	return rhs < lhs;
}

template <std::size_t N, typename CharT, typename Traits>
inline bool operator>(std::experimental::string_view lhs,
					  const basic_inplace_string<N, CharT, Traits>& rhs)
{
	return rhs < lhs;
}

template <std::size_t N, std::size_t M, typename CharT, typename Traits>
inline bool operator<=(const basic_inplace_string<N, CharT, Traits>& lhs,
					   const basic_inplace_string<M, CharT, Traits>& rhs)
{
	return !(rhs < lhs);
}

template <std::size_t N, typename CharT, typename Traits>
inline bool operator<=(const basic_inplace_string<N, CharT, Traits>& lhs,
					   const CharT* rhs)
{
	return !(rhs < lhs);
}

template <std::size_t N, typename CharT, typename Traits>
inline bool operator<=(const CharT* lhs,
					   const basic_inplace_string<N, CharT, Traits>& rhs)
{
	return !(rhs < lhs);
}

template <std::size_t N, typename CharT, typename Traits>
inline bool operator<=(const basic_inplace_string<N, CharT, Traits>& lhs,
					   std::experimental::string_view rhs)
{
	return !(rhs < lhs);
}

template <std::size_t N, typename CharT, typename Traits>
inline bool operator<=(std::experimental::string_view lhs,
					   const basic_inplace_string<N, CharT, Traits>& rhs)
{
	return !(rhs < lhs);
}

template <std::size_t N, std::size_t M, typename CharT, typename Traits>
inline bool operator>=(const basic_inplace_string<N, CharT, Traits>& lhs,
					   const basic_inplace_string<M, CharT, Traits>& rhs)
{
	return !(lhs < rhs);
}

template <std::size_t N, typename CharT, typename Traits>
inline bool operator>=(const basic_inplace_string<N, CharT, Traits>& lhs,
					   const CharT* rhs)
{
	return !(lhs < rhs);
}

template <std::size_t N, typename CharT, typename Traits>
inline bool operator>=(const CharT* lhs,
					   const basic_inplace_string<N, CharT, Traits>& rhs)
{
	return !(lhs < rhs);
}

template <std::size_t N, typename CharT, typename Traits>
inline bool operator>=(const basic_inplace_string<N, CharT, Traits>& lhs,
					   std::experimental::string_view rhs)
{
	return !(lhs < rhs);
}

template <std::size_t N, typename CharT, typename Traits>
inline bool operator>=(std::experimental::string_view lhs,
					   const basic_inplace_string<N, CharT, Traits>& rhs)
{
	return !(lhs < rhs);
}

namespace detail
{

template <typename CharT, typename Traits>
inline const CharT* search_substring(const CharT* first1, const CharT* last1, const CharT* first2, const CharT* last2)
{
	const std::size_t size2 = last2 - first2;

	while (true)
	{
		const std::size_t size1 = last1 - first1;
		if (size1 < size2)
			return nullptr;

		first1 = Traits::find(first1, size1, *first2);
		if (first1 == nullptr)
			return nullptr;

		if (Traits::compare(first1, first2, size2) == 0)
			return first1;

		++first1;
	}

	return nullptr;
}

}

template <std::size_t N> using inplace_string = basic_inplace_string<N, char>;
template <std::size_t N> using inplace_wstring = basic_inplace_string<N, wchar_t>;
template <std::size_t N> using inplace_u16string = basic_inplace_string<N, char16_t>;
template <std::size_t N> using inplace_u32string = basic_inplace_string<N, char32_t>;

namespace std
{

template <std::size_t N, typename CharT, typename Traits>
struct hash<basic_inplace_string<N, CharT, Traits>>
{
	size_t operator()(const basic_inplace_string<N, CharT, Traits>& str) const
	{
		using view = typename std::experimental::basic_string_view<CharT, Traits>;

		const view v(str.data(), str.size());
		return std::hash<view>()(v);
	}
};

}


