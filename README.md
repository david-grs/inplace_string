[![Build Status](https://travis-ci.org/david-grs/inplace_string.svg?branch=master)](https://travis-ci.org/david-grs/inplace_string)
[![Build status](https://ci.appveyor.com/api/projects/status/tjkd7gsfp7vcg4nx/branch/master?svg=true)](https://ci.appveyor.com/project/david-grs/inplace-string/branch/master)
[![Coverage Status](https://coveralls.io/repos/github/david-grs/inplace_string/badge.svg?branch=master)](https://coveralls.io/github/david-grs/inplace_string?branch=master)

inplace_string<N, CharT, Traits>
================================
An alternative to std::string with inplace storage. STL compliant, can be used as a full replacement of C++17's std::string.


Example
-------
```
{
  using Name = inplace_string<15>; // 16 bytes on stack, size included
  Name name = "foo"; 

  auto it = name.find("r");
  assert(it == Name::npos);

  name += "bar";
  
  std::string str(name); // implicit string_view construction

  ....
}

{
  inplace_string<5> too_small;
  too_small = "foobar"; // compiler error
}
```



Compatibility
-------------
inplace_string<N, CharT, Traits> implements C++17's std::string interface, plus:
  * `max_size()` and `capacity()` are `constexpr`
  * `inplace_string` can be constructed from `const CharT(&)[M])`, allowing a compile-time error it the input exceeds the maximum capacity

Supports Clang >= 3.4, GCC >= 5, VS >= 2017
