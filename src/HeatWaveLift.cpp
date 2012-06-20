/****************************************************************************/
/**
 ** @file HeatWaveLift.cpp
 ** @brief Contains the HeatWaveLift class function definitions.
 ** @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 ** 
 **/

#include "HeatWaveLift.hpp"
#define MOD_FOR_NOW 256

HeatWaveLift::HeatWaveLift()
{
  memset((char*)this,'\0',sizeof(HeatWaveLift));
  m_mod = MOD_FOR_NOW; // To change
}

HeatWaveLift::HeatWaveLift(SInt len)
{
  memset((char*)this,'\0',sizeof(HeatWaveLift));
  m_mod = MOD_FOR_NOW; // To change
  if ( len > 0 ){
    DoAllocate(len);
  }
}

HeatWaveLift::HeatWaveLift(const HeatWaveLift & oth)
{
  memset((char*)this,'\0',sizeof(HeatWaveLift));
  m_mod = MOD_FOR_NOW; // To change
  DoAllocate(oth.m_bufferLen);
}

HeatWaveLift::~HeatWaveLift()
{
  DoRelease();
}
 
void
HeatWaveLift::Split(Smpl * data, SInt len, SInt step,
                    Smpl* & even, Smpl* & odd)
{
  ASSERT ( len >= 0 );
  ASSERT ( step > 0 );
  SInt virLen = (len/step) + (len%step);
  if ( virLen <= 1 ){
    return;
  }
  DoAllocate(virLen);
  SInt even_len, odd_len;
  FindLengths(virLen, even_len, odd_len);
  
  for ( SInt i = 0; i < even_len ; ++i ){
    m_buffer[i] = data[(step*(i*2))];
  }
  for ( SInt i = 0; i < odd_len; ++i ){
    m_buffer[(even_len+i)] = data[(step*((i*2)+1))];
  }
  for ( SInt i = 0; i < virLen ; ++i ){
    data[i*step]=m_buffer[i];
  }
  even = data;
  odd  = data+(even_len*step);
}

void 
HeatWaveLift::Join(Smpl * data, SInt len, SInt step)
{
  ASSERT ( len >= 0 );
  ASSERT ( step > 0 );
  SInt virLen = (len/step) + (len%step);
  if ( virLen <= 1 ){
    return;
  }
  DoAllocate(virLen);
  SInt even_len, odd_len;
  FindLengths(virLen, even_len, odd_len);
  
  for ( SInt i = 0; i < virLen ; ++i ){
    m_buffer[i] = data[(i*step)];
  }
  for ( SInt i = 0; i < even_len ; ++i ){
    data[(step*(i*2))] = m_buffer[i];
  } 
  for ( SInt i = 0; i < odd_len; ++i ){
    data[(step*((i*2)+1))] = m_buffer[even_len+i];
  }
}

void
HeatWaveLift::SplitOpt(Smpl * data, SInt len, SInt step,
                       Smpl* & even, Smpl* & odd)
{
  ASSERT ( len >= 0 );
  ASSERT ( step > 0 );
  SInt virLen = (len/step) + (len%step);
  if ( virLen <= 1 ){
    return;
  }
  DoAllocate(virLen>>1); // half the space is needed
  SInt even_len, odd_len;
  FindLengths(virLen, even_len, odd_len);
  SInt cpy_loop = (virLen-1)>>1;
  
  for ( SInt i = 1, b = 0 ; b < cpy_loop ; (i+=2), ++b){
    m_buffer[b] = data[(step*i)];
    data[(step*(b+1))] = data[(step*(i+1))];
  }
  
  for ( SInt i = even_len, b = 0 ; b < cpy_loop ; ++i, ++b){
    data[(step*(i))] = m_buffer[b];
  }
  
  even = data;
  odd  = data+(even_len*step);
}

void 
HeatWaveLift::JoinOpt(Smpl * data, SInt len, SInt step)
{
  ASSERT ( len >= 0 );
  ASSERT ( step > 0 );
  SInt virLen = (len/step) + (len%step);
  if ( virLen <= 1 ){
    return;
  }
  DoAllocate(virLen>>1);
  SInt even_len, odd_len;
  FindLengths(virLen, even_len, odd_len);
  SInt cpy_loop = (virLen-1)>>1;
  
  // cpy first half of bytes (except 1)
  for ( SInt b = 0 ; b < cpy_loop ; ++b){
    m_buffer[b] = data[(step*(b+1))];
  }
  
  // spread the second half (sometimes skipping last 1, if even length-ed)
  for ( SInt b = 0 ; b < cpy_loop ; ++b){
    data[(step*((b*2)+1))] = data[(step*(b+even_len))];
    data[(step*((b*2)+2))] = m_buffer[b];
  }
}

void
HeatWaveLift::SplitZ(Smpl * data, SInt len, SInt step,
                     Smpl* & even, Smpl* & odd)
{
  ASSERT ( len >= 0 );
  ASSERT ( step > 0 );
  SInt virLen = (len/step) + (len%step);
  if ( virLen <= 1 ){
    return;
  }
  SInt even_len, odd_len;
  FindLengths(virLen, even_len, odd_len);
  
  SInt left_mark = 1;
  virLen -= 1;
  SInt rght_mark = virLen-(virLen%2);
  register Smpl swap_space;
  
  SInt lft = left_mark;
  SInt rht = rght_mark;
  for ( ; lft < rht ; ++lft , --rht ){
    for ( SInt mrk = lft ; mrk < rht ; mrk += 2){
      swap_space = data[step*mrk];
      data[step*mrk] = data[step*(mrk+1)];
      data[step*(mrk+1)] = swap_space;
    }
  }
  
  even = data;
  odd  = data+(even_len*step);
}

void 
HeatWaveLift::JoinZ(Smpl * data, SInt len, SInt step)
{
  ASSERT ( len >= 0 );
  ASSERT ( step > 0 );
  SInt virLen = (len/step) + (len%step);
  if ( virLen <= 1 ){
    return;
  }
  SInt even_len, odd_len;
  FindLengths(virLen, even_len, odd_len);

  SInt rght_mark = virLen-(virLen%2);
  SInt left_mark = 1;
  register Smpl swap_space;
  SInt rht = ((virLen/2) + (virLen%2));;
  SInt lft = rht - 1;
  
  for ( ; lft >= left_mark ; --lft , ++rht ){
    ASSERT ( rht <= (rght_mark) );
    for ( SInt mrk = lft ; mrk < rht ; mrk += 2){
      swap_space = data[step*mrk];
      data[step*mrk] = data[step*(mrk+1)];
      data[step*(mrk+1)] = swap_space;
    }
  }
}

