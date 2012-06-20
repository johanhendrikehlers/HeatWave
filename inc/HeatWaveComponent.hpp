/****************************************************************************/
/**
 ** @file   HeatWaveComponent.hpp
 ** @brief  Contains the HeatWaveComponent class definition.
 ** @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 **
 **/

#ifndef __HEATWAVECOMPONENT_HPP__
#define __HEATWAVECOMPONENT_HPP__

#include "HeatWaveEnums.hpp"
#include "HeatWaveLift.hpp"
#include "IIICommon.h"

/** Fixed lenght for sample data's in iii file. **/
#define HEATWAVEIIISAMPLELENGTH 10

/****************************************************************************/
/**
 ** The Component class. A variable precision signed or unsigned sample
 ** (integer based) data matrix.  These objects are the building blocks of
 ** images (frames). Some variables, for example the top left x and y
 ** coordinate and sampling periods relates to the owning image grid system.
 ** 
 **/

class HeatWaveComponent
{
public:

  /**
   *
   * Default constructor.
   *
   **/
  
  HeatWaveComponent();
  
  /**
   *
   * Normal parameterised constructor.
   *
   * @param tlx Top left x-coordinate.
   * @param tly Top left y-coordinate.
   * @param hst Horizontal sampling period, 1 or more.
   * @param vst Vertical sampling period, 1 or more.
   * @param wid Width.
   * @param hei Height.
   * @param sgn Data is signed.
   * @param prc Precision of data.
   * @param clr Color.
   *
   **/
  
  HeatWaveComponent(SInt tlx, SInt tly, SInt hst, SInt vst, SInt wid, 
                    SInt hei, Bool sgn, SInt prc, EnumColor clr);

  /**
   *
   * Special parameterised constructor.
   *
   * @param tlx Top left x-coordinate.
   * @param tly Top left y-coordinate.
   * @param hst Horizontal sampling period, 1 or more.
   * @param vst Vertical sampling period, 1 or more.
   * @param wid Width.
   * @param hei Height.
   * @param sgn Data is signed.
   * @param prc Precision of data.
   * @param clr Color.
   * @param dat The buffer holding the data. See note. 
   * @param row A buffer of Sample pointers large enough equal to height.
   * @param desMem Destroy any memory on exit or assignment.
   * @param cpyMem Copy the memory on the pointers.
   * @note This extra contructor is useful for testing and other non-usual
   * circumstances where the component data is managed else where. The
   * external data must be stored in a row by row fashion starting at top left
   * corner sample else the behaviour will be undefined.
   *
   **/
  
  HeatWaveComponent(SInt tlx, SInt tly, SInt hst, SInt vst, 
                    SInt wid, SInt hei, Bool sgn, SInt prc, EnumColor clr, 
                    Smpl * dat, Smpl ** row,
                    Bool desMem = False, Bool cpyMem = False);

  /**
   *
   * Copy constructor.
   *
   * @param oth The other HeatWaveComponent to copy.
   *
   **/

  HeatWaveComponent( const HeatWaveComponent & oth );
  
  /**
   *
   * Destructor.
   *
   **/
  
  ~HeatWaveComponent();

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
   * @return Return the horizontal sampling period.
   *
   **/

  SInt GetHStep() const;

  /**
   *
   * Set the horizontal sampling period.
   *
   * @param hstep Horizontal sampling period.
   * @param rsze Resize the component if different sampling period. (false by
   *        default)
   * @param trns The transform to use for interpolating when sampling
   *        is being reduced. (linear by default)
   *
   *
   **/

  void SetHStep(SInt hstep, Bool rsze = false, EnumTransform trns = Trn2_2);

  /**
   *
   * @return Return the vertical sampling period.
   *
   **/

  SInt GetVStep() const;

  /**
   *
   * Set the vertical sampling period.
   *
   * @param vstep Vertical sampling period.
   * @param rsze Resize the component if different sampling period. (false by
   *        default)
   * @param trns The transform to use for interpolating when sampling
   *        is being reduced. (linear by default)
   *
   **/

  void SetVStep(SInt vstep, Bool rsze = false, EnumTransform trns = Trn2_2);

  /**
   *
   * @return The component width.
   *
   **/

  SInt GetWidth() const;
  
  /**
   *
   * Set the component width. All possible data is saved.
   *
   * @param width The width.
   * @param def The default value for new data samples.
   * @param desMem Delete the new memory on exit or assignment, True by
   * default.
   *
   **/
  
