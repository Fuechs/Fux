#ifndef UTILITY_FUX_H
#define UTILITY_FUX_H

/* 
Copyright (c) 2020-2022 Fuechs
All rights reserved.
*/

void clear();
int error(int id, std::string message);
void debug(std::string message, bool debug);
void marked_log(std::string message, bool newline = true);

#endif
