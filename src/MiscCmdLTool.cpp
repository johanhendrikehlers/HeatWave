/****************************************************************************/
/**
 ** @file   MiscCmdLTool.cpp
 ** @brief  Contains the function definitions of class MiscCmdLTool.
 ** @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 **
 **/

#include "MiscCmdLTool.hpp"

/****************************************************************************/
/* Copy, clone, etc. functions below.                                       */

MiscCmdLTool::MiscCmdLTool()
{
  ASSERT ( False );
  // memset((Char*)this,'\0', sizeof(MiscCmdLTool));
  // jas_init();
}

MiscCmdLTool::MiscCmdLTool(const MiscCmdLTool & oth)
{
  ASSERT ( False );
}

MiscCmdLTool::MiscCmdLTool(const Char ** nams, const Char ** vers,
                           const Char ** hist, const Char ** auth,
                           const Char ** copy, const Char * date, 
                           const Char * time, SInt aMin, const Char * aNte, 
                           SInt cols, Bool verb, const Char * stdI, 
                           const Char * stdO, const Char * stdE)
{
  m_handlers = NULL;
  m_handlersTotal = 0;
  m_cmdName = NULL;
  m_names = nams; 
  m_versions = vers;
  m_history = hist;
  m_authors = auth;
  m_copyright = copy;
  m_argMin = aMin;
  m_argNote = aNte;
  m_dateCompiled = date;
  m_timeCompiled = time;
  m_maxColumns = cols;
  m_verbose = verb; 
  SetStdInput(stdI);
  SetStdOutput(stdO);
  SetErrOutput(stdE);
  DoGroupRegistration();
}

MiscCmdLTool::~MiscCmdLTool()
{
  DoCleanUp();
}

MiscCmdLTool & 
MiscCmdLTool::operator=(const MiscCmdLTool & oth)
{
  ASSERT ( False );
  return (*this);
}

Bool 
MiscCmdLTool::operator==(const MiscCmdLTool & oth) const
{
  ASSERT ( False );
  return False;
}

void 
MiscCmdLTool::DoCleanUp()
{
  
}

MiscCmdLTool * 
MiscCmdLTool::GetClone() const
{
  ASSERT ( false );
  return NULL;
}

/****************************************************************************/
/* Member access functions below.                                           */

const Char ** 
MiscCmdLTool::GetNames() const
{
  return m_names;
}

void 
MiscCmdLTool::SetNames(const Char ** nams)
{
  m_names = nams;
}

const Char ** 
MiscCmdLTool::GetVersions() const
{
  return m_versions;
}

void 
MiscCmdLTool::SetVersions(const Char ** vers)
{
  m_versions = vers;
}

const Char ** 
MiscCmdLTool::GetHistory() const
{
  return m_history;
}

void 
MiscCmdLTool::SetHistory(const Char ** hist)
{
  m_history = hist;
}

const Char ** 
MiscCmdLTool::GetAuthors() const
{
  return m_authors;
}

void 
MiscCmdLTool::SetAuthors(const Char ** auth)
{
  m_authors = auth;
}

const Char ** 
MiscCmdLTool::GetCopyrights() const
{
  return m_copyright;
}

void 
MiscCmdLTool::SetCopyNames(const Char ** copy)
{
  m_copyright = copy;
}

const Char * 
MiscCmdLTool::GetDateCompiled() const
{
  return m_dateCompiled;
}

void 
MiscCmdLTool::SetDateCompiled(const Char * date)
{
  m_dateCompiled = date;
}

const Char * 
MiscCmdLTool::GetTimeCompiled() const
{
  return m_timeCompiled;
}

void 
MiscCmdLTool::SetTimeCompiled(const Char * time)
{
  m_timeCompiled = time;
}

SInt 
MiscCmdLTool::GetMinimumArg() const
{
  return m_argMin;
}

void 
MiscCmdLTool::SetMinimumArg(SInt mini)
{
  ASSERT ( mini >= 1 );
  m_argMin = mini;
}

const Char * 
MiscCmdLTool::GetMinimumArgNote() const
{
  return m_argNote;
}

void 
MiscCmdLTool::SetMinimumArgNote(const Char * note)
{
  m_argNote = note;
}

SInt 
MiscCmdLTool::GetColumns() const
{
  return m_maxColumns;
}

