#ifndef HEADER_H
#define HEADER_H
#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <queue>
#include <list>

#include "proc.h"
#include "monitor.h"
#include "processor.h"
#include "readyqueue.h"
#include "blockqueue.h"

using namespace std;

int const SLAVES_NUMBER = 3;
int const TIME_UNIT = 10;

#endif