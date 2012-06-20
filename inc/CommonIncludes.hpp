/****************************************************************************/
/**
 ** @file   CommonIncludes.hpp
 ** @brief  Include all c library and other much needed headers here.
 ** @author Johan Hendrik Ehlers <https://github.com/johanhendrikehlers>
 **
 **/

#ifndef __COMMONINCLUDES_HPP__
#define __COMMONINCLUDES_HPP__

/****************************************************************************/
/* -- Specific files for desktop systems (windows 2000, linux, cygwin, ...) */
#if !defined(_WIN32_WCE) && !defined(__SYMBIAN32__) && !defined(__EPOC32__)
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <cmath>
#include <cfloat>
#include <climits>
#include <ctime>
#include <cassert>
#include <new>
#include <iostream>

/****************************************************************************/
/* -- Specific files for windows CE. */
#elif defined(_WIN32_WCE)
#include <cstring>
#include <cstdio>
#include <cmath>
#include <exception>
#include <windows.h>

/****************************************************************************/
/* -- Specific files for symbian. */
#elif defined(__SYMBIAN32__) || defined(__EPOC32__)
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <e32std.h>

/****************************************************************************/
/* -- Unrecognised system! */
#else
#error "Unrecognised system, please set up the right macros in SimpleDef.hpp!"
#endif

#endif // __COMMONINCLUDES_HPP__