void 
MiscCmdLTool::SetColumns(SInt wdth)
{
  if ( wdth <= 0 ){
    m_maxColumns = 78;
  }
  else{
    m_maxColumns = wdth;
  }
}

Bool 
MiscCmdLTool::GetVerboseMode() const
{
  return m_verbose;
}

void 
MiscCmdLTool::SetVerboseFlag(Bool mode)
{
  m_verbose = mode;
}
  
SInt 
MiscCmdLTool::GetTotalMainHandlers() const
{
  return m_handlersTotal;
}



/****************************************************************************/
/* Standard Input, Output and Error related functions below.                */

const Char * 
MiscCmdLTool::GetStdInput() const
{
  return m_stdIFile;
}

Bool 
MiscCmdLTool::SetStdInput(const Char * name, Bool exit, const Char * mode)
{
  const Char * def_mode = "rt";
  if ( mode == NULL ){
    mode = def_mode;
  }
  m_stdIFile = name;
  return DoOpenIOFile(m_stdI, name, mode, stdin, exit);
}

const Char * 
MiscCmdLTool::GetStdOutput() const
{
  return m_stdOFile;
}

Bool 
MiscCmdLTool::SetStdOutput(const Char * name, Bool exit, const Char * mode)
{
  const Char * def_mode = "wt";
  if ( mode == NULL ){
    mode = def_mode;
  }
  m_stdOFile = name;
  return DoOpenIOFile(m_stdO, name, mode, stdout, exit);
}

const Char * 
MiscCmdLTool::GetErrOutput() const
{
  return m_stdEFile;
}

Bool 
MiscCmdLTool::SetErrOutput(const Char * name, Bool exit, const Char * mode)
{
  const Char * def_mode = "wt";
  if ( mode == NULL ){
    mode = def_mode;
  }
  m_stdEFile = name;
  return DoOpenIOFile(m_stdE, name, mode, stderr, exit);
}

Bool
MiscCmdLTool::DoOpenIOFile(FILE *& open, const Char * name, 
                           const Char * mode, FILE * dflt, 
                           Bool exit)
{
  if ( name ){
    if ( open ){
      fclose ( open );
      // should we care if above fails?
    }
    open = fopen( name, mode );
    if ( open == NULL ){
      if ( exit ){
        abort();
      }
      else{
        return False;
      }
    }
  }
  else{
    open = dflt;
  }
  return True;
}

/****************************************************************************/
/* Functions for handling all the required arguments below.                 */

Bool 
MiscCmdLTool::DoArguments(SInt argc, const Char ** argv)
{
  // get command line name (notice because sometimes I use the vc compiled app
  // under cygwin we have to check both forward and reverse deliminators) ...
  m_cmdName = argv[0];
  if ( strrchr(m_cmdName,'\\') ){
    m_cmdName = (strrchr(m_cmdName,'\\'));
    m_cmdName++;
  }
  else if ( strrchr(m_cmdName,'/') ){
    m_cmdName = (strrchr(m_cmdName,'/'));
    m_cmdName++;
  }
  
  // quit if minimum args has not been specified ...
  if ( argc < m_argMin ){
    DoPrintHeader(m_argNote);
    return False;
  }
  argc -= 1;
  argv += 1;
  
  // phase A - recognize all options, if "leave" option is found perform and
  // exit ...
  SInt * flgs = NULL; 
  m_argFunction * func = NULL; 
  SInt * acnt = NULL;
  Bool ret = False;
  if (!DoArgumentsA(argc, argv, flgs, func, acnt)){
    ret = True;
    goto clean_up;
  }
  
  // phase B - Perform all requested tasks ...
  if (!DoArgumentsB(argv, func, acnt)){
    ret = True;
    goto clean_up;
  }
  
 clean_up:
  delete flgs;
  delete func;
  delete acnt;
  return ret;
}

