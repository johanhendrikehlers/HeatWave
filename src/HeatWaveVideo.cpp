/****************************************************************************/
/**
 ** @file   HeatWaveVideo.cpp
 ** @brief  Contains the HeatWaveVideo class definition.
 ** @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 **
 **/

#include "HeatWaveVideo.hpp"

HeatWaveVideo::HeatWaveVideo()
{
  m_width = 0;
  m_height = 0;
  m_space = SpcUnknown;
  m_imgn = 0;
  m_imga = NULL;
  for ( SInt i = 0 ; i < MAXCOLORSINANYSPACE ; ++i ){
    m_hsp[i]=1;
    m_vsp[i]=1;
  }
  m_desMem = True;
}

HeatWaveVideo::HeatWaveVideo(SInt wid, SInt hei, SInt img, EnumSpace spc, 
                             SInt * hsp, SInt * vsp)
{
  m_width = 0;
  m_height = 0;
  m_space = SpcUnknown;
  m_imgn = 0;
  m_imga = NULL;
  for ( SInt i = 0 ; i < MAXCOLORSINANYSPACE ; ++i ){
    m_hsp[i]=1;
    m_vsp[i]=1;
  }
  m_desMem = True;
  SetWidth(wid);
  SetHeight(hei);
  SetSpace(spc);
  SetHSPeriods(hsp);
  SetVSPeriods(vsp);
  DoCreate(img);
}

HeatWaveVideo::HeatWaveVideo(SInt wid, SInt hei, EnumSpace spc, SInt * hsp, 
                             SInt * vsp, SInt num, HeatWaveImage ** arr, 
                             Bool des, Bool cpy)
{
  m_width = 0;
  m_height = 0;
  m_space = SpcUnknown;
  m_imgn = 0;
  m_imga = NULL;
  for ( SInt i = 0 ; i < MAXCOLORSINANYSPACE ; ++i ){
    m_hsp[i]=1;
    m_vsp[i]=1;
  }
  m_desMem = des;
  SetWidth(wid);
  SetHeight(hei);
  SetSpace(spc);
  SetHSPeriods(hsp);
  SetVSPeriods(vsp);
  if ( cpy ){
    DoImageCopy(num,arr);
  }
  else{
    m_imgn = num;
    m_imga = arr;
  }
}

HeatWaveVideo::HeatWaveVideo(const HeatWaveVideo & rhs)
{
  m_width = 0;
  m_height = 0;
  m_desMem = True;
  m_space = SpcUnknown;
  m_imgn = 0;
  m_imga = NULL;
  DoCopy(rhs);
}
    
HeatWaveVideo::~HeatWaveVideo()
{
  DoDestroy();
}

SInt 
HeatWaveVideo::GetWidth() const
{
  return m_width;
}
  
void 
HeatWaveVideo::SetWidth(SInt width)
{
  m_width = width;
}
  
SInt 
HeatWaveVideo::GetHeight() const
{
  return m_height;
}
  
void 
HeatWaveVideo::SetHeight(SInt height)
{
  m_height = height;
}
  
EnumSpace
HeatWaveVideo::GetSpace() const
{
  return m_space;
}
  
Bool
HeatWaveVideo::SetSpace(EnumSpace schm, Bool fast, SInt grey_choice,
                        Bool lsls)
{
  m_space = schm;
  if ( fast ){
    return True;
  }
  Bool ret = True;
  for ( SInt i = 0; i < GetImageN() ; ++i ){
    ret &= GetImage(i).SetSpace(schm,fast,grey_choice,lsls);
  }
  return ret;
}
  
HeatWaveImage ** 
HeatWaveVideo::GetImageA() const
{
  return m_imga;
}

void 
HeatWaveVideo::SetImageA(HeatWaveImage ** compa)
{
  m_imga = compa;
}

SInt 
HeatWaveVideo::GetImageN() const
{
  return m_imgn;
}
  
void 
HeatWaveVideo::SetImageN(SInt num)
{
  m_imgn = num;
}

HeatWaveImage & 
HeatWaveVideo::GetImage(SInt num) const
{
  ASSERT(num >= 0);
  ASSERT(num < m_imgn);
  return (*m_imga[num]);
}