// SplitN & JoinN stack memory allocation, must be a power of 2
// BIG note this will only work for "4" now as I had to reach a compromise but if
// I have some more time then I will correct it and test it for different lengths
#define HEATWAVELIFTSTACKN (4)

// ptr to a
// ptr to b
// ptr to buffer
// size of sector
// size of step
inline 
void SwapN(Smpl * a, Smpl * b, Smpl * buf, SInt size, SInt step){
  for (SInt i = 0 ; i < size; ++i ){
    buf[i] = a[step*(i)]; // a to buf
    a[step*(i)]=b[step*(i)]; // b to a
  }
  
  for (SInt i = 0 ; i < size ; ++i ){
    b[step*(i)] = buf[i]; // buf to b
  }
}

// ptr to move to
// ptr to move from
// ptr to swap space
// ptr to size of remained, note this wont necaseraly be the size moved,
// this depend on what sector can fit into it
// note ptr to and from must be on same array,we dont care about length
inline
void MoveN(Smpl * to, Smpl * frm, Smpl * buf, SInt size, SInt step){
  ASSERT ( to <= frm );
  
  // from "frm" to "buf"
  for ( SInt i = 0 ; i < size ; ++i ){
    buf[i] = frm[step*i];
  }
  // move right by "size" until we hit "to"
  Smpl * move_to, * move_frm;
  while ( frm > to ){
    move_to = (frm+((size-1)*step));
    move_frm = (frm-(step));
    (*move_to) = (*move_frm);
    --frm;
  }
  // "buf" to "to"
  for ( SInt i = 0 ; i < size ; ++i ){
    to[step*i] = buf[i];
  }
}

void
HeatWaveLift::SplitN(Smpl * data, SInt len, SInt step,
                     Smpl* & even, Smpl* & odd)
{
  ASSERT ( len >= 0 ); ASSERT ( step > 0 );
  const SInt virSecLen = HEATWAVELIFTSTACKN;
  
  SInt virLen = (len/step) + (len%step);
  if ( virLen <= 1 ){
    return;
  }  
  register Smpl buff[virSecLen];
  SInt i_end, i_skip, i_ptr, i_len, i_nodes, i_small, i_mixed, i_lft, i_rht, i_mrk;
  
  // build double odds & evens ...
  i_skip = 4;
  i_ptr = 1;
  i_end = virLen - 1;
  while ( i_ptr < i_end ){
    SwapN(&(data[step*(i_ptr)]), &(data[step*(i_ptr+1)]), buff, 1, step);
    i_ptr += i_skip;
  }
  
  // build quatruple and up odds & evens ... upto 1/2 HEATWAVELIFTSTACKN
  SInt i = 2;
  for ( ; (i < HEATWAVELIFTSTACKN); i <<= 1 ) {
    i_small = virLen%i;
    i_len = virLen - i_small;
    i_skip = i << 2;
    i_ptr = i;
    i_nodes = (i_len-(i-1))/i;
    if ( (i_nodes/2) == 0 ){
      i_mixed = 0;
    }
    else{
      i_mixed = 1;
    }
    
    if ( i_mixed && i_small ){
      i_end = i_len - i;
    }
    else if ( i_mixed ){
      i_end = i_len - (i<<1);
    }
    else if ( i_small ){
      i_end = i_len - i;
    }
    else{
      i_end = i_len - i;
    }
    
    while ( i_ptr < i_end ){
      SwapN(&(data[step*(i_ptr)]), &(data[step*(i_ptr+i)]), buff, i, step);
      i_ptr += i_skip;
    }
  }
  
  // interchange the left sections ...
  i = HEATWAVELIFTSTACKN;
  i_nodes = (virLen/(i<<1))*2;
  i_small = virLen%(i<<1);
  // might have and extra node (we can ignore i_small then)
  if ( i_small == ((i<<1)-1) ){
    ++i_nodes;
    i_small = 0;
  }

  if ( i_nodes > 2 ){
    i_lft = 1;
    i_rht = ((i_nodes%2)==0) ? i_nodes - 1 : i_nodes;
    for ( ; i_lft < i_rht ; ++i_lft, --i_rht ){
      for ( i_mrk = i_lft ; i_mrk < i_rht ; i_mrk += 2 ){
        SwapN(&(data[step*(i_mrk*HEATWAVELIFTSTACKN)]), 
              &(data[step*((i_mrk+1)*HEATWAVELIFTSTACKN)]), 
              buff, HEATWAVELIFTSTACKN, step);
      }
    }
  }
  
  // deal with remainder ... 
  // NOTE this section is very specific for where we have a buffer of length 4
  // time is running out so I had to compromise
  if ( i_small != 0 ){
    Smpl * ptr_rem, size;
    ptr_rem = &(data[(i_nodes*i)*step]);
    if ( i_small > i ){
      MoveN(&(ptr_rem[2*step]), &(ptr_rem[4*step]), buff, 1, step);
    }
    size = (i_small/2)+(i_small%2);
    if ( i_nodes ){
      ASSERT ( (i_nodes%2) == 0 );
      MoveN(&(data[(((i_nodes/2)*i)*step)]), &(data[(((i_nodes)*i)*step)]), buff, 
            size, step);
    }   
  } 
  return;
}

void 
HeatWaveLift::JoinN(Smpl * data, SInt len, SInt step)
{
  ASSERT ( len >= 0 );
  ASSERT ( step > 0 );
  SInt virLen = (len/step) + (len%step);
  if ( virLen <= 1 ){
    return;
  }
  SInt even_len, odd_len;
  FindLengths(virLen, even_len, odd_len);

  SInt rght_mark = virLen-(virLen%2);
  SInt left_mark = 1;
  register Smpl swap_space;
  SInt rht = ((virLen/2) + (virLen%2));;
  SInt lft = rht - 1;
  
  for ( ; lft >= left_mark ; --lft , ++rht ){
    ASSERT ( rht <= (rght_mark) );
    for ( SInt mrk = lft ; mrk < rht ; mrk += 2){
      swap_space = data[step*mrk];
      data[step*mrk] = data[step*(mrk+1)];
      data[step*(mrk+1)] = swap_space;
    }
  }
}

void 
HeatWaveLift::FindLengths(const SInt orgL, SInt & evnL, SInt & oddL) const
{
  ASSERT ( orgL >= 0 );
  evnL = (orgL/2) + (orgL%2);
  oddL = (orgL/2);
  ASSERT ( (evnL+oddL) == orgL );
}

