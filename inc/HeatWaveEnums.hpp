/****************************************************************************/
/**
 ** @file HeatWaveEnums.hpp
 ** @brief Contains the global enumerations plus associated utility functions.
 ** @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 **
 ** $Date: 2007-09-06 18:16:41 $
 ** $Revision: 1.9 $
 **
 **/

#ifndef __HEATWAVEENUM_HPP__
#define __HEATWAVEENUM_HPP__

#include "CommonHeaders.hpp"
#include "HeatWaveTypes.hpp"

/** Maximum known colors in any color space. **/
#define MAXCOLORSINANYSPACE (3)

/** Growth granularity of internal vectors. (NB minimum of 1) **/
#define HEATWAVEVECTORGROWTH (100)

/** Maximum steps involved with any lifting procedure **/
#define HEATWAVELIFTMAXSTEPS (5)

/****************************************************************************/
/** 
 ** Supported (known) colors. (They say color you say colour I say get over 
 ** it!)
 **
 **/

enum EnumColor
  {
    /** Grey Color. */
    ClrGrey = 0,
    
    /*@{*/
    /** Red Color. */
    ClrRed = 1,
    ClrR = 1,
    /*@}*/
    
    /*@{*/
    /** Green Color. */
    ClrGreen = 2,
    ClrG = 2,
    /*@}*/
    
    /*@{*/
    /** Blue Color. */
    ClrBlue = 3,
    ClrB = 3,
    /*@}*/
    
    /*@{*/
    /** Luminance. */
    ClrLum = 4,
    ClrY = 4,
    /*@}*/
    
    /*@{*/
    /** Chrominance blue. */
    ClrChrmBlue = 5,
    ClrU = 5,
    /*@}*/
    
    /*@{*/
    /** Chrominance red. */
    ClrChrmRed = 6,
    ClrV = 6,
    /*@}*/

    /*@{*/
    /** Color is not set. */
    ClrUnknown = 7,
    /*@*/
    
    /** Total number of known colors. */
    ClrTotal = 8
  };

/**
 * 
 * Get the string name of a color.
 *
 * @param clr The color.
 * @param vrb Verbose info. (False by default)
 * @return The color name.
 *
 **/

inline const char *
ColorName (EnumColor clr, Bool vrb = False)
{
  switch (clr){
  case ClrGrey: return vrb?"the grey (luminance) channel":"grey";
  case ClrR: return vrb?"the red channel":"R";
  case ClrG: return vrb?"the green channel":"G";
  case ClrB: return vrb?"the blue channel":"B";
  case ClrY: return vrb?"the luminance (grey) channel":"Y";
  case ClrU: return vrb?"the blue chrominance channel":"U";
  case ClrV: return vrb?"the red chrominance channel":"V";
  default: return "ColorName() error!";
  }
}

/**
 *
 * Return the color enum number from text.
 *
 * @param str The string containing the color name.
 * @return The color number if found. (see EnumColor)
 *
 **/

inline EnumColor
ColorEnum(const char * str)
{
  for ( SInt i = 0 ; i < ClrTotal ; ++i ){
    if ( strcmp(str, ColorName((EnumColor)i)) == 0){
      return (EnumColor)i;
    }
  }
  return ClrUnknown;
}

/****************************************************************************/
/** 
 ** Supported (known) color spaces for images.
 **
 **/

enum EnumSpace
  {
    /** Single component with color grey. */
    SpcGrey = 0,
    
    /** Triple component with red, green and blue color channels. */
    SpcRGB,

    /** Triple componenet with one luminence (Y), and two chrominance 
     ** channels: blue (U/Cb), and red (V/Cr).*/
    SpcYUV,
    
    /** Total number of known color spaces. */
    SpcTotal,
    
    /** Unkown color space. */
    SpcUnknown
  };

/**
 * 
 * Get the string name of a color space.
 *
 * @param spc The color space.
 * @param vrb Verbose info. (False by default)
 * @return The color space name.
 *
 **/

inline const char *
SpaceName (EnumSpace spc, Bool vrb = False)
{
  switch (spc) {
  case SpcGrey: 
    return vrb?"single channel color space, grey or luminance channel":"grey";
  case SpcRGB:
    return vrb?"triple channel color space, consisting of (R) red channel,"
      " (G) green channel and (B) blue channel":"RGB";
  case SpcYUV:
    return vrb?"triple channel color space, consisting of (Y) luminance"
      " channel, (U) chrominance blue channel and (V) chrominance red "
      " channel":"YUV";
  default: return "SpaceName() error!";
  }
}

/**
 *
 * Return the color space enum number from text.
 *
 * @param str The string containing the color name.
 * @return The color space number if found. (see EnumSpace)
 *
 **/

inline EnumSpace
SpaceEnum(const char * str)
{
  for ( SInt i = 0 ; i < SpcTotal ; ++i ){
    if ( strcmp(str, SpaceName((EnumSpace)i)) == 0){
      return (EnumSpace)i;
    }
  }
  return SpcUnknown;
}

