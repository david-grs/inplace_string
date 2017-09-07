inplace_string
==============
A C++17 compliant version of basic_inplace_string<N, CharT, Traits>


Example
-------
```
using Name = inplace_string<5>; // 5 characters + NULL-byte
Name name = "foo"; // can be used as a std::string

name += "bar"; // throws std::length_error

name = "foobar"; // compiler error
```


