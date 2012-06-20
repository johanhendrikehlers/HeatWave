/****************************************************************************/
/**
 ** @file HeatWaveComponent.cpp
 ** @brief Contains the HeatWaveComponent class definitions.
 ** @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 **
 **/

#include "HeatWaveComponent.hpp"

HeatWaveComponent::HeatWaveComponent()
{
  m_tlx = 0;
  m_tly = 0;
  m_hstep = 1;
  m_vstep = 1;
  m_width = 0;
  m_height = 0;
  m_sgnd = False;
  m_prec = 8;
  m_clr = ClrGrey;
  m_trn = Trn0_0;
  m_lev = 0;
  m_data = NULL;
  m_rows = NULL;
  m_desMem = True;
  m_size = 0;
}

HeatWaveComponent::HeatWaveComponent(SInt tlx, SInt tly, SInt hstep, 
                                     SInt vstep, SInt width, SInt height, 
                                     Bool sgnd, SInt prec, EnumColor clr)
{
  m_tlx = tlx;
  m_tly = tly;
  m_hstep = hstep;
  m_vstep = vstep;
  m_sgnd = sgnd;
  m_prec = prec;
  m_clr = clr;
  m_data = NULL;
  m_rows = NULL;
  m_desMem = True;
  m_size = width*height;
  m_width = 0;
  m_height = 0;
  DoResize(width,height,False,0,True);
  SetRowPtrs();
  m_trn = Trn0_0;
  m_lev = 0;
}

HeatWaveComponent::HeatWaveComponent(SInt tlx, SInt tly, 
                                     SInt hstep, SInt vstep, 
                                     SInt width, SInt height, Bool sgnd, 
                                     SInt prec, EnumColor clr, Smpl * data, 
                                     Smpl ** rows, Bool desMem, Bool cpyMem)
{
  m_tlx = tlx;
  m_tly = tly;
  m_hstep = hstep;
  m_vstep = vstep;
  m_width = width;
  m_height = height;
  m_sgnd = sgnd;
  m_prec = prec;
  m_clr = clr;
  m_data = data;
  m_rows = rows;
  m_desMem = desMem;
  m_size = width*height;
  
  if ( cpyMem ) {
    m_data = NULL;
    m_rows = NULL;
    DoCreate(width,height);
    ASSERT ( data != NULL );
    for ( SInt i = 0; i < m_size; i++ ){
      m_data[i] = data[i];
    }
  }
  else{
    m_data = data;
    m_rows = rows;
  }

  SetRowPtrs();
  m_trn = Trn0_0;
  m_lev = 0;
}

HeatWaveComponent::HeatWaveComponent(const HeatWaveComponent & rhs)
{
  DoCopy(rhs);
}

HeatWaveComponent::~HeatWaveComponent()
{
  DoDestroy();
}

SInt 
HeatWaveComponent::GetTLX() const
{
  return m_tlx;
}

void 
HeatWaveComponent::SetTLX( SInt tlx )
{
  m_tlx = tlx;
}

SInt 
HeatWaveComponent::GetTLY() const
{
  return m_tly;
}

void 
HeatWaveComponent::SetTLY( SInt tly )
{
  m_tly = tly;
}

SInt
HeatWaveComponent::GetHStep() const
{
  return m_hstep;
}

void 
HeatWaveComponent::SetHStep(SInt hstep, Bool rsze, EnumTransform trns)
{
  ASSERT ( hstep >= 1 );
  if ( (!rsze) ){
    m_hstep = hstep;
  }
  else{
    //SetMinPrecSgn();
    //SInt prec = GetPrec();
    if ( hstep > m_hstep ){
      // reduce size
      for ( SInt i = (hstep-m_hstep); i > 0 ; --i){
        DoTransform(true, trns, m_tlx, m_tly, m_width, m_height, true, true,
                    false, true, true);
        SInt x, y, width, height; // only width nb!
        GetSubbandInfo ( 1, SubLL, x, y, width, height);  
        ASSERT ( width == (m_width/2) ); // might be a issue later?
        DoResize(width, m_height ,true ,0 , m_desMem);
      }
    }
    else if ( hstep < m_hstep ){
      // increase size
      for ( SInt i = 0; i < (m_hstep-hstep); ++i){
        DoResize(m_width*2, m_height ,true ,0 , m_desMem);
        // m_width now changed ...
        DoTransform(false, trns, m_tlx, m_tly, m_width, m_height, true, true,
                    false, true, true);
      }
    }
    //SetMinPrecSgn();
    //ASSERT( prec == GetPrec() );
  }
}


SInt 
HeatWaveComponent::GetVStep() const
{
  return m_vstep;
}

void 
HeatWaveComponent::SetVStep(SInt vstep, Bool rsze, EnumTransform trns)
{
  ASSERT ( vstep >= 1 );
  if ( (!rsze) ){
    m_vstep = vstep;
  }
  else{
    if ( vstep > m_vstep ){
      // reduce size
      for ( SInt i = (vstep-m_vstep); i > 0 ; --i){
        DoTransform(true, trns, m_tlx, m_tly, m_width, m_height, true, true,
                    true, false, true);
        SInt x, y, width, height; // only height nb!
        GetSubbandInfo ( 1, SubLL, x, y, width, height);  
        ASSERT ( height == (m_height/2) ); // might be a issue later?
        DoResize(m_width, height ,true ,0 , m_desMem);
      }
    }
    else if ( vstep < m_vstep ){
      // increase size
      for ( SInt i = 0; i < (m_vstep-vstep); ++i){
        DoResize(m_width, m_height*2 ,true ,0 , m_desMem);
        // m_width now changed ...
        DoTransform(false, trns, m_tlx, m_tly, m_width, m_height, true, true,
                    true, false, true);
      }
    }
  }  
  m_vstep = vstep;
}