Bool
MiscCmdLTool::DoArgumentsA(SInt argc, const Char ** argv, SInt *& flgs, 
                           m_argFunction *& func, SInt *& acnt)
{
  if (argc <= 0){
    return Err_None;
  }
  ASSERT ( flgs == NULL ); ASSERT ( func == NULL ); ASSERT (acnt == NULL);
  flgs = new SInt[m_handlersTotal]; func = new m_argFunction[argc+1];
  acnt = new SInt[argc+1];
  LEAVEONNULL(flgs); LEAVEONNULL(func); LEAVEONNULL(acnt);
  memset((Char*)flgs,'\0', m_handlersTotal*sizeof(SInt));
  memset((Char*)func,'\0', (argc+1)*sizeof(m_argFunction));
  memset((Char*)acnt,'\0', (argc+1)*sizeof(SInt));
  
  SInt func_offset = 0;
  SInt a = 0;
  while ( a < argc ){
    SInt tmp_args = 0;
    for ( SInt i = 0 ; i < m_handlersTotal ; ++ i){
      tmp_args = (this->*m_handlers[i])(Dty_Recognize, (argc-a), (argv+a));
      if ( tmp_args == Err_Missing ){
        fprintf(m_stdE,"%s missing requirements with argument \"%s\"!\n",
                ERR_M, argv[a]);
        return False;
      }
      else if ( tmp_args == Err_Duplicate ){
        fprintf(m_stdE,"%s disallowed duplicate option found with argument "
                "\"%s\"!\n",
                ERR_M, argv[a]);
        return False;
      }
      else if ( tmp_args == Err_Leave ){
        func[0] = m_handlers[i] ; func[1] = NULL;
        acnt[0] = (argc-a) ; acnt[1] = 0;
        DoArgumentsB((argv+a), func, acnt);
        return False;
      } 
      else if ( tmp_args == Err_Unrecognized ){
        continue;
      }
      else if ( tmp_args > 0 ){
        func[func_offset] = m_handlers[i];
        acnt[func_offset] = tmp_args;
        ++func_offset;
        ASSERT ( argc >= a );
        Bool set_before = (flgs[i] & (Att_Set)) ? True : False;
        flgs[i] |= (this->*m_handlers[i])(Dty_ReturnFlag,0, NULL);
        if ( (flgs[i] & (Att_Set)) && set_before ){
          if ( flgs[i] & (Att_S) ){
            fprintf(m_stdE,"%s duplicate arguments of type \"%s\" "
                    "not allowed!\n",
                    ERR_M, argv[a]);
            return False;
          }
        }
        a+=tmp_args;
        break;
      }
    }   
    if ( tmp_args == Err_Unrecognized ){
      fprintf(m_stdE,"%s unrecognized argument \"%s\", try -h for a list!\n",
              ERR_M, argv[a]);
      return False;
    }
  }
  
  ASSERT ( a == argc );
  return True;
}

Bool
MiscCmdLTool::DoArgumentsB(const Char ** argv, m_argFunction * func, SInt * acnt)
{
  const Char ** tmp_argv = argv;
  SInt ret = 0;
  for ( SInt f = 0 ; func[f] != NULL ; ++f ){
    ret = (this->*func[f])(Dty_Perform, acnt[f], tmp_argv);
    if ( ret < 0 ){
      fprintf(m_stdE,"%s following argument \"", ERR_M);
      for ( SInt i = 0 ; i < (acnt[f]-1) ; ++i ){
        fprintf(m_stdE,"%s ", tmp_argv[i]);
      }
      fprintf(m_stdE,"%s\" failed, try -H %s for more help!\n",
              tmp_argv[acnt[f]-1], tmp_argv[0]);
      return False;
    }
    tmp_argv += acnt[f];
  }
  return True;
}

void
MiscCmdLTool::DoGroupRegistration()
{
  Bool ok = true; 
  ok &= DoArgumentRegistration(&MiscCmdLTool::DoMainArgHelp);
  ok &= DoArgumentRegistration(&MiscCmdLTool::DoMainArgVerbose);
  ok &= DoArgumentRegistration(&MiscCmdLTool::DoMainArgSubInfo);
  ok &= DoArgumentRegistration(&MiscCmdLTool::DoMainArgVersion);
  ASSERTALWAYS( ok );
}

Bool
MiscCmdLTool::DoArgumentRegistration(m_argFunction fn)
{
  if ( (this->*fn)(Dty_RecognizeUnique,0, NULL) == False ){
    return False;
  }
  
  ASSERT ( VEC_GROWTH > 0 );
  if ((m_handlersTotal % VEC_GROWTH) == 0 ) {
    m_argFunction * m_tmp = m_handlers;
    m_handlers = new m_argFunction[m_handlersTotal+VEC_GROWTH];
    LEAVEONNULL(m_handlers);
    memcpy(m_handlers, m_tmp, m_handlersTotal*sizeof(m_argFunction));
    delete m_tmp;
  }
  
  m_handlers[m_handlersTotal] = fn;
  ++m_handlersTotal;
  return True;
}

