/****************************************************************************/
/**
 ** @file   MiscCmdLTool.hpp
 ** @brief  Contains the class MiscCmdLTool.
 ** @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 **
 **/

#ifndef __MISCCMDLTOOL_HPP__
#define __MISCCMDLTOOL_HPP__

#include "MiscIncludes.hpp"
#include "MiscDefines.hpp"
#include "MiscTextTool.hpp"

/****************************************************************************/
/**
 **
 ** A Stucture for defining and holding each option and if available its sub
 ** options. This structure should be created by each main option or
 ** requirement handler. To safely access the follows or tails of arguments,
 ** use MiscArgInfo::GetStr.
 **
 ** @todo write a usefull "how to" on how to use this struct safely.
 **
 **/
  
struct MiscArgInfo
{  
  /**
   * 
   * Constructor. 
   *
   * @param argc Count of different sub argument types. 
   *
   **/
  
  MiscArgInfo(SInt argc)
  {
    memset((Char*)this, '\0', sizeof(MiscArgInfo));
    if ( argc <= 0 ){
      return;
    }
    SInt gran = ((argc/VEC_GROWTH)*VEC_GROWTH)+VEC_GROWTH;
    NEW_ARRAY(subName, const Char*, gran);
    NEW_ARRAY(subDesc, const Char*, gran);
    NEW_ARRAY(subFlag, SInt, gran);
    NEW_ARRAY(subStr, const Char**, gran);
    NEW_ARRAY(subStrDes, const Char*, gran);
    NEW_ARRAY(subStrDef, const Char*, gran);
    NEW_ARRAY(subStrNum, SInt, gran);
    subNum = argc;
  }
  
  /**
   * 
   * Destructor.
   * 
   **/
  
  ~MiscArgInfo()
  {
    delete [] str;
    delete [] subName;
    delete [] subDesc;
    delete [] subFlag;
    for ( SInt i = 0 ; i < subNum ; ++i ){
      delete subStr[i];
    }
    delete [] subStr;
    delete [] subStrDes;
    delete [] subStrDef;
    delete [] subStrNum;
  }

  /**
   *
   * Function for accessing argument or requirement follow(s) or tail, in a
   * save manner.
   *
   * @param argn Argument or requirment number, for the main argument use a
   * negative number, which is the default.
   * @param foln The follow or tail number. 0 be default. 
   * @return The pointer if it exists, else NULL.
   *
   **/

  const Char * GetStr(SInt argn = -1, SInt foln = 0)
  {
    ASSERT( foln >= 0);
    if ( argn < 0 ){
      if ( str ){
        if (foln < strNum){
          return str[foln];
        }
      }
    }
    else{
      ASSERT( argn < subNum );
      if ( subStr[argn] ){
        if ( foln < subStrNum[argn] ){
          return subStr[argn][foln];
        }
      }
    }
    return NULL;
  }

  /**
   *
   * Function for accessing argument or requirement follow(s) or tail, in a
   * save manner.
   *
   * @param argn Argument or requirment number, for the main argument use a
   * negative number, which is the default.
   * @param foln The follow or tail number. 0 be default. 
   * @return The pointer if it exists, else a pointer to a empty string.
   *
   **/

  const Char * GetEmptyStr(SInt argn = -1, SInt foln = 0)
  {
    const Char * ret = GetStr(argn, foln);
    if ( !ret ){
      ret = "";
    }
    return ret;
  }

  /**
   *
   * Get a combined flag, by ORing each of the sub flags.
   *
   * @return A combined flag.
   *
   **/

  SInt GetCombinedFlag()
  {
    SInt ret_flag = flag;
    for (SInt i = 0 ; i < subNum ; ++i){
      ret_flag |= subFlag[i];
    }
    return ret_flag;
  }

  /** Short name, normal to start with single hyphen, for example "-z". **/
  const Char * singleName;
  
