/****************************************************************************/
/**
 ** @file HeatWaveImage.cpp
 ** @brief Contains the HeatWaveImage class definition.
 ** @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 **
 **/

#include "HeatWaveImage.hpp"

HeatWaveImage::HeatWaveImage()
{
  memset((char*)this,0,sizeof(HeatWaveImage));
  m_desMem = True;
  m_space = SpcUnknown;
}

HeatWaveImage::HeatWaveImage(SInt tlx, SInt tly, SInt width, SInt height, 
                             EnumSpace spc, SInt num, Bool empty) 
{
  m_tlx = tlx;
  m_tly = tly;
  m_width = width;
  m_height = height;
  m_space = spc;
  m_desMem = True;
  m_compn = 0;
  m_compa = NULL;
  m_sceneSwitch = False;
  if ( empty ){
    m_compn = num;
    m_compa = new HeatWaveComponent*[num];
    LEAVEONNULL(m_compa);
    for ( SInt i = 0 ; i < m_compn ; ++ i ){
      m_compa[i] = NULL;
    }
  }
  else{
    DoCreate(num,True,0);
  }
}

HeatWaveImage::HeatWaveImage(SInt tlx, SInt tly, SInt width, SInt height, 
                             EnumSpace spc, SInt compn, 
                             HeatWaveComponent ** compa, 
                             Bool desMem, Bool cpyMem) 
{
  m_tlx = tlx;
  m_tly = tly;
  m_width = width;
  m_height = height;
  m_space = spc;
  m_desMem = desMem;
  if ( cpyMem ){
    DoCompCopy(compn,compa);
  }
  else{
    m_compn = compn;
    m_compa = compa;
  }
}

HeatWaveImage::HeatWaveImage(const HeatWaveImage & rhs)
{
  memset((char*)this,'\0',sizeof(HeatWaveImage));
  DoCopy(rhs);
}
    
HeatWaveImage::~HeatWaveImage()
{
  DoDestroy();
}

SInt 
HeatWaveImage::GetTLX() const
{
  return m_tlx;
}

void 
HeatWaveImage::SetTLX( SInt tlx )
{
  m_tlx = tlx;
}

SInt 
HeatWaveImage::GetTLY() const
{
  return m_tly;
}
  
void 
HeatWaveImage::SetTLY( SInt tly )
{
  m_tly = tly;
}

SInt 
HeatWaveImage::GetWidth() const
{
  return m_width;
}
  
void 
HeatWaveImage::SetWidth(SInt width)
{
  m_width = width;
  m_size = m_width * m_height;
}
  
SInt 
HeatWaveImage::GetHeight() const
{
  return m_height;
}
  
void 
HeatWaveImage::SetHeight(SInt height)
{
  m_height = height;
  m_size = m_width * m_height;
}

SInt
HeatWaveImage::GetTransformLevel() const
{
  return m_lev;
}

void 
HeatWaveImage::SetTransformLevel(SInt lev)
{
  m_lev = lev;
}

EnumTransform
HeatWaveImage::GetTransformType() const
{
  return m_trn;
}

void 
HeatWaveImage::SetTransformType(EnumTransform type)
{
  m_trn = type;
}
  
EnumSpace
HeatWaveImage::GetSpace() const
{
  return m_space;
}
  