void 
MiscCmdLTool::DoPrintHeader(const Char * msge)
{
  const Char * name = m_cmdName;
  const Char * version = "*";
  const Char * author = "*";
  SInt i = 0;
  while ( m_names[i] != NULL ){
    name = m_names[i];
    ++i;
  }
  i = 0;
  while ( m_versions[i] != NULL ){
    version = m_versions[i];
    ++i;
  }
  i = 0;
  while ( m_authors[i] != NULL ){
    author = m_authors[i];
    ++i;
  }
  fprintf(m_stdO," %s Version %s by \n %s\n\n", name, version, author);
  if ( msge ){
    fprintf(m_stdO," %s\n", msge);
  }
}

/****************************************************************************/
/* Functions for handling the minor requests from handling functions below  */

SInt 
MiscCmdLTool::DoMinorDuty(EnumFunctionDuty duty, MiscArgInfo & info, 
                          SInt argc, const Char ** argv)
{
  Bool found = False;
  SInt dummy = 0;
  const Char * temp[] = {NULL, NULL};
  switch( duty ){
  case Dty_MinInfo:
    return DoArgMinInfoPrint(info);
  case Dty_MaxInfo:
    return DoArgMaxInfoPrint(info);
  case Dty_RecognizeMain:
    if (info.singleName){
      found |= (DoArgSingleRecognition(info.singleName, dummy, info.str, 
                                       dummy, argc, argv, info) > 0);
    }   
    if (info.doubleName){
      found |= (DoArgSingleRecognition(info.doubleName, dummy, info.str, 
                                       dummy, argc, argv, info) > 0);
    }
    return found;
  case Dty_Recognize:
    return DoArgInfoRecognition(info, argc, argv);
  case Dty_RecognizeUnique:
    for ( SInt i = 0 ; i < m_handlersTotal ; ++ i){
      temp[0] = info.singleName;
      ASSERT ( info.singleName != NULL );
      if( (this->*m_handlers[i])(Dty_RecognizeMain,1, temp) > 0){
        return False;
      }
      if( info.doubleName ){
        temp[0] = info.doubleName;
        if( (this->*m_handlers[i])(Dty_RecognizeMain,1, temp) > 0){
          return False;
        }
      }
    }
    return True;
  case Dty_ReturnFlag:
    return info.flag;
  default:
    WARN_IF ( False );
    return False;
  }
}

Bool
MiscCmdLTool::DoArgMinInfoPrint(const MiscArgInfo & arg)
{
  // I have decided to split the screen into 5 columns namely a,left,b,right
  // and c.  Columns a, b and c is only used to delimit columns ...
  const Char * col_a = "  | ";
  const Char * col_b = " | ";
  const Char * col_c = " ";
  const Char * see_oth = "see \"%s\" below";
  const Char * empty = "";
  const Char * brke = "\n ";
  const Char * skip = brke;
  const Char * cntu = "...";
  const SFloat64 l_2_r_ratio = 38.0/100.0;
  
  SInt wdth = m_maxColumns;
  wdth -= (strlen(col_a)+strlen(col_b)+strlen(col_c));
  SInt left_width = (int) ((double)wdth * l_2_r_ratio);
  SInt rght_width = (wdth - left_width);
  Char * arg_ptr;
  
  arg_ptr = CrtArg(arg.singleName, arg.flag, arg.strDes);
  m_txtTool.PutBox(arg_ptr, left_width, 1, m_stdO, col_a, col_b, 
                   NULL, cntu, skip, False);
  delete arg_ptr; 
  arg_ptr = NULL;
  
  if ( arg.doubleName ){
    arg_ptr = new Char[strlen(see_oth)+strlen(arg.doubleName)+10];
    LEAVEONNULL(arg_ptr);
    sprintf(arg_ptr, see_oth, arg.doubleName);
    m_txtTool.PutBox(arg_ptr, rght_width, 1, m_stdO, empty, col_c, 
                     NULL, cntu, skip);
    delete arg_ptr; 
    arg_ptr = NULL;
    arg_ptr = CrtArg(arg.doubleName, arg.flag, arg.strDes);
    m_txtTool.PutBox(arg_ptr, left_width, 1, m_stdO, col_a, col_b, 
                     NULL, cntu, skip, False);
    delete arg_ptr;
    arg_ptr = NULL;
  }
  
  SInt chars_done = 0;
  chars_done = m_txtTool.PutBox(arg.description, rght_width,1,m_stdO,
                                NULL, col_c, brke, cntu, skip);
  if ( (SInt) strlen ( arg.description ) != chars_done ){
    SInt tmp_lft_size = left_width + strlen(col_a) + strlen(col_b); 
    arg_ptr = new Char[ tmp_lft_size + 1];
    LEAVEONNULL(arg_ptr);
    memset(arg_ptr, '\0', tmp_lft_size + 1);
    memset(arg_ptr, ' ', tmp_lft_size);
    strncpy(arg_ptr, col_a , strlen(col_a) );
    strncpy(arg_ptr + (tmp_lft_size - strlen(col_b)), col_b, strlen(col_b));
    m_txtTool.PutBox(arg.description + chars_done, rght_width, -1, m_stdO,
                     arg_ptr, col_c, brke, cntu, skip);
    delete arg_ptr;
    arg_ptr = NULL;
  }     
  return True;
}