  /** Long name, normal to start with double hyphen, for example "--compress" 
   **/
  const Char * doubleName;
  
  /** Short description, used with "-h" or "--help" handler. **/
  const Char * description;
  
  /** Long description, used with "-H" or "--Help" handler. **/
  const Char * descriptionLong;

  /** Holder for flag(s), describing this option or requirement depending on
   ** flag settings. **/
  SInt flag;

  /**************************************************************************/
  
  /** (Use post MiscCmdLTool::DoMinorDuty function) Pointer to the follow(s)
   ** or tail, where the count is stored in strNum. **/
  const Char ** str;

  /** The default of the follow or tail if optional. **/
  const Char * strDef;

  /** Short description of the follow or tail used in help etc. **/
  const Char * strDes;

  /** (Use post MiscCmdLTool::DoMinorDuty function) The total number of
   ** follows or if a tail was specified. **/
  SInt strNum;

  /**************************************************************************/

  /** Sub option name(s). **/
  const Char ** subName;
    
  /** Sub option verbose description(s). **/
  const Char ** subDesc;

  /** Flags describing the sub option(s) . **/
  SInt * subFlag;

  /**************************************************************************/

  /** (Use post MiscCmdLTool::DoMinorDuty function) Pointer to a list of
   ** lists for each sub option holding the follow(s) or tail, where the count
   ** for each is stored in subStrNum array. **/
  const Char *** subStr;

  /** Short description sub option follow or tail's, used in help etc. **/
  const Char ** subStrDes;

  /** The default of the follow, tail or variable argument if optional. **/
  const Char ** subStrDef;
  
  /** The total number of follows for each sub option. **/
  SInt * subStrNum;

  /**************************************************************************/
  
  /** The total number of SubArg's. **/
  SInt subNum;
};

/****************************************************************************/

/**
 *
 * Enumeration of activitys expected from each of the argument handling
 * functions. Notice all dutys exept Dty_Perform is a minor dutys that can
 * be handled by DoMinorDuty function.
 *
 **/

enum EnumFunctionDuty {
  
  /** Print minimum info (Single line) **/
  Dty_MinInfo,
  
  /** Print maximum info (Many lines) **/
  Dty_MaxInfo,
    
  /** Recognize the correct number of arguments or return a error **/
  Dty_Recognize,

  /** Recognise only main argument. **/
  Dty_RecognizeMain,

  /** Recognise that argument name is unique **/
  Dty_RecognizeUnique,

  /** Return main argument flags. **/
  Dty_ReturnFlag,
    
  /** Perform the requested operation. **/
  Dty_Perform
};
  
/**
 *
 * Argument Attributes
 *
 **/

enum p_EnumOptionType{

  /** Leave **/
  Att_L = 1,

  /** Single **/
  Att_S = 2,
    
  /** Required **/
  Att_R = 4,
    
  /** Flag **/
  Att_F = 8,

  /** Optional Follow **/
  Att_FO = 16,
    
  /** Required Follow **/
  Att_FR = 32,
    
  /** Optional Tail **/
  Att_TO = 64,

  /** Required Tail **/
  Att_TR = 128,

  /** Boolean Next **/
  Att_BN = 256,

  /** Integer Next **/
  Att_IN = 512,

  /** Double Next **/
  Att_DN = 1024,
    
  /** String Next **/
  Att_SN = 2048,
    
  /** Usefull for returning the state true or set from functions. **/
  Att_Set = 4096,

  /** Multiple arguments allowed - only usefull when required or optional
   ** "follow" flag is set **/
  Att_Multi = 8192,

  /** Usefull for setting to a error state. **/
  Att_Error = 0xffffffff
};

/****************************************************************************/