SInt 
HeatWaveComponent::GetWidth() const
{
  return m_width;
}

void 
HeatWaveComponent::SetWidth(SInt width, Smpl def, Bool desMem)
{
  DoResize(width,m_height,True,def,desMem);
}

SInt
HeatWaveComponent::GetHeight() const
{
  return m_height;
}
  
void
HeatWaveComponent::SetHeight(SInt height, Smpl def, Bool desMem)
{
  DoResize(m_width,height,True,def,desMem);
}

SInt 
HeatWaveComponent::GetSize() const
{
  return m_size;
}
  
void 
HeatWaveComponent::SetSize(SInt width, SInt height, Smpl def, Bool desMem)
{
  ASSERT ( width > 0 );
  ASSERT ( height > 0 );
  DoResize(width,height,True,def,desMem);
}

Smpl * 
HeatWaveComponent::GetData() const
{
  return m_data;
}

void
HeatWaveComponent::SetData(Smpl * data)
{
  m_data = data;
}

Bool 
HeatWaveComponent::GetSgnd() const
{
  return m_sgnd;
}

void
HeatWaveComponent::SetSgnd(Bool sign, Bool fast)
{
  if ( fast ){
    m_sgnd = sign;
  }
  else{
    Bool sgnd;
    SInt prec, val = 0;
    GetMinPrecSgn(prec, sgnd);
    ASSERT ( prec > 1 );
    val = (1 << prec)/2;
    if ( sign != sgnd ){
      if ( sgnd ){
        (*this) += val;
      }
      else{
        (*this) -= val;
      }
    }
    m_sgnd = sign;
  }
}

void
HeatWaveComponent::SetPrec(SInt prec)
{
  m_prec = prec;
}

SInt 
HeatWaveComponent::GetPrec()
{
  return m_prec;
}

Bool
HeatWaveComponent::GetMinPrecSgn(SInt tlx, SInt tly, SInt width, SInt height,
                                 SInt & prec, Bool & sgnd, Bool min_sgnd,
                                 SInt min_prec) const
{
  ASSERT ( min_prec >= 0 );
  
  Smpl min, max, range_min, range_max, dummy;
  
  if (GetBasicStats(tlx, tly, width, height, min, max, dummy) < 0){
    return False;
  }
  
  prec = min_prec;
  
  sgnd = ((min < 0) || min_sgnd);
  
  for (;;){
    range_max = ((1 << prec)-1);
    range_min = 0;
    if ( sgnd ) {
      range_max = range_max >> 1;
      range_min = (range_max+1)*(-1);
    }
    if ( (min >= range_min) && (max <= range_max) ){
      break;
    }
    else {
      prec++;
      if ( prec == 64 ) {
        ASSERT( false );
      }
      continue;
    }
  }
  
  return True;
}

void
HeatWaveComponent::GetMinPrecSgn(SInt & prec, Bool & sgnd, Bool min_sgnd,
                                 SInt min_prec) const
{
  if (!GetMinPrecSgn(m_tlx, m_tly, m_width, m_height, prec, sgnd,
                     min_sgnd, min_prec)){
    ASSERT(False);
  }
}

void 
HeatWaveComponent::SetMinPrecSgn()
{
  GetMinPrecSgn(m_prec,m_sgnd);
}

Bool
HeatWaveComponent::SetNewPrecSgn(SInt tlx, SInt tly, SInt width, SInt height,
                                 SInt prec, Bool sgnd, Bool fxd,
                                 SInt fxd_prec, Bool fxd_sgnd)
{
  SInt prec_old;
  Bool sgnd_old;
  if (!GetMinPrecSgn(tlx,tly,width,height,prec_old,sgnd_old,sgnd,prec)){
    return False;
  }
  if (fxd){
    prec_old = fxd_prec;
    sgnd_old = fxd_sgnd;
    m_prec = fxd_prec + 1;
  }
  
  if ( (sgnd_old) && ( m_size > 0 ) ){
    SInt add = (1<<prec_old)>>1;
    for ( SInt x = tlx ; x < (tlx+width) ; ++ x){
      for ( SInt y = tly; y < (tly+height) ; ++ y ){
        SetSmpl(x,y,GetSmpl(x,y)+add);
      }
    }
  }
  
  if ( prec_old > prec ){
    SInt div = prec_old - prec;
    for ( SInt x = tlx ; x < (tlx+width) ; ++ x){
      for ( SInt y = tly; y < (tly+height) ; ++ y ){
        SetSmpl(x,y,GetSmpl(x,y) >> div);
      }
    }
  }
  else if ( prec > prec_old ){
    SInt ply = prec - prec_old;
    for ( SInt x = tlx ; x < (tlx+width) ; ++ x){
      for ( SInt y = tly; y < (tly+height) ; ++ y ){
        SetSmpl(x,y,GetSmpl(x,y) << ply);
      }
    }
  }
  else if ( prec == prec_old ){
    // do nothing
  }
  
  m_prec = prec;
  m_sgnd = sgnd;
  return True;
}

void
HeatWaveComponent::SetNewPrecSgn(SInt prec, Bool sgnd)
{
  if (!SetNewPrecSgn(m_tlx, m_tly, m_width, m_height, prec, sgnd)){
    ASSERT ( False );
  } 
}

