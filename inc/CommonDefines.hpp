/****************************************************************************/
/**
 ** @file   CommonDefines.hpp
 ** @brief  Common global macros are defined here.
 ** @author Johan Hendrik Ehlers <https://github.com/johanhendrikehlers>
 **
 **/

#ifndef __COMMONDEFINES_HPP__
#define __COMMONDEFINES_HPP__

#include "CommonIncludes.hpp"

/*****************************************************************************/
/** Universal macros. For all systems the same. */

#ifndef True
/** True **/
#define True 1
#endif

#ifndef False
/** False **/ 
#define False 0
#endif

#ifndef NULL
/** Null **/
#define NULL 0
#endif

/** Granularity of common vector growths (internal use) **/
#define COMMON_GRANULARITY 8

/** Undefine all macros. */
#ifdef ASSERT
#undef ASSERT
#endif
#ifdef ASSERTFALSE
#undef ASSERTFALSE
#endif
#ifdef ASSERTALWAYS
#undef ASSERTALWAYS
#endif
#ifdef ASSERTFALSEALWAYS
#undef ASSERTFALSEALWAYS
#endif
#ifdef TESTFAIL
#undef TESTFAIL
#endif
#ifdef TESTWORK
#undef TESTWORK
#endif
#ifdef WARN_IF
#undef WARN_IF
#endif

#if !defined (DEBUG) || defined (_DEBUG) 
#define DEBUG
#endif

/*****************************************************************************/
/** Macros for sytems with full stl library support **/
#if !defined(_WIN32_WCE) && !defined(__SYMBIAN32__)  && !defined(__EPOC32__)   

/** Debug & Release macros */

/** Thow exception on null pointer. */
#define LEAVEONNULL(x) if ( x == NULL ){        \
    throw std::bad_alloc();                     \
  }

/** Assert a True state in debug builds. */
#define ASSERTALWAYS(x) if ( !(x)){                 \
    std::cout << "ASSERT failed\n"                  \
              << "- file: " << __FILE__  << "\n"    \
              << "- line: " << __LINE__  << "\n";   \
    std::cin.ignore();                              \
    std::cout << "\n";}

/** Assert a False state in debug builds. */
#define ASSERTFALSEALWAYS(x) if ( (x)){             \
    std::cout << "ASSERTFALSE failed\n"             \
              << "- file: " << __FILE__  << "\n"    \
              << "- line: " << __LINE__  << "\n";   \
    std::cin.ignore();                              \
    std::cout << "\n";}

/** Debug only macros */
#if defined (DEBUG) || defined (_DEBUG) 

/** Assert a True state in debug builds. */
#define ASSERT(x) if ( !(x)){                       \
    std::cout << "ASSERT failed\n"                  \
              << "- file: " << __FILE__  << "\n"    \
              << "- line: " << __LINE__  << "\n";   \
    std::cin.ignore();                              \
    std::cout << "\n";}

/** Assert a False state in debug builds. */
#define ASSERTFALSE(x) if ( (x)){                   \
    std::cout << "ASSERTFALSE failed\n"             \
              << "- file: " << __FILE__  << "\n"    \
              << "- line: " << __LINE__  << "\n";   \
    std::cin.ignore();                              \
    std::cout << "\n";}

/** Assert a leaving|failing function. */
#define TESTFAIL(x){                                \
    bool missed = true;                             \
    try {x;}                                        \
    catch ( const char * e ){                       \
      missed = false; }                             \
    if ( missed ) {                                 \
      std::cout << "TESTFAIL failed\n"              \
                << "- file: " << __FILE__  << "\n"  \
                << "- line: " << __LINE__  << "\n"; \
      std::cin.ignore();                            \
      std::cout << "\n";}}

/** Assert a working|non-leaving function. */
#define TESTWORK(x){                                \
    bool caught = false;                            \
    try { x; }                                      \
    catch ( const char * e ){                       \
      caught = true; }                              \
    if ( caught ) {                                 \
      std::cout << "TESTWORK failed\n"              \
                << "- file: " << __FILE__  << "\n"  \
                << "- line: " << __LINE__  << "\n"; \
      std::cin.ignore();                            \
      std::cout << "\n";}}

/** Warn for certain state. */
#define WARN_IF(x) if ((x)){                        \
    std::cout << "WARNING triggered\n"              \
              << "- file: " << __FILE__  << "\n"    \
              << "- line: " << __LINE__  << "\n";   \
    std::cout << "\n";}
#else 
#define ASSERT(x) void();
#define ASSERTFALSE(x) void();       
#define TESTFAIL(x) void();                                   
#define TESTWORK(x) void();
#define WARN_IF(x) void();                            
#endif

/****************************************************************************/
/** Windows CE macros. */
#elif defined (_WIN32_WCE)

#ifdef ATOW
#error "ATOW Defined!"
#endif

#define CAST_ARRAY(ta, tt, f, l)                \
  for ( SInt i = 0 ; i < l ; ++i ){             \
    ta[i] = (tt) f[i];                          \
  }                                             \

#define LEAVEONNULL(x) if ( x == NULL ){                        \
    throw std::exception("memory allocation failed in file "    \
                         __FILE__);                             \
  }

