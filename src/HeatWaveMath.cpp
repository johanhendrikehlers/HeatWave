/**
 *
 * @file HeatWaveMath.cpp
 * @brief Class HeatWaveMath function definitions.
 * @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 *
 **/

#include "HeatWaveMath.hpp"

Smpl 
HeatWaveMath::GCD ( Smpl a, Smpl b)
{
  if ( (a == 0) || (b == 0) ){
    return 0;
  }
  if ( a < 0 ){
    a *= -1;
  }
  if ( b < 0 ) {
    b *= -1;
  }
  
  if ( a < b ){
    Swap(a,b);
  }
    
  Smpl t;
  while (b != 0) {
    t = b;
    b = a % b;
    a = t;
  }

  return a;
}

Smpl 
HeatWaveMath::Pow2(Smpl exponent)
{
  ASSERT ( exponent >= 0 );
  
  if ( exponent == 0 ){
    return 1;
  }
  else{
    return (2 << (exponent-1));
  }
}

Smpl 
HeatWaveMath::Pow(Smpl base,Smpl exponent)
{
  ASSERT ( exponent >= 0 );
  
  if ( base == 0 ){
    return 0;
  }
  else if ( base == 1 || exponent == 0 ){
    return 1;
  }

  Smpl ret = base; 

  for ( Smpl i = 0; i < (exponent-1); i++ ){
    ret *= base;
  }
  
  return ret;
}

Smpl 
HeatWaveMath::Min(Smpl a, Smpl b)
{
  return (a < b)?a:b;
}

Smpl 
HeatWaveMath::Max(Smpl a, Smpl b)
{
  return (a>b)?a:b;
}

void
HeatWaveMath::Swap(Smpl & a, Smpl & b)
{
  Smpl t = a;
  a = b;
  b = t;
}

SFloat64
HeatWaveMath::PDF(SFloat64 sigma, SFloat64 mean, SFloat64 value)
{
  SFloat64 B = sigma/sqrt(2.0L);
  return exp(-fabs(value-mean)/B)/2/B;
}

