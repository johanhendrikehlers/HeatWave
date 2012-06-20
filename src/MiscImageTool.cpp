/****************************************************************************/
/**
 ** @file   MiscImageTool.cpp
 ** @brief  Contains the function definitions of class MiscImageTool.
 ** @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 **
 **/

#include "MiscImageTool.hpp"

MiscImageTool::MiscImageTool()
{
  memset((char*)this,'\0',sizeof(MiscImageTool));
}

MiscImageTool::~MiscImageTool()
{
  DoCleanUp();
}

HeatWaveImage * 
MiscImageTool::GetHeatFromJasper(jas_image_t * image) const
{
  ASSERT ( image != NULL );
  
  /** create an empty image **/
  HeatWaveImage * ret =  NULL;
  ret =  new HeatWaveImage(jas_image_tlx(image), jas_image_tly(image),
                           jas_image_width(image), jas_image_height(image),
                           GetSpaceHeatFromJasper(jas_image_clrspc(image)),
                           jas_image_numcmpts(image),True);
  LEAVEONNULL(ret);
  
  /** create each component **/
  // SInt compNum = ret->GetComponentN();
  HeatWaveComponent ** compArr = ret->GetComponentA();
  for ( SInt i = 0 ; i < jas_image_numcmpts(image) ; ++ i ){
    
    /** copy the data across to a matrix **/
    SInt tlx = jas_image_cmpttlx(image,i);
    SInt tly = jas_image_cmpttly(image,i);
    SInt width = jas_image_cmptwidth(image,i);
    SInt height = jas_image_cmptheight(image,i);
    jas_matrix_t * matrix = jas_matrix_create(height,width);
    LEAVEONNULL(matrix);
    SInt state = jas_image_readcmpt(image,i,tlx,tly,width,height,matrix);
    ASSERT ( (state == 0) );
    compArr[i] = 
      new HeatWaveComponent(tlx,tly,
                            jas_image_cmpthstep(image,i),
                            jas_image_cmptvstep(image,i),
                            width,height,
                            (jas_image_cmptsgnd(image,i)!=0),
                            jas_image_cmptprec(image,i),
                            GetColorHeatFromJasper(jas_image_clrspc(image),
                                                   jas_image_cmpttype(image,i)),
                            (Smpl*)matrix->data_,(Smpl**)matrix->rows_,
                            True,False);
    matrix->rows_ = NULL;
    matrix->data_ = NULL;
    jas_matrix_destroy(matrix);
  }
  
  return ret;
}

jas_image_t * 
MiscImageTool::GetJasperFromHeat(const HeatWaveImage * image) const
{
  ASSERT ( image != NULL );
  jas_image_t * ret = jas_image_create0();
  LEAVEONNULL(ret);
  SInt ncomp = image->GetComponentN();
  HeatWaveComponent ** acomp = image->GetComponentA();
  jas_image_cmptparm_t parm;
  jas_matrix_t matrix;
  
  for ( SInt i = 0 ; i < ncomp ; ++i){
    parm.tlx = acomp[i]->GetTLX();
    parm.tly = acomp[i]->GetTLY();
    parm.hstep = acomp[i]->GetHStep();
    parm.vstep = acomp[i]->GetVStep();
    parm.width = acomp[i]->GetWidth();
    parm.height = acomp[i]->GetHeight();
    parm.prec = acomp[i]->GetPrec();
    parm.sgnd = acomp[i]->GetSgnd();
    matrix.flags_ = 0;
    matrix.xstart_ = acomp[i]->GetTLX();
    matrix.ystart_ = acomp[i]->GetTLY();
    matrix.xend_ = acomp[i]->GetTLX() + acomp[i]->GetWidth();
    matrix.yend_ = acomp[i]->GetTLY() + acomp[i]->GetHeight();
    matrix.numrows_ = acomp[i]->GetHeight();
    matrix.numcols_ = acomp[i]->GetWidth();
    matrix.rows_ = (jas_seqent_t**)acomp[i]->GetRows();
    matrix.maxrows_ = acomp[i]->GetHeight();
    matrix.data_ = (jas_seqent_t*)acomp[i]->GetData();
    matrix.datasize_ = acomp[i]->GetSize();
    if(jas_image_addcmpt(ret,i,&parm)){
      SInt * dummy = NULL;
      LEAVEONNULL(dummy);
    };
    if(jas_image_writecmpt(ret,i,parm.tlx,parm.tly,parm.width,parm.height,
                           &matrix)){
      ASSERT(False);
      SInt * dummy = NULL;
      LEAVEONNULL(dummy);
    }
    ASSERT(matrix.data_ == (jas_seqent_t*)acomp[i]->GetData());
    ASSERT(matrix.rows_ == (jas_seqent_t**)acomp[i]->GetRows());
    jas_image_setcmpttype(ret,i,GetColorJasperFromHeat(acomp[i]->GetColor()));
  }
  
  jas_image_setclrspc(ret, GetSpaceJasperFromHeat(image->GetSpace()));
  return ret;
}