/****************************************************************************/
/**
 ** Command Line Base Class.
 ** 
 ** From a programmes or maintainers point of view, the first goal of the
 ** class is to take the hard work out of maintaining the user interfaces and
 ** help interfaces, at the cost of a few hertz (come on if you cant afford a
 ** few cycles here or there for the sake of being home earlier you should try
 ** getting a life). For example if by mistake later you add a function which
 ** has same long option ("--long") as another it will complain in the first
 ** run.  Before you feel bit dissapointed these "extra" tests are only in
 ** debug builds.
 **
 ** The second goal is to catch any user mistakes as early as possible as some
 ** operations can take quite long. We dont want the user to wait for an
 ** operation if there is a clear error later in his request. So we check
 ** errors and exit if one is found.
 **
 ** From the command line point of view this class will process the commands
 ** from left to right. Internally the application will construct a array of
 ** images starting at the offset 0. Then any function should in theory
 ** operate only on the last image unless specificly told otherwise.
 **
 ** The general -h help command will not print out the suboptions for any
 ** command. The command -H should be used to print out more information.
 **
 **/

class MiscCmdLTool
{
public:
  
  /**
   *
   * Parameterized Constuctor.
   *
   * @param nams History of application names.
   * @param vers History of application versions.
   * @param hist History of application modifications.
   * @param auth History of application authors.
   * @param copy History of copyright notices.
   * @param date The date of program compilation.
   * @param time The time of program compilation.
   * @param aMin Minimum arguments to be specified else print default notice.
   * @param aNte Default notice to be printed if minimum arguments not given.
   * @param cols Character width of output terminal or file device.
   * @param vrbs Verbose mode, false by default.
   * @param stdI Standard input file, if null stdin is used.
   * @param stdO Standard output file, if null stdout is used.
   * @param stdE Standard error file, if null stderr is used.    
   * @note The arrays should all end with a NULL pointer. Data in arrays are
   * expected to start with oldest information or least important ending with
   * most recent additions or most important information.
   *
   **/
  
  MiscCmdLTool(const Char ** nams, const Char ** vers,
               const Char ** hist, const Char ** auth,
               const Char ** copy,
               const Char * date, const Char * time,
               SInt aMin, const Char * aNte, 
               SInt cols = 78, Bool verb = False, 
               const Char * stdI = NULL, const Char * stdO = NULL,
               const Char * stdE = NULL);
  
  /**
   *
   * Destructor.
   *
   **/
  
  virtual ~MiscCmdLTool();

  /**************************************************************************/
  
  /**
   *
   * @return The array of current and past application names.
   *
   **/

  const Char ** GetNames() const;

  /**
   *
   * Set the array of current and past application names.
   *
   * @param nams The array of current and past application names.
   *
   **/

  void SetNames(const Char ** nams);

  /**
   *
   * @return The array of current and past application versions.
   *
   **/

  const Char ** GetVersions() const;

  /**
   *
   * Set the array of current and past application versions.
   *
   * @param vers The array of current and past application versions.
   *
   **/

  void SetVersions(const Char ** vers);

  /**
   *
   * @return The array of past development events.
   *
   **/

  const Char ** GetHistory() const;

  /**
   *
   * Set the values of past development events.
   *
   * @param hist The values of past development events.
   *
   **/

  void SetHistory(const Char ** hist);

  /**
   *
   * @return The array of current and past author names.
   *
   **/

  const Char ** GetAuthors() const;

  /**
   *
   * Set the values of current and past authors.
   *
   * @param auth The values of current and past authors.
   *
   **/
  
  void SetAuthors(const Char ** auth);

  /**
   *
   * @return The array of past and current copyright notices.
   *
   **/

  const Char ** GetCopyrights() const;

  /**
   *
   * Set the values of past and current copyright notices.
   *
   * @param copy The values of past and current copyright notices.
   *
   **/

  void SetCopyNames(const Char ** copy);

  /**
   *
   * @return The date of compilation.
   *
   **/

  const Char * GetDateCompiled() const;

  /**
   *
   * Set the date of compilation.
   *
   * @param date The date of compilation.
   *
   **/

  void SetDateCompiled(const Char * date);

