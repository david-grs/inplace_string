#pragma once

#include <array>

namespace cxx { namespace detail
{

template <std::size_t N>
struct basic_small_string
{
	bool empty() const { return _size == 0; }
	std::size_t size() const { return _size; }

private:
	std::array<char, N> _data;
	char _size;
};

} }

template <std::size_t N>
using basic_small_string = cxx::detail::basic_small_string<N>;

using small_string = basic_small_string<31>;




