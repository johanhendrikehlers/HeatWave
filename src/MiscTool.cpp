/***************************************************************************/
/**
 *
 * @file   MiscTool.cpp
 * @brief  A command line client for the Miscalanious Tool Library.
 * @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 * 
 **/

#include "MiscTool.hpp"

/****************************************************************************/
/* Apllication Details                                                      */

const Char * glob_names[] = 
  {"MiscTool",
   NULL};

const Char * glob_versions[] = 
  {"0.1a",
   NULL};

const Char * glob_history[] = 
  {"- 2 November 2006\n"
   "MiscTool is born, the world goes crazy!",
   "- 3 November 2006\n"
   "New Borat Movie this weekend!",
   NULL};

const Char * glob_authors[] = 
  {"Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>",
   NULL};

const Char * glob_copyright[] =
  {"(c) 2006 all rights reserved including those that allow me to have a "
   "beer during lunch break.",
   NULL};

const Char * glob_date = __DATE__;
const Char * glob_time = __TIME__;

const SInt glob_minArguments = 2;
const Char * glob_minNotice = "type -h for further help\n";

/****************************************************************************/
/** 
 *
 * The main application function
 * @param argc Number of command line arguments.
 * @param argv Pointers to each command line argument. 
 * @return 0 on success, 1 to 255 on errors.
 * 
 **/

SInt
AppMain(const SInt argc, const Char ** argv)
{
  MiscTool Tool(glob_names,glob_versions,glob_history,glob_authors,
                glob_copyright,glob_date,glob_time,glob_minArguments,
                glob_minNotice);
  Tool.DoArguments(argc,argv);
  return 0;
}

/****************************************************************************/
/** 
 ** The process entry. 
 ** @param argc Number of command line arguments.
 ** @param argv Pointers to each command line argument. 
 ** @return 0 on success, 1 to 255 on errors.
 **
 **/

#include <iostream>
using namespace std;

int /* only place to use int because g++ under cygwin complains */
main (const SInt argc, const Char ** argv)
{
  try
    {
      return AppMain(argc, argv);
    }
  catch (const exception e)
    {
      cerr << "Exit : Very Bad!\n"
           << "Standard exeption caught: " << e.what() << endl;
      return 2;
    }
  catch (...)
    {
      cerr << "Exit : Extremely Bad!\n"
           << "Unknown exeption caught!" << endl;
      return 3;
    }
  return 0;
}

