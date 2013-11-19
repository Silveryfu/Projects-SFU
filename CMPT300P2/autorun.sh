#!/bin/bash
g++ -c *.cpp
g++ -o test *.o -lpthread
./test