  void SetWidth(SInt width, Smpl def = 0, Bool desMem = True);

  /**
   *
   * @return The component height.
   *
   **/

  SInt GetHeight() const;

  /**
   *
   * Set the component height. All possible data is saved.
   *
   * @param height The height.
   * @param def The default value for new data samples.
   * @param desMem Delete the new memory on exit or assignment, True by
   * default.
   *
   **/
  
  void SetHeight(SInt height, Smpl def = 0, Bool desMem = True);

  /**
   *
   * @return The component size.
   *
   **/

  SInt GetSize() const;

  /**
   *
   * Set the component size. All possible data is saved.
   *
   * @param width The width.
   * @param height The height.
   * @param def The default value for new data samples.
   * @param desMem Delete the new memory on exit or assignment, True by
   * default.
   *
   **/
  
  void SetSize(SInt width, SInt height, Smpl def = 0, Bool desMem = True);

  /**
   *
   * @return The sample data signed state.
   *
   **/

  Bool GetSgnd() const;

  /**
   *
   * Set the sample data signed state. In non fast mode the actual underlying
   * data is also moved.
   *
   * @param sign True for signed, False for unsigned.
   * @param fast Only change internal state in fast mode, else change
   * underlying data to. Fast by default.
   *
   **/
  
  void SetSgnd(Bool sign, Bool fast = True);

  /**
   *
   * @return The precision of the underlying sample data.
   *
   **/
  
  SInt GetPrec();

  /**
   *
   * Set the precision of the underlying Sample data.
   *
   * @param prec The precision of the underlying sample data.
   *
   **/
  
  void SetPrec(SInt prec);

  /**
   *
   * Recommend a minimum precision and signedness which would accommodate the
   * underlying component data.
   *
   * @param prec (OUT) The minimum precision.
   * @param sgnd (OUT) If data should be signed.
   * @param min_sgnd If data should be considered signed anyway, False by 
   * default.
   * @param min_prec The minimum precision for data even if less, 8 by 
   * default.
   *
   **/
  
  void GetMinPrecSgn(SInt & prec, Bool & sgnd , Bool min_sgnd = False,
                     SInt min_prec = 8) const;

  /**
   *
   * Recommend a minimum precision and signedness which would accommodate the
   * underlying component data.
   *
   * @param tlx Top left x-coordinate.
   * @param tly Top left y-coordinate.
   * @param width The width of area.
   * @param height The height of area.
   * @param prec (OUT) The minimum precision.
   * @param sgnd (OUT) If data should be signed.
   * @param min_sgnd If data should be considered signed anyway, False by 
   * default.
   * @param min_prec The minimum precision for data even if less, 8 by 
   * default.
   *
   **/
  
  Bool GetMinPrecSgn(SInt tlx, SInt tly, SInt width, SInt height,
                     SInt & prec, Bool & sgnd , Bool min_sgnd = False,
                     SInt min_prec = 8) const;
  
  /**
   *
   * Set the underlying precision and signedness to the bare minimum to
   * accommodate the sample data. See GetMinPrecSgn function.
   *
   **/
  
  void SetMinPrecSgn();

  /**
   *
   * Force the underlying precision and signedness of whole component.
   *
   * @param prec The new precision of underlying sample data, 8 by default.
   * @param sgnd Force samples to be signed, False by default.
   * @note This operation might result in the loss of information.
   *
   **/
  
  void SetNewPrecSgn(SInt prec = 8, Bool sgnd = False);
  
  /**
   *
   * Force the underlying precision and signedness of whole component.
   *
   * @param tlx Top left x-coordinate.
   * @param tly Top left y-coordinate.
   * @param width The width of area.
   * @param height The height of area.
   * @param prec The new precision of underlying sample data, 8 by default.
   * @param sgnd Force samples to be signed, False by default.
   * @param fxd Dont determine current data precision and signedness but use
   * the fixed values indicated in fixed_prec and fixed_sgnd. (False by def)
   * @param fxd_prec Fixed precision to convert from. (fxd must be True)
   * @param fxd_sgnd Fixed signed state. (fxd must be True)
   * @note This operation might result in the loss of information.
   *
   **/
  
  Bool SetNewPrecSgn(SInt tlx, SInt tly, SInt width, SInt height,
                     SInt prec = 8, Bool sgnd = False, Bool fxd = False,
                     SInt fxd_prec = 0, Bool fxd_sgnd = False);