void 
HeatWaveLift::SplitVideo(HeatWaveImage ** imgs, SInt ilen)
{
  HeatWaveImage ** tmp = new HeatWaveImage*[ilen];
  LEAVEONNULL(tmp);
  memset((char*)tmp,'\0',ilen*sizeof(HeatWaveImage*));
  SInt even_len, odd_len;
  FindLengths(ilen, even_len, odd_len);

  for ( SInt i = 0; i < even_len ; ++i ){
    tmp[i] = imgs[i*2];
  }
  for ( SInt i = 0; i < odd_len ; ++i ){
    tmp[i+even_len] = imgs[(i*2)+1];
  }
  for ( SInt i = 0; i <ilen ; ++i ){
    imgs[i] = tmp[i];
  }
  delete tmp;
}

void 
HeatWaveLift::JoinVideo(HeatWaveImage ** imgs, SInt ilen)
{
  HeatWaveImage ** tmp = new HeatWaveImage*[ilen];
  LEAVEONNULL(tmp);
  memset((char*)tmp,'\0',ilen*sizeof(HeatWaveImage*));
  SInt even_len = (ilen/2) + (ilen%2);
  SInt odd_len = (ilen/2);
  ASSERT ( (even_len+odd_len) ==ilen );
  for ( SInt i = 0; i < even_len ; ++i ){
    tmp[i*2] = imgs[i];
  }
  for ( SInt i = 0; i < odd_len ; ++i ){
    tmp[(i*2)+1] = imgs[i+even_len];
  }
  for ( SInt i = 0; i <ilen ; ++i ){
    imgs[i] = tmp[i];
  }
  delete tmp;
}

SInt
HeatWaveLift::GetFuncArray(HeatWaveLift::m_lftFunction * func,  
                           EnumTransform tran, Bool dirs, Bool prd, Bool upd)
{
  memset((char*)func,'\0',HEATWAVELIFTMAXSTEPS*sizeof(m_lftFunction));
  int j = 0;
  switch ( tran ){
  case Trn0_0: // The (0,0) transform
    break;    
  case Trn1_1: // The (1,1) transform
    if ( prd ){
      func[j] = &HeatWaveLift::Trn1_1Prd;
      ++j;
    }
    if ( upd ){
      func[j] = &HeatWaveLift::Trn1_1Upd;
      ++j;
    }
    break;
  case Trn1_1m: // The (1,1)+PPP transform
    if ( prd ){
      func[j] = &HeatWaveLift::Trn1_1mPrd;
      ++j;
    }
    if ( upd ){
      func[j] = &HeatWaveLift::Trn1_1mUpd;
      ++j;
    }
    break;
  case Trn2_2: // The (2,2) transform
    if ( prd ){
      func[j] = &HeatWaveLift::Trn2_2Prd;
      ++j;
    }
    if ( upd ){
      func[j] = &HeatWaveLift::Trn2_2Upd;
      ++j;
    }
    break;
  case Trn2_4: // The (2,4) transform
    if ( prd ){
      func[j] = &HeatWaveLift::Trn2_2Prd;
      ++j;
    }
    if ( upd ){
      func[j] = &HeatWaveLift::Trn4_4BUpd;
      ++j;
    }
    break;
  case Trn4_2: // The (4,2) transform
    if ( prd ){
      func[j] = &HeatWaveLift::Trn4_4Prd;
      ++j;
    }
    if ( upd ){
      func[j] = &HeatWaveLift::Trn2_2Upd;
      ++j;
    }
    break;
  case Trn4_4: // The (4,4) transform
    if ( prd ){
      func[j] = &HeatWaveLift::Trn4_4Prd;
      ++j;
    }
    if ( upd ){
      func[j] = &HeatWaveLift::Trn4_4Upd;
      ++j;
    }
    break;
  case Trn6_2: // The (6,2) transform
    if ( prd ){
      func[j] = &HeatWaveLift::Trn6_6Prd;
      ++j;
    }
    if ( upd ){
      func[j] = &HeatWaveLift::Trn2_2Upd;
      ++j;
    }
    break;
  case Trn6_6: // The (6,6) transform
    if ( prd ){
      func[j] = &HeatWaveLift::Trn6_6Prd;
      ++j;
    }
    if ( upd ){
      func[j] = &HeatWaveLift::Trn6_6Upd;
      ++j;
    }
    break;
  case Trn2p2_2: // The (2+2,2) transform
    if ( prd ){
      func[j] = &HeatWaveLift::Trn2_2Prd;
      ++j;
    }
    if ( upd ){
      func[j] = &HeatWaveLift::Trn2_2Upd;
      ++j;
    }
    if ( prd ){
      func[j] = &HeatWaveLift::Trn2p2_2Prd;
      ++j;
    }
    break;
  case TrnD4: // The (D4) transform
    if ( prd ){
      func[j] = &HeatWaveLift::TrnD4_1Prd;
      ++j;
    }
    if ( upd ){
      func[j] = &HeatWaveLift::TrnD4_Upd;
      ++j;
    }
    if ( prd ){
      func[j] = &HeatWaveLift::TrnD4_2Prd;
      ++j;
    }
    break;
  case Trn9m7: // The (9-7) transform
    if ( prd ){
      func[j] = &HeatWaveLift::Trn97_1Prd;
      ++j;
    }
    if ( upd ){
      func[j] = &HeatWaveLift::Trn97_1Upd;
      ++j;
    }
    if ( prd ){
      func[j] = &HeatWaveLift::Trn97_2Prd;
      ++j;
    }
    if ( upd ){
      func[j] = &HeatWaveLift::Trn97_2Upd;
      ++j;
    }
    break;
  default: // The Kamakazi transform
    WARN_IF ( False );
    break;
  }
  if ( (!dirs) && (j>1) ){
    // Reverse the functions.
    void (HeatWaveLift::*func_tmp[HEATWAVELIFTMAXSTEPS])(Smpl *, Smpl *, SInt, 
                                                         SInt, Bool) const;
    for ( SInt i = 0 ; i < j ; ++ i ){
      func_tmp[(j-1)-i] = func[i];
    }
    for ( SInt i = 0 ; i < j ; ++i ){
      func[i] = func_tmp[i];
    }
  }
  return j;
}

/****************************************************************************/
/*                             Transform (1,1)                              */

void 
HeatWaveLift::Trn1_1PrdFwd(const Smpl & a, Smpl & b) const
{
  b -= a;
}

void
HeatWaveLift::Trn1_1UpdFwd(Smpl & a, const Smpl & b) const
{
  a += b >> 1;
}

void 
HeatWaveLift::Trn1_1UpdRev(Smpl & a, const Smpl & b) const
{
  a -= b >> 1;
}

void 
HeatWaveLift::Trn1_1PrdRev(const Smpl & a, Smpl & b) const
{
  b += a;
}

