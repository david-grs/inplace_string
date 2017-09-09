inplace_string<T, CharT, Traits>
================================
A std::string's alternative with inplace storage, C++17 compliant.


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