  /**
   *
   * Perform a pixelisation of the image. Usefull for viewing the data. This
   * function will use a different treatment for LL area as its data is
   * normaly unsigned (i.e. half the precision as rest of image) after
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
   * @param min Valuas below this will be raised to this.
   * @param max Values above this will be lowered to this.
   * @param set Set object precision and signedness to new range. 
   * (True by defualt)
   *
   **/
  
  void DoCapData(SInt min, SInt max, Bool set = true);

  /**
   *
   * Cap data according to precision.
   *
   **/
  
  void DoCapDataToPrec();

  /**
   *
   * Clear the component reseting all values to 0.
   *
   * @param rplc The new replacement value, default is 0.
   *
   **/

  void DoClear(SInt rplc = 0);
  
  /**
   *
   * Get some basic information for whole of component.
   *
   * @param min (OUT) The minimum sample value.
   * @param max (OUT) The maximum sample value.
   * @param total (OUT) The sum of all the sample values.
   * @return The size of the underlying sample vector. 
   *
   **/

  SInt GetBasicStats(Smpl & min, Smpl & max, SInt & total) const;
  
  /**
   *
   * Get some basic information for a part of component.
   *
   * @param tlx Top left x-coordinate.
   * @param tly Top left y-coordinate.
   * @param width The width of area.
   * @param height The height of area.
   * @param min (OUT) The minimum sample value.
   * @param max (OUT) The maximum sample value.
   * @param total (OUT) The sum of all the sample values.
   * @return The size of the underlying sample vector. (-1 out of bounds)
   *
   **/

  SInt GetBasicStats(SInt tlx, SInt tly, SInt width, SInt height,
                     Smpl & min, Smpl & max, SInt & total) const;

  /**
   *
   * Get extended statistics for a specific area.
   *
   * @param tlx Top left x-coordinate.
   * @param tly Top left y-coordinate.
   * @param width The width of area.
   * @param height The height of area.
   * @param min (OUT) The minimum sample value.
   * @param max (OUT) The maximum sample value.
   * @param total (OUT) The sum of all the sample values.
   * @param mean (OUT) The average of all values.
   * @param std_dev (OUT) The standard deviation.
   * @param std_err (OUT) The standard error.
   * @param rms (OUT) The root mean square.
   * @return True if area exists, False if not.
   *
   **/
  
  Bool GetStats(SInt tlx, SInt tly, SInt width, SInt height,
                Smpl & min, Smpl & max, SInt & total, SFloat64 & mean,
                SFloat64 & std_dev, SFloat64 & std_err, SFloat64 & rms) const;

  /**
   *
   * Get extended statistics for whole componenet.
   *
   * @param min (OUT) The minimum sample value.
   * @param max (OUT) The maximum sample value.
   * @param total (OUT) The sum of all the sample values.
   * @param mean (OUT) The average of all values.
   * @param std_dev (OUT) The standard deviation.
   * @param std_err (OUT) The standard error.
   * @param rms (OUT) The root mean square.
   *
   **/
  
  void GetStats(Smpl & min, Smpl & max, SInt & total, SFloat64 & mean,
                SFloat64 & std_dev, SFloat64 & std_err, SFloat64 & rms) const;
  
  /**
   *
   * @return The color type of the component.
   *
   **/

  EnumColor GetColor() const;

  /**
   *
   * Set the color type of the component.
   *
   * @param clr The color type for the component.
   *
   **/

  void SetColor(EnumColor clr);  

  /**
   *
   * @return Pointer to underlying sample data.
   *
   **/
  
  Smpl * GetData() const;

  /**
   *
   * Set the sample data pointer.
   *
   * @param data The sample data pointer.
   *
   **/
  
  void SetData(Smpl * data);
  
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
   * @return The array of smpl pointers used for pointing to each row.
   *
   **/

  Smpl ** GetRows() const;

  /**
   *
   * Set the array of sample pointers used for pointing to each row.
   *
   * @param rows An array of sample pointers.
   *
   **/

  void SetRows(Smpl ** rows);
  
  /**
   *
   * Get a Smpl data value.
   * 
   * @param x The x-coordinate, which should be more than or equal to top left
   * x-coordinate and less than top left x-coordinate plus width.
   * @param y The y-coordinate, which should be more than or equal to top left
   * y-coordinate and less than top left y-coordinate plus height.
   * @return The Smpl value.
   *
   **/
  