void 
HeatWaveComponent::DoPixelise(SInt prec, Bool sgnd)
{
  if ( m_lev ){
    SInt tlx, tly , width, height, r_prec, l_prec, max_prec;
    Bool r_sgnd = False, l_sgnd= False , or_sgnd;
    GetSubbandInfo(m_lev, SubLL ,tlx, tly, width, height);
    if (!SetNewPrecSgn(tlx, tly, width, height, prec, sgnd)){
      ASSERTALWAYS(False);
    }
    if (!GetMinPrecSgn((tlx+width),0,(m_width-width),m_height, r_prec, 
                       r_sgnd)){
      ASSERTALWAYS(False);
    }
    if (!GetMinPrecSgn(0,height,width,(m_height-height), l_prec, r_sgnd)){
      ASSERTALWAYS(False);
    }
    max_prec = HeatWaveMath::Max(r_prec,l_prec);
    or_sgnd = (r_sgnd || l_sgnd);
    if (!SetNewPrecSgn((tlx + width),0,(m_width-width),m_height, prec, sgnd,
                       True, max_prec, or_sgnd)){
      ASSERTALWAYS(False);
    }
    if (!SetNewPrecSgn(0,height,width,(m_height-height), prec, sgnd,
                       True, max_prec, or_sgnd)){
      ASSERTALWAYS(False);
    }
  }
  else{
    SetNewPrecSgn(prec,sgnd);
  }
}

void
HeatWaveComponent::DoCapData(SInt min, SInt max, Bool set)
{
  for ( SInt i = 0 ; i < m_size ; ++i ){
    if ( this->m_data[i] > max ){
      this->m_data[i] = max;
    }
    else if ( this->m_data[i] < min ){
      this->m_data[i] = min;
    } 
  }
  if ( set ){
    SetMinPrecSgn();
  }
}

void 
HeatWaveComponent::DoCapDataToPrec(){
  SInt range_max, range_min;
  range_max = ((1 << m_prec)-1);
  range_min = 0;
  if ( m_sgnd ) {
    range_max = range_max >> 1;
    range_min = (range_max+1)*(-1);
  }
  DoCapData(range_min, range_max, False);
}

void 
HeatWaveComponent::DoClear(SInt rplc){
  for ( SInt i = 0 ; i < m_size ; ++i ){
    this->m_data[i] = rplc;
  } 
}

SInt 
HeatWaveComponent::GetBasicStats(SInt tlx, SInt tly, SInt width, SInt height,
                                 Smpl & min, Smpl & max, SInt & total) const
{
  if ( ! ((width > 0) && (height > 0) && 
          ValidateCoords ( tlx, tly ) && 
          ValidateCoords ( (tlx+width)-1, (tly+height)-1 ))){
    return -1;
  }
  
  SInt size = (width*height);
  min = 0;
  max = 0;
  total = 0;
  
  if ( m_size > 0 ){
    min = max = GetSmpl(tlx,tly);
    for ( SInt x = tlx ; x < (tlx+width) ; ++x ){
      for ( SInt y = tly ; y < (tly+height) ; ++y ){
        min = HeatWaveMath::Min(min,GetSmpl(x,y));
        max = HeatWaveMath::Max(max,GetSmpl(x,y));
        total += GetSmpl(x,y);
        ++size;
      }
    }
  }
  
  return size;
}

SInt 
HeatWaveComponent::GetBasicStats(Smpl & min, Smpl & max, SInt & total) const
{
  return GetBasicStats(m_tlx,m_tly,m_width,m_height,min,max,total);
}

Bool 
HeatWaveComponent::GetStats(SInt tlx, SInt tly, SInt width, SInt height,
                            Smpl & min, Smpl & max, SInt & total, 
                            SFloat64 & mean, SFloat64 & std_dev, 
                            SFloat64 & std_err, SFloat64 & rms) const
{
  if ( ! ((width > 0) && (height > 0) && 
          ValidateCoords ( tlx, tly ) && 
          ValidateCoords ( (tlx+width)-1, (tly+height)-1 ))){
    return False;
  }
  
  register Smpl tmp_smpl = 0;
  register SFloat64 tmp_flt = 0;
  total = 0;
  min = max = GetSmpl(tlx,tly);

  for ( SInt y = tly ; y < (tly+height) ; ++ y ){
    for ( SInt x = tlx ; x < (tlx+width) ; ++ x ){
      tmp_smpl = GetSmpl(x,y);
      if ( tmp_smpl < min ){
        min = tmp_smpl;
      }
      else if ( tmp_smpl > max ){
        max = tmp_smpl;
      }
      total += tmp_smpl;
    }
  }
  
  SFloat64 size = width*height;
  SFloat64 flt_total = total;
  mean = flt_total/size;
  std_dev = 0;
 
  for ( SInt y = tly ; y < (tly+height) ; ++ y ){
    for ( SInt x = tlx ; x < (tlx+width) ; ++ x ){
      tmp_flt = GetSmpl(x,y); 
      tmp_flt -= mean;
      tmp_flt = pow(tmp_flt,2);
      std_dev += tmp_flt;
    }
  }
  
  std_dev = sqrt(std_dev/size);
  std_err = std_dev/sqrt(size);
  rms = sqrt(pow(mean,2)+pow(std_dev,2));
  
  return True;
}

void 
HeatWaveComponent::GetStats(Smpl & min, Smpl & max, SInt & total, 
                            SFloat64 & mean, SFloat64 & std_dev, 
                            SFloat64 & std_err, SFloat64 & rms) const
{
  Bool result = GetStats(m_tlx, m_tly, m_width, m_height, min, max, total, 
                         mean, std_dev, std_err, rms);
  ASSERT ( result );
}

EnumColor
HeatWaveComponent::GetColor() const
{
  return m_clr;
}

void
HeatWaveComponent::SetColor(EnumColor clr)
{
  m_clr = clr;
}

