/****************************************************************************/
/**
 ** @file   MiscTextTool.cpp
 ** @brief  Contains the MiscTextTool class function definitions.
 ** @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 **
 **/

#include "MiscTextTool.hpp"


MiscTextTool::MiscTextTool()
{
}

MiscTextTool::MiscTextTool(SInt max)
{
}
  
MiscTextTool::MiscTextTool(const MiscTextTool & oth)
{
}
  
MiscTextTool::~MiscTextTool()
{
}
  
SInt 
MiscTextTool::GetBufferSizeMax() const
{
  return m_bufferSize;
}

void 
MiscTextTool::SetBufferSizeMax( SInt size )
{
  m_bufferSize = size;
}

const Char * 
MiscTextTool::GetVectorEnd(const Char ** vctr, const Char * dflt) const
{
  if ( vctr == NULL ){
    return dflt;
  }
  else if (vctr[0] == NULL){
    return dflt;
  }
  SInt i = 0;
  while (vctr[i+1] != NULL){
    ++i;
  }
  return vctr[i];
}

SInt
MiscTextTool::AddPtr(const Char **& rvec, const Char * rstr, SInt vecs, 
                     SInt vecg)
{
  if ( (rvec == NULL) || ((vecs%vecg) == 0 && (vecs != 0)) ){
    const Char ** temp = new const Char * [vecs+vecg];
    if ( rvec ){
      memcpy(temp,rvec,(vecs+vecg)*sizeof(const Char*));
      delete [] rvec;
    }
    rvec=temp;
  }
  rvec[vecs] = rstr;
  return (vecs+1);
}

void 
MiscTextTool::Chop(Char * guil, SInt leng, const Char * tail)
{
  if ( ! guil ){
    return;
  }
  SInt guil_len = strlen ( guil );
  SInt tail_len = 0;
  if ( tail ){
    tail_len = strlen ( tail );
  }
  
  if ( guil_len >= leng ){
    guil[leng]='\0';
    while ( tail_len && guil_len ){
      guil[--guil_len]=tail[--tail_len];
    }
  }
}

void 
MiscTextTool::PutCTimes(const Char type, SInt numb, FILE * strm)
{
  ASSERT ( strm != NULL );
  if ( numb <= 0 ){
    return;
  }
  
  for ( SInt i = 0 ; i < numb ; ++i ){
    fputc(type,strm);
  }
}

void 
MiscTextTool::PutSTimes(const Char * strp, SInt numb, FILE * strm)
{
  ASSERT ( strm != NULL );
  ASSERT ( strp != NULL );
  if ( numb <= 0 ){
    return;
  }
  SInt str_len = strlen(strp);
  
  for ( SInt i = 0 ; i < min(numb,str_len) ; ++i ){
    fputc(strp[i],strm);
  }
}

SInt
MiscTextTool::PutBox(const Char * mesg, SInt wdth, SInt hght, FILE * strm, 
                     const Char * left, const Char * rght, const Char * brak, 
                     const Char * wcon, const Char * skip, Bool prnl)
{
  ASSERT ( mesg != NULL );
  ASSERT ( wdth > 0 );
  ASSERT ( strm != NULL );
  
  const Char * mesg_ptr = mesg;
  const Char * fond_ptr = NULL;
  const Char * empty = "";
  const Char * left_ptr = left?left:empty;
  const Char * rght_ptr = rght?rght:empty;
  const Char * brak_ptr = brak?brak:empty;
  const Char * wcon_ptr = wcon?wcon:empty;
  const Char * skip_ptr = skip?skip:empty;
  const Char * new_line = "\n";
  
  SInt nl_offset = 0;
  SInt rows = 0;
  SInt var = 0;
  SInt print_so_far = 0;
  while ( mesg_ptr[0] ){
    if ( (hght >= 0) && (rows >= hght) ){
      break;
    }
    nl_offset = 0;
    fprintf(strm,left_ptr);

    // step 0 : we skip those unwanted letters ...
    while ( ChkCGrp(mesg_ptr[0],skip_ptr )){
      mesg_ptr++;
    }

    // step 1 : we check for new lines ...
    fond_ptr = this->StrChr ( mesg_ptr, new_line, wdth);
    if ( fond_ptr ){
      nl_offset = (fond_ptr - mesg_ptr);
      fond_ptr = NULL;
      var = nl_offset+1;
      PutSTimes(mesg_ptr,var-1,strm);
      PutCTimes(' ',wdth-(var-1),strm);
      mesg_ptr += var;
      print_so_far += var;
      var = wdth;
      goto done;
    }

    // step 2 : we deal with the case where it all fits in ...
    if ( (SInt)strlen(mesg_ptr) <= wdth ){
      print_so_far += fprintf(strm,mesg_ptr);
      PutCTimes(' ',wdth-strlen(mesg_ptr),strm);
      var = wdth;
      mesg_ptr += strlen(mesg_ptr);
      goto done;
    }

    // step 3 : use good break point (note I skip the break character) ...
    fond_ptr = this->StrChr(mesg_ptr,brak_ptr,wdth,0,False);
    if ( fond_ptr ){
      var = fond_ptr - mesg_ptr;
      PutSTimes(mesg_ptr, var, strm);
      print_so_far += var;
      mesg_ptr += var;
      PutCTimes(' ',wdth-(var),strm);
      var = wdth;
      goto done;
    }
    
    // step 4 : use a break point at end of row using the users connector ...
    var = wdth - strlen(wcon_ptr);
    PutSTimes(mesg_ptr, var, strm);
    print_so_far += var;
    mesg_ptr += var;
    var += fprintf(strm, wcon_ptr);
    
  done:
    ASSERT ( var == wdth );
    fprintf(strm,rght_ptr);
    rows++;
    if ( rows == hght ){
      break;
    }
    else if ( mesg_ptr[0] != '\0' ){
      fprintf(strm,"\n");
    }
  }
  
  if ( prnl ){
    fprintf(strm,"\n");
  }
  return print_so_far;
}