Bool
HeatWaveImage::SetSpace(EnumSpace schm, Bool fast, SInt grey_choice,
                        Bool lsls)
{
  if ( fast ) {
    m_space = schm;
    return True;
  }
  if ( schm == m_space ){
    return True;
  }
  
  if ( m_space == SpcGrey ){
    // grey to RGB/YUV
    HeatWaveComponent ** m_temp = new HeatWaveComponent*[MAXCOLORSINANYSPACE];
    for (SInt i = 0 ; i < MAXCOLORSINANYSPACE ; ++i){
      m_temp[i] = new HeatWaveComponent(*m_compa[0]);
    }
    m_compa[0] = NULL;
    delete m_compa;
    m_compa = m_temp;
    if ( schm == SpcYUV ){
      m_compa[0]->SetColor(ClrY);
      m_compa[1]->SetColor(ClrU);
      m_compa[2]->SetColor(ClrV);
    }
    else {
      m_compa[0]->SetColor(ClrR);
      m_compa[1]->SetColor(ClrG);
      m_compa[2]->SetColor(ClrB);
    }
    m_space = schm;
    return True;
  }
  
  else if ( m_space == SpcRGB ){
    // RGB to grey
    if ( schm == SpcGrey ){
      // use a specific component ... 
      if ( (grey_choice >= 0) && (grey_choice < m_compn) ){
        for ( SInt i = 0 ; i < m_compn ; ++i ){
          if ( i != grey_choice ){
            ASSERT ( m_compa[i] != NULL );
            delete m_compa[i];
            m_compa[i] = NULL;
          }
          else {
            if ( i != 0 ){
              ASSERT ( m_compa[i] != NULL );
              m_compa[0] = m_compa[i];
              m_compa[i] = NULL;
            }
            m_compa[0]->SetColor(ClrGrey);  
          }
        }
      }
      else {
        // use all components ...
        DoICT_RGBtoYUV();
        m_compa[0]->SetColor(ClrGrey);
        delete m_compa[1]; m_compa[1] = NULL;
        delete m_compa[2]; m_compa[2] = NULL;
      }
      m_compn = 1;
      m_space = schm;
      return False;
    }
    // RGB to yuv
    else {
      if ( lsls && AllComponentsRelate() ){
        DoRCT_RGBtoYUV();
        m_space = schm;
        return True;
      }
      else{
        // asume only sampling differ
        for ( SInt i = 0 ; i < m_compn ; ++i ){
          m_compa[i]->SetHStep(1, true);
          m_compa[i]->SetVStep(1, true);
        }
        ASSERT ( AllComponentsRelate() );
        DoICT_RGBtoYUV();
        DoCapDataToPrec();
        m_space = schm;
        return False;
      }
    }
  }
  
  else {
    ASSERT ( m_space == SpcYUV );
    // YUV to grey
    if ( schm == SpcGrey ){
      
      // use Y for grey
      if ( grey_choice < 0 ){
        grey_choice = 0;
      }
      
      ASSERT ( ((grey_choice >= 0) && (grey_choice < m_compn)) );
        
      for ( SInt i = 0 ; i < m_compn ; ++i ){
        if ( i != grey_choice ){
          ASSERT ( m_compa[i] != NULL );
          delete m_compa[i];
          m_compa[i] = NULL;
        }
        else {
          if ( i != 0 ){
            ASSERT ( m_compa[i] != NULL );
            m_compa[0] = m_compa[i];
            m_compa[i] = NULL;
          }
          m_compa[0]->SetColor(ClrGrey);  
        }
      }
      m_compn = 1;
      m_space = schm;
      return False;
    }
    // YUV to RGB
    else {
      if ( lsls && AllComponentsRelate() ){
        DoRCT_YUVtoRGB();
        m_space = schm;
        return True;
      }
      else{
        // asume only sampling differ
        for ( SInt i = 0 ; i < m_compn ; ++i ){
          m_compa[i]->SetHStep(1, true);
          m_compa[i]->SetVStep(1, true);
        }
        ASSERT ( AllComponentsRelate() );
        DoICT_YUVtoRGB();
        DoCapDataToPrec();
        m_space = schm;
        return False;
      }
    }
  }
}
  
HeatWaveComponent ** 
HeatWaveImage::GetComponentA() const
{
  return m_compa;
}

void 
HeatWaveImage::SetComponentA(HeatWaveComponent ** compa)
{
  m_compa = compa;
}

SInt 
HeatWaveImage::GetComponentN() const
{
  return m_compn;
}
  
void 
HeatWaveImage::SetComponentN(SInt num)
{
  m_compn = num;
}

HeatWaveComponent & 
HeatWaveImage::GetComponent(SInt num) const
{
  ASSERT(num >= 0);
  ASSERT(num < m_compn);
  return (*m_compa[num]);
}

void 
HeatWaveImage::SetDesMem(Bool des)
{
  m_desMem = des;
}

Bool 
HeatWaveImage::GetDesMem() const
{
  return m_desMem;
}

void 
HeatWaveImage::SetSceneSwitch(Bool flg)
{
  m_sceneSwitch = flg; 
}

Bool 
HeatWaveImage::GetSceneSwitch() const
{
  return m_sceneSwitch;
}

void 
HeatWaveImage::SetMinPrecSgn()
{
  for ( SInt i = 0 ; i < m_compn ; ++ i){
    m_compa[i]->SetMinPrecSgn();
  }
}

HeatWaveImage *
HeatWaveImage::GetClone() const
{
  HeatWaveImage * ret = NULL;
  ret = new HeatWaveImage(*this);
  LEAVEONNULL(ret);
  return ret;
}

