# AVL Binary Tree

This program creates an AVL self-balancing binary tree data structure written in C++ and runs it through a series of performance tests of randomly generated key-value pairs.

An initial test (avl_test.cpp) can be called to check if the AVL tree has been implemented correctly.

After the performance test, the terminal will display instances of:

- add
- remove
- find (or search)
- range find
- sort

in the test file and arranged by the number of calls, total time, and average time for each. 
The height of the tree will also be displayed at the end.



**Note:** Must have CMake installed.

## Setup CMake in Terminal
```
cmake CMakeLists.txt
make
```

## Test/Check if AVL Tree works
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