void 
HeatWaveLift::Trn1_1Prd(Smpl * even, Smpl * odd, SInt len, SInt step, 
                        Bool forward) const
{
  void (HeatWaveLift::*fnc) (const Smpl & a, Smpl & b) const;
  SInt odd_len = len >> 1;
  ASSERT ( step >= 1 );
  
  if ( len < 2 ){
    return;
  }
  
  if ( forward ){
    fnc = &HeatWaveLift::Trn1_1PrdFwd;
  }
  else{
    fnc = &HeatWaveLift::Trn1_1PrdRev;
  }
  
  for ( SInt i = 0 ; i < odd_len ; ++i ){
    (this->*fnc)(even[i*step],odd[i*step]);
  }
}

void 
HeatWaveLift::Trn1_1Upd(Smpl * even, Smpl * odd, SInt len, SInt step, 
                        Bool forward) const
{
  void (HeatWaveLift::*fnc) (Smpl & a, const Smpl & b) const;
  SInt odd_len = len >> 1;
  ASSERT ( step >= 1 );
  
  if ( len < 2 ){
    return;
  }
  
  if ( forward ){
    fnc = &HeatWaveLift::Trn1_1UpdFwd;
  }
  else{
    fnc = &HeatWaveLift::Trn1_1UpdRev;
  }
  
  for ( SInt i = 0; i < odd_len ; ++i ){
    (this->*fnc)(even[i*step],odd[i*step]);
  }
}

/****************************************************************************/
/*                             Transform (1,1)+PPP                          */

void 
HeatWaveLift::Trn1_1mPrdFwd(const Smpl & a, Smpl & b) const
{
  b = (m_mod+(b-a))%m_mod;
  ASSERT(b >= 0);
  ASSERT(b < m_mod);
}

void
HeatWaveLift::Trn1_1mUpdFwd(Smpl & a, const Smpl & b) const
{
  a = ((m_mod-a)+((m_mod<<1)-b))%m_mod;
  ASSERT(a >= 0);
  ASSERT(a < m_mod);
}

void 
HeatWaveLift::Trn1_1mUpdRev(Smpl & a, const Smpl & b) const
{
  a = ((((m_mod+1)>>1)*a)+(((m_mod+1)>>1*b)))%m_mod;
  ASSERT(a >= 0);
  ASSERT(a < m_mod);
}

void 
HeatWaveLift::Trn1_1mPrdRev(const Smpl & a, Smpl & b) const
{
  b = (a+b)%m_mod;
  ASSERT(b >= 0);
  ASSERT(b < m_mod);
}

void 
HeatWaveLift::Trn1_1mPrd(Smpl * even, Smpl * odd, SInt len, SInt step, 
                         Bool forward) const
{
  void (HeatWaveLift::*fnc) (const Smpl & a, Smpl & b) const;
  SInt odd_len = len >> 1;
  ASSERT ( step >= 1 );
  
  if ( len < 2 ){
    return;
  }
  
  if ( forward ){
    fnc = &HeatWaveLift::Trn1_1mPrdFwd;
  }
  else{
    fnc = &HeatWaveLift::Trn1_1mPrdRev;
  }
  
  for ( SInt i = 0 ; i < odd_len ; ++i ){
    (this->*fnc)(even[i*step],odd[i*step]);
  }
}

void 
HeatWaveLift::Trn1_1mUpd(Smpl * even, Smpl * odd, SInt len, SInt step, 
                         Bool forward) const
{
  void (HeatWaveLift::*fnc) (Smpl & a, const Smpl & b) const;
  SInt odd_len = len >> 1;
  ASSERT ( step >= 1 );
  
  if ( len < 2 ){
    return;
  }
  
  if ( forward ){
    fnc = &HeatWaveLift::Trn1_1mUpdFwd;
  }
  else{
    fnc = &HeatWaveLift::Trn1_1mUpdRev;
  }
  
  for ( SInt i = 0; i < odd_len ; ++i ){
    (this->*fnc)(even[i*step],odd[i*step]);
  }
}

/****************************************************************************/
/*                             Transform (2,2)                              */

void 
HeatWaveLift::Trn2_2PrdFwd(const Smpl & a, Smpl & b, const Smpl & c) const
{
  b -= (((a+c)+1)>>1);
}

void 
HeatWaveLift::Trn2_2UpdFwd(const Smpl & a, Smpl & b, const Smpl & c) const
{
  b += (((a+c)+2)>>2);
}

void 
HeatWaveLift::Trn2_2UpdRev(const Smpl & a, Smpl & b, const Smpl & c) const
{
  b -= (((a+c)+2)>>2);
}

void 
HeatWaveLift::Trn2_2PrdRev(const Smpl & a, Smpl & b, const Smpl & c) const
{
  b += (((a+c)+1)>>1);
}

void 
HeatWaveLift::Trn2_2Prd(Smpl * even, Smpl * odd, SInt len, SInt step, 
                        Bool forward) const
{
  void (HeatWaveLift::*fnc) (const Smpl & a, Smpl & b, const Smpl & c) const;
  Bool is_even = (len%2==0);
  Smpl odd_len = len >> 1;
  ASSERT ( odd_len >= 1 );
  
  if ( len < 4 ){
    Trn1_1Prd(even,odd,len,step,forward);
    return;
  }
  
  if ( forward ){
    fnc = &HeatWaveLift::Trn2_2PrdFwd;
  }
  else{
    fnc = &HeatWaveLift::Trn2_2PrdRev;
  }
  
  SInt i = 0;
  
  for ( ; i < (odd_len-(is_even?1:0)) ; ++i ){
    (this->*fnc)(even[i*step],odd[i*step],even[(i+1)*step]);
  }
  
  /* if last odd sample is on edge */
  if ( is_even ){
    (this->*fnc)(even[i*step],odd[i*step],even[i*step]);
  }
}

void 
HeatWaveLift::Trn2_2Upd(Smpl * even, Smpl * odd, SInt len, SInt step, 
                        Bool forward) const
{
  void (HeatWaveLift::*fnc) (const Smpl & a, Smpl & b, const Smpl & c) const;
  Bool is_even = (len%2==0);
  Smpl even_len = (len+1) >> 1;
  
  if ( len < 4 ){
    Trn1_1Upd(even,odd,len,step,forward);
    return;
  }
  
  if ( forward ){
    fnc = &HeatWaveLift::Trn2_2UpdFwd;
  }
  else{
    fnc = &HeatWaveLift::Trn2_2UpdRev;
  }
  
  SInt i = 0;
  
  /* First even sample needs special arrangemet */
  (this->*fnc)(odd[(i+0)*step], even[(i)*step], odd[(i+0)*step]);
  i++;
  
  for ( ; i < even_len-(is_even?0:1) ; ++i ){
    (this->*fnc)(odd[(i-1)*step], even[i*step], odd[i*step]);
  }
  
  /* Last even sample needs special arrangement */
  if (!is_even){
    (this->*fnc)(odd[(i-1)*step], even[i*step], odd[(i-1)*step]);
  }
}

