﻿/*
* =====================================================================================
*
*       Filename:  utils.h
*
*    Description:  Utility typedefs and objects to make life easier
*
*        Created:  2/20/2013 8:30:14 PM
*
* =====================================================================================
*/
#ifndef UTILS_H
#define UTILS_H
#include"headers.h"
typedef enum
{
	OPPONENT_PIECE=-1,
	NO_PIECE,
	MY_PIECE,
    THREATS
} movetype;
typedef enum
{
	TIME_UP,
    WIN,
    LOSS,
	TOO_MANY
} ERRORS;
typedef std::chrono::duration<int,std::milli> ms;
typedef std::chrono::high_resolution_clock hclock;
typedef std::pair<int, int> _mv;
typedef std::pair<_mv, movetype> mv;
typedef std::vector<std::vector<movetype>> states;
#define LOGGING 0
#define LOGGING2 0
#define LOGGING3 1
#define LOGGING4 0
#define LOGGING5 1

#endif
