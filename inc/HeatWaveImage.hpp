/****************************************************************************/
/**
 ** @file HeatWaveImage.hpp
 ** @brief Contains the HeatWaveImage class definition.
 ** @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 **
 **/

#ifndef __HEATWAVEIMAGE_HPP__
#define __HEATWAVEIMAGE_HPP__

#include "HeatWaveEnums.hpp"
#include "HeatWaveComponent.hpp"

/****************************************************************************/
/**
 ** A image class made up of one or more components.
 **
 **/

class HeatWaveImage
{
public:
  
  /**
   *
   * Default constructor.
   *
   **/
  
  HeatWaveImage();

  /**
   *
   * Parameterized constructor.
   *
   * @param tlx Top left x-coordinate.
   * @param tly Top left y-coordinate.
   * @param wid The image width.
   * @param hei The image height.
   * @param spc The color space.
   * @param num The number of components.
   * @param dum Don't create any components, False by default.
   *
   **/
  
  HeatWaveImage(SInt tlx, SInt tly, SInt wid, SInt hei, EnumSpace spc, 
                SInt num, Bool dum= False);
  
  /**
   *
   * Extra-parameterized constructor.
   *
   * @param tlx Top left x-coordinate.
   * @param tly Top left y-coordinate.
   * @param wid The image width.
   * @param hei The image height.
   * @param spc The color scheme.
   * @param num The number of components.
   * @param arr The array of components.
   * @param des Destroy the memory on exit or assignment.
   * @param cpy Copy the memory on the pointers.
   *
   **/
  
  HeatWaveImage(SInt tlx, SInt tly, SInt wid, SInt hei, EnumSpace spc, 
              SInt num, HeatWaveComponent ** arr, Bool des = False, 
              Bool cpy = False);

  /**
   *
   * Copy constructor.
   *
   * @param oth The other HeatWaveImage to copy.
   *
   **/

  HeatWaveImage(const HeatWaveImage & oth);
  
  /**
   *
   * Destructor.
   *
   **/
  
  ~HeatWaveImage();
  
  /**
   *
   * @return Top left x-coordinate.
   *
   **/
  
  SInt GetTLX() const;
  
  /**
   *
   * Set the top left x-coordinate.
   *
   * @param tlx Top left x-coordinate.
   *
   **/
  
  void SetTLX( SInt tlx );
  
  /**
   *
   * @return The top left y-coordinate.
   *
   **/
  
  SInt GetTLY() const;
  
  /**
   *
   * Set the top left y-coordinate.
   *
   * @param tly Top left y-coordinate.
   *
   **/
  
  void SetTLY( SInt tly );
  
  /**
   *
   * @return The image width.
   *
   **/
  
  SInt GetWidth() const;
  
  /**
   *
   * Set the image width.
   *
   * @param width The width.
   *
   **/
  
  void SetWidth(SInt width);
  
  /**
   *
   * @return The image height.
   *
   **/
  
  SInt GetHeight() const;
  
  /**
   *
   * Set the image height.
   *
   * @param height The height.
   *
   **/
  
  void SetHeight(SInt height);

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
   * @return The color space of the image.
   *
   **/
  
  EnumSpace GetSpace() const;
  
  /**
   *
   * Set the color space of the image. By default no underlying data is
   * changed (setup mode), except for the color space variable.
   *
   * However it is possible to use this function to successfully convert
   * underlying data as well (change mode):
   * - YUV or RGB to Grey will always result in a data loss. 
   * - YUV or RGB to each other requires that all components are sampled at
   *   same ratio
   *
   * @param spc The color space for the image.
   * @param fast Fast mode. (True by default)
   * @param grey_choice If switching to grey space use a single component.
   * @param lsls Lossless color transform (Only valid for switching between
   * YUV and RGB).
   * @return True if operation is reversible, false otherwise.
   *
   **/
  
  Bool SetSpace(EnumSpace spc, Bool fast = true, SInt grey_choice = -1, 
                Bool lsls = True);  
  