Bool 
HeatWaveImage::IsComparable(const HeatWaveImage & other)
{
  Bool ret = True;
  ret &= (m_compn == other.m_compn);
  ret &= (m_space == other.m_space);
  if ( ret ){
    for ( SInt i = 0 ; i < m_compn ; ++ i){
      ret &= (m_compa[i]->GetWidth() == other.m_compa[i]->GetWidth());
      ret &= (m_compa[i]->GetHeight() == other.m_compa[i]->GetHeight());
      ret &= (m_compa[i]->GetColor() == other.m_compa[i]->GetColor());
    }
  }
  return ret;
} 

Bool
HeatWaveImage::DoRCT(EnumSpace spc)
{
  ASSERT ((( spc == SpcRGB ) || (spc == SpcYUV)));
  ASSERT ( ValidateSanity() );
  
  if ( (m_space == SpcGrey) || (m_space == SpcUnknown) ){
    return False;
  }
  
  if ( !AllComponentsRelate() ){
    return False;
  }

  DoCrctClrOrd();
  
  if ( m_space == SpcRGB ){
    DoRCT_RGBtoYUV();
    m_space = SpcYUV;
  }
  else{
    DoRCT_YUVtoRGB();
    m_space = SpcRGB;
  }
  
  return True;
}

Bool
HeatWaveImage::DoICT(EnumSpace spc)
{
  ASSERT ((( spc == SpcRGB ) || (spc == SpcYUV)));
  ASSERT ( ValidateSanity() );
  
  if ( (m_space == SpcGrey) || (m_space == SpcUnknown) ){
    return False;
  }
  
  if ( !AllComponentsRelate() ){
    return False;
  }

  DoCrctClrOrd();
  
  if ( m_space == SpcRGB ){
    DoICT_RGBtoYUV();
    m_space = SpcYUV;
  }
  else{
    DoICT_YUVtoRGB();
    m_space = SpcRGB;
  }
  
  return True;
}

SInt
HeatWaveImage::DoSpectralTransform(EnumTransform trn, SInt lev, 
                                   Bool fwd, SInt cur)
{
  if ( m_compn < 2 ){
    return 0;
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
    if ( !DoSpectralTransform(fwd, trn, 0, length, 
                              True, True, True) ){
      break;
    }
    fwd ? ( ++m_lev ) : ( --m_lev );
  }
  return m_lev;
}

SInt
HeatWaveImage::DoPyramidTransform(EnumTransform trn, SInt lev, 
                                  Bool fwd, SInt cur)
{
  SInt ret = -1;
  for ( SInt i = 0; i < m_compn ; ++i ){
    ret = (m_compa[i]->DoPyramidTransform(trn,lev,fwd,cur));
  }
  return ret;
}


/*
  Bool 
  HeatWaveImage::DoInterCTransform(Bool fwd, EnumTransform trn, 
  SInt tlx, SInt tly, 
  SInt wid, SInt hei, 
  Bool prd, Bool upd,
  Bool vrt, Bool hrz, 
  Bool rng)
  {
  return True;
  }
*/

void 
HeatWaveImage::DoPixelise(SInt prec, Bool sgnd)
{
  for ( SInt i = 0 ; i < m_compn ; ++i ){
    m_compa[i]->DoPixelise(prec, sgnd);
  }
}

void
HeatWaveImage::DoCapData(SInt min, SInt max, Bool set)
{
  for ( SInt i = 0 ; i < m_compn ; ++i ){
    m_compa[i]->DoCapData(min,max,set);
  }
}

void 
HeatWaveImage::DoCapDataToPrec(){
  for ( SInt i = 0 ; i < m_compn ; ++i ){
    m_compa[i]->DoCapDataToPrec();
  }
}

void 
HeatWaveImage::DoClear(SInt rplc)
{
  for ( SInt i = 0; i < m_compn ; ++i ){
    m_compa[i]->DoClear(rplc);
  }
}

void
HeatWaveImage::DoHE()
{
  for ( SInt i = 0; i < m_compn ; ++i ){
    m_compa[i]->DoHE();
  }
}

Bool 
HeatWaveImage::AllComponentsRelate() const
{
  Bool same_memb = True;
  for ( int i = 0; i < (GetComponentN()-1) ; ++ i){
    same_memb &= (m_compa[i]->GetWidth() == m_compa[i+1]->GetWidth());
    same_memb &= (m_compa[i]->GetHeight() == m_compa[i+1]->GetHeight());
    same_memb &= (m_compa[i]->GetTLX() == m_compa[i+1]->GetTLX());
    same_memb &= (m_compa[i]->GetTLY() == m_compa[i+1]->GetTLY());
  }
  return same_memb;
}