EnumSpace
MiscImageTool::GetSpaceHeatFromJasper(const jas_image_colorspc_t spc)const
{
  switch(spc){
  case JAS_CLRSPC_SRGB:
    return SpcRGB;
  case JAS_CLRSPC_GENRGB:
    return SpcRGB;
  case JAS_CLRSPC_SYCBCR:
    return SpcYUV;
  case JAS_CLRSPC_GENYCBCR:
    return SpcYUV;
  case JAS_CLRSPC_SGRAY:
    return SpcGrey;
  case JAS_CLRSPC_GENGRAY:
    return SpcGrey;
  default:
    return SpcUnknown;
  }
}

jas_image_colorspc_t 
MiscImageTool::GetSpaceJasperFromHeat(const EnumSpace spc)const
{
  switch (spc){
  case SpcRGB:
    return JAS_CLRSPC_SRGB;
  case SpcYUV:
    return JAS_CLRSPC_SYCBCR;
  case SpcGrey:
    return JAS_CLRSPC_SGRAY;
  case SpcUnknown:
    return JAS_CLRSPC_UNKNOWN;
  default:
    ASSERT(False);
    return JAS_CLRSPC_UNKNOWN;
  }
}

EnumColor
MiscImageTool::GetColorHeatFromJasper(const jas_image_colorspc_t spc, 
                                      const jas_image_smpltype_t type) const
{
  EnumSpace sim_spc = GetSpaceHeatFromJasper(spc);
  
  switch (sim_spc){
  case SpcRGB:
    switch (type){
    case 0:
      return ClrRed;
    case 1:
      return ClrGreen;
    case 2:
      return ClrBlue;
    default:
      ASSERT(False);
      return ClrUnknown;
    }
  case SpcYUV:
    switch (type){
    case 0:
      return ClrY;
    case 1:
      return ClrU;
    case 2:
      return ClrV;
    default:
      ASSERT(False);
      return ClrUnknown;
    }
  case SpcGrey:
    switch (type){
    case 0:
      return ClrGrey;
    default:
      ASSERT(False);
      return ClrUnknown;
    }
  default:
    ASSERT(False);
    return ClrUnknown;
  }
}

jas_image_smpltype_t 
MiscImageTool::GetColorJasperFromHeat(const EnumColor type)const
{
  switch (type){
  case ClrRed:
  case ClrY:
  case ClrGrey:
    return 0;
  case ClrGreen:
  case ClrU:
    return 1;
  case ClrBlue:
  case ClrV:
    return 2;
  case ClrUnknown:
    return (jas_image_smpltype_t)JAS_IMAGE_CT_UNKNOWN;
  default:
    ASSERT(False);
    return -1;
  }
}

Char * 
MiscImageTool::GetLastErrorMessage()
{
  return m_error;
}

void 
MiscImageTool::CpyLastErrorMessage(const Char * errm)
{
  if (m_error){
    delete m_error;
    m_error = NULL;
  }
  m_error = new Char[strlen(errm)+1];
  LEAVEONNULL(m_error);
  strcpy(m_error,errm);
}

void 
MiscImageTool::SetLastErrorMessage(Char * errm)
{
  if (m_error){
    delete m_error;
    m_error = NULL;
  }
  m_error = errm;
}

