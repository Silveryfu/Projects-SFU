echo "Start debugging\n".
g++ -c *.cpp
g++ -o test *.o -lpthread
echo "Start running, seriously?\n"
./test