Bool 
HeatWaveImage::operator==(const HeatWaveImage & rhs ) const
{
  Bool ret = True;
  ret &= (m_tlx == rhs.m_tlx);
  ret &= (m_tly == rhs.m_tly);
  ret &= (m_width == rhs.m_width);
  ret &= (m_height == rhs.m_height);
  ret &= (m_space == rhs.m_space);
  ret &= (m_compn == rhs.m_compn);
  if ( ret ){
    for ( SInt i = 0; i < m_compn ; ++i ){
      ret &= (*m_compa[i]) == (*rhs.m_compa[i]);
    }
  }
  return ret;
}

/* Protected functions ******************************************************/

Bool
HeatWaveImage::DoSpectralTransform(Bool fwd, EnumTransform trn, 
                                   SInt strt, SInt len, Bool prd, Bool upd, 
                                   Bool rng)
{
  ASSERT ( ((len > 0) && (len <= (strt+GetComponentN()))) );
  ASSERT ( (( strt >= 0 ) && (strt < GetComponentN())) );
  
  Smpl * data = NULL;
  SInt tlx, tly, width, height;
  if ( ! (SelectMaximumBox(tlx, tly, width, height) && GetComponentN()) ){
    return False;
  }
  void (HeatWaveLift::*func[HEATWAVELIFTMAXSTEPS])
    (Smpl *, Smpl *, SInt, SInt, Bool) const;
  NEW_ARRAY(data, Smpl, len);
  Smpl * even;
  Smpl * odd;
  SInt steps = m_lift.GetFuncArray(func, trn, fwd, prd, upd);
  for ( SInt x = tlx; x < (tlx + width); ++x ){
    for ( SInt y = tly; y < (tly + height); ++y ){
      GetSpectralVector(strt, len ,x, y, data);
      if ( fwd ){
        m_lift.Split(data, len, 1, even, odd);
      }
      else{
        even = data;
        odd = data + ((len/2)+(len%2));
      }
      for ( SInt s = 0 ; s < steps ; ++ s){
        (m_lift.*func[s])(even, odd, len, 1, fwd);
      }
      if ( !fwd ){
        m_lift.Join(data, len, 1);
      }
      SetSpectralVector(strt, len, x, y, data);
    }
  }
  DEL_ARRAY(data);
  return True;
}

void
HeatWaveImage::GetSpectralVector(SInt strt, SInt len,
                                 SInt x, SInt y, Smpl * data)
{
  ASSERT ( data );
  ASSERT ( ((len > 0) && (len <= (strt+GetComponentN()))) );
  ASSERT ( (( strt >= 0 ) && (strt < GetComponentN())) );
  
  for ( SInt i = strt ; i < (len+strt) ; ++i ){
    data[i-strt] = GetComponent(i).GetSmpl(x,y);
  }
}

void
HeatWaveImage::SetSpectralVector(SInt strt, SInt len,
                                 SInt x, SInt y, Smpl * data)
{
  ASSERT ( data );
  ASSERT ( ((len > 0) && (len <= (strt+GetComponentN()))) );
  ASSERT ( (( strt >= 0 ) && (strt < GetComponentN())) );
  
  for ( SInt i = strt ; i < (len+strt) ; ++i ){
    GetComponent(i).SetSmpl(x,y,data[i-strt]);
  }
}

SInt 
HeatWaveImage::GetLowBandLength(SInt levl)
{
  ASSERT ( levl >= 0 );
  SInt low_length = m_compn;
  for ( SInt i = 0 ; i < levl ; ++i ){
    low_length = (low_length/2) + (low_length%2);
  }
  return low_length;
}

Bool 
HeatWaveImage::SelectMaximumBox(SInt & tlx, SInt & tly,
                                SInt & width, SInt & height)
{
  ASSERT( m_compn > 0 );
  HeatWaveComponent & cmp = GetComponent(0);
  SInt min_tlx = cmp.GetTLX();
  SInt min_tly = cmp.GetTLY();
  SInt max_brx = (cmp.GetTLX()+cmp.GetWidth())-1;
  SInt max_bry = (cmp.GetTLY()+cmp.GetHeight())-1;
  for ( SInt nxt = 1 ; nxt < GetComponentN(); ++nxt ){
    cmp = GetComponent(nxt);
    min_tlx = min_tlx > cmp.GetTLX() ? min_tlx : cmp.GetTLX();
    min_tly = min_tly > cmp.GetTLY() ? min_tly : cmp.GetTLY();
    max_brx = max_brx < (cmp.GetTLX() + cmp.GetWidth() - 1) ? max_brx : 
      (cmp.GetTLX() + cmp.GetWidth() - 1);  
    max_bry = max_bry < (cmp.GetTLY() + cmp.GetHeight() - 1) ? max_bry : 
      (cmp.GetTLY() + cmp.GetHeight() - 1);  
  }
  
  if ( (min_tlx >= max_brx) || (min_tly >= max_bry) ){
    return False;
  }
  else {
    tlx = min_tlx;
    tly = min_tly;
    width = max_brx - min_tlx;
    height = max_bry - min_tly;
    return True;
  }
}