/****************************************************************************/
/** 
 ** Supported (known) color space transforms for images. (Added later hence
 ** a bit unconsistend in naming)
 **
 **/

enum EnumCT
  {
    /** Reversible color transform. */
    RCT = 0,
    
    /** Irreversible color transform. */
    ICT,

    /** Total number of known color transforms. */
    TotalCT,
    
    /** Unkown color space. */
    UnknownCT
  };

/**
 * 
 * Get the string name of a color transform.
 *
 * @param ct The color transform.
 * @param vrb Verbose info. (False by default)
 * @return The color transform name.
 *
 **/

inline const char *
CTName (EnumCT ct, Bool vrb = False)
{
  switch (ct) {
  case RCT: 
    return vrb?
      "A reversible color transform, as specified for JPEG2000. RGB to YUV"
      " means that the chrominance channels (U/Cb and V/Cr) will expand by"
      " 1 bit."
      :"RCT";
  case ICT:
    return vrb?
      "A irreversible color transform from RGB to YUV and vica versa."
      " No precision expansion"
      :"ICT";
  default: return "CTName() error!";
  }
}

/**
 *
 * Return the color transform enum number from text.
 *
 * @param str The string containing the color transform name.
 * @return The color transform number if found. (see CTEnum)
 *
 **/

inline EnumCT
CTEnum(const char * str)
{
  for ( SInt i = 0 ; i < TotalCT ; ++i ){
    if ( strcmp(str, CTName((EnumCT)i)) == 0){
      return (EnumCT)i;
    }
  }
  return UnknownCT;
}

/****************************************************************************/
/**
 ** The four subband regions of a one-step transform. If there was more than
 ** one step performed we'll find the same structure in the LL part again. The
 ** two rows are: LL, HL, LH and HH.
 **
 **/

enum EnumSubband
  {     
    /** Area low low */
    SubLL = 0,
    
    /** Area high low */
    SubHL,
    
    /** Area low high */
    SubLH,

    /** Area high high */
    SubHH,

    /** Total areas. */
    SubTotal,

    /** Unkown area. **/
    SubUnknown
  };

/**
 *
 * Get the string name of a subband.
 *
 * @param sub The subband.
 * @param vrb Verbose info. (False by default).
 * @return The subband name.
 *
 **/

inline const char *
SubbandName (EnumSubband sub, Bool vrb = False)
{
  switch (sub) {
  case SubLL: return vrb?"LL sub-band(top left)":"LL";
  case SubHL: return vrb?"HH channel (top right)":"HL";
  case SubLH: return vrb?"low high channel (bottom left)":"LH";
  case SubHH: return vrb?"high high channel (bottom right)":"HH";
  default: return "SubbandName() error!";
  }
}

/**
 *
 * Return the subband enum number from text.
 *
 * @param str The string containing the subband name.
 * @return The subband number if found. (see EnumArea)
 *
 **/

inline EnumSubband
SubbandEnum(const char * str)
{
  for ( SInt i = 0 ; i < SubTotal ; ++i ){
    if ( strcmp(str, SubbandName((EnumSubband)i)) == 0){
      return (EnumSubband)i;
    }
  }
  return SubUnknown;
}

/****************************************************************************/
/**
 ** Enumeration of different types of transforms for wavelet lifting.  See
 ** "'Wavelet transforms that map integers to integers' by A. R. Calderbank,
 ** Ingrid Daubechies, Wim Sweldens, and Boon-Lock Yeo", August 1996
 **
 **/

enum EnumTransform
  {
    /** (0,0) transform (Only splitting occurs, has its uses.) */
    Trn0_0 = 0,
    
    /** (1,1) transform */
    Trn1_1,

    /** (1,1) transform with PPP */
    Trn1_1m,
    
    /** (2,2) transform */
    Trn2_2,
    
    /** (2,4) transform */
    Trn2_4,

    /** (4,2) transform */
    Trn4_2,

    /** (4,4) transform */
    Trn4_4,
    
    /** (6,2) transform */
    Trn6_2,

    /** (6,6) transform */
    Trn6_6,

    /** (2+2,2) transform */
    Trn2p2_2,

    /** (D4) transform */
    TrnD4,

    /** (9-7) transform */
    Trn9m7,
    
    /** Total filters. */
    TrnTotal,

    /** Unknown filter. */
    TrnUnknown
  };

/**
 *
 * Get the string name of a transform.
 *
 * @param trn The transform.
 * @param vrb Verbose info. (False by defualt)
 * @return The transform name.
 *
 **/