/****************************************************************************/
/*                             Transform (2+2,2)                            */

void 
HeatWaveLift::Trn2p2_2PrdFwd(const Smpl & a, const Smpl & b, Smpl & c, 
                             const Smpl & d, const Smpl & e) const
{  
  c += ((a + e) - (d + b) + 8) >> 4;
}

void
HeatWaveLift::Trn2p2_2PrdRev(const Smpl & a, const Smpl & b, Smpl & c, 
                             const Smpl & d, const Smpl & e) const
{
  c -= ((a + e) - (d + b) + 8) >> 4;
}

void
HeatWaveLift::Trn2p2_2Prd(Smpl * even, Smpl * odd, SInt len, SInt step, 
                          Bool forward) const
{
  void (HeatWaveLift::*fnc) (const Smpl & a, const Smpl & b, Smpl & c, 
                             const Smpl & d, const Smpl & e) const;
  Bool is_even = (len%2==0);
  Smpl odd_len = len >> 1;
  ASSERT ( odd_len >= 1 );
  
  if ( len < 8 ){
    return;
  }
  
  if ( forward ){
    fnc = &HeatWaveLift::Trn2p2_2PrdFwd;
  }
  else{
    fnc = &HeatWaveLift::Trn2p2_2PrdRev;
  }

  SInt i = 0;
  
  /* first odd sample needs special attention */
  (this->*fnc)(even[(i+0)*step],even[(i+0)*step],odd[(i+0)*step],
               even[(i+1)*step],even[(i+2)*step]);
  i++;
  
  for ( ; i < odd_len-(is_even?2:1) ; ++i ){
    (this->*fnc)(even[(i-1)*step],even[i*step],odd[i*step],
                 even[(i+1)*step],even[(i+2)*step]);
  }
  
  /* last/second last odd sample needs special attention */
  (this->*fnc)(even[(i-1)*step],even[i*step],odd[i*step],
               even[(i+1)*step],even[(i+1)*step]);
  
  if ( is_even ){
    ++i;
    /* last odd sample needs special attention */
    (this->*fnc)(even[(i-1)*step],even[i*step],odd[i*step],
                 even[i*step],even[(i-1)*step]);
  }
}

/****************************************************************************/
/*                             Transform (4,4)                              */

void 
HeatWaveLift::Trn4_4PrdFwd(const Smpl & a, const Smpl & b, Smpl & c, 
                           const Smpl & d, const Smpl & e) const
{
  c -= ((9*(b+d))-(1*(a+e))+8) >> 4;
}

void 
HeatWaveLift::Trn4_4UpdFwd(const Smpl & a, const Smpl & b, Smpl & c, 
                           const Smpl & d, const Smpl & e) const
{
  c += ((9*(b+d))-(1*(a+e))+16) >> 5;
}

void 
HeatWaveLift::Trn4_4UpdRev(const Smpl & a, const Smpl & b, Smpl & c, 
                           const Smpl & d, const Smpl & e) const
{
  c -= ((9*(b+d))-(1*(a+e))+16) >> 5;
}

void
HeatWaveLift::Trn4_4PrdRev(const Smpl & a, const Smpl & b, Smpl & c, 
                           const Smpl & d, const Smpl & e) const
{
  c += ((9*(b+d))-(1*(a+e))+8) >> 4;
}

void 
HeatWaveLift::Trn4_4BUpdFwd(const Smpl & a, const Smpl & b, Smpl & c, 
                            const Smpl & d, const Smpl & e) const
{
  c += ((19*(b+d))-(3*(a+e))+32) >> 6;
}

void 
HeatWaveLift::Trn4_4BUpdRev(const Smpl & a, const Smpl & b, Smpl & c, 
                            const Smpl & d, const Smpl & e) const
{
  c -= ((19*(b+d))-(3*(a+e))+32) >> 6;
}

void
HeatWaveLift::Trn4_4Prd(Smpl * even, Smpl * odd, SInt len, SInt step, 
                        Bool forward) const
{
  void (HeatWaveLift::*fnc) (const Smpl & a, const Smpl & b, Smpl & c, 
                             const Smpl & d, const Smpl & e) const;
  Bool is_even = (len%2==0);
  Smpl odd_len = len >> 1;
  ASSERT ( odd_len >= 1 );
  
  if ( len < 8 ){
    Trn2_2Prd(even, odd, len, step, forward);
    return;
  }
  
  if ( forward ){
    fnc = &HeatWaveLift::Trn4_4PrdFwd;
  }
  else{
    fnc = &HeatWaveLift::Trn4_4PrdRev;
  }

  SInt i = 0;
  
  /* first odd sample needs special attention */
  (this->*fnc)(even[(i+0)*step],even[(i+0)*step],odd[i*step],
               even[(i+1)*step],even[(i+2)*step]);
  ++i;
  
  for ( ; i < odd_len-(is_even?2:1) ; ++i ){
    (this->*fnc)(even[(i-1)*step],even[i*step],odd[i*step],
                 even[(i+1)*step],even[(i+2)*step]);
  }
  
  /* last/second last odd sample needs special attention */
  (this->*fnc)(even[(i-1)*step],even[i*step],odd[i*step],
               even[(i+1)*step],even[(i+1)*step]);
  
  if ( is_even ){
    ++i;
    /* last odd sample needs special attention */
    (this->*fnc)(even[(i-1)*step],even[i*step],odd[i*step],
                 even[i*step],even[(i-1)*step]);
  }
}

void 
HeatWaveLift::Trn4_4Upd(Smpl * even, Smpl * odd, SInt len, SInt step, 
                        Bool forward) const
{
  void (HeatWaveLift::*fnc) (const Smpl & a, const Smpl & b, Smpl & c, 
                             const Smpl & d, const Smpl & e) const;
  Bool is_even = (len%2==0);
  Smpl even_len = (len+1) >> 1;
  
  if ( len < 8 ){
    Trn2_2Upd(even, odd, len, step, forward);
    return;
  }
  
  if ( forward ){
    fnc = &HeatWaveLift::Trn4_4UpdFwd;
  }
  else{
    fnc = &HeatWaveLift::Trn4_4UpdRev;
  }
  
  SInt i = 0;
  
  /* first even sample needs special attention */
  (this->*fnc)(odd[(i+1)*step],odd[(i+0)*step],even[(i)*step],
               odd[(i+0)*step],odd[(i+1)*step]);
  ++i;
  
  /* second even sample needs special attention */
  (this->*fnc)(odd[(i-1)*step],odd[(i-1)*step],even[(i)*step],
               odd[(i+0)*step],odd[(i+1)*step]);
  ++i;
  
  for ( ; i < even_len-(is_even?1:2) ; ++i ){
    (this->*fnc)(odd[(i-2)*step],odd[(i-1)*step],even[i*step],
                 odd[(i+0)*step],odd[(i+1)*step]);
  }
  
  /* last/second last even sample needs special attention */
  (this->*fnc)(odd[(i-2)*step],odd[(i-1)*step],even[i*step],
               odd[(i+0)*step],odd[(i+0)*step]);
  
  if ( !is_even ){
    ++i;
    /* last even sample needs special attention */
    (this->*fnc)(odd[(i-2)*step],odd[(i-1)*step],even[i*step],
                 odd[(i-1)*step],odd[(i-2)*step]);
  }
}