void
HeatWaveImage::DoCrctClrOrd()
{
  EnumColor order[3];
  if ( m_space == SpcRGB ){
    order[0] = ClrR;
    order[1] = ClrG;
    order[2] = ClrB;
  }
  else if ( m_space == SpcYUV ){
    order[0] = ClrY;
    order[1] = ClrU;
    order[2] = ClrV;
  }
  else {
    ASSERT ( False );
  }
  
  HeatWaveComponent * cmp;
  for ( SInt a = 2; a > 0 ; --a ){
    for ( SInt b = 0 ; b < a ; ++b ){
      if ( m_compa[b]->GetColor() != order[b] ){
        cmp = m_compa[b+1];
        m_compa[b+1] = m_compa[b];
        m_compa[b] = cmp;
      }
    }
  }
  
  ASSERT ( m_compa[0]->GetColor() == order[0]);
  ASSERT ( m_compa[1]->GetColor() == order[1]);
  ASSERT ( m_compa[2]->GetColor() == order[2]);
}

void 
HeatWaveImage::DoRCT_RGBtoYUV()
{
  SInt c_y,c_u,c_v,c_r,c_g,c_b;
  SInt comp_tlx = m_compa[0]->GetTLX();
  SInt comp_tly = m_compa[0]->GetTLY();
  SInt comp_width = comp_tlx + m_compa[0]->GetWidth();
  SInt comp_height = comp_tly + m_compa[0]->GetHeight();

  for ( SInt i = 0; i < 3 ; ++ i){
    m_compa[i]->SetPrec(m_compa[i]->GetPrec()+1);
    m_compa[i]->SetSgnd(True);
  }
  
  for ( SInt x = comp_tlx ; x < comp_width ; ++ x ){
    for ( SInt y = comp_tly ; y < comp_height ; ++ y ){
      c_r = m_compa[0]->GetSmpl(x,y);
      c_g = m_compa[1]->GetSmpl(x,y);
      c_b = m_compa[2]->GetSmpl(x,y);
      c_y = (c_r + (c_g << 1) + c_b) >> 2;
      c_u = c_b - c_g;
      c_v = c_r - c_g;
      m_compa[0]->SetSmpl(x,y,c_y);
      m_compa[1]->SetSmpl(x,y,c_u);
      m_compa[2]->SetSmpl(x,y,c_v);
    }
  }

  m_compa[0]->SetColor(ClrY);
  m_compa[1]->SetColor(ClrU);
  m_compa[2]->SetColor(ClrV);
}

void 
HeatWaveImage::DoRCT_YUVtoRGB()
{
  SInt c_y,c_u,c_v,c_r,c_g,c_b;
  SInt comp_tlx = m_compa[0]->GetTLX();
  SInt comp_tly = m_compa[0]->GetTLY();
  SInt comp_width = comp_tlx + m_compa[0]->GetWidth();
  SInt comp_height = comp_tly + m_compa[0]->GetHeight();

  for ( SInt i = 0; i < 3 ; ++ i){
    m_compa[i]->SetPrec(m_compa[i]->GetPrec()+1);
    m_compa[i]->SetSgnd(True);
  }
  
  for ( SInt x = comp_tlx ; x < comp_width ; ++ x ){
    for ( SInt y = comp_tly ; y < comp_height ; ++ y ){
      c_y = m_compa[0]->GetSmpl(x,y);
      c_u = m_compa[1]->GetSmpl(x,y);
      c_v = m_compa[2]->GetSmpl(x,y);
      c_g = c_y - ((c_u + c_v) >> 2);
      c_r = c_v + c_g;
      c_b = c_u + c_g;
      m_compa[0]->SetSmpl(x,y,c_r);
      m_compa[1]->SetSmpl(x,y,c_g);
      m_compa[2]->SetSmpl(x,y,c_b);
    }
  }

  m_compa[0]->SetColor(ClrR);
  m_compa[1]->SetColor(ClrG);
  m_compa[2]->SetColor(ClrB);
}

/* macros relating to ICT below (some ideas come from jpeg and jasper code) */