Bool
MiscCmdLTool::DoArgMaxInfoPrint(const MiscArgInfo & arg)
{
  const Char * usage =     " Usage: %s %s %s %s %s\n";
  const Char * sub_usage = " Sub options are:";
  const Char * back = "\b";
  const Char * extra = "[...]";
  const Char * sub_arg = "[SUB(s)] ";
  const Char * dbl_name_mask = "%s | %s";
  const Char * sgl_name_mask = "%s";
  const Char * lft = " ";
  const Char * rgt = " ";
  const Char * brk = "\n ";
  const Char * skp = brk;
  const Char * cnt = "...";
  
  Char * full_name = NULL;
  Char * full_name_args = NULL;
  SInt name_len = strlen(arg.singleName) + strlen(sgl_name_mask);
  if ( arg.doubleName ){
    name_len += strlen(arg.doubleName) + strlen(dbl_name_mask);
  }
  full_name = new Char[name_len];
  LEAVEONNULL(full_name);
  if ( arg.doubleName ){
    sprintf(full_name, dbl_name_mask, arg.singleName, arg.doubleName);
  }
  else{
    sprintf(full_name, sgl_name_mask, arg.singleName);
  }
  full_name_args = CrtArg(full_name, arg.flag, arg.strDes);
  fprintf(m_stdO, usage, m_cmdName,
          (arg.flag & (Att_S|Att_L))?back:extra,
          full_name_args,
          (arg.subNum)?sub_arg:back,
          (arg.flag & (Att_S|Att_L))?back:extra);

  // print the short message if its there ...
  if ( arg.description ){
    fprintf(m_stdO,"\n");
    m_txtTool.PutBox(arg.description, m_maxColumns ,-1, m_stdO, 
                     lft, rgt, brk, cnt, skp);
  }

  // print the long message if its there ...
  if ( arg.descriptionLong ){
    m_txtTool.PutBox(arg.descriptionLong, m_maxColumns ,-1, m_stdO, 
                     lft, rgt, brk, cnt, skp);
  }

  // print each of the sub options ...
  if ( arg.subNum ){
    m_txtTool.PutBox(sub_usage, m_maxColumns, -1, m_stdO,
                     lft, rgt, brk, cnt, skp);
    for ( SInt opt = 0 ; opt < arg.subNum ; ++opt ){
      PrtArg(arg.subName[opt], NULL, arg.subFlag[opt], arg.subStrDes[opt],
             arg.subDesc[opt]);
    }
  }
 
  return True;
}