HeatWaveComponent & 
HeatWaveVideo::GetComponent(SInt num, SInt com) const
{
  ASSERT ( CheckComponent(num, com) );
  return GetImage(num).GetComponent(com);
}

Bool 
HeatWaveVideo::CheckImage(SInt num) const
{
  if ((num >= m_imgn) || (num < 0)){
    return False;
  }
  else{
    return True;
  }
}

Bool 
HeatWaveVideo::CheckComponent(SInt num, SInt com) const
{
  if ( CheckImage(num) ){
    if ((com >= GetImage(num).GetComponentN()) || (com < 0)) {
      return False;
    }
    else {
      return True;
    }
  }
  else {
    return False;
  }
}

void
HeatWaveVideo::AddImage(HeatWaveImage * imge)
{
  if ( imge == NULL ){
    return;
  }
  ASSERT ( HEATWAVEVECTORGROWTH > 0 );
  if ((m_imgn % HEATWAVEVECTORGROWTH) == 0 ) {
    HeatWaveImage ** tmp = m_imga;
    m_imga= new HeatWaveImage*[m_imgn+HEATWAVEVECTORGROWTH];
    LEAVEONNULL(m_imga);
    memcpy(m_imga,tmp,m_imgn*sizeof(HeatWaveImage*));
    delete tmp;
  }
  
  m_imga[m_imgn] = imge;
  ++m_imgn;
}

void 
HeatWaveVideo::CpyImage(const HeatWaveImage & imge)
{
  AddImage(imge.GetClone());
}
  
const SInt * 
HeatWaveVideo::GetHSPeriods() const
{
  return m_hsp;
}

void 
HeatWaveVideo::SetHSPeriods(SInt * hsp)
{
  if ( hsp ) {
    for ( SInt i = 0; i < MAXCOLORSINANYSPACE ; ++i ){
      m_hsp[i] = hsp[i];
    }
  }
}

const SInt * 
HeatWaveVideo::GetVSPeriods() const
{
  return m_vsp;
}
  
void 
HeatWaveVideo::SetVSPeriods(SInt * vsp)
{
  if ( vsp ) {
    for ( SInt i = 0; i < MAXCOLORSINANYSPACE ; ++i ){
      m_vsp[i] = vsp[i];
    }
  }
}

SInt
HeatWaveVideo::GetTransformLevel() const
{
  return m_lev;
}

void 
HeatWaveVideo::SetTransformLevel(SInt lev)
{
  m_lev = lev;
}

EnumTransform
HeatWaveVideo::GetTransformType() const
{
  return m_trn;
}

void 
HeatWaveVideo::SetTransformType(EnumTransform type)
{
  m_trn = type;
}

void 
HeatWaveVideo::SetMinPrecSgn()
{
  for ( SInt i = 0 ; i < m_imgn ; ++i ){
    GetImage(i).SetMinPrecSgn();
  }
}

HeatWaveVideo *
HeatWaveVideo::GetClone() const
{
  HeatWaveVideo * ret = NULL;
  ret = new HeatWaveVideo(*this);
  LEAVEONNULL(ret);
  return ret;
}

Bool 
HeatWaveVideo::IsComparable(const HeatWaveVideo & other)
{
  /** @todo Implement me! **/
  return True;
}

Bool 
HeatWaveVideo::IsSpatiallyComparable() const
{
  for ( SInt i = 1 ; i < m_imgn ; ++i ){
    if ( !GetImage(0).IsComparable(GetImage(i)) ){
      return False;
    }
  }
  return True;
}

SInt
HeatWaveVideo::DoSpatialTransform(EnumTransform trn, SInt lev, Bool fwd, 
                                  SInt cur)
{
  SInt ret = 0;
  for ( SInt i = 0 ; i < m_imgn ; ++i ){
    ret = GetImage(i).DoPyramidTransform(trn,lev,fwd,cur);
  }
  return ret;
}

SInt 
HeatWaveVideo::DoTemporalTransform(EnumTransform trn, SInt lev, Bool fwd, 
                                   SInt cur)
{
  if ( !IsSpatiallyComparable() || (m_imgn < 2) ){
    return -1;
  }
  SInt ret;
  for ( SInt i = 0 ; i < GetImage(0).GetComponentN() ; ++i ){
    ret = DoComponentTransform(GetImage(0).GetComponent(i).GetColor(),
                               trn,lev,fwd,cur);
  }
  return ret;
}