  Smpl GetSmpl(const SInt x, const SInt y) const;
  
  /**
   *
   * Set a Smpl data value.
   * 
   * @param x The x-coordinate, which should be more than or equal to top left
   * x-coordinate and less than top left x-coordinate plus width.
   * @param y The y-coordinate, which should be more than or equal to top left
   * y-coordinate and less than top left y-coordinate plus height.
   * @param val The Smpl value.
   *
   **/

  void SetSmpl(const SInt x, const SInt y, const Smpl val);

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
   * Set the HeatWaveLift member.
   *
   * @param lft The HeatWaveLift to copy.
   *
   **/

  void SetLift(const HeatWaveLift & lft);
  
  /** 
   *
   * @return A copy of the HeatWaveLift member.
   *
   **/

  HeatWaveLift GetLift() const;

  /**
   *
   * Perform a (traditional) wavelet-lift type transform on a certain area of
   * the component.
   *
   * @param fwd Forward tranform else inverse transform.
   * @param trn Type of transform.
   * @param tlx Top left x-coordinate.
   * @param tly Top left y-coordinate.
   * @param wid The width of the area.
   * @param hei The height of the area.
   * @param prd Include prediction, True by default.
   * @param upd Include update, True by default.
   * @param vrt Include vertical transform, True by default.
   * @param hrz Include horizontal transform, True by default.
   * @param rng Include updateing underlying data precision and signdess, True 
   * by default.
   * @return True if transformed, False otherwise.
   *
   **/

  Bool DoTransform(Bool fwd, EnumTransform trn, SInt tlx, SInt tly,
                   SInt wid, SInt hei, Bool prd = True, Bool upd = True,
                   Bool vrt = True, Bool hrz = True, Bool rng = True);
  
  /**
   *
   * Pyramid type transform.
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
   * Perform a (non-traditional) shift-invariant type wavelet-lift type
   * transform on a certain area of the component. This type of transform is
   * being research but its nothing like the traditional type so please see my
   * notes on shift-invariant wavelet for image compression.
   *
   * @param fwd Forward or inverse transform.   
   * @param act Type of action to carry out. See EnumTransform.
   * @param tlx Top left x-coordinate.
   * @param tly Top left y-coordinate.
   * @param width The width of the area to transform.
   * @param height The height of the area to transform.
   * @param vert Perform vertical transform.
   * @param horz Perform horizontal transform.
   * @param range Update the component data range after.
   * @return True if transformed OK, False otherwise.
   *
   **/
  
  Bool DoShiftTransform(Bool fwd, EnumTransform act, SInt tlx, SInt tly,
                        SInt width, SInt height, Bool vert = True, 
                        Bool horz = True, Bool range = True);

  /**
   *
   * Perform a Histogram equalization on a specific area.
   *
   * @param tlx Top left x-coordinate.
   * @param tly Top left y-coordinate.
   * @param width The width of the area.
   * @param height The height of the area.
   * @return True if succesfull, false if not. If false area is out of bounds.
   *
   **/

  Bool DoHE(SInt tlx, SInt tly, SInt width, SInt height);

  /**
   *
   * Perform a Histogram Equaliszation on whole component.
   *
   **/

  void DoHE();
    
  /**
   *
   * Resize component with the option of maintaining as much data as possible, 
   * plus what the new data's default values should be.
   *
   * @param width The new width of component.
   * @param height The new height of component.
   * @param save Save as much data as possible, else nothing.
   * @param def Default value for new data samples.
   * @param desMem Delete the new memory on exit or assignment, True by
   * default.
   *
   **/

  void DoResize(SInt width, SInt height, Bool save, Smpl def, Bool desMem);
  
  /**
   *
   * Get the top-left xy-coordinate, width and height for a sub-band of a
   * particular resolution level.
   *
   * @param res The resolution level.
   * @param sub The sub-band for the resolution level.
   * @param x (out) The top left x-coordinate.
   * @param y (out) The top left y-coordinate.
   * @param width (out) The width.
   * @param height (out) The height.
   * @return True if area is valid or reachable, false otherwise.
   *
   **/

  Bool GetSubbandInfo ( SInt res, EnumSubband sub, SInt & x, SInt & y,
                        SInt & width, SInt & height ) const;

  /**
   *
   * Get a clone of this component.
   *
   **/

  HeatWaveComponent * GetClone();

  /**
   *
   * Compare this component with another using a specified method.
   *
   * @param other The other component to compare with.
   * @param cmp The comparison method.
   * @return The result.
   *
   **/

