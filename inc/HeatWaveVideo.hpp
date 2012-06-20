/****************************************************************************/
/**
 ** @file   HeatWaveVideo.hpp
 ** @brief  Contains the HeatWaveVideo class definition.
 ** @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 **
 **/

#ifndef __HEATWAVEVIDEO_HPP__
#define __HEATWAVEVIDEO_HPP__

#include "HeatWaveEnums.hpp"
#include "HeatWaveComponent.hpp"
#include "HeatWaveImage.hpp"
#include "HeatWaveLift.hpp"

/****************************************************************************/
/**
 ** A video class made up of n images. Provides temporal wavelet transform 
 ** capability.
 **
 **/

class HeatWaveVideo
{
public:
  
  /**
   *
   * Default constructor.
   *
   **/
  
  HeatWaveVideo();

  /**
   *
   * Normal parameterised constructor.
   *
   * @param wid The image widths.
   * @param hei The image heights.
   * @param img The number of images.
   * @param spc The color space.
   * @param hsp Horizontal Sampling Period of components. See the note.
   * @param vsp Vertical Sampling Period of components. See the note.
   *
   * @note Parameter hsp and vsp must point to triple lenght arrays if the
   * sampling periods of componenets is not all 1. Only possible with RGB or
   * YUV color space, else its ignored. If Null values is passes in all
   * components have a 1 to 1 sampling ratio to underlying image grid. For
   * example one sampling ratio that is quite common on embedded systems is
   * the YUV 411 which would have a HSP and a VSP of {1,2,2}. The PDA Database
   * was recorded with this YUV format, under FOURCC web site it is listed as
   * IYUV.  <a href="http://www.fourcc.org"> See FOURCC.org.</a>
   *
   **/
  
  HeatWaveVideo(SInt wid, SInt hei, SInt img, EnumSpace spc, 
                SInt * hsp = NULL, SInt * vsp = NULL);

  /**
   *
   * Special parameterised constructor.
   *
   * @param wid The image widths.
   * @param hei The image heights.
   * @param img The number of images.
   * @param spc The color space.
   * @param hsp Horizontal Sampling Period of components.
   * @param vsp Vertical Sampling Period of components. 
   * @param num The number of images on arrays.
   * @param arr The array of images.
   * @param des Destroy the memory on exit or assignment.
   * @param cpy Copy the memory on the pointers.
   * @note This extra contructor is useful for testing and other non-usual
   * circumstances where the component data is managed else where. The
   * external data must be one componenet following the next, where each
   * component is in a row by row fashion starting at top left corner sample
   * else the behaviour will be undefined.
   *
   **/
  
  HeatWaveVideo(SInt wid, SInt hei, EnumSpace spc, SInt * hsp, SInt * vsp,
                SInt num, HeatWaveImage ** arr, 
                Bool des = False, Bool cpy = False);

  /**
   *
   * Copy constuctor.
   *
   * @param oth The other HeatWaveVideo to copy.
   *
   **/

  HeatWaveVideo(const HeatWaveVideo & oth);
  
  /**
   *
   * Destructor.
   *
   **/
  
  ~HeatWaveVideo();
  
  /**
   *
   * @return The video width.
   *
   **/
  
  SInt GetWidth() const;
  
  /**
   *
   * Set the video width.
   *
   * @param width The width.
   *
   **/
  
  void SetWidth(SInt width);
  
  /**
   *
   * @return The video height.
   *
   **/
  
  SInt GetHeight() const;
  
  /**
   *
   * Set the video height.
   *
   * @param height The height.
   *
   **/
  
  void SetHeight(SInt height);
  
  /**
   *
   * @return The color space of the video.
   *
   **/
  
  EnumSpace GetSpace() const;
  
  /**
   *
   * See HeatWaveImage::SetSpace().
   *
   **/
  
  Bool SetSpace(EnumSpace spc, Bool fast = true, SInt grey_choice = -1, 
                Bool lsls = True);  
  
  /**
   *
   * @return The array of HeatWaveImage pointers.
   *
   **/
  
  HeatWaveImage ** GetImageA() const;

  /**
   *
   * Set the array of HeatWaveImage pointers.
   *
   * @param arr An array of HeatWaveImage pointers.
   *
   **/
  
  void SetImageA(HeatWaveImage ** arr);

  /**
   *
   * @return The number of images.
   *
   **/
  
  SInt GetImageN() const;
  
  /**
   *
   * Set the number of images.
   *
   * @param num The number of images.
   *
   **/
  
  void SetImageN(SInt num);

  /**
   *
   * @return The horizontal sampling periods array, length is
   * HEATWAVEVIDEO_MAX_COMP.
   *
   **/
  
  const SInt * GetHSPeriods() const;
  
  /**
   *
   * Set the horizontal sampling periods.
   *
   * @param hsp The horizontal sampling periods.
   * @note hsp Must be of lenght HEATWAVEVIDEO_MAX_COMP if not NULL.
   *
   **/
  
