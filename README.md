# AVL Binary Tree

This program creates an AVL self-balancing binary tree data structure written in C++ and runs it through a series of performance tests of randomly generated key-value pairs.

After the performance test, the terminal will display instances of:

- add
- remove
- find (or search)
- range find
- sort

in the test file and arranged by the number of calls, total time, and average time for each. 

**Note:** Must have CMake installed.

## Setup CMake in Terminal
```
cmake CMakeLists.txt
make
```

## Test
```
./avlTst
```

## Performance Test
```
./avlPerf <INSERT .TXT FILE HERE>
```

## Example
```
./avlPerf rand-10k.txt
```
