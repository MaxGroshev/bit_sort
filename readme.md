# Bit_sort
- This program make bitonic sort with computing on openCL

## Required programs:

-  Cmake version  3.21


# Install and run
```
> git clone git@github.com:MaxGroshev/bit_sort.git
> mkdir build
> cd build
> cmake ..
> make
> ./build/bit_sort

```

# Tests
Required programs:

- Python


#### How to run end to end tests?
Run this command from build project directory
```
> python ../tests/end_to_end_tests/e2e_test.py

```
---

# Test generator
Required programs:
python
```
> python3 ../tests/end_to_end_tests/test_generator.py

```
 - -c  [--count] sets count of matrixes
 - -f [--file] sets name of file for data

#### How to run debug mode?
- go to  ./CMakeLists.txt
- change CMAKE_BUILD_TYPE "Release" to "Debug"

---
