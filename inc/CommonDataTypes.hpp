/****************************************************************************/
/**
 ** @file   CommonDataTypes.hpp
 ** @brief  Define my basic data types.
 ** @author Johan Hendrik Ehlers <https://github.com/johanhendrikehlers>
 ** 
 **/

#ifndef __COMMONDATATYPES_HPP__
#define __COMMONDATATYPES_HPP__

/** 64-bit (or more) signed floating point number. */
typedef double SFloat64;

/** 32-bit signed floating point number. */
typedef float SFloat32;

/** 32-bit (or more) signed integer. */
typedef signed int SInt;

/** 32-bit (or more) unsigned integer. */
typedef unsigned int UInt;

/** 32-bit signed integer. */
typedef signed int SInt32;

/** 32-bit unsigned integer. */
typedef unsigned int UInt32;

/** 16-bit signed integer. */
typedef signed short SInt16;

/** 16-bit unsigned integer. */
typedef unsigned short UInt16;

/** 8-bit signed integer. */
typedef signed char SInt8;

/** 8-bit unsigned integer. */
typedef unsigned char UInt8;

/** A boolean. */
typedef bool Bool;

/** A character. */
typedef char Char;

/** W character. */
typedef wchar_t WChar;

/****************************************************************************/

#ifdef SMPLOFSIZE_U32

/** Sample Type **/
typedef UInt32 Smpl;
/** Sample Max Size **/
#define SMPL_MAX ( 4294967295)
/** Sample Min Size **/
#define SMPL_MIN (          0)
/** Sample Size **/
#define SMPL_SIZEOF (4)
/** Sample Signed **/
#define SMPL_SIGNED (FALSE)

#elif defined SMPLOFSIZE_S32

/** Sample Type **/
typedef SInt32 Smpl;
/** Sample Max Size **/
#define SMPL_MAX ( 2147483647)
/** Sample Min Size **/
#define SMPL_MIN (-2147483648)
/** Sample Size **/
#define SMPL_SIZEOF (4)
/** Sample Signed **/
#define SMPL_SIGNED (TRUE)

#elif defined SMPLOFSIZE_U16

/** Sample Type **/
typedef UInt16 Smpl;
/** Sample Max Size **/
#define SMPL_MAX (      65535)
/** Sample Min Size **/
#define SMPL_MIN (          0)
/** Sample Size **/
#define SMPL_SIZEOF (2)
/** Sample Signed **/
#define SMPL_SIGNED (FALSE)

#elif defined SMPLOFSIZE_S16

/** Sample Type **/
typedef SInt16 Smpl;
/** Sample Max Size **/
#define SMPL_MAX (      32767)
/** Sample Min Size **/
#define SMPL_MIN (     -32768)
/** Sample Size **/
#define SMPL_SIZEOF (2)
/** Sample Signed **/
#define SMPL_SIGNED (TRUE)

#elif defined SMPLOFSIZE_U8

/** Sample Type **/
typedef UInt8 Smpl;
/** Sample Max Size **/
#define SMPL_MAX (        255)
/** Sample Min Size **/
#define SMPL_MIN (          0)
/** Sample Size **/
#define SMPL_SIZEOF (1)
/** Sample Signed **/
#define SMPL_SIGNED (FALSE)

#elif defined SMPLOFSIZE_S8

/** Sample Type **/
typedef SInt8 Smpl;
/** Sample Max Size **/
#define SMPL_MAX (        127)
/** Sample Min Size **/
#define SMPL_MIN (       -128)
/** Sample Size **/
#define SMPL_SIZEOF (1)
/** Sample Signed **/
#define SMPL_SIGNED (TRUE)

#else

/** Sample Type **/
typedef SInt32 Smpl;
/** Sample Max Size **/
#define SMPL_MAX ( 2147483647)
/** Sample Min Size **/
#define SMPL_MIN (-2147483648)
/** Sample Size **/
#define SMPL_SIZEOF (4)
/** Sample Signed **/
#define SMPL_SIGNED (TRUE)

#endif

#endif //__COMMONDATATYPES_HPP__