SInt 
MiscCmdLTool::DoArgSingleRecognition(const Char * name, SInt & flag, 
                                     const Char **& ptr, SInt & lcnt, 
                                     SInt argc, const Char ** argv,
                                     MiscArgInfo & info)
{
  SInt args_used = 0;
  
  // first we try to recognise argument 0 ...
  if ( flag & ( Att_TO | Att_TR ) ){
    if (strncmp(name, argv[0], strlen(name)) != 0){
      return Err_Unrecognized;
    }
  }
  else {
    if (strcmp(name, argv[0]) != 0){
      return Err_Unrecognized;
    }
  }
  ++args_used;
  
  // deal with follow requirements ...
  if ( flag & (Att_FR | Att_FO) ){
    if ((flag & Att_FR) & (argc <= 1) ){
      return Err_Missing;
    }
    SInt max_follows = (flag & Att_Multi) ? (argc-args_used) : min(1, argc);
    SInt min_follows = (flag & Att_FR) ? 1 : 0;
    while ( max_follows > 0 ){
      if ( min_follows <= 0 ){
        // see if its not recognized by other
        if ( IsSubArgument ( argv[args_used], info) || 
             IsMainArgument (argv[args_used]) ){
          break;
        }
      }
      lcnt = m_txtTool.AddPtr(ptr, argv[args_used], lcnt, VEC_GROWTH);
      ++args_used;
      max_follows--;
      min_follows--;
    }
  }
  
  // deal with tail requirements ...
  else if ( flag & (Att_TR | Att_TO) ){
    if ( (flag & Att_TR) && ( strlen ( name ) == strlen ( argv[0] ) ) ){
      return Err_Missing;
    }
    lcnt = m_txtTool.AddPtr(ptr, argv[0], lcnt, VEC_GROWTH);
    ptr[0] += strlen(name);
  }
  
  // indicate all went ok and return ...
  flag |= Att_Set;
  return args_used;
}

SInt
MiscCmdLTool::DoArgInfoRecognition(MiscArgInfo & info, 
                                   SInt argc, const Char ** argv)
{
  SInt offset = 0; 
  
  // Recognise the main option name first ...
  offset = DoArgSingleRecognition(info.singleName, info.flag, info.str, 
                                  info.strNum, argc, argv, info);
  if ( (offset == Err_Unrecognized) && (info.doubleName) ){
    offset = DoArgSingleRecognition(info.doubleName, info.flag, info.str, 
                                    info.strNum, argc, argv, info);
  }
  if ( offset <= 0 ){
    return offset;
  }
  
  // recognise the sub option name(s) if present ...
  if(info.subNum){
    while ( (argc-offset) > 0 ){
      SInt sub_offset = 0;
      for ( SInt i = 0 ; i < info.subNum ; ++i ){
        sub_offset = DoArgSingleRecognition(info.subName[i], info.subFlag[i], 
                                            info.subStr[i], info.subStrNum[i],
                                            argc-offset, argv+offset, info);
        if ( sub_offset > Err_None ){
          offset+=sub_offset;
          i = info.subNum;
        }
        else if ( sub_offset < Err_None ){
          return sub_offset;
        }
      }
      if ( sub_offset == Err_None ){
        break;
      }
    }
  }
  
  // see if we have any missing sub option(s) ...
  if ( !ChkArgInfo(info) ){
    return Err_Missing;
  }

  // see if we have a leave flag, now is the time to indicate it(s) ...
  if ( info.GetCombinedFlag() & Att_L ){
    return Err_Leave;
  }
  
  return offset;
}

Bool 
MiscCmdLTool::IsSubArgument(const Char * name, MiscArgInfo & info)
{
  for ( SInt i = 0 ; i < info.subNum ; ++i ){
    if ( info.flag & ( Att_TO | Att_TR ) ){
      ASSERT ( info.subName[i] != NULL );
      if (strncmp(info.subName[i], name, strlen(info.subName[i])) == 0){
        return True;
      }
    }
    else {
      if (strcmp(info.subName[i], name) == 0){
        return True;
      }
    }
  }
  return False;
}

Bool 
MiscCmdLTool::IsMainArgument(const Char * name, Bool prnt)
{
  const Char * temp[] = {name, NULL};
  for ( SInt i = 0 ; i < m_handlersTotal ; ++ i){
    if( (this->*m_handlers[i])(Dty_RecognizeMain,1, temp) > 0){
      if ( prnt ){
        (this->*m_handlers[i])(Dty_MaxInfo, 1, temp);
      }
      return True;
    }
  }
  return False;
}

/****************************************************************************/
/* Functions for helping the minor duty functions above, are below.         */