  /**
   *
   * @return The array of HeatWaveComponent pointers.
   *
   **/
  
  HeatWaveComponent ** GetComponentA() const;

  /**
   *
   * Set the array of HeatWaveComponent pointers.
   *
   * @param arr An array of HeatWaveComponent pointers.
   *
   **/
  
  void SetComponentA(HeatWaveComponent ** arr);

  /**
   *
   * @return The number of HeatWaveComponents.
   *
   **/
  
  SInt GetComponentN() const;
  
  /**
   *
   * Set the number of HeatWaveComponents.
   *
   * @param num The number of HeatWaveComponents.
   *
   **/
  
  void SetComponentN(SInt num);

  /** 
   *
   * Return a reference to a component.
   *
   * @param num Component number.
   * @return The reference.
   *
   **/

  HeatWaveComponent & GetComponent(SInt num) const;

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
   * Set the flag to indicate that this image is the start of a new scene.
   * 
   * @param flg The boolean flag.
   * @note This flag relates to the owning video.
   * 
   **/
  
  void SetSceneSwitch(Bool flg);
  
  /**
   *
   * @return The flag to indicate if this is a scene switch.
   * @note This flag relates to the owning video.
   *
   **/
  
  Bool GetSceneSwitch() const;

  /**
   *
   * Set underlying precision and signedness of all components to the bare
   * minimum to accommodate the sample data. See
   * HeatWaveComponent::GetMinPrecSgn function.
   *
   **/
  
  void SetMinPrecSgn();
  
  /**
   *
   * Create a clone of this image.
   *
   **/

  HeatWaveImage * GetClone() const;

  /**
   *
   * Get the comparability of an image, returns true if both images share
   * same amount of components, color space and components are of similar
   * size.
   *
   * @param other The other image.
   * @return True if comparable, False otherwise.
   *
   **/
  
  Bool IsComparable(const HeatWaveImage & other); 

  /**
   *
   * Reversible Color Transform. Transform the Image from RGB to YUV and
   * reverse in a lossless fashion.
   * 
   * @param schm The color space to transform the image to.
   * @return True if image was transformed, False otherwise.
   * @note The space of the image must be either RGB or YUV, plus the
   * individual components must be of equal size.
   *
   **/

  Bool DoRCT(EnumSpace schm);

  /**
   *
   * Irreversible Color Transform. Transform the Image from RGB to YUV and
   * reverse in a lossy fashion.
   * 
   * @param schm The color space to transform the image to.
   * @return True if image was transformed, False otherwise.
   * @note The space of the image must be either RGB or YUV, plus the
   * individual components must be of equal size.
   *
   **/
  
  Bool DoICT(EnumSpace schm);

  /**
   *
   * Check that this image can carry out a colour transform. (YUV to RGB) 
   * first three component must have same size (no subsampling) and cover
   * same spatial area (top left x- and y-coordinates must be the same)
   *
   * @return True if it can, flase otherwise.
   *
   **/
  
  Bool CanCT() const;
  
  /**
   *
   * Pyramid type spectral transform across all sub-components.
   *
   * @param trn The transform type.
   * @param lev The level to transform to.
   * @param fwd Forward transform, else inverse transform. (Forward by default)
   * @param cur The current level, if less then 0 the internal level is used.
   * (-1 by default)
   * @return The new level of transform.
   *
   **/
  
  SInt DoSpectralTransform(EnumTransform trn, SInt lev, 
                           Bool fwd = True, SInt cur = -1);

  /**
   *
   * Pyramid type transform all sub-components.
   *
   * @param trn The transform type.
   * @param lev The level to transform to.
   * @param fwd Forward transform, else inverse transform. (Forward by default)
   * @param cur The current level, if less then 0 the internal level is used.
   * (-1 by default)
   * @return The new level of transform.
   *
   **/
  
  SInt DoPyramidTransform(EnumTransform trn, SInt lev,
                          Bool fwd = True, SInt cur = -1);
  