  /**
   *
   * @return The time of compilation.
   *
   **/

  const Char * GetTimeCompiled() const;

  /**
   *
   * Set the time of compilation.
   *
   * @param time The time of compilation.
   *
   **/

  void SetTimeCompiled(const Char * time);

  /**
   *
   * @return The minimum number of arguments required.
   *
   **/

  SInt GetMinimumArg() const;

  /**
   *
   * Set the minimum number of arguments required.
   *
   * @param mini The minimum number of arguments required.
   *
   **/

  void SetMinimumArg(SInt mini);

  /**
   *
   * @return The notice to display if less than minimum arguments given.
   *
   **/

  const Char * GetMinimumArgNote() const;

  /**
   *
   * Set the notice to display if less than minimum arguments given.
   *
   * @param note The notice to display if less than minimum arguments given.
   *
   **/

  void SetMinimumArgNote(const Char * note);

  /**
   *
   * @return The maximum column width used for output.
   *
   **/

  SInt GetColumns() const;

  /**
   *
   * Set the maximum column width used for output.
   *
   * @param wdth The maximum column width used for output.
   * @note If 0 or less 78 is used.
   *
   **/

  void SetColumns(SInt wdth);

  /**
   *
   * @return The verbose mode.
   *
   **/

  Bool GetVerboseMode() const;

  /**
   *
   * Set the verbose mode.
   *
   * @param mode The verbose mode to set to.
   *
   **/

  void SetVerboseFlag(Bool mode);

  /**************************************************************************/

  /**
   *
   * @return The name of the file used for standard input.
   *
   **/
  
  const Char * GetStdInput() const;

  /**
   *
   * Open a file to be used for standard input.
   *
   * @param name The name of the file to be used for standard input.
   * @param exit Exit process on error. (True by default)
   * @param mode If null file will be opened as text file in read mode, "rt".
   * @return True if opened succesfull, false otherwise. (Only if exit is 
   * false!)
   *
   **/

  Bool SetStdInput(const Char * name, Bool exit = True, 
                   const Char * mode = NULL);

  /**
   *
   * @return The name of the file used for standard output.
   *
   **/

  const Char * GetStdOutput() const;

  /**
   *
   * Open a file to be used for standard output.
   *
   * @param name The name of the file to be used for standard output.
   * @param exit Exit process on error. (True by default)
   * @param mode If null file will be opened as text file in write mode, "at".
   * @return True if opened succesfull, false otherwise. (Only if exit is 
   * false!)
   *
   **/
  
  Bool SetStdOutput(const Char * name, Bool exit = True, 
                    const Char * mode = NULL);

  /**
   *
   * @return The name of the file used for error output.
   *
   **/

  const Char * GetErrOutput() const;
  
  /**
   *
   * Open a file to be used for error output.
   *
   * @param name The name of the file to be used for error output.
   * @param exit Exit process on error. (True by default)
   * @param mode If null file will be opened as text file in write mode, "at".
   * @return True if opened succesfull, false otherwise. (Only if exit is 
   * false!)
   *
   **/

  Bool SetErrOutput(const Char * name, Bool exit = True, 
                    const Char * mode = NULL);

  /**************************************************************************/

  /**
   *
   * Handle all the command line arguments.
   *
   * @param argc The argument array lenght.
   * @param argv The argument array values.
   * @return True if all arguments was handled succesfully, false otherwise.
   *
   **/
  
  Bool DoArguments(SInt ArgC, const Char ** ArgV);

  /**************************************************************************/
  
private:
  
  /**
   *
   * Default Constuctor. (Disabled for forseeble future)
   *
   **/

  MiscCmdLTool();

  /**
   *
   * Copy constructor. (Disabled for forseeble future)
   * 
   * @param oth The other MiscCmdLTool object to copy.
   *
   **/
  
  MiscCmdLTool(const MiscCmdLTool & oth);