Smpl 
HeatWaveComponent::GetSmpl(const SInt x, const SInt y) const
{
  if ( (m_tlx != 0 ) || (m_tly != 0) ){
    int i = 0;
    i += 1;
    ASSERT ( i == 1);
  }
  ASSERT(ValidateCoords(x,y));
  
  /* NB! leave y infront! */
  return m_rows[y-m_tly][x-m_tlx]; 
}

void
HeatWaveComponent::SetSmpl(const SInt x, const SInt y, const Smpl val)
{
  ASSERT(ValidateCoords(x,y));
  ASSERT(ValidateSample(val));
  
  /* NB! leave y infront! */
  m_rows[y-m_tly][x-m_tlx] = val; 
}

SInt
HeatWaveComponent::GetTransformLevel() const
{
  return m_lev;
}

void 
HeatWaveComponent::SetTransformLevel(SInt lev)
{
  m_lev = lev;
}

EnumTransform
HeatWaveComponent::GetTransformType() const
{
  return m_trn;
}

void 
HeatWaveComponent::SetTransformType(EnumTransform type)
{
  m_trn = type;
}

Smpl ** 
HeatWaveComponent::GetRows() const
{
  return m_rows;
}

void 
HeatWaveComponent::SetRows(Smpl ** rows)
{
  m_rows = rows;
}

void 
HeatWaveComponent::SetDesMem(Bool des)
{
  m_desMem = des;
}

Bool 
HeatWaveComponent::GetDesMem() const
{
  return m_desMem;
}

void
HeatWaveComponent::DoResize(SInt width, SInt height, Bool keap, Smpl def, 
                            Bool desMem)
{
  if ( (width == m_width) && (height == m_height) ){
    return;
  }
    
  if ( keap && (m_width >0) && (m_height > 0) ){
    ASSERT ( ValidateSanity() );
    ASSERT ( width >= 1 );
    ASSERT ( height >= 1 );
    HeatWaveComponent Temp(m_tlx, m_tly, m_hstep, m_vstep, m_width, m_height,
                           m_sgnd, m_prec, m_clr, m_data, m_rows,
                           m_desMem, False);
    ASSERT ( Temp.ValidateSanity() );
    
    m_rows = NULL; m_data = NULL;
    SInt minW = HeatWaveMath::Min(m_tlx+width, m_tlx+m_width);
    SInt minH = HeatWaveMath::Min(m_tly+height, m_tly+m_height);
    m_width = width;
    m_height = height;
    DoCreate(m_width, m_height, True, def, desMem);
    
    for ( SInt x = m_tlx ; x < minW ; ++x ){
      for ( SInt y = m_tly ; y < minH ; ++ y){
        SetSmpl(x,y,Temp.GetSmpl(x,y));
      }
    }
  }
  else{
    DoDestroy();
    m_width = width;
    m_height = height;
    DoCreate(width, height, True, def, desMem);
    m_width = width;
    m_height = height;
  } 
}

Bool 
HeatWaveComponent::DoTransform(Bool fwd, EnumTransform trn, 
                               SInt tlx, SInt tly, 
                               SInt width, SInt height, Bool pred, Bool upd, 
                               Bool vert, Bool horz, Bool range)
{
  if ( !(ValidateCoords(tlx,tly)&&ValidateCoords(tlx+width-1,tly+height-1)&&
         (width>1)&&(height>1)) ){
    return False;
  }
  Smpl * tmp_data = &(m_rows[tly-m_tly][tlx-m_tlx]); 
  if ( fwd ){
    if ( horz )
      DoTransformInternal(fwd,trn,width,height,True,tmp_data,pred,upd);
    if ( vert )
      DoTransformInternal(fwd,trn,width,height,False,tmp_data,pred,upd);
  }
  else { // inverse
    if ( vert )
      DoTransformInternal(fwd,trn,width,height,False,tmp_data,pred,upd);
    if ( horz )
      DoTransformInternal(fwd,trn,width,height,True,tmp_data,pred,upd);
  }
  if ( range ){
    SetMinPrecSgn();
  }
  m_trn = trn;
  return True;
}

SInt 
HeatWaveComponent::DoPyramidTransform(EnumTransform trn, SInt lev, Bool fwd, 
                                      SInt cur)
{
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
    SInt x, y, width, height;
    Bool check;
    check =  GetSubbandInfo ( (m_lev-(fwd?0:1)), SubLL , x, y, width, 
                              height );
    ASSERT ( check );
    if(!DoTransform(fwd, trn, x, y, width, height, True, True, True, True, 
                    True)){
      break;
    }
    ASSERT ( check );
    fwd ? ( ++m_lev ) : ( --m_lev );
  }
  return m_lev;
}

Bool
HeatWaveComponent::DoShiftTransform(Bool fwd, EnumTransform trn, 
                                    SInt tlx, SInt tly, 
                                    SInt width, SInt height,
                                    Bool vert, Bool horz, Bool range)
{
  /**
     @todo Fix me!

     if ( !(ValidateCoords(tlx,tly)&&ValidateCoords
     (tlx+width-1,tly+height-1))){
     return False;
     }
     Smpl * tmp_data = &(m_rows[tly-m_tly][tlx-m_tlx]);
     if (fwd){
     if ( horz )
     DoShiftInternal(trn,trn,width,height,True,tmp_data);
     if ( vert )
     DoShiftInternal(trn,trn,width,height,False,tmp_data);
     }
     else { //inverse
     if ( vert )
     DoShiftInternal(trn,trn,width,height,True,tmp_data);
     if ( horz )
     DoShiftInternal(trn,trn,width,height,False,tmp_data);
     }
     if ( range ){
     SetMinPrecSgn();
     }
  */
  return True;
}

