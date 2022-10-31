/**
 * @file time.hpp
 * @author fuechs
 * @brief fux time header
 * @version 0.1
 * @date 2022-10-31
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#pragma once

class Clock {
public:
    static unsigned long long realTimeInSecs();
    static int __os_time(int ty);
};

#define NANO_TOMICRO(x) ((x) / 1000L)
#define NANO_TOMILL(x) (NANO_TOMICRO(x) / 1000L)
#define NANO_TOSEC(x) (NANO_TOMILL(x) / 1000L)