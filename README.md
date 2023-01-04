# hex

Performance comparison of dec-to-hex implementations

- C++11
  - `std::snprintf`
  - `std::stringstream` with `std::hex`
  - `fmt::format`
- C++17
  - `std::to_chars`
- C++20
  - `std::format`
- Custom hex functions
  - `Naive` : 4-bit shift then calculate ASCII value
  - `LUT1` : 4-bit shift with a 16-bit lookup table
  - `LUT2` : 16-bit shift with a 16-bit lookup table
  - `LUT3` : 16-bit shift with a 512-bit lookup table
  - `SWAR` : use SIMD Within A Register (SWAR) techniques

## Build

```sh
# Use a workflow preset to configure/build/test at once
cmake --workflow --preset windows
```

## Benchmark Results

### MSVC

![msvc](benchmark/result/msvc.png)

### Clang

[Quick-bench](https://quick-bench.com/q/k4TcmGTNlKF58qYx-KPXktUJMUw)

![clang](benchmark/result/clang.png)

### GCC

[Quick-bench](https://quick-bench.com/q/bh0ya3v_6iaQ-id1ODezBcURS0w)

![gcc](benchmark/result/gcc.png)

## References

- `std::to_chars` implementations
  - [MSVC](https://github.com/microsoft/STL/blob/8ddf4da23939b5c65587ed05f783ff39b8801e0f/stl/inc/charconv#L116-L121)
  - [Clang](https://github.com/llvm/llvm-project/blob/b20dd2b186fdc76828219b705a2b58f5830f4b9d/libcxx/include/charconv#L443-L466)
  - [GCC](https://github.com/gcc-mirror/gcc/blob/d423e8dc59045d8f281dcb1a02559cb6f91456bf/libstdc%2B%2B-v3/include/std/charconv#L169-L213)
- [Conversion numbers to hexadecimal representation](http://0x80.pl/articles/convert-to-hex.html)
