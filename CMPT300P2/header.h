#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>

int const NO_INTTERUPT = 1;
int const MID_TERM_TO_BQ = 2;
int const MID_TERM_BQ_TO_RQ = 3;
int const LONG_TERM_TO_RQ = 4;
int const LONG_TERM_EXIT = 5;