Char *
MiscCmdLTool::CrtArg(const Char * name, const SInt flag, 
                     const Char * folo)
{
  // a whole list of constants ...
  SInt max_len = strlen(name) + 50;
  if ( folo ){
    max_len += strlen(folo);
  }
  Char * ret;
  NEW_ARRAY(ret, Char, max_len);
  const Char * mskN = "%s";
  const Char * mskTO = "%s[%s]";
  const Char * mskTR = "%s<%s>";
  const Char * mskFO = "%s [%s]";
  const Char * mskFR = "%s <%s>";
  const Char * mskFOE = "%s [%s] [%s(s)]";
  const Char * mskFRE = "%s <%s> [%s(s)]";
  const Char * strBol = "Y|N";
  const Char * strInt = "int";
  const Char * strDbl = "dbl";
  const Char * strStr = "str";
  const Char * follow;
  
  // decide on the tail or follow type ...
  if ( folo ){
    follow = folo;
  }
  else{
    if ( flag & Att_BN ){
      follow = strBol;
    }
    else if ( flag & Att_IN ){
      follow = strInt;
    }
    else if ( flag & Att_DN ){
      follow = strDbl;
    }
    else{
      follow = strStr;
    }
  }
  
  // print the single argument with tail or follow ...
  if ( flag & (Att_TO) ){
    sprintf(ret, mskTO , name, follow);
  }
  else if ( flag & Att_TR ){
    sprintf(ret, mskTR , name, follow);
  }
  else if ( flag & (Att_FO)) {
    if ( flag & (Att_Multi) ){
      sprintf(ret, mskFOE , name, follow, follow);
    }
    else{
      sprintf(ret, mskFO , name, follow);
    }
  }
  else if ( flag & (Att_FR)) {
    if ( flag & (Att_Multi) ){
      sprintf(ret, mskFRE , name, follow, follow);
    }
    else{
      sprintf(ret, mskFR , name, follow);
    }
  } 
  else{
    sprintf(ret, mskN , name);
  }
  return ret;
}

void 
MiscCmdLTool::PrtArg (const Char * sngl, const Char * dobl, const SInt flag,
                      const Char * type, const Char * note)
{
  // I have decided to split the screen into 5 columns namely a,left,b,right
  // and c.  Columns a, b and c is only used to delimit columns ...
  const Char * col_a = "  | ";
  const Char * col_b = " | ";
  const Char * col_c = " ";
  const Char * see_oth = "see long name below";
  const Char * empty = "";
  const Char * brke = "\n ";
  const Char * skip = brke;
  const Char * cntu = "...";
  const SFloat64 l_2_r_ratio = 38.0/100.0;
  
  SInt wdth = m_maxColumns;
  wdth -= (strlen(col_a)+strlen(col_b)+strlen(col_c));
  SInt left_width = (int)((double)wdth * l_2_r_ratio);
  SInt rght_width = (wdth - left_width);
  Char * arg_ptr;
  
  arg_ptr = CrtArg(sngl, flag, type);
  m_txtTool.PutBox(arg_ptr, left_width, 1, m_stdO, col_a, col_b, 
                   NULL, cntu, skip, False);
  delete arg_ptr; 
  arg_ptr = NULL;
  
  if ( dobl ){
    m_txtTool.PutBox(see_oth, rght_width, 1, m_stdO, empty, col_c, 
                     NULL, cntu, skip);
    arg_ptr = CrtArg(dobl, flag, type);
    m_txtTool.PutBox(arg_ptr, left_width, 1, m_stdO, col_a, col_b, 
                     NULL, cntu, skip, False);
    delete arg_ptr;
    arg_ptr = NULL;
  }
  
  SInt chars_done = 0;
  chars_done = m_txtTool.PutBox(note, rght_width,1,m_stdO,
                                NULL, col_c, brke, cntu, skip);
  if ( (SInt) strlen ( note ) != chars_done ){
    SInt tmp_lft_size = left_width + strlen(col_a) + strlen(col_b); 
    arg_ptr = new Char[ tmp_lft_size + 1];
    LEAVEONNULL(arg_ptr);
    memset(arg_ptr, '\0', tmp_lft_size + 1);
    memset(arg_ptr, ' ', tmp_lft_size);
    strncpy(arg_ptr, col_a , strlen(col_a) );
    strncpy(arg_ptr + (tmp_lft_size - strlen(col_b)), col_b, strlen(col_b));
    m_txtTool.PutBox(note + chars_done, rght_width, -1, m_stdO,
                     arg_ptr, col_c, brke, cntu, skip);
    delete arg_ptr;
    arg_ptr = NULL;
  }     
}

