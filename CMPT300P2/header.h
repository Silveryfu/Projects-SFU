#ifndef HEADER_H
#define HEADER_H
#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <queue>
#include <list>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include "monitor.h"

using namespace std;

#define PROC_EXIT -1
#define PROC_BLOCK 0
#define PROC_RUN 1

int const LEVEL=3;
int const SLAVES_NUMBER = 3;
int const TIME_UNIT = 10;
int const MAX_PROCESS_NUMBER = 16;
int const IO_WAIT_TIME = 3;
double const CREATE_PROC_FREQUENCY = 10;

#endif