  /**
   *
   * @return A clone of this object. (Disabled for forseeble future)
   *
   **/

  MiscCmdLTool * GetClone() const;

  /**
   *
   * Assignment operator. (Disabled for forseeble future)
   *
   * @param rhs The other component to copy data and state from.
   * @return A reference to this object.
   *
   **/
  
  MiscCmdLTool & operator=(const MiscCmdLTool & oth);

  /**
   *
   * Equality comparison. (Disabled for forseeble future)
   *
   * @param rhs The other component to compare against.
   * @return True if equal, False if un-equal.
   *
   **/
  
  Bool operator==(const MiscCmdLTool & oth) const;

  /**************************************************************************/
  
protected:

  /**************************************************************************/

  /**
   *
   * Error types. Relates to MiscCmdLTool::p_EnumOptionType.
   *
   **/

  enum p_EnumErrorType{
    /** No error or unrecognized command line option. **/
    Err_None = 0,
     
    /** No error or unrecognized command line option. **/
    Err_Unrecognized = 0,
    
    /** Missing command line option, follow or tail, as a result of one or
     ** more of following flag(s) being set "Required", "Required Follow" or
     ** "Required Tail". **/
    Err_Missing = -1,

    /** Duplicate command line option found, but "Single" flag is set.  **/
    Err_Duplicate = -2,

    /** Option with "Leave" flag encounterd. (Not really a error but handled
     ** this way). **/
    Err_Leave = -3,
    
    /** All other. (.I.E out of bounds request etc.) **/
    Err_Other = -4
  };
  
  /**
   *
   * Analyse sub option and compleet the MiscArgInfo struct with information.
   *
   * @param info (IN&OUT) The information regarding the specific argument that
   * will be used to analyse then filled.
   * @param argc The number of command line arguments relating to this sub 
   * option.
   * @param argv The arguments.
   *
   **/

  void DoArgInfoAnalysis(MiscArgInfo & info, SInt argc, const Char ** argv);

  /** 
   *
   * Argument handlers (typedef) 
   *
   **/
  
  typedef SInt (MiscCmdLTool::*m_argFunction)(EnumFunctionDuty duty, 
                                              SInt argc, const Char ** argv);
  /**
   *
   * Argument registration function.
   *
   * @param fn New function for handling a main argument.
   * @return True if registerd succesfully false other wise. 
   *
   **/
  
  Bool DoArgumentRegistration(m_argFunction fn);
  
  /*@{*/
  /**
   *
   * Base main argument handling functions. 
   *
   * @param duty The reason for the call.
   * @param argc The number of arguments.
   * @param argv The value of arguments.
   * @return Negative value on error else result requested.
   * 
   **/
  
  SInt DoMainArgHelp (EnumFunctionDuty duty, int argc, const Char ** argv);
  SInt DoMainArgVerbose (EnumFunctionDuty duty, int argc, const Char ** argv);
  SInt DoMainArgSubInfo (EnumFunctionDuty duty, SInt argc, const Char ** argv);
  SInt DoMainArgVersion (EnumFunctionDuty duty, SInt argc, const Char ** argv);
  /*@}*/

  /**
   *
   * Open a file to use as standard input, output or error.
   *
   * @param open [IN&OUT] The file to open. If not null a close will be 
   * attempted first.
   * @param name The file name to open if null use default file.
   * @param mode The mode to open file with.
   * @param dflt The default file to use as input or output.
   * @param exit Abort process on error, true by default.
   * @return True on succes else False.
   *
   **/

  Bool DoOpenIOFile(FILE *& open, const Char * name, const Char * mode,
                    FILE * dflt, Bool exit = True);

  /**
   *
   * @return The number of main argument handlers registered.
   *
   **/
  
  SInt GetTotalMainHandlers() const;

  /**
   *
   * Regster all the arugment handlers.
   *
   **/
  
  void DoGroupRegistration();

