/****************************************************************************/
/**
 ** @file   MiscToolBase.cpp
 ** @brief  Defines base functions of MiscTool class.
 ** @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 **
 **/

#include "MiscTool.hpp"

MiscTool::MiscTool(const Char ** nams, const Char ** vers,
                   const Char ** hist, const Char ** auth,
                   const Char ** copy, const Char * date, 
                   const Char * time, SInt aMin, const Char * aNte, 
                   SInt cols, Bool verb, const Char * stdI, 
                   const Char * stdO, const Char * stdE)
  :MiscCmdLTool(nams, vers, hist, auth, copy, date, time, aMin, 
                aNte, cols, verb, stdI, stdO, stdE)
{
  DoGroupRegistration();
  jas_init();
}

MiscTool::~MiscTool()
{
  DoCleanUp();
}

void 
MiscTool::DoCleanUp()
{
  
}

/****************************************************************************/

Bool 
MiscTool::CheckImgNum(SInt inum, SInt imin)
{
  if ( m_images.GetImageN() < imin){
    fprintf(m_stdE,"%s at least %d images required but only %d on list\n",
            ERR_M, imin, m_images.GetImageN());
    return False;
  }
  if (( inum < 0 )||(inum >= m_images.GetImageN())){
    fprintf(m_stdE,"%s image number %d is out of bounds [0,%d]\n", ERR_M,
            imin, (m_images.GetImageN()-1));
    return False;
  }
  return True;
}

Bool
MiscTool::CheckCmpNum(SInt inum, SInt icmp)
{
  if (( inum < 0 )||(inum >= m_images.GetImageN())){
    fprintf(m_stdE, "%s image number %d is out of bounds [0,%d]\n", ERR_M,
            inum, (m_images.GetImageN()-1));
    return False;
  }
  HeatWaveImage & img = m_images.GetImage(inum);
  if (( icmp < 0 )||icmp >= img.GetComponentN()){
    fprintf(m_stdE, "%s component number %d is out of bounds [0,%d]\n", 
            ERR_M, icmp, (img.GetComponentN()-1));
    return False;
  }
  return True;
}

SInt 
MiscTool::GetTotalImagesLoaded() const
{
  return m_images.GetImageN();
}

#define ___INFO_CHECK___(test)                  \
  ASSERT ( arg_offs >= 0 );                     \
  ASSERT ( arg_offs < info.subNum );            \
  ASSERT ( info.subName[arg_offs] test 0 );     \
  ASSERT ( info.subDesc[arg_offs] test 0 );     \
  ASSERT ( info.subFlag[arg_offs] test 0 );

void 
MiscTool::InfoUsesImgs(MiscArgInfo & info, SInt arg_offs)
{
  ___INFO_CHECK___(==);
    
  info.subName[arg_offs] = "img";
  info.subDesc[arg_offs] = "specify image(s) to use";
  info.subFlag[arg_offs] = Att_S|Att_TR|Att_SN;
}

void 
MiscTool::InfoUsesCmps(MiscArgInfo & info, SInt arg_offs)
{
  ___INFO_CHECK___(==);
  
  info.subName[arg_offs] = "cmp";
  info.subDesc[arg_offs] = "specify component(s) to use";
  info.subFlag[arg_offs] = Att_S|Att_TR|Att_SN;
}

void 
MiscTool::InfoUsesAreas(MiscArgInfo & info, SInt arg_offs)
{
  ___INFO_CHECK___(==);
  
  info.subName[arg_offs] = "area";
  info.subDesc[arg_offs] = "specify area to use";
  info.subFlag[arg_offs] = Att_S|Att_TR|Att_SN;
  info.subDesc[arg_offs] = 
    "in following order  <level><LL|HL|LH|HH>[,<level><LL|HL|LH|HH>[,...]]";
}

SIntA 
MiscTool::InfoAtoiImgs(MiscArgInfo & info, SInt arg_offs, 
                       SInt min_img, Bool frm_end)
{
  if ( !CheckImgNum(0, min_img) || m_images.GetImageN() == 0 ){
    return SIntA(0);
  }
  ___INFO_CHECK___(!=);
  
  SIntA ret;
  if ( info.subFlag[arg_offs] & Att_Set ){
    for ( SInt i = 0; i < info.subStrNum[arg_offs] ; ++i ){
      if (!GetArrFromStr(ret,info.subStr[arg_offs][i],
                         0, m_images.GetImageN()-1,
                         "%s bad request, image %d is not in array [%d,%d]\n"))
        {
          return SIntA(0);
        }
    }
  }
  else {
    for ( SInt i = 0 ; i < min_img ; i++ ){
      if (frm_end){
        ret.PushBack((m_images.GetImageN()-1)-i);
      }
      else{
        ret.PushBack(i);
      }
    }
  }
  return ret;
}

  
SIntA 
MiscTool::InfoAtoiCmps(MiscArgInfo & info, SInt arg_offs, SInt imgn,
                       SInt min_cmp, Bool frm_end)
{
  ASSERT ( imgn >= 0 );
  ASSERT ( imgn < m_images.GetImageN() );
  ___INFO_CHECK___(!=);
  HeatWaveImage & img_ref = m_images.GetImage(imgn);
  if ( min_cmp > img_ref.GetComponentN() ){
    return SIntA(0);
  }
  
  SIntA ret;
  if ( info.subFlag[arg_offs] & Att_Set ){
    for ( SInt i = 0; i < info.subStrNum[arg_offs] ; ++i ){
      if (!GetArrFromStr(ret, info.subStr[arg_offs][i],
                         0, ((img_ref.GetComponentN())-1),
                         "%s bad request, component %d does not exist in "
                         "array [%d,%d]\n")) {
        return SIntA(0);
      }
    }
  }
  else {
    for ( SInt i = 0 ; i < min_cmp ; i++ ){
      if (frm_end){
        ret.PushBack((img_ref.GetComponentN()-1)-i);
      }
      else{
        ret.PushBack(i);
      }
    }
  }
  return ret;
}