  /**
   *
   * Perform a (traditional) wavelet type transform, between (inter)
   * components.
   *
   * @param fwd Forward transform else inverse transform.
   * @param trn Type of transform.
   * @param tlx Top left x-coordinate.
   * @param tly Top left y-coordinate.
   * @param wid The width of the area.
   * @param hei The height of the area.
   * @param prd Include prediction, True by default.
   * @param upd Include update, True by default.
   * @param vrt Include vertical transform, True by default.
   * @param hrz Include horizontal transform, True by default.
   * @param rng Include updating underlying data precision and signedness, True
   * by default.
   * @return True if transformed, False otherwise.
   *
   **/

  Bool DoInterCompTrans(Bool fwd, EnumTransform trn, SInt tlx, SInt tly,
                        SInt wid, SInt hei, Bool prd = True, Bool upd = True,
                        Bool vrt = True, Bool hrz = True, Bool rng = True);

  /**
   *
   * Perform a pixelisation of the image. Useful for viewing the data. This
   * function will use a different treatment for LL area as its data is
   * normally unsigned (i.e. half the precision as rest of image) after
   * transforms.
   *
   * @param prec Precision to pixelise to, 8 by default.
   * @param sgnd Underlying data should be signed, False by default.
   * @note This operation might result in the loss of information.
   *
   **/

  void DoPixelise(SInt prec = 8, Bool sgnd = False);

  /**
   *
   * Cap data.
   *
   * @param minv Values below this will be raised to this.
   * @param maxv Values above this will be lowered to this.
   * @param set Set object precision and signedness to new range. 
   * (True by defualt)
   *
   **/

  void DoCapData(SInt minv, SInt maxv, Bool set = True);

  /**
   *
   * Cap data according to precision.
   *
   **/
  
  void DoCapDataToPrec();
  
  /**
   *
   * Clear all underlying components setting the data to 0.
   *
   * @param rplc The default value for all data. Default is 0.
   *
   **/

  void DoClear(SInt rplc = 0);

  /**
   *
   * Perform a Histogram Equalization on all sub components.
   *
   **/

  void DoHE();

  /**
   *
   * Returns true if the underlying components have similar sizes, sampling
   * ratios and space offset (top left x- and y-coordinate).
   *
   * @return True if component are spatialy equivelant.
   *
   **/

  Bool AllComponentsRelate() const;

  /**
   *
   * Assignment operator.
   *
   * @param rhs The other image to copy data and state from.
   * @return A reference to this object.
   *
   **/
  
  HeatWaveImage& operator=(const HeatWaveImage& rhs);
  
  /**
   *
   * Equality comparison.
   *
   * @param rhs The other image to compare against.
   * @return A reference to this object.
   *
   **/

  Bool operator==(const HeatWaveImage & rhs ) const;

protected:

  /**
   *
   * Perform a spectral wavelet transform.
   *
   * @param fwd Forward transform else inverse transform.
   * @param trn Type of transform.
   * @param strt Start position of transform, i.e. the component number. 
   * @param len The length of the transform, i.e. the number of components.
   * @param prd Include prediction, True by default.
   * @param upd Include update, True by default.
   * @param rng Include updating underlying data precision and signedness, True 
   * by default.
   * @return True if transformed, False otherwise.
   *
   **/

  Bool DoSpectralTransform(Bool fwd, EnumTransform trn, 
                           SInt strt, SInt len, Bool prd = True, 
                           Bool upd = True, Bool rng = True);

  /**
   *
   * Get the "low band" (Wavelet Term) length for a certain level.
   *
   * @param levl The level to get the length for.
   * @return The length for the L of requested level.
   *
   **/

  SInt GetLowBandLength(SInt levl);

  /**
   *
   * Get a spectral vector, i.e. a cross component vector.
   *
   * @param strt Component to start vector.
   * @param len The length of the vector, or number of components to include.
   * @param x The x-coordinate.
   * @param y The y-coordinate.
   * @param data A vector of length 'len' that will receive a copy of the data.
   * @note No boundary checking, do it your self.
   *
   **/
   