  SFloat64 GetComparison(const HeatWaveComponent & other, EnumComparison cmp);

  /**
   *
   * Return the copy of a vector of sample data for a certain area.
   *
   * @param tlx Top left x-coordinate.
   * @param tly Top left y-coordinate.
   * @param width The width of the area.
   * @param height The height of the area.
   * @return The vector if possible, else NULL.
   *
   **/

  Smpl * GetVector(SInt tlx, SInt tly, SInt width, SInt height) const;

  /**
   *
   * Return the copy of a vector of a certain resolutions sub-band.
   *
   * @param res Resolution level.
   * @param area The sub-band area.
   * @return The vector if possible, else NULL.
   *
   **/

  Smpl * GetVector(SInt res, EnumSubband area) const;

  /**
   *
   * Return the histogram of a certain specified area.
   *
   * @param tlx Top left x-coordinate.
   * @param tly Top left y-coordinate.
   * @param width The width of the area.
   * @param height The height of the area.
   * @param hist (OUT) The pointer to the histogram.
   * @param range The range of underlying data.
   * @param shift The smpl range shift. For unsigned data this will be 0,
   * else half the range.
   * @note The range and offset parameters can be used in for loops to safely
   * enumerate the Histogram. For example a grey image with 255 colors will 
   * have a range of 256 and the offset will be 0. If it was signed then the
   * range would have remained 256 but the offset will be 128. So value at
   *
   **/
  
  Bool GetHistogram(SInt tlx, SInt tly, SInt width, SInt height, SmplPtr &hist,
                    SInt &range, SInt &shift) const;

  /**
   *
   * Return the histogram of whole component.
   *
   * @param hist (OUT) The pointer to the histogram.
   * @param range (OUT) The range of underlying data.
   * @param shift (OUT) The smpl range shift.
   * @note The range and offset parameters can be used in for loops to safely
   * enumerate the Histogram. For example a grey image with 255 colors will 
   * have a range of 256 and the offset will be 0. If it was signed then the
   * range would have remained 256 but the offset will be -128. So value at
   *
   **/
  
  Bool GetHistogram(SmplPtr &hist, SInt &range, SInt &shift) const;

  /**
   *
   * Return the entropy of a certain specified area.
   *
   * @param tlx Top left x-coordinate.
   * @param tly Top left y-coordinate.
   * @param width The width of the area.
   * @param height The height of the area.
   * @return Return the entropy if possible else 0.
   * @note For more info see http://cnx.org/content/m0070/latest/.
   *
   **/
  
  SFloat64 GetEntropy(SInt tlx, SInt tly, SInt width, SInt height) const;

  /**
   *
   * Return the entropy of whole component.
   *
   * @note For more info see http://cnx.org/content/m0070/latest/.
   *
   **/
  
  SFloat64 GetEntropy() const;

  /**
   *
   * Write a so called intermediate integer image (iii). Very similar to a pgm
   * file but we use fixed lengths spaces for samples as this makes reading
   * and writing a specific sample alot faster.
   *
   * @param name The name of the file to write.
   * @param file If name is empty write to this file object.
   * @return NULL if error if file can't be opened, else pointer to file.
   * @note To close the file if not used any more.
   *
   **/

  FILE * WriteIII(const Char * name, FILE * file = NULL) const;

  /**
   *
   * Read a so called intermediate integer image (iii). Very similar to a pgm
   * file but we use fixed lengths spaces for samples as this makes reading
   * and writing a specific sample alot faster. All existing data will be
   * erased!
   *
   * @param name The name of the file to read from.
   * @param file If name is empty read from this file object.
   * @return NULL if error if file can't be opened, else pointer to file.
   * @note To close the file if not used any more.
   *
   **/

  FILE * ReadIII(const Char * name, FILE * file = NULL);

  /**
   *
   * Validate the coordinate is on given component.
   *
   * @param x The x-coordinate value.
   * @param y The y-coordinate value.
   * @return True if coordinate is valid, False otherwise.
   *
   **/

  Bool ValidateCoords(SInt x, SInt y) const;

  /**
   *
   * Validate the Sample data.
   *
   * @param dat The Smpl|Smpl data to validate.
   * @return True if Smpl|Smpl is valid, False otherwise.
   * 
   **/

  Bool ValidateSample(SInt dat) const;
  