SInt 
HeatWaveVideo::DoComponentTransform(EnumColor clr, EnumTransform trn, 
                                    SInt lev, Bool fwd, SInt cur)
{
  if ( !IsSpatiallyComparable() || (m_imgn < 2) ){
    return -1;
  }
  SInt cmp = -2;
  for ( SInt c = 0 ; c < (m_imga[0])->GetComponentN() ; ++c ){
    if ( clr == m_imga[0]->GetComponent(c).GetColor() ){
      cmp = c;
    }	
  }
  if ( cmp < 0 ){
    return -2;
  }
  if ( lev < 0 ){
    lev = 0;
  }
  if ( cur >= 0 ){
    m_lev = cur;
  }
  if ( fwd ){
    if ( m_lev >= lev ){
      return m_lev;
    }
  }
  else{
    if ( m_lev <= lev ){
      return m_lev;
    }
  }
  while ( m_lev != lev ){
    SInt length = GetLowBandLength(m_lev-(fwd?0:1));
    if ( fwd && (length <= 1) ){
      break;
    }
    if ( !DoTemporalTransform(cmp, fwd, trn, 0, length, 
                              True, True, True) ){
      break;
    }
    fwd ? ( ++m_lev ) : ( --m_lev );
  }
  return m_lev;
}

void 
HeatWaveVideo::DoAppendVideo(HeatWaveVideo *& oth)
{
  if ( oth == NULL ){
    return;
  }
  HeatWaveImage ** img = oth->GetImageA();
  for ( SInt i = 0; i < oth->GetImageN() ; ++i ){
    AddImage(img[i]);
    img[i] = NULL;
  }
  delete oth;
  oth = NULL;
}

void 
HeatWaveVideo::DoHE()
{
  for ( SInt i = 0; i < GetImageN() ; ++i ){
    m_imga[i]->DoHE();
  }
}

HeatWaveImage *
HeatWaveVideo::GetFusedImage()
{
  if ( !IsSpatiallyComparable() || (m_imgn == 0) ){
    return NULL;
  }
  
  HeatWaveImage * ret = m_imga[0]->GetClone();
  ret->DoClear();
  
  // Do each component
  for ( SInt c = 0 ; c < ret->GetComponentN() ; ++c){
    HeatWaveComponent & cmp = ret->GetComponent(c);
    cmp.SetPrec(24);
    SInt x,y,w,h;
    SInt level = cmp.GetTransformLevel();
    cmp.GetSubbandInfo(level,SubLL,x,y,w,h);
    SFloat64 ** SFloat64_img = NewMatrixSFloat64(w, h, 0);
    
    for ( SInt imgs = 0; imgs < m_imgn ; ++imgs ){
      HeatWaveComponent & cmp_two = m_imga[imgs]->GetComponent(c);
      for (SInt x_i = x; x_i < (x+w); ++x_i){
        for (SInt y_i = y; y_i < (y+h); ++y_i){
          SFloat64 sample = (SFloat64)cmp_two.GetSmpl(x_i,y_i);
          SFloat64_img[(x_i-x)][(y_i-y)] += (sample/(SFloat64)m_imgn);
        }
      }
      
      for (SInt x_i = x; x_i < cmp.GetWidth(); x_i++){
        for (SInt y_i = y; y_i < cmp.GetHeight(); y_i++){
          SInt current = cmp.GetSmpl(x_i,y_i);
          SInt other = cmp_two.GetSmpl(x_i,y_i);
          if ( abs(other) > abs(current) ){
            cmp.SetSmpl(x_i,y_i,other);
          }
        }
      }  
    }
    
    for (SInt x_i = x; x_i < (x+w); x_i++){
      for (SInt y_i = y; y_i < (h+y); y_i++){
        SInt sample = (SInt)SFloat64_img[(x_i-x)][(y_i-y)] ;
        cmp.SetSmpl(x_i,y_i,sample);
      }
    }
    DelMatrixSFloat64(SFloat64_img);
    cmp.SetMinPrecSgn();
  }
  return ret;
}