void 
HeatWaveLift::Trn4_4BUpd(Smpl * even, Smpl * odd, SInt len, SInt step, 
                         Bool forward) const
{
  void (HeatWaveLift::*fnc) (const Smpl & a, const Smpl & b, Smpl & c, 
                             const Smpl & d, const Smpl & e) const;
  Bool is_even = (len%2==0);
  Smpl even_len = (len+1) >> 1;
  
  if ( len < 8 ){
    Trn2_2Upd(even, odd, len, step, forward);
    return;
  }
  
  if ( forward ){
    fnc = &HeatWaveLift::Trn4_4BUpdFwd;
  }
  else{
    fnc = &HeatWaveLift::Trn4_4BUpdRev;
  }
  
  SInt i = 0;
  
  /* first even sample needs special attention */
  (this->*fnc)(odd[(i+1)*step],odd[(i+0)*step],even[(i)*step],
               odd[(i+0)*step],odd[(i+1)*step]);
  ++i;
  
  /* second even sample needs special attention */
  (this->*fnc)(odd[(i-1)*step],odd[(i-1)*step],even[(i)*step],
               odd[(i+0)*step],odd[(i+1)*step]);
  ++i;
  
  for ( ; i < even_len-(is_even?1:2) ; ++i ){
    (this->*fnc)(odd[(i-2)*step],odd[(i-1)*step],even[i*step],
                 odd[(i+0)*step],odd[(i+1)*step]);
  }
  
  /* last/second last even sample needs special attention */
  (this->*fnc)(odd[(i-2)*step],odd[(i-1)*step],even[i*step],
               odd[(i+0)*step],odd[(i+0)*step]);
  
  if ( !is_even ){
    ++i;
    /* last even sample needs special attention */
    (this->*fnc)(odd[(i-2)*step],odd[(i-1)*step],even[i*step],
                 odd[(i-1)*step],odd[(i-2)*step]);
  }
}

/****************************************************************************/
/*                             Transform (6,6)                              */

void 
HeatWaveLift::Trn6_6PrdFwd(const Smpl & a, const Smpl & b, const Smpl & c, 
                           Smpl & d, const Smpl & e, const Smpl & f,
                           const Smpl & g) const
{
  d -= (150*(c+e)-25*(b+f)+3*(a+g)+128)>>8;
}

void 
HeatWaveLift::Trn6_6UpdFwd(const Smpl & a, const Smpl & b, const Smpl & c, 
                           Smpl & d, const Smpl & e, const Smpl & f,
                           const Smpl & g) const
{
  d += (150*(c+e)-25*(b+f)+3*(a+g)+256)>>9;
}

void 
HeatWaveLift::Trn6_6UpdRev(const Smpl & a, const Smpl & b, const Smpl & c, 
                           Smpl & d, const Smpl & e, const Smpl & f,
                           const Smpl & g) const
{
  d -= (150*(c+e)-25*(b+f)+3*(a+g)+256)>>9;
}

void 
HeatWaveLift::Trn6_6PrdRev(const Smpl & a, const Smpl & b, const Smpl & c, 
                           Smpl & d, const Smpl & e, const Smpl & f,
                           const Smpl & g) const
{
  d += (150*(c+e)-25*(b+f)+3*(a+g)+128)>>8;
}

void
HeatWaveLift::Trn6_6Prd(Smpl * even, Smpl * odd, SInt len, SInt step, 
                        Bool forward) const
{
  void (HeatWaveLift::*fnc) (const Smpl & a, const Smpl & b, const Smpl & c, 
                             Smpl & d, const Smpl & e, const Smpl & f,
                             const Smpl & g) const;
  Bool is_even = (len%2==0);
  Smpl odd_len = len >> 1;
  ASSERT ( odd_len >= 1 );
  
  if ( len < 16 ){
    Trn4_4Prd(even, odd, len, step, forward);
    return;
  }
  
  if ( forward ){
    fnc = &HeatWaveLift::Trn6_6PrdFwd;
  }
  else{
    fnc = &HeatWaveLift::Trn6_6PrdRev;
  }
  
  SInt i = 0;
  
  /* first odd sample needs special attention */
  (this->*fnc)(even[(i+1)*step], even[(i-0)*step], even[(i-0)*step],
               odd [(i+0)*step],
               even[(i+1)*step], even[(i+2)*step], even[(i+3)*step]);
  i++;
  
  /* second odd sample needs special attention */
  (this->*fnc)(even[(i-1)*step], even[(i-1)*step], even[(i-0)*step],
               odd [(i+0)*step],
               even[(i+1)*step], even[(i+2)*step], even[(i+3)*step]);
  i++;
  
  for ( ; i < odd_len-(is_even?3:2) ; ++i ){
    (this->*fnc)(even[(i-2)*step], even[(i-1)*step], even[(i-0)*step],
                 odd [(i+0)*step],
                 even[(i+1)*step], even[(i+2)*step], even[(i+3)*step]);
  }
  
  /* third/second last odd sample needs special attention */
  (this->*fnc)(even[(i-2)*step], even[(i-1)*step], even[(i-0)*step],
               odd [(i+0)*step],
               even[(i+1)*step], even[(i+2)*step], even[(i+2)*step]);
  ++i;
  
  /* second/last last odd sample needs special attention */
  (this->*fnc)(even[(i-2)*step], even[(i-1)*step], even[(i-0)*step],
               odd [(i+0)*step], 
               even[(i+1)*step], even[(i+1)*step], even[(i-0)*step]);
  ++i;
  
  if ( is_even ){ 
    /* last odd sample needs special attention */
    (this->*fnc)(even[(i-2)*step], even[(i-1)*step], even[(i-0)*step],
                 odd [(i+0)*step],
                 even[(i-0)*step], even[(i-1)*step], even[(i-2)*step]);
  }
}