#define SCALEBITS     (16) /* should speed things up by using byte size */
#define ONE_HALF      ((SInt32) 1 << (SCALEBITS - 1))
#define FIXIT(t,x)    ((SInt32) ((t)(x) * (t)(1L << SCALEBITS) + (t)0.5L))
#define FIX(x)        (FIXIT(SFloat64,x))
#define UNFIXIT(x)    (((x)) >> SCALEBITS)

void 
HeatWaveImage::DoICT_RGBtoYUV()
{
  ASSERT(CanCT());
  enum { red = 0, green, blue};
  enum { Y = 0, Cb, Cr}; 
  SInt c_r, c_g, c_b, c_y, c_u, c_v;
  SInt offset = (1 << (m_compa[Cb]->GetPrec()-1));
  /* wieghts */
  SInt wy[] = {FIX(0.299),FIX(0.587),FIX(0.114)};
  SInt wu[] = {FIX(-0.16875),FIX(-0.33126),FIX(0.5)};
  SInt wv[] = {FIX(0.5),FIX(-0.41869),FIX(-0.08131)};
  /* assume underlying comp's are the same */
  SInt comp_tlx = m_compa[0]->GetTLX();
  SInt comp_tly = m_compa[0]->GetTLY();
  SInt comp_width = comp_tlx + m_compa[0]->GetWidth();
  SInt comp_height = comp_tly + m_compa[0]->GetHeight();
  for ( SInt x = comp_tlx ; x < comp_width ; ++ x ){
    for ( SInt y = comp_tly ; y < comp_height ; ++ y ){
      c_r = m_compa[red]->GetSmpl(x,y);
      c_g = m_compa[green]->GetSmpl(x,y);
      c_b = m_compa[blue]->GetSmpl(x,y);
      c_y = (c_r*wy[red])+(c_g*wy[green])+(c_b*wy[blue]);
      c_u = (c_r*wu[red])+(c_g*wu[green])+(c_b*wu[blue]);
      c_v = (c_r*wv[red])+(c_g*wv[green])+(c_b*wv[blue]);
      c_y = UNFIXIT((c_r*wy[red])+(c_g*wy[green])+(c_b*wy[blue]));
      c_u = UNFIXIT((c_r*wu[red])+(c_g*wu[green])+(c_b*wu[blue]));
      c_v = UNFIXIT((c_r*wv[red])+(c_g*wv[green])+(c_b*wv[blue]));
      m_compa[Y]->SetSmpl (x,y,c_y);
      m_compa[Cb]->SetSmpl(x,y,c_u+offset);
      m_compa[Cr]->SetSmpl(x,y,c_v+offset);
    }
  }
  m_compa[Y]->SetColor(ClrY);
  m_compa[Cb]->SetColor(ClrU);
  m_compa[Cr]->SetColor(ClrV);
}

void 
HeatWaveImage::DoICT_YUVtoRGB()
{
  ASSERT(CanCT());
  enum { red = 0, green, blue};
  enum { Y = 0, Cb, Cr}; 
  SInt c_y, c_u, c_v, c_r, c_g, c_b;
  SInt offset = (1 << (m_compa[Cb]->GetPrec()-1));
  /* wieghts */
  /* inverse matrix */
  SInt wr[] = {FIX(+1.000),FIX(+0.00000),FIX(+1.40200)};
  SInt wg[] = {FIX(+1.000),FIX(-0.34413),FIX(-0.71414)};
  SInt wb[] = {FIX(+1.000),FIX(+1.77200),FIX(+0.00000)};
  /* precalculated matrix ( where the sums add up )*/
  /*
    SInt wr[] = {19595,38470,7471};
    SInt wg[] = {-11059,-21709,32768};
    SInt wb[] = {32768,-27439,-5329};
    ASSERT ( (wr[0]+wr[1]+wr[2]) == 65536 );
    ASSERT ( (wg[0]+wg[1]+wg[2]) == 0 );
    ASSERT ( (wb[0]+wb[1]+wb[2]) == 0 );
  */
  /* better aproximation of inverse matrix */
  /*
    SInt wr[] = {FIX(+0.9999823),FIX(-0.0000072),FIX(+1.4019727)};
    SInt wg[] = {FIX(+0.9999788),FIX(-0.3441270),FIX(-0.7141302)};
    SInt wb[] = {FIX(+1.0000000),FIX(+1.7719711),FIX(+0.0000402)};
  */
  /* another matrix with some scaling */
  /*
    SInt wr[] = {FIX(+0.5643),FIX(+0.0000),FIX(+0.7912)};
    SInt wg[] = {FIX(+0.5643),FIX(-0.1942),FIX(-0.4030)};
    SInt wb[] = {FIX(+0.5643),FIX(+1.0000),FIX(+0.0000)};
  */
  /* assume underlying comp's are the same */
  SInt comp_tlx = m_compa[0]->GetTLX();
  SInt comp_tly = m_compa[0]->GetTLY();
  SInt comp_width = comp_tlx + m_compa[0]->GetWidth();
  SInt comp_height = comp_tly + m_compa[0]->GetHeight();
  for ( SInt x = comp_tlx ; x < comp_width ; ++ x ){
    for ( SInt y = comp_tly ; y < comp_height ; ++ y ){
      c_y = m_compa[Y]->GetSmpl(x,y);
      c_u = m_compa[Cb]->GetSmpl(x,y)-offset;
      c_v = m_compa[Cr]->GetSmpl(x,y)-offset;
      c_r = (c_y*wr[Y])+  /*(c_u*wy[Cb])   +*/ (c_v*wr[Cr])   + ONE_HALF;
      c_g = (c_y*wg[Y])+    (c_u*wg[Cb])   +   (c_v*wg[Cr])   + ONE_HALF;
      c_b = (c_y*wb[Y])+    (c_u*wb[Cb]) /*+   (c_v*wb[Cr])*/ + ONE_HALF;
      c_r = UNFIXIT(c_r);
      c_g = UNFIXIT(c_g);
      c_b = UNFIXIT(c_b);
      m_compa[red]->SetSmpl(x,y,c_r);
      m_compa[green]->SetSmpl(x,y,c_g);
      m_compa[blue]->SetSmpl(x,y,c_b);
    }
  }
  m_compa[red]->SetColor(ClrR);
  m_compa[green]->SetColor(ClrG);
  m_compa[blue]->SetColor(ClrB);
}