  /**
   *
   * Assignment operator.
   *
   * @param rhs The other component to copy data and state from.
   * @return A reference to this object.
   *
   **/

  HeatWaveComponent & operator=(const HeatWaveComponent & rhs);
  
  /**
   *
   * Equality comparison.
   *
   * @param rhs The other component to compare against.
   * @return True if equal, False if un-equal.
   *
   **/

  Bool operator==(const HeatWaveComponent & rhs ) const;
  
  /**
   *
   * Perform a Bitwise OR and assignment on each of the underlying samples.
   *
   * @param mask The Bit mask to OR each sample with.
   * @return The mask.
   *
   **/

  Smpl operator|=(Smpl mask);

  /**
   *
   * Perform a Bitwise EXCLUSIVE OR and assignment on each of the underlying
   * samples.
   *
   * @param mask The Bit mask to EXCLUSIVE OR each sample with.
   * @return The mask.
   *
   **/

  Smpl operator^=(Smpl mask);

  /**
   *
   * Perform a Bitwise AND and assignment on each of the underlying samples.
   *
   * @param mask The Bit mask to AND each sample with.
   * @return The mask.
   *
   **/
  
  Smpl operator&=(Smpl mask);

  /**
   *
   * Add a value to each underlying sample value.
   *
   * @param val The value to add.
   * @return The value val.
   *
   **/

  Smpl operator+=(Smpl val);

  /**
   *
   * Subtract a value from each underlying sample value.
   *
   * @param val The value to subtract.
   * @return The value val.
   *
   **/

  Smpl operator-=(Smpl val);

protected:

  /**
   *
   * Set up the row pointers. 
   *
   **/
  
  void SetRowPtrs();

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
   * Internal transform function.
   *
   * @param fwd Forward transform if true, else inverse transform.
   * @param trn The type of transform.
   * @param wid The width.
   * @param hei The height.
   * @param hor Do a horizontal transform, else vertical.
   * @param mem The pointer to the first sample.
   * @param prd To do precision, true by default.
   * @param upd To do update, true by default.
   *
   **/
  
  void DoTransformInternal(Bool fwd, EnumTransform trn, SInt wid, SInt hei, 
                           Bool hor, Smpl * mem, Bool prd = True, 
                           Bool upd = True);

  /**
   *
   * Internal shift transform function.
   *
   * @param fwd Forward transform if true, else inverse transform.
   * @param trn The type of transform.
   * @param wid The width.
   * @param hei The height.
   * @param hor Do a horizontal transform, else vertical.
   * @param mem The pointer to the first sample.
   *
   **/
  
  void DoShiftInternal(Bool fwd, EnumTransform trn, SInt wid, SInt hei, 
                       Bool hor, Smpl * mem);
  
  /**
   *
   * Create the necessary memory buffers to hold the data.
   *
   * @param width The width of the component.
   * @param height The height of the component.
   * @param set Set sample values to param def, by default True. 
   * @param def The default value to set samples to, by default 0.
   * @param desMem Destroy the newly created memory on exit or assignment,
   * by default set to True.
   *
   **/
  
  void DoCreate(SInt width, SInt height, Bool set = True, Smpl def = 0,
                Bool desMem = True);

  /**
   *
   * Destroy allocated memory.
   *
   **/

  void DoDestroy();

  /**
   *
   * Void copy command.
   *
   * @param rhs The object to copy.
   *
   **/

  void DoCopy(const HeatWaveComponent & rhs);
  
  /** Top left x-coordinate. */
  SInt m_tlx;
  
  /** Top left y-coordinate. */
  SInt m_tly;

  /** Horizontal step. */
  SInt m_hstep;

  /** Vertical step. */
  SInt m_vstep;

  /** Width. */
  SInt m_width;
  
  /** Height. */
  SInt m_height;

  /** Signed. */
  Bool m_sgnd;

  /** Precision. */
  SInt m_prec;

  /** Color of component. */
  EnumColor m_clr;

  /** Last transform performed on this component. (Spatial) */
  EnumTransform m_trn;

  /** Level of transform. (Spatial) */
  SInt m_lev;

  /** Destroy memory pointer on delete or assignment. */
  Bool m_desMem;

  /** (Internal variable) The size of component when created. */
  SInt m_size;

  /** The data. */
  Smpl * m_data;

  /** Array of pointers to each row. */
  Smpl ** m_rows;

  /** A HeatWaveLift member. */
  HeatWaveLift m_lift;
};

#endif
