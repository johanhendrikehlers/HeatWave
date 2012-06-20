/****************************************************************************/
/**
 ** @file   CommonInlines.hpp
 ** @brief  Common Inline functions are defined here.
 ** @author Johan Hendrik Ehlers <https://github.com/johanhendrikehlers>
 **
 **/

#ifndef __COMMONINLINES_HPP__
#define __COMMONINLINES_HPP__

#include "CommonIncludes.hpp"

/**
 *
 * Return a matrix of SFloat64's.
 *
 * @param rows Numbers of rows.
 * @param cols Numberd of columns.
 * @param defv The default value. Default is 0.
 *
 **/

inline SFloat64 ** NewMatrixSFloat64(SInt rows, SInt cols, SFloat64 defv = 0)
{
  ASSERT ( rows > 0 );
  ASSERT ( cols > 0 );
  SFloat64 * tmp = new SFloat64[rows*cols];
  LEAVEONNULL(tmp);
  SFloat64 ** ret = new SFloat64*[rows];
  LEAVEONNULL(ret);
  for ( SInt i = 0 ; i < rows ; ++i ){
    ret[i] = (&(tmp[i*cols])); 
  }
  for ( SInt i = 0; i < (rows*cols) ; ++i ){
    tmp[i] = defv;
  }
  return ret;
}

/**
 *
 * Delete a matrix of SFLoat64's and set pointer to NULL.
 *
 * @param mtrx The matrix to clear up.
 *
 **/

inline void DelMatrixSFloat64(SFloat64 **& mtrx)
{
  ASSERT ( mtrx[0] != NULL );
  ASSERT ( mtrx != NULL );
  delete mtrx[0];
  delete mtrx;
  mtrx = NULL;
}

#endif // __COMMONINLINES_HPP__