Bool 
HeatWaveComponent::DoHE(SInt tlx, SInt tly, SInt width, SInt height)
{
  SInt prec = 8;
  Bool sgnd = false;
  Bool was_sgnd = false;
  SInt cRange;
  if ( ! GetMinPrecSgn(tlx, tly, width, height, prec, sgnd)){
    return False;
  }
  
  if ( sgnd ){
    was_sgnd = True;
    SetSgnd(False,False);
  }

  cRange = 1 << prec;
  // Frequncy distrubution of pArray
  SInt * freqDistr = NULL;    
  // Cumalitive Frequency Distrubution
  SInt * cumFreqDistr = new int[cRange]; 
  LEAVEONNULL(cumFreqDistr);
  // Ideal Frequency
  SInt * idealFreq = new int[cRange]; 
  LEAVEONNULL(idealFreq);
  // Cumalitive Ideal frequancy
  SInt * cumIdealFreq = new int[cRange];
  LEAVEONNULL(cumIdealFreq);
  // Mapping Table
  SInt * mappingTable = new int[cRange];
  LEAVEONNULL(cumIdealFreq);

  // Clear all arrays to 0;
  for(int i = 0; i < cRange; i++){
    cumFreqDistr[i] = 0; idealFreq[i] = 0; 
    cumIdealFreq[i] = 0; mappingTable[i] = 0;
  }

  // STEP ONE
  // Construct the histogram.
  SInt range, offset;
  if ( ! GetHistogram(tlx, tly, width, height, freqDistr, range, offset) ){
    ASSERT ( False );
  }
  ASSERT ( offset == 0 );
  ASSERT ( range > 0 );

  // STEP TWO
  // Calculate the cumulative frequency distribution
  SInt cumulative = 0;
  for(SInt i = 0; i < cRange; ++i)
    {
      cumulative += freqDistr[i];
      cumFreqDistr[i] = cumulative;
    }

  // STEP THREE
  // Calculate the ideal frequency distribution
  SInt each = cumulative / cRange;
  SInt remain = cumulative % cRange;
  SInt start = 0;
  if (remain % 2 == 1) {
    start--;
  }
  start = start + (cRange / 2 - remain / 2);
  for(int i = 0; i < cRange; i++) {
    idealFreq[i] = each;
    if ((i >= start) && (remain > 0)) {
      idealFreq[i]++;
      remain--;
    }
  }

  // STEP FOUR
  // Calculate the cumulative frequency for the ideal frequency
  SInt idealCumalitive = 0;
  for(int i = 0; i < cRange; i++) {
    idealCumalitive += idealFreq[i];
    cumIdealFreq[i] = idealCumalitive;
  }

  // STEP FIVE
  // Do the Mapping
  SInt checkup = 0;
  for(int i = 0; i < cRange; i++) { 
    for (int k = 0; k < cRange; k++) {
      if (cumFreqDistr[i] == cumIdealFreq[k]) {
        mappingTable[i] = k;
        checkup++;
        break;
      }
      
      SInt postEntry;
      SInt preEntry;
	  
      if (k == 0) {
        preEntry = cumIdealFreq[k];
      }
      else {
        preEntry = cumIdealFreq[k-1];
      }
	  
      if (k == (cRange - 1)) {
        postEntry = cumIdealFreq[k];
      }
      else {
        postEntry = cumIdealFreq[k+1];
      }
      
      if (cumFreqDistr[i] > postEntry) {
        continue;
      }

      SInt preDiff = cumFreqDistr[i] - preEntry;
      SInt postDiff = cumFreqDistr[i] - postEntry;
      preDiff = (preDiff < 0? -preDiff: preDiff);
      postDiff = (postDiff < 0? -postDiff: postDiff);
      
      if (preDiff < postDiff) {
        if(k == 0) {
          mappingTable[i] = k;
        }
        else {
          mappingTable[i] = k-1;
        }
        checkup++;
        break;
      }
      else {
        mappingTable[i] = k + 1;
        checkup++;
        break;
      }
    }
  }

  // STEP SIX
  // Change the original array
  for (SInt x = tlx ; x < (tlx+width) ; ++x) {
    for (SInt y = tly ; y < (tly+height) ; ++y) {
      Smpl mapped = (Smpl)(mappingTable[GetSmpl(x, y)]);
      SetSmpl (x, y, mapped);
    }
  }

  if (was_sgnd) {
    SetSgnd(True,False);
  }

  //Clean Up
  delete freqDistr;
  delete cumFreqDistr;
  delete idealFreq;
  delete cumIdealFreq;
  delete mappingTable;
  return True;
} 

void
HeatWaveComponent::DoHE()
{
  if ( !DoHE ( m_tlx, m_tly, m_width, m_height ) ){
    ASSERT ( False );
  }
}

Bool 
HeatWaveComponent::GetSubbandInfo ( SInt res, EnumSubband sub, 
                                    SInt & x, SInt & y, 
                                    SInt & width, SInt & height ) const
{
  ASSERT ( res >= 0 );
  x = m_tlx;
  y = m_tly;
  width = m_width;
  height = m_height;
  
  if ( res == 0 ){
    return (sub == SubLL) ? True : False;
  }
  for ( SInt i = 0; i < (res-1) ; ++i ){
    width = (width>>1) + (width%2);
    height = (height>>1) + (height%2);
  }
  if ( (width < 2) || (height < 2) ){
    return False;
  }
  
  switch (sub) {
  case SubLL:
    width = (width>>1) + (width%2);
    height = (height>>1) + (height%2);
    x = m_tlx;
    y = m_tly;
    break;
  case SubHL:
    x = m_tlx + (width>>1) + (width%2);
    y = m_tly;
    width = (width>>1);
    height = (height>>1) + (height%2);
    break;
  case SubLH:
    x = m_tlx;
    y = m_tly + (height>>1) + (height%2);
    width = (width>>1) + (width%2);
    height = (height>>1);
    break;
  case SubHH:
    x = m_tlx + (width>>1) + (width%2);
    y = m_tly + (height>>1) + (height%2);
    width = (width>>1);
    height = (height>>1);
    break;
  case SubTotal:
  default:
    ASSERT ( False );
  }
  
  return True;
}

