/****************************************************************************/
/**
 ** @file   Miscalanious.hpp
 ** @brief  Includes description and header files for the Miscalanious 
 **         Library.
 ** @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 **
 **/

#ifndef __MISCALANIOUS_HPP__
#define __MISCALANIOUS_HPP__

#include "MiscIncludes.hpp"
#include "MiscDataTypes.hpp"
#include "MiscDefines.hpp"
#include "MiscCmdLTool.hpp"

/**
 ** @mainpage The Miscalanious Library
 ** 
 ** @section into_sec Introduction
 **
 ** This library contains a list of classes for all those jobs that is kind of
 ** globally generic. Specific generic tasks for example that relates to image
 ** processing extends these base classes. Some common tasks include command
 ** line and option file parsing (See page \ref page1), common image
 ** processing tasks including internalising and externalising image and video
 ** files. The main design criteria is portability. I don't recommend building
 ** with missing libraries, programs etc. Intended platforms include PC systems
 ** (Windows and Linux) and MOBILE systems like Symbian, Windows CE etc.
 **
 **/

/**
 ** @page page1 Command Line & Option File Parsing
 ** @section over_sec Overview
 **
 ** The class dealing with command line parsing and option file parsing is
 ** called MiscCmdLTool. See section \ref cmdl_sec on command line argument
 ** parsing and section \ref file_sec on option file parsing.
 **
 ** @section cmdl_sec Command Line Parsing
 ** 
 ** Command line parsing deals with arguments passed to applications through a
 ** command line environment or through a batch file.
 ** 
 ** @subsection intro_cmdl_sec Introduction
 **
 ** First of all to clarify certain terminology lest look at a small program
 ** being called from the command line, the idea of this program is to load a
 ** image and then to save it into another format:
 **
 ** @code ./myprog -l my_image type=jpg -o my_image.bmp @endcode
 **
 ** Here we have a total of <b>6 arguments</b> including the application name.
 ** We can immediately break these down further into <b>3 main arguments, 2
 ** "follows"</b> and <b>1 sub arguments</b>.
 ** Where arguments:
 ** -# <b>./myprog</b> is the main application name and is for simplicity
 ** treated as a main argument.
 ** -# <b>-l</b> is telling the application to load a image file and is also a
 ** main argument.
 ** -# <b>my_image</b> is a "follow" and forms part of the -l main argument.
 ** -# <b>type=jpg</b> is a sub argument of -l that indicates the file being
 ** loaded is a jpg type. 
 ** -# <b>-o</b> is a main argument telling the application to save a file.
 ** -# <b>my_image.bmp</b> is a "follow" as it is part of the saving
 ** operation.
 ** 
 ** This example is relatively simple and you might be wondering why the
 ** split? But like I said before the context I was working in relates to
 ** image processing on different platforms etc. and allot of test were
 ** done. So for example a wavelet transform might have many sub arguments
 ** specifying regions, filters, steps etc. yet its still only a single
 ** transform. The idea is to split the arguments and hence the command line
 ** help interface for example become much less cluttered. We steal the
 ** <c>-H</c> option from CVS which if specified tells the application to
 ** print out the list of sub arguments for a specific argument. For example
 ** to have seen all the argument relating to <c>--transform</c> we would
 ** type:
 **
 ** @code ./myprog -H --transform @endcode
 **
 ** This would print out a list of allowed sub arguments for the transform
 ** operation.
 **
 ** @subsection attributes_cmdl_sec Argument Attributes
 **
 ** Arguments (main and sub) can have a <b>combination of the following</b>
 ** attributes:
 **
 ** - Leave (<b>L</b>)<br>
 ** When this arguments is encountered it is performed and all other arguments
 ** will be ignored.
 ** - Single (<b>S</b>)<br>
 ** Only single argument allowed, for example only one error output can be
 ** specified.
 ** - Required (<b>R</b>)<br>
 ** This argument is not optional and has to be specified else your hard drive
 ** will be deleted.
 ** - Flag (<b>F</b>)<br>
 ** The argument is of a flag type. Switches from on to off and vica versa.
 ** - One of the following:
 **   - Optional Follow (<b>OF</b>)<br>
 ** The next sub argument might belong to this argument. For example the
 ** popular program <c>cat</c> will use the standard in stream if no file is
 ** specified.
 **   - Required Follow (<b>RF</b>)<br>
 ** Same as above but not optional.
 **   - Optional Tail (<b>OT</b>)<br>
 ** The argument might include a tail. For example our old friend g++ has a 
 ** argument -I which uses tails as such -I../borats_travel_list.
 **   - Required Tail (<b>RT</b>)<br>
 ** Same as above but not optional.
 **   - Variable Type (<b>VT</b>)<br>
 ** Similar to "Required Tail", but we have a equal sign inbetween which will
 ** be ignored, for example <c>filter=Daub4</c>
 ** - Optionally followed by one of these if <b>OF,RF,OT,RT</b> or <b>VT</b>
 **   was used:
 **   - Integer Next (<b>IN</b>)<br>
 ** The extra data specified by the follow,tail or variable point to a integer
 ** or integer list (See MiscTextTool).
 **   - Double Next (<b>DN</b>)<br>
 ** The extra data specified by the follow,tail or variable point to a double
 ** or double list (See MiscTextTool).
 **   - String Next <b>SN</b>)<br>
 ** The extra data specified by the follow,tail or variable point to a string
 ** or string list (See MiscTextTool).
 **
 ** @todo Explain above a bit more clearer if possible.
 **
 ** @subsection mnames_cmdl_sec Main Argument Names
 **
 ** Each main argument (except of course main application name) is given a
 ** short name and a not so short but more descriptive name. Short names start
 ** with single hyphenation and longer names with double hyphenation. For
 ** example a compression argument might take the following two names:<br> 
 ** - Short name: <c> -z </c>
 ** - Long name: <c> --compress </c>
 **
 ** @subsection snames_cmdl_sec Sub Argument Names
 **
 ** Each sub argument has only one name. Sub argument names should not have
 ** hyphenation to distinguish them, but it is allowed.
 **
 ** @section file_sec Option File Parsing
 ** File Option Parsing
 **
 ** @todo Implement File Option Parsing.
 ** 
 ** @todo Write something about file option parsing.
 **
 **/

#endif  //__MISCALANIOUS_HPP__