/** Assert a True state in debug builds. */
#define ASSERTALWAYS(x) if ( !(x)){                 \
    Char str[200];                                  \
    WChar strw[200];                                \
    sprintf(str, "- file: %s\n- line: %d",__FILE__, \
            __LINE__);                              \
    CAST_ARRAY(strw,WChar,str,200);                 \
    MessageBoxW (NULL,strw,                         \
                 L"ASSERT failed!", MB_ICONSTOP);   \
  };

/** Assert a False state in debug builds. */
#define ASSERTFALSEALWAYS(x) if ( (x)){                 \
    Char str[200];                                      \
    WChar strw[200];                                    \
    sprintf(str,"- file: %s\n- line: %d",__FILE__,      \
            __LINE__);                                  \
    CAST_ARRAY(strw,WChar,str,200);                     \
    MessageBoxW (NULL,strw,                             \
                 L"ASSERTFALSE failed!", MB_ICONSTOP);  \
  };

/** Debug only macros */
#if defined (DEBUG)

#define ASSERT(x) if ( !(x)){                       \
    Char str[200];                                  \
    WChar strw[200];                                \
    sprintf(str,"- file: %s\n- line: %d",__FILE__,  \
            __LINE__);                              \
    CAST_ARRAY(strw, WChar, str, 200);              \
    MessageBoxW (NULL,strw,                         \
                 L"ASSERT failed!", MB_ICONSTOP);   \
  };

#define ASSERTFALSE(x) if ((x)){                        \
    Char str[200];                                      \
    WChar strw[200];                                    \
    sprintf(str,"- file: %s\n- line: %d",__FILE__,      \
            __LINE__);                                  \
    CAST_ARRAY(strw,WChar,str,200);                     \
    MessageBoxW (NULL,strw,                             \
                 L"ASSERTFALSE failed!", MB_ICONSTOP);  \
  };

#define TESTFAIL(x){                                    \
    bool missed = true;                                 \
    try {x;}                                            \
    catch ( const char * e ){                           \
      missed = false; }                                 \
    if ( missed ) {                                     \
      Char str[200];                                    \
      WChar strw[200];                                  \
      sprintf(str,"- file: %s\n- line: %d",__FILE__,    \
              __LINE__);                                \
      CAST_ARRAY(strw,WChar,str,200);                   \
      MessageBoxW (NULL,strw,                           \
                   L"TESTFAIL failed!", MB_ICONSTOP);   \
    }                                                   \
  };

#define TESTWORK(x){                                    \
    bool worked = true;                                 \
    try {x;}                                            \
    catch ( const char * e ){                           \
      worked = false; }                                 \
    if ( !worked ) {                                    \
      Char str[200];                                    \
      WChar strw[200];                                  \
      sprintf(str,"- file: %s\n- line: %d",__FILE__,    \
              __LINE__);                                \
      CAST_ARRAY(strw,WChar,str,200);                   \
      MessageBoxW (NULL,strw,                           \
                   L"TESTWORK failed!", MB_ICONSTOP);   \
    }                                                   \
  };

#define WARN_IF(x)if ( !(x)){                               \
    Char str[200];                                          \
    WChar strw[200];                                        \
    sprintf(str,"- file: %s\n- line: %d",__FILE__,          \
            __LINE__);                                      \
    CAST_ARRAY(strw,WChar,str,200);                         \
    MessageBoxW (NULL,strw,                                 \
                 L"WARNING TRIGGERED!", MB_ICONWARNING);    \
  };

#else // Release builds

#define ASSERT(x) void();
#define ASSERTFALSE(x) void();
#define TESTFAIL(x) void();
#define TESTWORK(x) void();
#define WARN_IF(x) void();

#endif

/****************************************************************************/
/** Symbian macros. */
#elif defined (__SYMBIAN32__) || (__EPOC32__) || (__VC32__)

#ifndef abs
/** @return Absolute value of integer.
 ** @param i The integer to get absolute value off. */
inline int abs(int i){ return i >= 0 ? i : -i;};
#endif

#define LEAVEONNULL(x) if ( x == NULL ){        \
    User::LeaveNoMemory();                      \
  }

#define ASSERT(x) void();
#define ASSERTALWAYS(x) void();
#define ASSERTFALSE(x) void();
#define TESTFAIL(x) void();                                   
#define TESTWORK(x) void();
#define WARN_IF(x) void();

#endif

#if defined(WIN32) || defined(_WIN32_WCE) || (__VC32__)
/** Override the normal for to overcome scope issue. **/
# define for if(0);else for
#endif

/****************************************************************************/
/** Common Macros. */

/** Return a array of a certain type, if out of memory LEAVEONNULL is called,
 ** else the array is set to zero. The variable name must pre defined. 
 ** 
 ** @param a_arr Array name. 
 ** @param a_tpe Array type.
 ** @param a_len Array length.
 **/
#define NEW_ARRAY(a_arr, a_tpe, a_len)                  \
  a_arr = new a_tpe[a_len];                             \
  LEAVEONNULL(a_arr);                                   \
  memset((Char*)a_arr, '\0', sizeof(a_tpe)*(a_len));

/** Delete an array and set the pointer to NULL.
 **
 ** @param a_arr The array name.
 **/
#define DEL_ARRAY(a_arr)                        \
  delete [] a_arr;                              \
  a_arr = NULL;

#endif // __COMMONDEFINES_HPP__
