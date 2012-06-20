/****************************************************************************/
/**
 ** @file   IIICommon.h
 ** @brief  Contains the Intermediate Integer Image (III) defintions.
 ** @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 **
 **/

#ifndef __IIICOMMON_H__
#define __IIICOMMON_H__

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

/** Process argument - name **/
#define IIIARGNAME 0
/** Process argument - x-coordinate **/
#define IIIARGX 1
/** Process argument - y-coordinate **/
#define IIIARGY 2
/** Process argument (GET) - file name (optional) **/
#define IIIARGGETFILE 3
/** Process argument (SET) - sample value **/
#define IIIARGSETSAMPLE 3
/** Process argument (SET) - file name (optional) **/
#define IIIARGSETFILE 4
/** Get Command mode - file name has been given as a option **/
#define IIIGETCMDMODE 4
/** Get Enviroment mode - file name might have been given as a enviroment
 ** option, else read the standard input **/
#define IIIGETENVMODE 3
/** Get Command mode - file name has been given as a option **/
#define IIISETCMDMODE 5
/** Get Enviroment mode - file name might have been given as a enviroment
 ** option, else read the standard input **/
#define IIISETENVMODE 4
/** Name of enviroment variable holding the file name **/
#define IIIENVNAME "III_HOLDER"
/** The usage of the IIIGet program **/
#define IIIUSAGEGET                                                 \
  "IIIGet Version 0.1a by\n"                                        \
  "Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>\n"	    \
  "\n"                                                              \
  " usage: IIIGet <x-pos> <y-pos> [iii-file]\n"                     \
  "\n"                                                              \
  " If the iii-file is not specified the enviroment variable\n"     \
  " III_HOLDER can be used to point to a valid iii file, else\n"    \
  " standard input will be read\n"
/** The usage of the IIISet program **/
#define IIIUSAGESET                                                 \
  "IIISet Version 0.1a by\n"                                        \
  "Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>\n"           \
  "\n"                                                              \
  " usage: IIISet <x-pos> <y-pos> <new_value> [iii-file]\n"         \
  "\n"                                                              \
  " If the iii-file is not specified the enviroment variable\n"     \
  " III_HOLDER can be used to point to a valid iii file, else\n"    \
  " standard input will be read\n"
/** The III file header format **/
#define IIIHEADER                               \
  "width:     %8d\n"                            \
  "height:    %8d\n"                            \
  "color:     %8s\n"                            \
  "precision: %8d\n"                            \
  "signed:    %8s\n"
/** The III sample format out **/
#define IIISAMPLEOUT "%7d\n"
/** The III sample format in **/
#define IIISAMPLEIN "%d"
/** The III true **/
#define IIITRUE "TRUE"
/** The III false **/
#define IIIFALSE "FALSE"
/** The III header size **/
#define IIIHSIZE 100
/** The III data size **/
#define IIIDSIZE 8
/** Coordinate is out of range **/
#define IIIERRCOORD "%s coordinate (%d) is out of range [0,%d]\n"
/** Sample is out of range **/
#define IIIERRSAMPLE "sample %d is out of range [%d,%d]\n"
/** File is damaged cant read or write data **/
#define IIIERRFILESMALL "file \"%s\" might be damaged, too small!\n"
/** File is damaged cant find correct information **/
#define IIIERRFILEINFO "file \"%s\" might be damaged, missing information!\n"
/** File is damaged cant find correct information **/
#define IIIERRFILEWRITE "file \"%s\" might be damaged, write error!\n"
/** Cant open the file **/
#define IIIERRFILEOPEN "cant open file \"%s\"\n"
/** No file name given **/
#define IIIERRFILENONE "must specify a file name of a valid iii\n"
/** print more info **/
#define IIIPERROR "more error information"

#endif
