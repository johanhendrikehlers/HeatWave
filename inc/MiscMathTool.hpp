/****************************************************************************/
/**
 **
 ** @file   MiscMathTool.hpp
 ** @brief  Contains the MiscMathTool class definition.
 ** @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 **
 **/

#ifndef __MISCMATHTOOL_HPP__
#define __MISCMATHTOOL_HPP__

#include "MiscIncludes.hpp"

/****************************************************************************/
/**
 **
 ** A static/inline based math class.
 ** 
 **/

class MiscMathTool
{
public:

  /**
   *
   * Return the greatest common deviser.
   *
   * @param a Sample one.
   * @param b Sample two.
   * @return The greatest common deviser.
   *
   **/
  
  static Smpl GCD ( Smpl a, Smpl b);
  
  /**
   *
   * Return 2 raised to a exponent.
   *
   * @param exponent The power to raise 2 by. Must be 0 or greater.
   * @return 2 raised to the given exponent.
   *
   **/

  static Smpl Pow2(Smpl exponent);

  /**
   *
   * Return the power of a base number to a given exponent.
   *
   * @param base The base number.
   * @param exponent The power to raise to. Must be 0 or greater.
   *
   **/
  
  static Smpl Pow(Smpl base,Smpl exponent);
  
  /**
   *
   * Return the minimum of two samples.
   *
   * @param a Sample one.
   * @param b Sample two.
   * @return The minimum of a or b.
   *
   **/
  
  static Smpl Min(Smpl a, Smpl b);
  
  /**
   *
   * Return the maximum of two samples.
   *
   * @param a Sample one.
   * @param b Sample two.
   * @return The maximum of a or b.
   *
   **/
  
  static Smpl Max(Smpl a, Smpl b);

  /**
   *
   * Swap two Sample's.
   *
   * @param a Sample one.
   * @param b Sample two.
   *
   **/
  
  static void Swap(Smpl & a, Smpl & b);

  /**
   *
   * Return a laplacian probability frequency using a x offset.
   *
   * @param sigma The sigma of the data range.
   * @param mean The mean of the data range.
   * @param value The data value (x axis).
   * @return The probability (y axis).
   *
   **/

  static SFloat64 PDF(SFloat64 sigma, SFloat64 mean, SFloat64 value);
  
};

#endif /* __MISCMATHTOOL_HPP__ */