  void SetHSPeriods(SInt * hsp);

  /**
   *
   * @return The vertical sampling periods array, length is
   * HEATWAVEVIDEO_MAX_COMP.
   *
   **/
  
  const SInt * GetVSPeriods() const;
  
  /**
   *
   * Set the vertical sampling periods.
   *
   * @param vsp The vertical sampling periods.
   * @note vsp Must be of lenght HEATWAVEVIDEO_MAX_COMP if not NULL.
   *
   **/
  
  void SetVSPeriods(SInt * vsp);

  /**
   *
   * Set the flag to destroy the memory buffers or delete or assignment.
   *
   * @param des True to destroy on exit or assignment, false not.
   *
   **/
  
  void SetDesMem(Bool des);
  
  /**
   *
   * @return The destroy memory flag.
   *
   **/

  Bool GetDesMem() const;
  
  /** 
   *
   * Return a reference to a image.
   *
   * @param num The image number.
   * @return The reference.
   *
   **/
  
  HeatWaveImage & GetImage(SInt num) const;

  /**
   *
   * Return a reference to a component of a image.
   *
   * @param img The image number.
   * @param com The component number.
   * @return The reference.
   *
   **/

  HeatWaveComponent & GetComponent(SInt num, SInt com) const;

  /**
   *
   * Check to see if a image exists.
   *
   * @param num The image number.
   * @return True if it does, else false.
   *
   **/

  Bool CheckImage(SInt num) const;

  /**
   *
   * Check to see if a component exists.
   *
   * @param num The image number.
   * @param com The component number.
   * @return True if it does, else false.
   *
   **/

  Bool CheckComponent(SInt num, SInt com) const;

  /**
   *
   * Add an image to end of internal array.
   *
   * @param imge The image to point to.
   *
   **/

  void AddImage(HeatWaveImage * imge);

  /**
   *
   * Copy an image and place at end of internal array.
   *
   * @param imge The image to copy.
   *
   **/

  void CpyImage(const HeatWaveImage & imge);

  /**
   *
   * @return The current transform level.
   *
   **/
  
  SInt GetTransformLevel() const;
  
  /**
   *
   * Set the current transform level.
   *
   * @param lev The current transform level.
   *
   **/
  
  void SetTransformLevel(SInt lev);
  
  /**
   *
   * @return The (last) transform method used.
   *
   **/

  EnumTransform GetTransformType() const;
  
  /**
   *
   * Set the (last) transform method type used.
   *
   * @param type The transform method.
   *
   **/
  
  void SetTransformType(EnumTransform type);

  /**
   *
   * Set underlying precision and signedness of all images to the bare
   * minimum to accommodate the sample data. See
   * HeatWaveComponent::GetMinPrecSgn function.
   *
   **/
  
  void SetMinPrecSgn();
  
  /**
   *
   * Create a clone of this video.
   *
   **/
  
  HeatWaveVideo * GetClone() const;
  
  /**
   *
   * Get the comparibility of an video, returns true if both videos share
   * same amount of images, color space and images are of similar
   * size.
   *
   * @param other The other video.
   * @return True if comparible, False otherwise.
   *
   **/
  
  Bool IsComparable(const HeatWaveVideo & other); 

  /**
   *
   * Check spatial compatibilty of images. This means all internal images must
   * cover similar spatial areas plus be in same color spaces.
   *
   * @return True if spatially comparable, else false.
   *
   **/
  
  Bool IsSpatiallyComparable() const;

  /**
   *
   * Spatial pyramid type transform all sub-images.
   *
   * @param trn The transform type.
   * @param lev The level to transform to.
   * @param fwd Forward transform, else inverse transform. (Forward by default)
   * @param cur The current level, if less then 0 the internal level is used.
   * (-1 by default)
   * @return The new level of transform.
   *
   **/
  
  SInt DoSpatialTransform(EnumTransform trn, SInt lev, 
                          Bool fwd = True, SInt cur = -1);
  
  /**
   *
   * Temporal pyramid type transform across all components for sub-images.
   *
   * @param trn The transform type.
   * @param lev The level to transform to.
   * @param fwd Forward transform, else inverse transform. (Forward by default)
   * @param cur The current level, if less then 0 the internal level is used.
   * (-1 by default)
   * @return The new level of transform.
   *
   **/
  
  SInt DoTemporalTransform(EnumTransform trn, SInt lev, 
                           Bool fwd = True, SInt cur = -1);

  /**
   *
   * Temporal pyramid type transform a specific color component.
   *
   * @param clr The color component to transform.
   * @param trn The transform type.
   * @param lev The level to transform to.
   * @param fwd Forward transform, else inverse transform. (Forward by default)
   * @param cur The current level, if less then 0 the internal level is used.
   * (-1 by default)
   * @return The new level of transform, else -1 if images is not spatialy
   * comparable or -2 if color does not exist.
   *
   **/
  
  SInt DoComponentTransform(EnumColor clr, EnumTransform trn, SInt lev, 
                            Bool fwd = True, SInt cur = -1);