Bool 
HeatWaveImage::CanCT() const
{
  if (m_compn < 3){
    return False;
  }
  else{
    Bool ret = True;
    for ( SInt i = 0 ; i < 2 ; ++i ){
      ret &= (m_compa[i]->GetTLX() == m_compa[i+1]->GetTLX());
      ret &= (m_compa[i]->GetTLY() == m_compa[i+1]->GetTLY());
      ret &= (m_compa[i]->GetWidth() == m_compa[i+1]->GetWidth());
      ret &= (m_compa[i]->GetHeight() == m_compa[i+1]->GetHeight());      
    }
    return ret;
  }
}

/* old code
void 
HeatWaveImage::DoICT_RGBtoYUV()
{
SFloat64 c_y,c_u,c_v,c_r,c_g,c_b;
SInt comp_tlx = m_compa[0]->GetTLX();
  SInt comp_tly = m_compa[0]->GetTLY();
  SInt comp_width = comp_tlx + m_compa[0]->GetWidth();
  SInt comp_height = comp_tly + m_compa[0]->GetHeight();

  for ( SInt i = 0; i < 3 ; ++ i){
    m_compa[i]->SetPrec(m_compa[i]->GetPrec()+1);
    m_compa[i]->SetSgnd(True);
  }
  
  for ( SInt x = comp_tlx ; x < comp_width ; ++ x ){
    for ( SInt y = comp_tly ; y < comp_height ; ++ y ){
      c_r = (SFloat64)m_compa[0]->GetSmpl(x,y);
      c_g = (SFloat64)m_compa[1]->GetSmpl(x,y);
      c_b = (SFloat64)m_compa[2]->GetSmpl(x,y);
      c_y = (c_r * 0.299) + (c_g * 0.587) + (c_b * 0.114);
      c_u = (c_r * -0.16875) + (c_g * -0.33126) + (c_b * 0.5);
      c_v = (c_r * 0.5) + (c_g * -0.41869) + (c_b * -0.08131);
      m_compa[0]->SetSmpl(x,y,(Smpl)c_y);
      m_compa[1]->SetSmpl(x,y,(Smpl)c_u);
      m_compa[2]->SetSmpl(x,y,(Smpl)c_v);
    }
  }

  m_compa[0]->SetColor(ClrY);
  m_compa[1]->SetColor(ClrU);
  m_compa[2]->SetColor(ClrV);
}

void 
HeatWaveImage::DoICT_YUVtoRGB()
{
  SFloat64 c_y,c_u,c_v,c_r,c_g,c_b;
  SInt comp_tlx = m_compa[0]->GetTLX();
  SInt comp_tly = m_compa[0]->GetTLY();
  SInt comp_width = comp_tlx + m_compa[0]->GetWidth();
  SInt comp_height = comp_tly + m_compa[0]->GetHeight();

  for ( SInt i = 0; i < 3 ; ++ i){
    m_compa[i]->SetPrec(m_compa[i]->GetPrec()+1);
    m_compa[i]->SetSgnd(True);
  }
  
  for ( SInt x = comp_tlx ; x < comp_width ; ++ x ){
    for ( SInt y = comp_tly ; y < comp_height ; ++ y ){
      c_y = (SFloat64)m_compa[0]->GetSmpl(x,y);
      c_u = (SFloat64)m_compa[1]->GetSmpl(x,y);
      c_v = (SFloat64)m_compa[2]->GetSmpl(x,y);
      c_r = (c_y * 1.0) + (c_u * 0.0) + (c_v * 1.402);
      c_g = (c_y * 1.0) - (c_u * 0.34413) - (c_v * 0.71414);
      c_b = (c_y * 1.0) + (c_u * 1.772) + (c_v * 0.0);
      m_compa[0]->SetSmpl(x,y,c_r);
      m_compa[1]->SetSmpl(x,y,c_g);
      m_compa[2]->SetSmpl(x,y,c_b);
    }
  }

  m_compa[0]->SetColor(ClrR);
  m_compa[1]->SetColor(ClrG);
  m_compa[2]->SetColor(ClrB);
}
*/
  