void
HeatWaveLift::Trn6_6Upd(Smpl * even, Smpl * odd, SInt len, SInt step, 
                        Bool forward) const
{
  void (HeatWaveLift::*fnc) (const Smpl & a, const Smpl & b, const Smpl & c, 
                             Smpl & d, const Smpl & e, const Smpl & f,
                             const Smpl & g) const;
  Bool is_even = (len%2==0);
  Smpl even_len = (len+1) >> 1;
  
  if ( len < 16 ){
    Trn4_4Upd(even, odd, len, step, forward);
    return;
  }
  
  if ( forward ){
    fnc = &HeatWaveLift::Trn6_6UpdFwd;
  }
  else{
    fnc = &HeatWaveLift::Trn6_6UpdRev;
  }
  
  SInt i = 0;
  
  /* even sample 0 needs special attention */
  (this->*fnc)(odd [(i+2)*step], odd[(i+1)*step], odd[(i+0)*step],
               even[(i+0)*step],
               odd [(i+0)*step], odd[(i+1)*step], odd[(i+2)*step]);
  ++i;
  
  /* even sample 1 needs special attention */
  (this->*fnc)(odd [(i+0)*step], odd[(i-1)*step], odd[(i-1)*step],
               even[(i+0)*step],
               odd [(i+0)*step], odd[(i+1)*step], odd[(i+2)*step]);
  ++i;
  
  /* even sample 2 needs special attention */
  (this->*fnc)(odd [(i-2)*step], odd[(i-2)*step], odd[(i-1)*step],
               even[(i+0)*step],
               odd [(i+0)*step], odd[(i+1)*step], odd[(i+2)*step]);
  ++i;
  
  for ( ; i < even_len-(is_even?2:3) ; ++i ){
    (this->*fnc)(odd [(i-3)*step], odd[(i-2)*step], odd[(i-1)*step],
                 even[(i+0)*step],
                 odd [(i+0)*step], odd[(i+1)*step], odd[(i+2)*step]);
  }
  
  /* even sample n-(is_even?1:2) needs special attention */
  (this->*fnc)(odd [(i-3)*step], odd[(i-2)*step], odd[(i-1)*step],
               even[(i+0)*step],
               odd [(i+0)*step], odd[(i+1)*step], odd[(i+1)*step]);
  ++i;
  
  /* even sample n-(is_even?0:1) needs special attention */
  (this->*fnc)(odd [(i-3)*step], odd[(i-2)*step], odd[(i-1)*step],
               even[(i+0)*step],
               odd [(i+0)*step], odd[(i+0)*step], odd[(i-1)*step]);
  ++i;
  
  if ( !is_even ){
    /* even sample n needs special attention */
    (this->*fnc)(odd [(i-3)*step], odd[(i-2)*step], odd[(i-1)*step],
                 even[(i+0)*step],
                 odd [(i-1)*step], odd[(i-2)*step], odd[(i-3)*step]);
  }
}

/****************************************************************************/
/*                             Transform (D4)                               */

void 
HeatWaveLift::TrnD4_1PrdFwd(const Smpl & a, Smpl & b) const
{
  b -= ((111*a)+32) >> 6;
}

void 
HeatWaveLift::TrnD4_UpdFwd(const Smpl & a, Smpl & b, const Smpl & c) const
{
  b += (((111*c)-(17*a))+128) >> 8;
}

void
HeatWaveLift::TrnD4_2PrdFwd(const Smpl & a, Smpl & b) const
{
  b += a;
}

void
HeatWaveLift::TrnD4_2PrdRev(const Smpl & a, Smpl & b) const
{
  b -= a;
}

void 
HeatWaveLift::TrnD4_UpdRev(const Smpl & a, Smpl & b, const Smpl & c) const
{
  b -= (((111*c)-(17*a))+128) >> 8;
}

void 
HeatWaveLift::TrnD4_1PrdRev(const Smpl & a, Smpl & b) const
{
  b += ((111*a)+32) >> 6;
}

void 
HeatWaveLift::TrnD4_1Prd(Smpl * even, Smpl * odd, SInt len, SInt step, 
                         Bool forward) const
{
  void (HeatWaveLift::*fnc) (const Smpl & a, Smpl & b) const;
  SInt odd_len = len >> 1;
  ASSERT ( step >= 1 );
  
  if ( len < 4 ){
    return;
  }
  
  if ( forward ){
    fnc = &HeatWaveLift::TrnD4_1PrdFwd;
  }
  else{
    fnc = &HeatWaveLift::TrnD4_1PrdRev;
  }
  
  for ( SInt i = 0 ; i < odd_len ; ++i ){
    (this->*fnc)(even[i*step],odd[i*step]);
  }
}

void 
HeatWaveLift::TrnD4_Upd(Smpl * even, Smpl * odd, SInt len, SInt step, 
                        Bool forward) const
{
  void (HeatWaveLift::*fnc) (const Smpl & a, Smpl & b, const Smpl & c) const;
  Bool is_even = (len%2==0);
  Smpl even_len = (len+1) >> 1;
  
  if ( len < 4 ){
    return;
  }
  
  if ( forward ){
    fnc = &HeatWaveLift::TrnD4_UpdFwd;
  }
  else{
    fnc = &HeatWaveLift::TrnD4_UpdRev;
  }
  
  SInt i = 0;
  
  /* First even sample needs special arrangemet */
  (this->*fnc)(odd[(i+0)*step], even[(i)*step], odd[(i+0)*step]);
  i++;
  
  for ( ; i < (even_len-(is_even?0:1)) ; ++i ){
    (this->*fnc)(odd[(i-1)*step], even[i*step], odd[i*step]);
  }
  
  /* Last even sample needs special arrangement */
  if (!is_even){
    (this->*fnc)(odd[(i-1)*step], even[i*step], odd[(i-1)*step]);
  }
}

void 
HeatWaveLift::TrnD4_2Prd(Smpl * even, Smpl * odd, SInt len, SInt step, 
                         Bool forward) const
{
  void (HeatWaveLift::*fnc) (const Smpl & a, Smpl & b) const;
  SInt even_len = (len+1) >> 1;
  ASSERT ( step >= 1 );
  
  if ( len < 4 ){
    return;
  }
  
  if ( forward ){
    fnc = &HeatWaveLift::TrnD4_2PrdFwd;
  }
  else{
    fnc = &HeatWaveLift::TrnD4_2PrdRev;
  }
  
  for ( SInt i = 1 ; i < even_len ; ++i ){
    (this->*fnc)(even[i*step],odd[(i-1)*step]);
  }
}

/****************************************************************************/
/*                             Transform (9-7)                              */

void 
HeatWaveLift::Trn97_1PrdFwd(const Smpl & a, Smpl & b, const Smpl & c) const
{
  b -= ((203*(a+c))+64) >> 7;
}