HeatWaveComponent *
HeatWaveComponent::GetClone()
{
  HeatWaveComponent * ret = NULL;
  ret = new HeatWaveComponent(*this);
  LEAVEONNULL(ret);
  return ret;
}

SFloat64
HeatWaveComponent::GetComparison(const HeatWaveComponent & other, 
                                 EnumComparison cmp)
{
  if(!((m_width == other.m_width) && (m_height == other.m_height))){
    return 0;
  }
  
  ASSERT ( ValidateSanity() );
  ASSERT ( other.ValidateSanity() );
  SFloat64 mean = 0.0;
  SFloat64 max = 0.0;
  SFloat64 & sum = mean;
  SFloat64 & peak = mean;
  SFloat64 & diff = max;
  
  switch ( cmp ){
  case CmpPSNR:
    mean = GetComparison(other,CmpMSE);
    max = ((1 << m_prec) - 1);
    return (20.0 * log10(max/sqrt(mean)));
    break;
  case CmpMSE:   
    for (SInt i = 0; i < m_size ;++i) {
      diff = m_data[i] - other.m_data[i];
      sum += (diff*diff);
    }
    return sum / ((double) m_size);
    break;
  case CmpRMSE:
    return sqrt(GetComparison(other,CmpMSE));
    break;
  case CmpPAE:
    for (SInt i = 0; i < m_size ;++i) {
      diff = abs(m_data[i] - other.m_data[i]);
      if (diff > peak) {
        peak = diff;
      }
    }
    return peak;
    break;
  case CmpMAE:
    for (SInt i = 0; i < m_size ;++i) {
      diff = m_data[i] - other.m_data[i];
      sum += fabs(diff);
    }
    return sum / ((double) m_size);
    break;
  case CmpEqual:
    for ( SInt i = 0 ; i < m_size ; ++ i ){
      if ((m_data[i]) != (other.m_data[i])){
        return 0;
      }		    
    }
    return 1;
    break;
  default:
    ASSERT( False );
    return -1;
  }
  ASSERT ( False );
  return -1;
}

Smpl * 
HeatWaveComponent::GetVector(SInt tlx, SInt tly, SInt width, SInt height) 
  const
{
  if ( ! ((width > 0) && (height > 0) && 
          ValidateCoords ( tlx, tly ) && 
          ValidateCoords ( (tlx+width)-1, (tly+height)-1 ))){
    return NULL;
  }
  
  Smpl * ret = new Smpl[width*height];
  LEAVEONNULL(ret);
  
  SInt i = 0;
  for ( SInt y = tly ; y < (tly+height) ; ++ y ){
    for ( SInt x = tlx ; x < (tlx+width) ; ++ x ){
      ret[i] = GetSmpl(x,y);
      i++;
    }
  }
  
  return ret;
}

Smpl *
HeatWaveComponent::GetVector(SInt res, EnumSubband sub) const
{
  SInt tlx = -1, tly = -1, width = 0, height = 0;
  if ( !GetSubbandInfo(res,sub,tlx,tly,width,height)){
    return NULL;
  }else{
    return GetVector(tlx,tly,width,height);
  }
}

Bool 
HeatWaveComponent::GetHistogram(SInt tlx, SInt tly, SInt width, SInt height, 
                                SmplPtr & hist, SInt & range, SInt & offset) 
  const
{
  // note we ignore the object variables for prec & sgndness.
  SInt precision;
  Bool is_signed;
  GetMinPrecSgn(precision, is_signed);
  WARN_IF ( hist != NULL );
  
  if ( ! ((width > 0) && (height > 0) && 
          ValidateCoords ( tlx, tly ) && 
          ValidateCoords ( (tlx+width)-1, (tly+height)-1 ))){
    return False;
  }
  
  range = 1 << precision;
  offset = is_signed ? (range>>1) : 0;
  hist = new Smpl[range];
  LEAVEONNULL(hist);
  
  for (SInt i = 0; i < range; ++i){
    hist[i] = 0;
  }

  for ( SInt y = tly ; y < (tly+height) ; ++ y ){
    for ( SInt x = tlx ; x < (tlx+width) ; ++ x ){
      //++(hist[GetSmpl(x,y)+offset]);
      Smpl tmp = GetSmpl(x,y) + offset;
      hist[tmp] += 1;
    }
  }  
  return True;
}

Bool 
HeatWaveComponent::GetHistogram(Smpl *& hist, SInt & range, SInt & shift) 
  const
{
  return GetHistogram(m_tlx,m_tly,m_width,m_height,hist,range,shift);
}

SFloat64 
HeatWaveComponent::GetEntropy(SInt tlx, SInt tly, SInt width, SInt height) 
  const
{
  SFloat64 ret = 0.0;
  SmplPtr hist = NULL;
  SInt range = 0;
  SInt shift = 0;
  SInt size = width * height;
  
  if ( !GetHistogram(tlx, tly, width, height, hist, range, shift) ){
    return -99;
  }
  
  for ( SInt i = 0; i < range ; ++i ){
    if ( hist[i] ){
      // printf(" HIT : %d\n",hist[i]);
      SFloat64 Pr = hist[i];
      Pr /= size;
      // printf(" PRB : %f\n",Pr);
      ret += Pr*(log(Pr)/log(2));
    }
  }
  
  delete hist;
  if ( ret ){
    return -ret;
  }
  else {
    return 0.0;
  }
}