Bool 
MiscCmdLTool::ChkArgInfo(MiscArgInfo & info)
{
  // see if any of the required options are missing ...
  for ( SInt i = 0 ; i < info.subNum ; ++i ){
    if ( info.subFlag[i] & (Att_FR) ){
      if (!(info.subFlag[i] & (Att_Set))){
        return False;
      }
    } 
  }
  for ( SInt i = 0 ; i < info.subNum ; ++i ){
    if (!(info.subFlag[i] & (Att_Set))){
      SInt dummy = 0;
      m_txtTool.AddPtr(info.subStr[i], info.subStrDef[i], dummy, VEC_GROWTH);
    }
  }
  return True;
}


/****************************************************************************/
/* default command line handlers                                            */

SInt
MiscCmdLTool::DoMainArgHelp (EnumFunctionDuty duty, SInt argc, 
                             const Char ** argv)
{
  // set up a ArgInfo struct
  MiscArgInfo info(0);
  info.singleName = "-h";
  info.doubleName = "--help";
  info.description = 
    "prints out a short usage message with the list of the"
    " main arguments.";
  info.flag = Att_L|Att_S;
  
  // perform the minor duty's
  if( duty != Dty_Perform ){
    return DoMinorDuty(duty, info, argc, argv);
  };
  
  DoPrintHeader();
  fprintf(m_stdO," Usage: %s [option [sub_option]] ...\n\n",
          m_cmdName);
  fprintf(m_stdO," Main options are:\n\n");
  for ( SInt i = 0; i < m_handlersTotal; ++i ){
    int check = (this->*m_handlers[i])(Dty_MinInfo,0, NULL);
    ASSERT ( check >= 0);
  }
  fprintf(m_stdO,"\n");
  return True;
}

SInt 
MiscCmdLTool::DoMainArgVerbose (EnumFunctionDuty duty, SInt argc, const Char ** argv)
{
  // set up a ArgInfo struct
  MiscArgInfo info(0);
  info.singleName = "-v";
  info.doubleName = "--verbose";
  info.description = "switch verbose mode on";
  
  // perform the minor duty's
  if( duty != Dty_Perform ){
    return DoMinorDuty(duty, info, argc, argv);
  };
  
  if (m_verbose){
    m_verbose = False;
  }
  else{
    m_verbose = True;
  }
  
  if (m_verbose){
    fprintf(m_stdO,"%s verbose mode switched on\n", VRB_M);
  }
  return True;
}

SInt 
MiscCmdLTool::DoMainArgSubInfo (EnumFunctionDuty duty, SInt argc, const Char ** argv)
{
  // set up a ArgInfo struct ...
  MiscArgInfo info(0);
  info.singleName = "-H";
  info.doubleName = "--Help";
  info.description = "print help on specific command";
  info.flag = Att_L|Att_S|Att_FR|Att_SN;
  info.strDes = "argument";
  
  // perform the minor duty's
  if( duty != Dty_Perform ){
    return DoMinorDuty(duty, info, argc, argv);
  };

  // perform requested task 
  DoArgInfoRecognition(info, argc, argv);
  ASSERT ( info.str );
  ASSERT ( info.str[0] );
  if ( !IsMainArgument( info.str[0], True) ){
    fprintf(m_stdE,"%s can not find handler for \"%s\" anywhere!\n",
            ERR_M, info.str[0]);
    return False;
  }
  
  return True;
}

SInt
MiscCmdLTool::DoMainArgVersion (EnumFunctionDuty duty, SInt argc, const Char ** argv)
{
  // set up a ArgInfo struct
  MiscArgInfo info(0);
  info.singleName = "-V";
  info.doubleName = "--Version";
  info.description = "print application version";
  info.flag = Att_L|Att_S;
  
  // perform the minor duty's
  if( duty != Dty_Perform ){
    return DoMinorDuty(duty, info, argc, argv);
  };

  const Char * dflt = "not set";
  const Char * name = m_txtTool.GetVectorEnd(m_names, dflt);
  const Char * vers = m_txtTool.GetVectorEnd(m_versions, dflt);
  const Char * date = (m_dateCompiled)?m_dateCompiled:dflt;
  const Char * time = (m_timeCompiled)?m_timeCompiled:dflt;
  fprintf(m_stdO," %s Version %s Compiled %s @ %s.\n",
          name, vers, date, time);
  return True;
}

