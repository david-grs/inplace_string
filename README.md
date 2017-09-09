inplace_string<T, CharT, Traits>
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
inplace_string<T, CharT, Traits> implements C++17's std::string interface, plus:
  * `max_size()` and `capacity()` are `constexpr`
  * `inplace_string` can be constructed from `const CharT(&)[M])`, allowing a compile-time error it the input exceeds the maximum capacity

It is compatible with GCC >= 5.