  void GetSpectralVector(SInt strt, SInt len, SInt x, SInt y,
                         Smpl * data);

  /**
   *
   * Set a spectral vector, i.e. a cross component vector.
   *
   * @param strt Component to start vector.
   * @param len The length of the vector, or number of components to include.
   * @param x The x-coordinate.
   * @param y The y-coordinate.
   * @param data A vector of length 'len' that will be copied back to components.
   * @note No boundary checking, do it your self.
   *
   **/
   
  void SetSpectralVector(SInt strt, SInt len, SInt x, SInt y,
                         Smpl * data);
  
  /**
   *
   * Select a box or area that spans the maximum area overlapped by the
   * individual components.
   *
   * @param tlx [OUT] Top left x-coordinate.
   * @param tly [OUT] Top left y-coordinate.
   * @param wdh [OUT] The width.
   * @param hgt [OUT] The height.
   * @return True if there is such an area, else false.
   *
   **/

  Bool SelectMaximumBox(SInt & tlx,SInt & tly,SInt & width, SInt & height);
  
  /**
   *
   * Validate the sanity of this object, particularly that no Null pointers
   * exist.
   *
   * @return True if sane, False if insane.
   *
   **/
  
  Bool ValidateSanity() const;

  /**
   *
   * Reversible color transform. RGB to YUV.
   *
   **/

  void DoRCT_RGBtoYUV();

  /**
   *
   * Reversible color transform. YUV to RGB.
   *
   **/

  void DoRCT_YUVtoRGB();

  /**
   *
   * Irreversible color transform. RGB to YUV.
   *
   **/

  void DoICT_RGBtoYUV();

  /**
   *
   * Irreversible color transform. YUV to RGB.
   *
   **/
  
  void DoICT_YUVtoRGB();
  
  /**
   *
   * Order the components in correct order. Red followed by green followed by
   * blue for RGB and luminance (Y) followed by chrominance blue (U) followed
   * by chrominance red (V) in YUV.
   *
   **/

  void DoCrctClrOrd();

  /**
   *
   * Create a number of components for image.
   *
   * @param num Number of components to create.
   * @param set Set sample values to param def, by default True. 
   * @param def The default value to set samples to, by default 0.
   * @note Other values can be set after wards if need be.
   *
   **/

  void DoCreate(SInt num, Bool set, Smpl def = 0);   

  /**
   *
   * Copy the components.
   *
   * @param compn The number of components to copy.
   * @param compa The components to copy. 
   *
   **/

  void DoCompCopy(SInt compn, HeatWaveComponent ** compa);

  /**
   *
   * Copy from another HeatWaveImage. 
   *
   * @param rhs The other HeatWaveImage to copy.
   *
   **/

  void DoCopy(const HeatWaveImage & rhs);
  
  /**
   *
   * Destroy allocated memory.
   *
   **/

  void DoDestroy();
  
  /** Top left x-coordinate. **/
  SInt m_tlx;
  
  /** Top left y-coordinate. **/
  SInt m_tly;

  /** Width. **/
  SInt m_width;
  
  /** Height. **/
  SInt m_height;

  /** Last transform performed on this image. (Spectral) */
  EnumTransform m_trn;
  
  /** Level of transform. (Spectral) */
  SInt m_lev;
  
  /** Destroy memory pointer on delete or assignment. **/
  Bool m_desMem;

  /** (Internal variable) The size of image. **/
  SInt m_size;
  
  /** Color space of image. **/
  EnumSpace m_space;
  
  /** The number of components. **/
  SInt m_compn;
  
  /** The list of components. **/
  HeatWaveComponent ** m_compa;

  /** The scene switch flag. **/
  Bool m_sceneSwitch;
  
  /** Transform engine. **/
  HeatWaveLift m_lift;
};

#endif