HeatWaveVideo& 
HeatWaveVideo::operator=(const HeatWaveVideo& rhs)
{
  DoCopy(rhs);
  return (*this);
}

Bool 
HeatWaveVideo::operator==(const HeatWaveVideo & rhs ) const
{
  Bool ret = True;
  ret &= (m_width == rhs.m_width);
  ret &= (m_height == rhs.m_height);
  ret &= (m_space == rhs.m_space);
  ret &= (m_imgn == rhs.m_imgn);
  for ( SInt i = 0 ; i < MAXCOLORSINANYSPACE ; ++i ){
    ret &= (m_hsp[i] == rhs.m_hsp[i]);
    ret &= (m_vsp[i] == rhs.m_vsp[i]);
  }     
  if ( m_imga == NULL ){
    if ( rhs.m_imga != NULL ){
      return False;
    }
  }
  else if ( ret ){
    for ( SInt i = 0; i < m_imgn ; ++i ){
      if ( m_imga[i] == NULL ){
        if (rhs.m_imga[i] == NULL){
          continue;
        }
        else {
          return False;
        }      
      }        
      ret &= (*m_imga[i]) == (*rhs.m_imga[i]);
      if (!ret ){
        return ret;
      }
    }
  }
  return ret;
}

/* Protected functions ******************************************************/

Bool
HeatWaveVideo::DoTemporalTransform(SInt cmp, Bool fwd, EnumTransform trn, 
                                   SInt strt, SInt len, Bool prd, Bool upd, 
                                   Bool rng)
{
  if ( ! ( ( m_imgn > 0 ) && ( m_imga[0]->GetComponentN() > cmp ) &&
           ( IsSpatiallyComparable() ) && ( len >= 1 ) && ( strt >= 0 ) &&
           ( (strt+len) <= m_imgn ) ) ){
    return False;
  }
  void (HeatWaveLift::*func[HEATWAVELIFTMAXSTEPS])
    (Smpl *, Smpl *, SInt, SInt, Bool)const;
  Smpl * data = new Smpl[len];
  LEAVEONNULL(data);
  Smpl * even;
  Smpl * odd;
  SInt steps = m_lift.GetFuncArray(func,trn,fwd,prd,upd);
  SInt tlx = m_imga[0]->GetComponent(cmp).GetTLX();
  SInt tly = m_imga[0]->GetComponent(cmp).GetTLY();
  SInt width = m_imga[0]->GetComponent(cmp).GetWidth();
  SInt height = m_imga[0]->GetComponent(cmp).GetHeight();
  for ( SInt x = tlx; x < (tlx+width); ++x ){
    for ( SInt y = tly; y < (tly+height); ++y ){
      GetTemporalVector(cmp,strt,len,x,y,data);
      if ( fwd ){
        m_lift.Split(data, len, 1, even, odd);
      }
      else{
        even = data;
        odd = data + ((len/2)+(len%2));
      }
      for ( SInt s = 0 ; s < steps ; ++ s){
        (m_lift.*func[s])(even,odd,len,1,fwd);
      }
      if ( !fwd ){
        m_lift.Join(data,len,1);
      }
      SetTemporalVector(cmp,strt,len,x,y,data);
    }
  }
  delete data;
  return True;
}

void
HeatWaveVideo::GetTemporalVector(SInt cmp, SInt strt, SInt len,
                                 SInt x, SInt y, Smpl * data)
{
  for ( SInt i = strt ; i < (strt + len) ; ++i ){
    if ( m_imga[i]->GetComponent(cmp).ValidateCoords(x,y)){
      data[i-strt] = m_imga[i]->GetComponent(cmp).GetSmpl(x,y);
    }
    else{
      data[i-strt] = 0;
    }
  }
}

void
HeatWaveVideo::SetTemporalVector(SInt cmp, SInt strt, SInt len,
                                 SInt x, SInt y, Smpl * data)
{
  for ( SInt i = strt ; i < (strt + len) ; ++i ){
    if ( m_imga[i]->GetComponent(cmp).ValidateCoords(x,y)){
      m_imga[i]->GetComponent(cmp).SetSmpl(x,y,data[i-strt]);
    }
  }
}