  /**
   *
   * Append another video at end of this video. Another video pointer
   * is clean up appropriatly and set to NULL on return.
   *
   * @param oth The other video to append.
   * 
   **/

  void DoAppendVideo(HeatWaveVideo *& oth);

  /**
   *
   * Perform a Histogram Equaliszation on all sub components.
   *
   **/
  
  void DoHE();
  
  /**
   *
   * Create a fused image from all sub-images. The LL area is always done
   * using the averag and the non-LL using the sample with the highest
   * absolute max. If underlying images is not comparable returns NULL.
   *
   * @return Fused image else NULL.
   *
   **/

  HeatWaveImage * GetFusedImage();
  
  /**
   *
   * Assignment operator.
   *
   * @param rhs The other video to copy data and state from.
   * @return A reference to this object.
   *
   **/
  
  HeatWaveVideo& operator=(const HeatWaveVideo& rhs);
  
  /**
   *
   * Equality comparison.
   *
   * @param rhs The other video to compare against.
   * @return A reference to this object.
   *
   **/
  
  Bool operator==(const HeatWaveVideo & rhs ) const;
  
protected:

  /**
   *
   * Perform a temporal wavelet-lift type transform on a range of images for
   * a certain componenet.
   *
   * @param cmp Component number.
   * @param fwd Forward tranform else inverse transform.
   * @param trn Type of transform.
   * @param strt Start image number. 
   * @param len The length of the transform.
   * @param prd Include prediction, True by default.
   * @param upd Include update, True by default.
   * @param rng Include updateing underlying data precision and signdess, True 
   * by default.
   * @return True if transformed, False otherwise.
   *
   **/

  Bool DoTemporalTransform(SInt cmp, Bool fwd, EnumTransform trn, SInt strt, 
                           SInt len, Bool prd = True, Bool upd = True, 
                           Bool rng = True);

  /**
   *
   * Get the "low band" (Wavelet Term) length for a certain level.
   *
   * @param levl The level to get the lenght for.
   * @return The length for the L of requested level.
   *
   **/

  SInt GetLowBandLength(SInt levl);

  /**
   *
   * Get a vector that crosses image boundarys.
   *
   * @param cmp The component number.
   * @param strt The start image.
   * @param len The lenght of the vector requested.
   * @param x The x-coordinate.
   * @param y The y-coordinate.
   * @param data A vector of lenght len that will recieve the data.
   * @return True if succesfull else false.
   *
   **/
   
  void GetTemporalVector(SInt cmp, SInt strt, SInt len, SInt x, SInt y,
                         Smpl * data);

  /**
   *
   * Set a vector that crosses image boundarys.
   *
   * @param cmp The component number.
   * @param strt The start image.
   * @param len The lenght of the vector requested.
   * @param x The x-coordinate.
   * @param y The y-coordinate.
   * @param data A vector of lenght len that will contain the data.
   * @return True if succesfull else false.
   *
   **/
   
  void SetTemporalVector(SInt cmp, SInt strt, SInt len, SInt x, SInt y,
                         Smpl * data);
  
  /**
   *
   * Validate the sampling periods.
   *
   * @return True if ok, False if not.
   *
   **/
  
  Bool ValidateSamplingPeriods() const;
  
  /**
   *
   * Create a number of images for video.
   *
   * @param num Number of images to create.
   * @note Make sure the m_imgn amd m_imga are 0 and NULL respectively.
   *
   **/
  
  void DoCreate(SInt num);   
  
  /**
   *
   * Copy the images.
   *
   * @param imgn The number of images to copy.
   * @param imga The images to copy. 
   *
   **/
  
  void DoImageCopy(SInt imgn, HeatWaveImage ** imga);
  
  /**
   *
   * Copy from another HeatWaveVideo. 
   *
   * @param rhs The other HeatWaveVideo to copy.
   *
   **/
  
  void DoCopy(const HeatWaveVideo & rhs);
  
  /**
   *
   * Destroy allocated memory.
   *
   **/
  
  void DoDestroy();
  
  /** Width. **/
  SInt m_width;
  
  /** Height. **/
  SInt m_height;
  
  /** Color space of video. **/
  EnumSpace m_space;
  
  /** The number of images. **/
  SInt m_imgn;
  
  /** The list of images. **/
  HeatWaveImage ** m_imga;
  
  /** Horizontal sampling ratio. **/
  SInt m_hsp[MAXCOLORSINANYSPACE];

  /** Vertical sampling ratio. **/
  SInt m_vsp[MAXCOLORSINANYSPACE];
  
  /** Destroy memory pointers on delete or assignment. **/
  Bool m_desMem;

  /** Scene length. **/
  SInt m_scene;

  /** Temporal transform level. **/
  SInt m_lev;

  /** Temporal transform type. **/
  EnumTransform m_trn;

  /** Transform engine. **/
  HeatWaveLift m_lift;
};

#endif
