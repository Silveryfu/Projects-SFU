#ifndef HEADER_H
#define HEADER_H
#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>

#include "proc.h"
#include "monitor.h"
#include "processor.h"
#include "readyqueue.h"
#include "blockqueue.h"

int const NO_INTTERUPT = 1;
int const MID_TERM_TO_BQ = 2;
int const MID_TERM_BQ_TO_RQ = 3;
int const LONG_TERM_TO_RQ = 4;
int const LONG_TERM_EXIT = 5;

#endif