SInt 
HeatWaveVideo::GetLowBandLength(SInt levl)
{
  ASSERT ( levl >= 0 );
  SInt low_length = m_imgn;
  for ( SInt i = 0 ; i < levl ; ++i ){
    low_length = (low_length/2) + (low_length%2);
  }
  return low_length;
}

Bool 
HeatWaveVideo::ValidateSamplingPeriods() const
{
  Bool ret = True;
  for ( SInt i = 0 ; i < MAXCOLORSINANYSPACE ; ++i ){
    ret &= (m_hsp[i] > 0);
    ret &= (m_vsp[i] > 0);
    if ( ret ){
      ret &= (((m_width)%(m_hsp[i]))==0);
      ret &= (((m_height)%(m_vsp[i]))==0);
    }
    else{
      break;
    }
  }
  return ret;
}

void 
HeatWaveVideo::DoCreate(SInt num)
{
  HeatWaveComponent ** arr;
  ASSERT ( m_imgn == 0 );
  ASSERT ( m_imga == NULL );
  ASSERT ( num > 0 );
  ASSERT ( m_width > 0 );
  ASSERT ( m_height > 0 );
  ASSERT ( ValidateSamplingPeriods() );
  m_imga = new HeatWaveImage*[num];
  LEAVEONNULL(m_imga);
  for ( SInt i = 0; i < num ; ++ i){
    if ( m_space == SpcGrey ){
      m_imga[i] = new HeatWaveImage(0,0,m_width,m_height,SpcGrey,1,True);
      LEAVEONNULL(m_imga[i]);
      arr = m_imga[i]->GetComponentA();
      arr[0] = new HeatWaveComponent(0,0,1,1,m_width,m_height,False,8,ClrGrey);
      LEAVEONNULL(arr[0]);
    }
    else if ( m_space == SpcRGB || m_space == SpcYUV ){
      m_imga[i] = new HeatWaveImage(0,0,m_width,m_height,m_space,3,True);
      LEAVEONNULL(m_imga[i]);
      arr = m_imga[i]->GetComponentA();
      for ( SInt i = 0 ; i < 3 ; ++ i ){
        arr[i] = new HeatWaveComponent(0,0,m_hsp[0],m_vsp[0],m_width/m_hsp[0],
                                       m_height/m_vsp[0],False,8,ClrGrey);
        LEAVEONNULL(arr[i]);
      }
      if ( m_space == SpcRGB ){
        arr[0]->SetColor(ClrR);
        arr[1]->SetColor(ClrG);
        arr[2]->SetColor(ClrB);
      }
      else{
        arr[0]->SetColor(ClrY);
        arr[1]->SetColor(ClrU);
        arr[2]->SetColor(ClrV);
      }
    }
    else{
      ASSERT(False); 
    }
  }
  m_imgn = num;
}

void 
HeatWaveVideo::DoCopy(const HeatWaveVideo & rhs)
{
  ASSERT(rhs.ValidateSamplingPeriods());
  m_width = rhs.m_width;
  m_height = rhs.m_height;
  m_desMem = rhs.m_desMem;
  m_space = rhs.m_space;
  for ( SInt i = 0 ; i < MAXCOLORSINANYSPACE ; ++i ){
    m_hsp[i] = rhs.m_hsp[i];
  }
  DoDestroy();
  DoImageCopy(rhs.m_imgn,rhs.m_imga);
}

void
HeatWaveVideo::DoImageCopy(SInt imgn, HeatWaveImage ** imga)
{
  ASSERT ( m_imga == NULL );
  ASSERT ( m_imgn == 0 );
  m_imgn = imgn;
  m_imga = new HeatWaveImage*[imgn];
  LEAVEONNULL(m_imga);
  
  for ( SInt i = 0 ; i < m_imgn ; ++i ){
    m_imga[i] = imga[i]->GetClone();
    LEAVEONNULL(m_imga[i]);
  }
}

void HeatWaveVideo::DoDestroy()
{
  if ( m_desMem ) {
    for ( SInt i = 0 ; i < m_imgn ; ++i ){
      delete m_imga[i];
      m_imga[i] = NULL;
    }
    delete [] m_imga;
    m_imga = NULL;
  }
  else{
    m_imga = NULL;
  }
}