HeatWaveImage * 
MiscImageTool::ReadImage(const Char * file, const Char * form, const Char * opts)
{
  jas_stream_t * in = NULL;
  jas_image_t * image = NULL;
  SInt inFmt = -1;
  Char * inOpts = NULL;
  HeatWaveImage * tmp = NULL;
  Char * temp_str = NULL;
  
  if ( file ){
    if (!(in = jas_stream_fopen(file,"rb"))){
      CpyLastErrorMessage("failed to open file");
      goto clean_up;
    }
  } 
  else{
    if (!(in = jas_stream_fdopen(0,"rb"))) {
      CpyLastErrorMessage("failed to read stdin");
      goto clean_up;
    }
  }
  if ( form ){
    NEW_ARRAY(temp_str, Char, (strlen(form)+1)); 
    strcpy(temp_str, form);
    inFmt = jas_image_strtofmt(temp_str);
    DEL_ARRAY(temp_str);
    if ( inFmt < 0 ){
      CpyLastErrorMessage("unrecognized type/format given");
      goto clean_up;
    }
  }
  if (inFmt < 0) {
    if ((inFmt = jas_image_getfmt(in)) < 0) {
      CpyLastErrorMessage("cant understand image format");
      goto clean_up;
    }
  }      
  if (!(image = jas_image_decode(in, inFmt, inOpts))) {
    CpyLastErrorMessage("failed while decoding image");
    goto clean_up;
  }
  tmp = GetHeatFromJasper(image);
  ASSERT(tmp!=NULL);  
  
 clean_up:
  if ( in ){
    jas_stream_close(in);
  }
  if ( image ){
    jas_image_destroy(image);
  }
  return tmp;
}

Bool
MiscImageTool::WriteImage(const HeatWaveImage & imgs, const Char * file, 
                          const Char * form, const Char * opts, Bool pixl)
{
  jas_stream_t * out = NULL;
  HeatWaveImage * tmpimg = NULL;
  jas_image_t * jasimg = NULL;
  SInt outfmt = -1;
  Bool ret = True;
  Char * temp_str = NULL;

  // Check image type. 
  if (form){
    NEW_ARRAY(temp_str, Char, (strlen(form)+1));
    strcpy(temp_str, form);
    outfmt = jas_image_strtofmt(temp_str);
    DEL_ARRAY(temp_str);
    if ( outfmt < 0 ){
      CpyLastErrorMessage("unrecognized image format");
      return False;
    }
  }
  if (outfmt < 0){
    NEW_ARRAY(temp_str, Char, (strlen(file)+1));
    strcpy(temp_str, file);
    outfmt = jas_image_fmtfromname(temp_str);
    DEL_ARRAY(temp_str);
    if ( outfmt < 0 ){
      CpyLastErrorMessage("unable to get format from name");
      return False;
    }
  }

  // Check output 
  if (file) {
    // The output image is to be written to a file.
    if (!(out = jas_stream_fopen(file, "w+b"))) {
      CpyLastErrorMessage("unable to open file");
      return False;
    }
  } 
  else {
    // The output image is to be written to standard output.
    if (!(out = jas_stream_fdopen(1, "w+b"))) {
      CpyLastErrorMessage("unable to use standard output");
      return False;
    }
  }
  
  if ( pixl ){
    tmpimg = imgs.GetClone();
    tmpimg->DoPixelise();
    jasimg = GetJasperFromHeat(tmpimg);
  }
  else{
    jasimg = GetJasperFromHeat(&(imgs));
  }
  
  if ( opts ){
    NEW_ARRAY(temp_str, Char, (strlen(opts)+1));
    strcpy(temp_str, opts);
  }
  
  if ((jas_image_encode(jasimg, out, outfmt, temp_str))) {
    CpyLastErrorMessage("image encoding failed");
    ret = False;
    goto clean_up;
  }
  
  if (jas_stream_close(out)) {
    CpyLastErrorMessage("failed to close image");
    ret = False;
    goto clean_up;
  }
    
 clean_up:
  DEL_ARRAY(temp_str);
  if ( jasimg ){
    jas_image_destroy(jasimg);
  }
  if ( tmpimg ){
    delete tmpimg;
  }
  return ret;
}

MiscImageTool * 
MiscImageTool::GetClone() const
{
  /** @todo implement me **/
  ASSERT ( false );
  return NULL;
}

MiscImageTool & 
MiscImageTool::operator=(const MiscImageTool & oth)
{
  /** @todo implement me **/
  ASSERT ( False );
  return (*this);
}

Bool 
MiscImageTool::operator==(const MiscImageTool & oth) const
{
  /** @todo implement me **/
  ASSERT ( False );
  return False;
}

void 
MiscImageTool::DoCleanUp()
{
  delete m_error;
}

