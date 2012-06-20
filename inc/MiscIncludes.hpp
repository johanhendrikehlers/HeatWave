/****************************************************************************/
/**
 ** @file   MiscIncludes.hpp
 ** @brief  Include all c library and other much needed headers here.
 ** @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 **
 **/

#ifndef __MISCINCLUDES_HPP__
#define __MISCINCLUDES_HPP__

/* -- Specific files for desktop systems (windows 2000 & linux). */
#if !defined(_WIN32_WCE) && !defined(__SYMBIAN32__) && !defined(__EPOC32__)
#include <CommonHeaders.hpp>
#include <HeatWave.hpp>
#include <jasper/jasper.h>
#if !defined(WIN32)
// need to get mhash to work on Windows
#include <mhash.h>
#endif
/* Note below is used for Huffman compressor in SimpleHuff */
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

/* -- Specific files for windows CE. */
#elif defined(_WIN32_WCE)
#include <CommonHeaders.hpp>
#include <HeatWave.hpp>

/* -- Specific files for symbian. */
#elif defined(__SYMBIAN32__) || defined(__EPOC32__)
#include <CommonHeaders.hpp>
#include <HeatWave.hpp>

/* -- Unrecognised system! */
#else
#error "Unrecognised system!"
#endif

#endif // __MISCINCLUDES_HPP__