SFloat64 
HeatWaveComponent::GetEntropy() const
{
  return GetEntropy(m_tlx,m_tly,m_width,m_height);
}

FILE * 
HeatWaveComponent::WriteIII(const Char * name, FILE * file) const
{
  if (name){
    ASSERTFALSE ( file );
    file = fopen(name,"wb");
    if (!file){
      return file;
    }
  }
  ASSERT(file);
  SInt precision;
  Bool is_signed;
  GetMinPrecSgn(precision, is_signed);
  SInt width = GetWidth(), height = GetHeight();
  fprintf(file,IIIHEADER, width, height, ColorName(GetColor()), precision, 
          is_signed? IIITRUE : IIIFALSE);
  for ( SInt i = 0 ; i < GetSize() ; ++i ){
    fprintf(file, IIISAMPLEOUT, (m_data[i]));
  }
  return file;
}

FILE * 
HeatWaveComponent::ReadIII(const Char * name, FILE * file)
{
  Bool open_file = False;
  if (name){
    ASSERTFALSE ( file );
    file = fopen(name,"rb");
    open_file = True;
    if (!file){
      return file;
    }
  }
  ASSERT(file);
  SInt width = 0, height = 0, precision = 0, count = 0;
  Char color_name[10], is_signed[10]; 
  memset ( (color_name) , 0 , 10 );
  memset ( (is_signed) , 0 , 10 );
  EnumColor clr_tmp = ClrUnknown;
  count = fscanf(file, IIIHEADER, &width, &height, 
                 color_name, &precision, is_signed);
  color_name[9] = '\0';
  is_signed[9] = '\0';
  if ( count != 5 ){
    goto error;
  }
  clr_tmp = ColorEnum(color_name);
  if ( clr_tmp == ClrUnknown ){
    goto error;
  }
  if ( ! ( ( strcmp(is_signed, IIITRUE) == 0 ) ||
           ( strcmp(is_signed, IIIFALSE) == 0 ) ) ){
    goto error;
  }
  if ( (width < 0) || (height < 0) ){
    goto error;
  }

  m_clr = clr_tmp;
  m_sgnd = (strcmp(is_signed, IIITRUE) == 0) ? True : False;
  DoDestroy();
  DoCreate(width, height, True, 0, True);
  
  for ( SInt i = 0 ; i < GetSize() ; ++i ){
    count = fscanf(file, IIISAMPLEIN, &(m_data[i]));
    if ( count != 1 ){
      goto error;
    }
  }
  
  return file;
 error:
  if ( open_file ){
    fclose(file);
  }
  return NULL;
}

Bool 
HeatWaveComponent::ValidateCoords(SInt x, SInt y) const
{
  Bool ret = True;
  ret &= (x >= m_tlx);
  ret &= (y >= m_tly);
  ret &= (x < (m_tlx+m_width));
  ret &= (y < (m_tly+m_height));
  return ret;
}

Bool
HeatWaveComponent::ValidateSample(SInt dat) const
{
  Bool ret = True;
  SInt max = 1 << m_prec;
  SInt min = 0;
  if ( m_sgnd ){
    max = max >> 1;
    min = (-max)-1;
  }
  ret &= (dat <= max);
  ret &= (dat >= min);
  if (!ret){
    ret=True;
  }
  return ret;
}

HeatWaveComponent & 
HeatWaveComponent::operator=(const HeatWaveComponent & rhs)
{
  ASSERT ( ValidateSanity() );
  ASSERT ( rhs.ValidateSanity() );
  DoDestroy();
  DoCopy(rhs);
  return (*this);
}

Bool
HeatWaveComponent::operator==(const HeatWaveComponent & rhs ) const
{
  ASSERT ( False );
  ASSERT ( ValidateSanity() );
  ASSERT ( rhs.ValidateSanity() );
  
  Bool ret = True;
  ret &= (m_tlx == rhs.m_tlx);
  ret &= (m_tly == rhs.m_tly);
  ret &= (m_hstep == rhs.m_hstep); 
  ret &= (m_vstep == rhs.m_vstep);
  ret &= (m_width == rhs.m_width);
  ret &= (m_height == rhs.m_height); 
  ret &= (m_sgnd == rhs.m_sgnd); 
  ret &= (m_prec == rhs.m_prec); 
  ret &= (m_clr == rhs.m_clr); 
  ret &= (m_trn == rhs.m_trn); 
  ret &= (m_lev == rhs.m_lev); 
  ret &= (m_desMem == rhs.m_desMem); 
  ret &= (m_size == rhs.m_size); 
    
  if ( ret ) {
    for ( SInt i = 0 ; i < m_size ; ++ i ){
      ret &= (m_data[i] == rhs.m_data[i]);
    }
  }
  
  return ret;
}

Smpl 
HeatWaveComponent::operator|=(Smpl mask)
{
  for ( SInt i = 0 ; i < m_size ; ++ i ){
    m_data[i] |= mask;
  }
  return mask;
}

Smpl 
HeatWaveComponent::operator^=(Smpl mask)
{
  for ( SInt i = 0 ; i < (m_height*m_width) ; ++ i ){
    m_data[i] ^= mask;
  }
  return mask;
}

Smpl 
HeatWaveComponent::operator&=(Smpl mask)
{
  for ( SInt i = 0 ; i < (m_height*m_width) ; ++ i ){
    m_data[i] &= mask;
  }
  return mask;
}