RctA 
MiscTool::InfoAtoiRcts(MiscArgInfo & info, SInt arg_offs, SInt imgn, SInt cmpn)
{
  /** @todo refactorise me and add some more functionality. **/
  ASSERT ( imgn >= 0 );
  ASSERT ( imgn < m_images.GetImageN() );
  ASSERT ( cmpn >= 0 );
  ASSERT ( cmpn < m_images.GetImage(imgn).GetComponentN() );
  ___INFO_CHECK___(!=);

  RctA ret;
  HeatWaveComponent & ref = m_images.GetImage(imgn).GetComponent(cmpn);
  if ( info.subFlag[arg_offs] & Att_Set ){
    for ( SInt i = 0; i < info.subStrNum[arg_offs] ; ++i ){
      const Char * str = info.subStr[arg_offs][i];
      SInt lev;
      EnumSubband sub;
      Rct rct;
      if ( sscanf( str, "%d", &lev ) != 1 ){
        return RctA(0);
      }
      while ( isdigit(str[0]) ) str++;
      if ( strncmp (str,"TL",2) == 0 ){
        sub = SubLL;
      }
      else if ( strncmp (str,"TR",2) == 0 ){
        sub = SubHL;
      }
      else if ( strncmp (str,"BL",2) == 0 ){
        sub = SubLH;
      }
      else if ( strncmp (str,"BR",2) == 0 ){
        sub = SubHH;
      }
      else{
        return RctA(0);
      }
      if ( !ref.GetSubbandInfo(lev, sub, rct.x, rct.y, rct.w, rct.h) ){
        return RctA(0);
      }
      ret.PushBack(rct);
    }
  }
  else {
    Rct rct;
    if ( !ref.GetSubbandInfo(0, SubLL, rct.x, rct.y, rct.w, rct.h) ){
      return RctA(0);
    }
    ret.PushBack(rct);
  }
  return ret;
}

Bool
MiscTool::GetArrFromStr(SIntA & arr, const Char * strv, SInt min, 
                        SInt max, const Char * errm)
{
  SIntA a = m_txtTool.AToSIntA( strv );
  if ( a.Size() <= 0 ){
    fprintf(m_stdE, "%s string \"%s\" is not a valid integer or integer "
            "list\n", ERR_M, strv);
    return False;
  }
  for ( SInt j = 0; j < a.Size() ; ++j ){
	SInt num = a.Get(j);
    if ( num < min || num > max ){
      fprintf(m_stdE, errm, ERR_M, num, min, max);
      return False;
    }
    else{
      arr.PushBack(num);
    }
  }
  return True;
}

void
MiscTool::DoGroupRegistration()
{
  Bool ok = true;
  ok &= DoArgumentRegistration((MiscCmdLTool::m_argFunction)
                               &MiscTool::DoMainImgLoad);
  ok &= DoArgumentRegistration((MiscCmdLTool::m_argFunction)
                               &MiscTool::DoMainImgSave);
  ok &= DoArgumentRegistration((MiscCmdLTool::m_argFunction)
                               &MiscTool::DoMainImgVidL);
  ok &= DoArgumentRegistration((MiscCmdLTool::m_argFunction)
                               &MiscTool::DoMainImgSpat);
  ok &= DoArgumentRegistration((MiscCmdLTool::m_argFunction)
                               &MiscTool::DoMainImgTemp);
  ok &= DoArgumentRegistration((MiscCmdLTool::m_argFunction)
                               &MiscTool::DoMainImgFuse);
  ok &= DoArgumentRegistration((MiscCmdLTool::m_argFunction)
                               &MiscTool::DoMainImgHash);
  ok &= DoArgumentRegistration((MiscCmdLTool::m_argFunction)
                               &MiscTool::DoMainImgClrT);
  ok &= DoArgumentRegistration((MiscCmdLTool::m_argFunction)
                               &MiscTool::DoMainImgComp);
  ok &= DoArgumentRegistration((MiscCmdLTool::m_argFunction)
                               &MiscTool::DoMainImgHiEq);
  ok &= DoArgumentRegistration((MiscCmdLTool::m_argFunction)
                               &MiscTool::DoMainImgList);
  ok &= DoArgumentRegistration((MiscCmdLTool::m_argFunction)
                               &MiscTool::DoMainImgInfo);
  ok &= DoArgumentRegistration((MiscCmdLTool::m_argFunction)
                               &MiscTool::DoMainImgHIII);
  ok &= DoArgumentRegistration((MiscCmdLTool::m_argFunction)
                               &MiscTool::DoMainImgPlot);
  ASSERTALWAYS( ok );
}