const Char * 
MiscTextTool::StrChr(const Char * srch, const Char * find, SInt lnth, 
                     SInt offs, Bool frst)
{
  ASSERT(srch);
  ASSERT(find);
  
  // handle the offset if set ...
  if ( (offs >= 0) && (offs < (SInt)strlen(srch)) ){
    srch += offs;
  }
  
  // start searching the string and return first occurance ...
  for ( SInt f = 0 ; f < (SInt)strlen(find) ; ++f ){
    SInt d = frst?+1:-1;
    SInt s = frst?0:min((SInt)strlen(srch),lnth);
    for ( ; s ; s += d ){
      if ( srch[s] == find[f] ){
        return &(srch[s]);
      }
    }
  }

  // oops nothing found ...
  return NULL;
}

Bool 
MiscTextTool::ChkCGrp(const Char test, const Char * grup)
{
  // if no test group return false ...
  if ( !grup || strlen(grup) == 0 ){
    return False;
  }
  
  // check each character ...
  for ( SInt f = 0 ; f < (SInt)strlen(grup) ; ++f ){
    if ( test == grup[f] ){
      return True;
    }
  }
  return False;
}
  
MiscTextTool * 
MiscTextTool::GetClone() const
{
  MiscTextTool * ret = new MiscTextTool(*this);
  LEAVEONNULL(ret);
  return ret;
}

MiscTextTool& 
MiscTextTool::operator=(const MiscTextTool& rhs)
{
  return (*this);
}
  
Bool 
MiscTextTool::operator==(const MiscTextTool & rhs ) const
{
  return False;
}

void 
MiscTextTool::DoCopy(const MiscTextTool & rhs)
{
}
  
void 
MiscTextTool::DoDestroy()
{
}
  
SIntA 
MiscTextTool::AToSIntA(const Char * strn)
{
  // Not finished with this function,
  // have to do the some refactoring, and implement the random "~" choice
  // and "@" choice
  const Char del = ',';
  const Char nul = '\0';
  ASSERT(strn);
  SIntA ret;
  
  Char * s_tmp;
  const Char * s_strt = strn;
  const Char * s_end = NULL;
  Char * non_digit = NULL;
  SInt a,b;
  
  do{
    if ( !(s_end = strchr(s_strt, del)) ){
      s_end = strchr(s_strt, nul); 
    }
    NEW_ARRAY(s_tmp, Char, (s_end-s_strt)+1);
    strncpy(s_tmp, s_strt, s_end-s_strt);
    s_tmp[s_end-s_strt] = '\0';
    //printf("first string %s: ", s_tmp);
    // < ---------------------------------------------
    if ( sscanf(s_tmp, "%d", &a) == 1 ){
      //printf(" single digit %d ", a);
      non_digit = s_tmp;
      while ( isdigit(non_digit[0]) ) non_digit++;
      if ( non_digit[0] ){
        Bool range = True;
        switch(non_digit[0]){
        case '~':
          range = False;
        case ':':
          //printf(" good symbol found \"%c\" ", non_digit[0]);
          if ( sscanf(&(non_digit[1]), "%d", &b) != 1 ){
            //printf(" follow digit is %d ", b);
            return SIntA(0);
          }
          if ( range ){
            if ( b < a ){
              SInt spare = a;
              b = a;
              a = spare;
            }
            for ( SInt i = a ; i <= b ; ++i ){
              ret.PushBack(i);
            }
          }
          else{
            printf(" random number requested but not yet done!\n");
          }
          break;
        default:
          //printf(" symbols \"%c\" bad\n", non_digit[0]);
          return SIntA(0);
        }
        //printf(" first non digit %s\n", non_digit);
      }
      else{
        ret.PushBack(a);
      }
      //printf("\n");
    }
    // < ----------------------------------------
    if ( s_end[0] == del ){ 
      s_strt = &(s_end[1]); 
    }
    DEL_ARRAY(s_tmp);
  } while ( s_end[0] );
  
  return ret;
}
  