Smpl 
HeatWaveComponent::operator+=(Smpl val)
{
  for ( SInt i = 0 ; i < m_size ; ++ i ){
    m_data[i] += val;
  }
  return val;
}

Smpl 
HeatWaveComponent::operator-=(Smpl val)
{
  for ( SInt i = 0 ; i < m_size ; ++ i ){
    m_data[i] -= val;
  }
  return val;
}

/** Protected|Internal functions ********************************************/

void 
HeatWaveComponent::SetRowPtrs()
{
  for ( SInt i = 0 ; i < m_height ; ++ i ){
    m_rows[i] = m_data + (m_width*i);
  }
}

Bool 
HeatWaveComponent::ValidateSanity() const
{
  Bool ret = True;
  ret &= ( m_size == (m_height*m_width) );
  ret &= ( m_height > 0 );
  ret &= ( m_width > 0 );
  ret &= ( m_data != NULL );
  ret &= ( m_rows != NULL );
  
  if ( ret ){
    for ( SInt i = 0 ; i < m_size ; ++i ){
      ret &= ValidateSample(m_data[i]);
    }
  }
  
  return ret;
}

void 
HeatWaveComponent::DoTransformInternal(Bool fwd, EnumTransform trn, SInt wid, 
                                       SInt hei, Bool hor, Smpl * mem, 
                                       Bool prd, Bool upd)
{
  SInt inter_step, intra_step, nsteps, length;
  void (HeatWaveLift::*func[HEATWAVELIFTMAXSTEPS])
    (Smpl *, Smpl *, SInt, SInt, Bool)const;
  Smpl * data = mem;
  Smpl * even;
  Smpl * odd;
  Smpl even_len;

  if ( hor ){ 
    // horizontal transform
    intra_step = 1;
    inter_step = m_width;
    nsteps = hei;
    length = wid;
    even_len = ((wid>>1) + (wid%2));
  }
  else {     
    // vertical transform
    intra_step = m_width;
    inter_step = 1;
    nsteps = wid;
    length = hei;
    even_len = ((hei>>1) + (hei%2))*m_width; 
  }
  
  SInt j = m_lift.GetFuncArray(func,trn,fwd,prd,upd);
  
  // Perform the transform
  for ( SInt i = 0 ; i < nsteps ; ++ i){
    if ( fwd ){
      m_lift.Split(data, length, intra_step, even, odd);
    }
    else{
      even = data;
      odd = data + even_len;
    }
    
    for ( SInt n = 0; n < j ; ++n ){
      (m_lift.*func[n])(even,odd,length,intra_step,fwd);
    }
    
    if ( !fwd ){
      m_lift.Join(data,length,intra_step);
    }
    data+=inter_step;
  }
}

void
HeatWaveComponent::DoShiftInternal(Bool fwd, EnumTransform trn, SInt wid, 
                                   SInt hei, Bool hor, Smpl * mem)
{
  /** @todo Fix me!
      SInt inter_step, intra_step, nsteps, length;
      void (*fptr)(Smpl *, SInt, SInt)= NULL;
      Smpl * data = mem;
      
      if ( dir == DirHorizontal ){
      intra_step = 1;
      inter_step = m_width;
      nsteps = height;
      length = width;
      }
      else { // ( dir == DirVertical ){
      ASSERT ( dir == DirVertical );
      intra_step = m_width;
      inter_step = 1;
      nsteps = width;
      length = height;
      }
      
      switch ( type ){
      case Trn1_1:
      if ( trans == TrnAnalyse ){
      fptr = HeatWaveLift::FwdConstantShift;
      }
      else{
      fptr = HeatWaveLift::RevConstantShift;
      }
      break;
      case ActLinear:
      if ( trans == TrnAnalyse ){
      fptr = HeatWaveLift::FwdLinearShift;
      }
      else{
      fptr = HeatWaveLift::RevLinearShift;
      }
      break;
      case ActCubic:
      if ( trans == TrnAnalyse ){
      fptr = HeatWaveLift::FwdCubicShift;
      }
      else{
      fptr = HeatWaveLift::RevCubicShift;
      }
      break;
      case ActFinite:
      default:
      ASSERT ( False );
      return;
      break;
      }
  
      for ( SInt i = 0 ; i < nsteps ; ++ i){
      (*fptr)(data,length,intra_step);
      data+=inter_step;
      }
  */
}

void
HeatWaveComponent::DoCreate(SInt width, SInt height, Bool set, Smpl def,
                            Bool desMem)
{
  ASSERT ( width > 0 );
  ASSERT ( height > 0 );
  ASSERT ( m_data == NULL );
  ASSERT ( m_rows == NULL );
  m_data = new Smpl[width*height];
  m_rows = new Smpl*[height];
  m_size = width*height;
  m_width = width;
  m_height = height;
  
  LEAVEONNULL(m_data);
  LEAVEONNULL(m_rows);
  
  if ( set ) {
    for ( SInt i = 0; i < m_size; ++i ){
      m_data[i] = def;
    }
  }

  SetRowPtrs();
  m_desMem = desMem;
}

void 
HeatWaveComponent::DoDestroy()
{
  if (!m_desMem){
    return;
  }
  
  delete [] m_data;
  m_data = NULL;
  delete [] m_rows;
  m_rows = NULL;
}

void 
HeatWaveComponent::DoCopy(const HeatWaveComponent & rhs)
{  
  memcpy((char*)this,(char*)&rhs,sizeof(HeatWaveComponent));
  m_data = NULL;
  m_rows = NULL;
  DoCreate(m_width,m_height,False,0);
  for ( SInt i = 0 ; i < (m_height*m_width) ; ++ i ){
    m_data[i] = rhs.m_data[i];
  }
}