inline const char *
TransformName (EnumTransform trn, Bool vrb = False)
{
  switch (trn){
  case Trn0_0:return vrb?"empty transform":"0_0";
  case Trn1_1:return vrb?"(1,1) interpolating":"1_1";
  case Trn1_1m:return vrb?"(1,1) interpolating with PPP":"1_1m";
  case Trn2_2:return vrb?"(2,2) interpolating":"2_2";
  case Trn2_4:return vrb?"(2,4) interpolating":"2_4";
  case Trn4_2:return vrb?"(4,2) interpolating":"4_2";
  case Trn4_4:return vrb?"(4,4) interpolating":"4_4";
  case Trn6_2:return vrb?"(6,2) interpolating":"6_2";
  case Trn6_6:return vrb?"(6,6) interpolating":"6_6";
  case Trn2p2_2:return vrb?"(2+2,2) interpolating":"2p2_2";
  case TrnD4:return vrb?"(D4) orthogonal transform":"D4";
  case Trn9m7:return vrb?"(9-7) symmetric biorthogonal transform":"9m7";
  default:return "TransformName() error!";
  }
}

/**
 *
 * Return the transform enum number from a string.
 *
 * @param str The string containing the transform name.
 * @return The transform number if found. (see EnumTransform)
 *
 **/

inline EnumTransform
TransformEnum(const char * str)
{
  for ( SInt i = 0; i < TrnTotal; ++i ){
    if ( strcmp(str,TransformName((EnumTransform)i)) == 0){
      return (EnumTransform)i;
    }
  }
  return TrnUnknown;
}

/****************************************************************************/
/**
 ** Enumeration of image (data) comparison methods.
 **
 **/

enum EnumComparison
  {    
    /** Peak signal to noise ratio. */
    CmpPSNR = 0,
    
    /** Mean squared error. */
    CmpMSE,
    
    /** Root mean squared error. */
    CmpRMSE,
    
    /** Peak absolute error. */
    CmpPAE,
    
    /** Mean absolute error. */
    CmpMAE,

    /** Equality. */
    CmpEqual,

    /** Total number of comparison tests. */
    CmpTotal,

    /** Unknown equality test. */
    CmpUnknown
  };

/**
 *
 * Get the string name of a comparison.
 *
 * @param cmp The comparison.
 * @param vrb Verbose info. (False by default)
 * @return The comparison name.
 *
 **/

inline const char *
ComparisonName (EnumComparison cmp, Bool vrb = False)
{
  switch (cmp){
  case CmpPSNR:return vrb?"peak signal to noise ratio":"PSNR";
  case CmpMSE:return vrb?"mean squared error":"MSE";
  case CmpRMSE:return vrb?"root mean squared error":"RMSE";
  case CmpPAE:return vrb?"peak absolute error":"PAE";
  case CmpMAE:return vrb?"mean absolute error":"MAE";
  case CmpEqual:return vrb?"equality":"equal";
  default: return "ComparisonName() error!";
  }
}

/**
 *
 * Return the comparision enum number from a string.
 *
 * @param str The string containing the filter name.
 * @return The comparison number if found. (see EnumComparison)
 *
 **/

inline EnumComparison
ComparisonEnum(const char * str)
{
  for ( SInt i = 0; i < CmpUnknown ; ++i ){
    if ( strcmp(str,ComparisonName((EnumComparison)i)) == 0){
      return (EnumComparison)i;
    }
  }
  return CmpUnknown;
}

/****************************************************************************/
/**
 ** Enumeration of handable video input and output formats. We use the AVI
 ** container for some unfortunate reasons. Any way these are types of "raw"
 ** storage types we understand and want to access in its stored format. I.E.
 ** we dont care to convert them first to some general format. We use them
 ** as they are!
 **
 **/

enum EnumVideoType
  {    
    /** IYUV type */
    VidIYUV = 0,
    
    /** GREY type */
    VidGREY,
    
    /** Total FCC */
    VidTotal,

    /** Unknown video type. */
    VidUnknown
  };

/**
 *
 * Get the string name of a video type.
 *
 * @param vid The video type.
 * @param vrb Verbose info. (False by default)
 * @return The video type name.
 *
 **/

inline const char *
VideoTypeName (EnumVideoType vid, Bool vrb = False)
{
  switch (vid){
  case VidIYUV:return vrb?"YUV 4:1:1":"IYUV";
  case VidGREY:return vrb?"Grey 1":"GREY";
  default: return "VideoTypeName() error!";
  }
}

/**
 *
 * Return the EnumVideoType enum number from a string.
 *
 * @param str The string containing the VideoType name.
 * @return The video type number if found, else VidUnknown.
 *
 **/

inline EnumVideoType
VideoTypeEnum(const char * str)
{
  for ( SInt i = 0; i < VidUnknown ; ++i ){
    if ( strcmp(str,VideoTypeName((EnumVideoType)i)) == 0){
      return (EnumVideoType)i;
    }
  }
  return VidUnknown;
}

/****************************************************************************/

#endif // __HEATWAVEENUM_HPP__