  /**
   *
   * Clean up.
   *
   **/

  void DoCleanUp();

  /**
   *
   * Fase A of the DoArguments() function. First we recognizes all arguments
   * (including any sub options), then we construct a flag list for each
   * handlers, indicating how it should be treated.  Note however if a
   * "leave" flag is set, the argument is executed immediatly, through a call
   * to the function DoArgumentsB, and this always returns False (even not
   * strictly a error it was decided to handle it this way). Two lists are
   * returned; one with function pointers (note they are in order as given at
   * command line; left to right), and a second with how many arguments each
   * will use. If all succeeds True is returned.
   *
   * @param argc The argument array lenght.
   * @param argv The argument array values.
   * @param flgs [OUT] Flag for each handler.
   * @param func [OUT] Pointers to functions that shall be called with order.
   * @param acnt [OUT] Argument count for each function handler.
   * @return True on succes, false otherwise.
   *
   * @note Arguments flgs, func and argc should be cleared up by calling
   * function, flgs will be equal in size to m_handlersTotal, func and args
   * will be equal in size to argc+1 always ending with a NULL. Arguments
   * argc and argv should not include application name.
   *
   **/
  
  Bool DoArgumentsA(SInt argc, const Char ** argv, SInt *& flgs,
                    m_argFunction *& func, SInt *& acnt);
  
  /**
   *
   * Fase B of the DoArguments() function. Here we execute requested tasks in
   * there correct order. On error return False else True.
   *
   * @param argv The argument(s) and sub-arguments(s).
   * @param func Pointers to handling functions (note in order to be called).
   * @param acnt Argument count for each function handler.
   * @return True on success, false otherwise.
   *
   **/
   
  Bool DoArgumentsB(const Char ** argv, m_argFunction * func, SInt * acnt);
  
  /**
   *
   * Print a header for application.
   *
   * @msge Optionaly print a message below if given.
   *
   **/

  void DoPrintHeader(const Char * msge = NULL);


  /*@{*/
  /**
   * 
   * Print related functions.
   * 
   * @param info The MiscArgInfo struct to print.
   * @return True on success else False.
   *
   **/
  
  Bool DoArgMinInfoPrint(const MiscArgInfo & info);
  Bool DoArgMaxInfoPrint(const MiscArgInfo & info);
  /*@}*/

  /**
   *
   * String format a single argument according to its flags.
   *
   * @param name The single argument name.
   * @param flag The flags of this argument.
   * @param folo The type of follow else flag is used.
   * @return The string containing the formatted string.
   *
   **/

  Char * CrtArg(const Char * name, const SInt flag, const Char * folo);

  /**
   *
   * Print a single option (using both its short name and long name) to the
   * standard output.
   *
   * @param sngl The short name for the option to print. (Cannot be NULL)
   * @param dobl The long name for the option to print. (Can be NULL)
   * @param flag The flag of the particular option.
   * @param type The type of the follow or tail. (Can be NULL)
   * @param note The note to print with the option.
   *
   **/
   
  void PrtArg (const Char * sngl, const Char * dobl, const SInt flag,
               const Char * type, const Char * note);

  /**
   *
   * Process a single main or sub argument.
   *
   * @param name The name of argument.
   * @param flag [OUT] The flag holder of the argument.
   * @param lptr [OUT] Pointer to follow(s), tail or variable.
   * @param lcnt [OUT] Count of follow(s), or if tail or variable was specified.
   * @param argc Count or arguments specified.
   * @param arbv Values of arguments specified.
   * @param info Reference to current MiscArgInfo being processed.
   * 
   **/
  
  SInt DoArgSingleRecognition(const Char * name, SInt & flag, 
                              const Char **& lptr, SInt & lcnt, 
                              SInt argc, const Char ** argv,
                              MiscArgInfo & info);
   
  /**
   *
   * Process a single main argument with all its sub arguments.
   *
   * @param info Struct to explain main argument with sub argument.
   * @param argc Count or arguments specified.
   * @param arbv Values of arguments specified.
   * 
   **/
  
