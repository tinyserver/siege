/*
    Copyright (c) 2007 SIEGE Development Team
    All rights reserved.

    This file is part of libSIEGE.

    This software is copyrighted work licensed under the terms of the
    2-clause BSD license. Please consult the file "COPYING.txt" for
    details.

    If you did not recieve the file with this program, please email
    Tim Chas <darkuranium@gmail.com>.
*/
#ifndef __SIEGE_UTIL_TIME_H__
#define __SIEGE_UTIL_TIME_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "../common.h"

#define SG_NANOSECONDS_IN_A_SECOND 1000000000LL

/**
 * Function: sgGetTime
 * 
 * Returns: nanoseconds from a fixed, unspecified, origin.
 */
SGlong SG_EXPORT sgGetTime(void);

/**
 * Function: sgSleep 
 *  
 * Sleep for the given amount of nanoseconds.
 */
void SG_EXPORT sgSleep(SGlong nseconds);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __SIEGE_UTIL_TIME_H__
