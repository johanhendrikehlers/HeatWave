/****************************************************************************/
/**
 ** @file HeatWaveTypes.hpp
 ** @brief HeatWave types and descriptors for speeding up things.
 ** @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 **
 **/

#ifndef __HEATWAVETYPES_HPP__
#define __HEATWAVETYPES_HPP__

#include "CommonDataTypes.hpp"

/** Sample/coeffecient/pixel data type. **/
typedef SInt32 Smpl;

/** Ptr tp sample/coeffecient/pixel data type. **/
typedef Smpl * SmplPtr; 

#define SMPLPRECISION (32)
#define SMPLSIGNED    (1)
#define SMPLMAX       ( 2147483647)
#define SMPLMIN       (-2147483648) 

/****************************************************************************/
/**
 **
 ** A Stucture for defining a square area.
 **
 **/

struct Crd
{
  SInt x;
  SInt y;
};

struct Sqr
{
  SInt x;
  SInt y;
  SInt l;
};

struct Rct
{
  SInt x;
  SInt y;
  SInt w;
  SInt h;
};

CREATE_A_VECTOR_DESCRIPTOR(CrdA, Crd)
CREATE_A_VECTOR_DESCRIPTOR(SqrA, Sqr)
CREATE_A_VECTOR_DESCRIPTOR(RctA, Rct)

/****************************************************************************/

#endif // __HEATWAVETYPES_HPP__