  SInt DoArgInfoRecognition(MiscArgInfo & info, SInt argc, const Char ** argv);

  /**
   *
   * Check if all required arguments is specified and if argument was
   * not specified point to defaults.
   *
   * @param flag The flag of the arguments.
   * @param dflt The default if the arguments was not specified.
   * @param Return True if all required variables has been met, False
   * otherwise.
   * 
   **/
  
  Bool ChkArgInfo(MiscArgInfo & info);
  
  /** 
   *
   * Function to handle all "minor" dutys, these include Dty_MinInfo,
   * Dty_MaxInfo, Dty_Recognize, Dty_RecognizeMain, Dty_RecognizeUnique, and
   * Dty_ReturnFlag but does not include Dty_Perform.
   *
   * @param duty The duty to carry out.
   * @param info Struct to explain argument and sub argument(s).
   * @param argc Count or arguments specified.
   * @param arbv Values of arguments specified.
   * 
   **/
  
  SInt DoMinorDuty(EnumFunctionDuty duty, MiscArgInfo & info, 
                   SInt argc, const Char ** argv);

  /** 
   *
   * Determine if argument is a sub argmunet or not.
   *
   * @param name Name to check.  
   * @param info The MiscArgInfo to process.
   * @return True if it is found, else false.
   *
   */

  Bool IsSubArgument(const Char * name, MiscArgInfo & info);
   
  /**
   *
   * Determine if argument is a main argument or not.
   *
   * @param name The argument  string.
   * @param prnt Print the arguments verbose usage. (False by default.)
   * @return True if it is found, else false.
   *
   **/

  Bool IsMainArgument(const Char * name, Bool prnt = False);

  /**
   *
   * Check to see if a image number exists and if enough images on
   * list else print a message and return.
   *
   * @param inum The image number.
   * @param imin Minimum images required for this operation.
   * @return True if image number is ok, else false.
   *
   **/

  Bool CheckImgNum(SInt inum, SInt imin);

  /**
   *
   * Check to see if a image component exists.
   *
   * @param inum The image number.
   * @param icom The component number.
   * @return True if image and component number is ok, else false.
   *
   **/

  Bool CheckCmpNum(SInt inum, SInt cnum);
  
  /** List of argument handlers. (dynamic) **/
  MiscCmdLTool::m_argFunction * m_handlers;
  
  /** number of argument handlers. **/
  SInt m_handlersTotal;  

  /** Command line name. **/
  const Char * m_cmdName;
  
  /** Past and present application names **/
  const Char ** m_names;
               
  /** Past and present application verions **/
  const Char ** m_versions;

  /** Application history **/
  const Char ** m_history;
  
  /** Past and present authors **/
  const Char ** m_authors;

  /** Past and present (C) notices **/
  const Char ** m_copyright;

  /** Date of compilation. **/
  const Char * m_dateCompiled;

  /** Time of compilation. **/
  const Char * m_timeCompiled;

  /** Minumum number of arguments required. **/
  SInt m_argMin;
  
  /** Notice to display is minimum argument requirement fails. **/
  const Char * m_argNote;
  
  /** Default output width in characters **/
  SInt m_maxColumns; 
  
  /** Verbose mode **/
  Bool m_verbose;

  /** If not null indicates a file being used as standard in. **/
  const Char * m_stdIFile;
               
  /** If not null indicates a file being used as standard output. **/ 
  const Char * m_stdOFile;
               
  /** If not null indicates a file being used as error output. **/
  const Char * m_stdEFile;

  /** File to read standard input from. **/
  FILE * m_stdI;

  /** File to write standard output to. **/
  FILE * m_stdO;
  
  /** File to write error output to . **/
  FILE * m_stdE;

  /** Text tool. **/
  MiscTextTool m_txtTool;
};

#endif
