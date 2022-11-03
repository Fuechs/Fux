/**
 * @file time.cpp
 * @author fuechs
 * @brief fux time 
 * @version 0.1
 * @date 2022-10-31
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#include "time.hpp"
#include <time.h>
#include <chrono>

struct tm tstruct;
int Clock::__os_time(int ty) {
    time_t now = time(0);
    tstruct = *localtime(&now);
    switch (ty) {
        case 1: return tstruct.tm_min;
        case 2: return tstruct.tm_hour;
        case 3: return tstruct.tm_mday;
        case 4: return tstruct.tm_mon;
        case 5: return tstruct.tm_year;
        case 6: return tstruct.tm_wday;
        case 7: return tstruct.tm_yday;
        case 8: return tstruct.tm_isdst;
        default: return tstruct.tm_sec; // case 0 included
    }
}

// the total amount of time in nano seconds since 01/01/1970 (UTC)
unsigned long long Clock::realTimeInSecs() {
    return std::chrono::duration_cast<std::chrono::nanoseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count();
}