void 
HeatWaveLift::Trn97_1UpdFwd(const Smpl & a, Smpl & b, const Smpl & c) const
{
  b -= ((217*(a+c))+2048) >> 12;
}

void 
HeatWaveLift::Trn97_2PrdFwd(const Smpl & a, Smpl & b, const Smpl & c) const
{
  b += ((113*(a+c))+64) >> 7;
}

void 
HeatWaveLift::Trn97_2UpdFwd(const Smpl & a, Smpl & b, const Smpl & c) const
{
  b += ((1817*(a+c))+2048) >> 12;
}

void 
HeatWaveLift::Trn97_2UpdRev(const Smpl & a, Smpl & b, const Smpl & c) const
{
  b -= ((1817*(a+c))+2048) >> 12;
}

void 
HeatWaveLift::Trn97_2PrdRev(const Smpl & a, Smpl & b, const Smpl & c) const
{
  b -= ((113*(a+c))+64) >> 7;
}

void 
HeatWaveLift::Trn97_1UpdRev(const Smpl & a, Smpl & b, const Smpl & c) const
{
  b += ((217*(a+c))+2048) >> 12;
}

void 
HeatWaveLift::Trn97_1PrdRev(const Smpl & a, Smpl & b, const Smpl & c) const
{
  b += ((203*(a+c))+64) >> 7;
}

void 
HeatWaveLift::Trn97_1Prd(Smpl * even, Smpl * odd, SInt len, SInt step, 
                         Bool forward) const
{
  void (HeatWaveLift::*fnc) (const Smpl & a, Smpl & b, const Smpl & c) const;
  Bool is_even = (len%2==0);
  Smpl odd_len = len >> 1;
  ASSERT ( odd_len >= 1 );
  
  if ( len < 4 ){
    return;
  }
  
  if ( forward ){
    fnc = &HeatWaveLift::Trn97_1PrdFwd;
  }
  else{
    fnc = &HeatWaveLift::Trn97_1PrdRev;
  }
  
  SInt i = 0;
  
  for ( ; i < (odd_len-(is_even?1:0)) ; ++i ){
    (this->*fnc)(even[i*step],odd[i*step],even[(i+1)*step]);
  }
  
  /* if last odd sample is on edge */
  if ( is_even ){
    (this->*fnc)(even[i*step],odd[i*step],even[i*step]);
  }
}

void 
HeatWaveLift::Trn97_1Upd(Smpl * even, Smpl * odd, SInt len, SInt step, 
                         Bool forward) const
{
  void (HeatWaveLift::*fnc) (const Smpl & a, Smpl & b, const Smpl & c) const;
  Bool is_even = (len%2==0);
  Smpl even_len = (len+1) >> 1;
  
  if ( len < 4 ){
    return;
  }
  
  if ( forward ){
    fnc = &HeatWaveLift::Trn97_1UpdFwd;
  }
  else{
    fnc = &HeatWaveLift::Trn97_1UpdRev;
  }
 
  SInt i = 0;
  
  /* First even sample needs special arrangemet */
  (this->*fnc)(odd[(i+0)*step], even[(i)*step], odd[(i+0)*step]);
  i++;
  
  for ( ; i < (even_len-(is_even?0:1)) ; ++i ){
    (this->*fnc)(odd[(i-1)*step], even[i*step], odd[i*step]);
  }
  
  /* Last even sample needs special arrangement */
  if (!is_even){
    (this->*fnc)(odd[(i-1)*step], even[i*step], odd[(i-1)*step]);
  }
}

void 
HeatWaveLift::Trn97_2Prd(Smpl * even, Smpl * odd, SInt len, SInt step, 
                         Bool forward) const
{
  void (HeatWaveLift::*fnc) (const Smpl & a, Smpl & b, const Smpl & c) const;
  Bool is_even = (len%2==0);
  Smpl odd_len = len >> 1;
  ASSERT ( odd_len >= 1 );
  
  if ( len < 4 ){
    return;
  }
  
  if ( forward ){
    fnc = &HeatWaveLift::Trn97_2PrdFwd;
  }
  else{
    fnc = &HeatWaveLift::Trn97_2PrdRev;
  }
  
  SInt i = 0;
  
  for ( ; i < (odd_len-(is_even?1:0)) ; ++i ){
    (this->*fnc)(even[i*step],odd[i*step],even[(i+1)*step]);
  }
  
  /* if last odd sample is on edge */
  if ( is_even ){
    (this->*fnc)(even[i*step],odd[i*step],even[i*step]);
  }
}

void 
HeatWaveLift::Trn97_2Upd(Smpl * even, Smpl * odd, SInt len, SInt step, 
                         Bool forward) const
{
  void (HeatWaveLift::*fnc) (const Smpl & a, Smpl & b, const Smpl & c) const;
  Bool is_even = (len%2==0);
  Smpl even_len = (len+1) >> 1;
  
  if ( len < 4 ){
    return;
  }
  
  if ( forward ){
    fnc = &HeatWaveLift::Trn97_2UpdFwd;
  }
  else{
    fnc = &HeatWaveLift::Trn97_2UpdRev;
  }
  
  SInt i = 0;
  
  /* First even sample needs special arrangemet */
  (this->*fnc)(odd[(i+0)*step], even[(i)*step], odd[(i+0)*step]);
  i++;
  
  for ( ; i < (even_len-(is_even?0:1)) ; ++i ){
    (this->*fnc)(odd[(i-1)*step], even[i*step], odd[i*step]);
  }
  
  /* Last even sample needs special arrangement */
  if (!is_even){
    (this->*fnc)(odd[(i-1)*step], even[i*step], odd[(i-1)*step]);
  }
}

/****************************************************************************/
/*                               Other class functions                      */

SInt 
HeatWaveLift::GetBufferSize() const
{
  return m_bufferLen;
}

void 
HeatWaveLift::SetBufferSize(SInt siz)
{
  DoAllocate(siz);
}

Bool 
HeatWaveLift::operator==(const HeatWaveLift & oth) const
{
  return (m_bufferLen == oth.m_bufferLen);
}

HeatWaveLift & 
HeatWaveLift::operator=(const HeatWaveLift & oth)
{
  DoAllocate(oth.m_bufferLen);
  return (*this);
}

void 
HeatWaveLift::DoAllocate(SInt len)
{
  if ( len < m_bufferLen ){
    return;
  }
  if ( m_bufferLen > 0){
    DoRelease();
  }
  if ( len < 1 ){
    m_bufferLen = 0;
    return;
  }
  m_bufferLen = len;
  m_buffer = new Smpl[m_bufferLen];
  LEAVEONNULL(m_buffer);
}

void 
HeatWaveLift::DoRelease()
{
  delete [] m_buffer;
  m_buffer = NULL;
}