Bool 
HeatWaveImage::ValidateSanity() const
{
  Bool ret = True;
  SInt clr_n = 0;
  EnumColor clr_ok[3];
  Bool clr_found[3] = {False,False,False};
  
  switch ( m_space ){
  case SpcGrey:
    clr_n = 1;
    clr_ok[0] = ClrGrey;
    break;
  case SpcRGB:
    clr_n = 3;
    clr_ok[0] = ClrR;
    clr_ok[1] = ClrG;
    clr_ok[2] = ClrG;
    break;
  case SpcYUV:
    clr_n = 3;
    clr_ok[0] = ClrY;
    clr_ok[1] = ClrU;
    clr_ok[2] = ClrV;
    break;
  default:
    ret = false;
  }
  
  ret &= ( clr_n == m_compn );
  
  if ( !ret ){
    return ret;
  }
  
  for ( SInt i = 0 ; i < clr_n ; ++ i ){
    ret &= ( m_compa[i] != NULL );
  };
  
  if ( !ret ){
    return ret;
  }
  
  for ( SInt n = 0 ; n < clr_n ; ++ n ){
    for ( SInt i = 0 ; i < clr_n ; ++ i ){
      clr_found[n] |= ( m_compa[i]->GetColor() == clr_ok[n] );
    };
  };
  
  for ( SInt i = 0 ; i < clr_n ; ++ i ){
    ret &= clr_found[i];
  };
  
  return ret;
}

void 
HeatWaveImage::DoCreate(SInt num, Bool set, Smpl def)
{
  ASSERT ( m_compn == 0 );
  ASSERT ( m_compa == NULL );
  ASSERT ( num > 0 );
  ASSERT ( m_width > 0 );
  ASSERT ( m_height > 0 );
  m_compa = new HeatWaveComponent*[num];
  LEAVEONNULL(m_compa);
  for ( SInt i = 0; i < num ; ++ i){
    m_compa[i] = new HeatWaveComponent();
    LEAVEONNULL(m_compa[i]);
    m_compa[i]->SetTLX(m_tlx);
    m_compa[i]->SetTLY(m_tly);
    m_compa[i]->SetHStep(1);
    m_compa[i]->SetVStep(1);
    m_compa[i]->SetSize(m_width, m_height, def);
    m_compa[i]->SetColor(ClrGrey);
  }
  m_compn = num;
}

void 
HeatWaveImage::DoCopy(const HeatWaveImage & rhs)
{
  DoDestroy();
  memcpy((char*)this,(char*)&rhs,sizeof(HeatWaveImage));
  m_compa = NULL;
  m_compa = NULL;
  DoCompCopy(rhs.m_compn,rhs.m_compa);
}

void
HeatWaveImage::DoCompCopy(SInt compn, HeatWaveComponent ** compa)
{
  ASSERT ( m_compa == NULL );
  m_compn = compn;
  m_compa = new HeatWaveComponent*[compn];
  LEAVEONNULL(m_compa);
  for ( SInt i = 0 ; i < m_compn ; ++i ){
    m_compa[i] = compa[i]->GetClone();
    LEAVEONNULL(m_compa[i]);
  }
}

void HeatWaveImage::DoDestroy()
{
  if ( m_desMem ) {
    for ( SInt i = 0 ; i < m_compn ; ++i ){
      delete m_compa[i];
      m_compa[i] = NULL;
    }
    delete [] m_compa;
    m_compa = NULL;
  }
  else{
    m_compa = NULL;
  }
}
