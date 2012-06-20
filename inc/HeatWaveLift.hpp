/*****************************************************************************/
/**
 ** @file   HeatWaveLift.hpp
 ** @brief  Contains the HeatWaveLift class definition.
 ** @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 **
 **/

#ifndef __HEATWAVELIFT_HPP__
#define __HEATWAVELIFT_HPP__

#include "HeatWaveEnums.hpp"
#include "HeatWaveMath.hpp"
class HeatWaveImage;

/****************************************************************************/
/**
 ** A set of predefined wavelet filters using the lifting implementation.
 **
 ** All of the wavelet transforms used in this class is referenced in the 
 ** following papars:
 ** <ul><li>
 ** <a href="http://citeseer.ist.psu.edu/407090.html">
 ** "Wavelet transforms that map integers to integers"</a> by A.R. Calderbank,
 ** Ingrid Daubechies, Wim Sweldens, and Book-Lock Yeo.
 ** </li><li>
 ** <a href="http://www.ece.uvic.ca/~mdadams/publications/phdthesis.pdf">
 ** "Reversible Integer-To-Integer Wavelet Transforms For Image Coding"</a> by
 ** Michael David Adams
 ** </li></ul>
 **
 ** @todo 1. Finite Data Range (FDR) or Property of Precision Preservation
 ** (PPP) is possible. Default is not to use it.<br> 2. Edges are dealt with
 ** using the half or full sample symmetry model.  <a
 ** href="http://www.ece.uvic.ca/~mdadams/publications/phdthesis.pdf"> See
 ** section 4.2.2. of M.Adams's PhD thesis.</a> Default is half symmetry.
 ** 
 **/

class HeatWaveLift
{
public:

  /**
   *
   * Default constructor.
   *
   **/

  HeatWaveLift();
  
  /**
   *
   * Parameterized constructor.
   *
   * @param len Internal buffer lenght. (-1 by default, auto assigned.)
   *
   **/
  
  HeatWaveLift(SInt len);

  /**
   *
   * Copy constructor.
   *
   * @param oth The other HeatWaveLift object to copy.
   *
   **/

  HeatWaveLift(const HeatWaveLift & oth);
  
  /**
   *
   * Destructor.
   *
   **/
  
  ~HeatWaveLift();

  /**
   *
   * @return The size of the internal buffer.
   *
   */
  
  SInt GetBufferSize() const;

  /**
   *
   * Set the size of the internal buffer.
   *
   * @param siz The size.
   *
   **/
  
  void SetBufferSize(SInt siz);
  
  /**
   *
   * Split a signal, putting the even Samples first followed by odd Samples.
   * Also sometimes refered to as "lazy" wavelet.
   *
   * @param dat The pointer to the data buffer.
   * @param len The length of the data to be split. (Including date to be 
   * skipped, see stp below)
   * @param stp Offset of next sample. Must be 1 or greater. (Usefull for
   * skipping a number of data values as might be the case for 2d data: images)
   * @param evn (out) Return a pointer to start of even samples.
   * @param odd (out) Return a pointer to start of odd samples.
   * @note To use step on returned pointers.
   *
   **/
  
  void Split(Smpl * dat, SInt len, SInt stp, Smpl* & evn, Smpl* & odd);
  
  /**
   *
   * Join a signal, interleaving the last samples with first samples.
   *
   * @param dat The pointer to the data stream|buffer.
   * @param len The length of the data to be split.
   * @param stp The inter sample spaces. Must be 1 or greater.
   *
   **/
  
  void Join(Smpl * dat, SInt len, SInt stp);

  /**
   * 
   * Functionaly same as HeatWaveLift::Split but uses 1/2 the memory overhead,
   * speed might be affect (+ or - ?)
   *
   **/
  
  void SplitOpt(Smpl * dat, SInt len, SInt stp, Smpl* & evn, Smpl* & odd);
  
  /**
   *
   * Functionaly same as HeatWaveLift::Join but uses 1/2 the memory overhead,
   * speed might be affect (+ or - ?)
   *
   **/
  
  void JoinOpt(Smpl * dat, SInt len, SInt stp);

  /**
   *
   * Same as HeatWaveLift::Split but uses 1 memory unit as overhead, speed
   * might be affect (+ or - ?)
   *
   **/
  
  void SplitZ(Smpl * dat, SInt len, SInt stp, Smpl* & evn, Smpl* & odd);
  
  /**
   *
   * Join a signal, interleaving the last samples with first samples.
   *
   **/
  
  void JoinZ(Smpl * dat, SInt len, SInt stp);

  /**
   *
   * Same as HeatWaveLift::Split but uses N memory units as overhead, speed
   * might be affect (+ or - ?) (Where N is equal to HEATWAVELIFTSTACKN) 
   *
   **/
  
  void SplitN(Smpl * dat, SInt len, SInt stp, Smpl* & evn, Smpl* & odd);
  
  /**
   *
   * Join a signal, interleaving the last samples with first samples.
   *
   **/
  
  void JoinN(Smpl * dat, SInt len, SInt stp);

  /**
   *
   * Usefull function for finding out what the length of the even samples and
   * odd samples are.
   *
   * @param orgL (in)  Original length.
   * @param evnL (out) Even length.
   * @param oddL (out) Odd length.
   *
   **/

  void FindLengths(const SInt orgL, SInt &evnL, SInt  &oddL) const;
  
  /**
   *
   * Split a video signal, putting the even images first followed by odd
   * images.
   *
   * @param imgs The pointer to the video buffer.
   * @param ilen The number of images on buffer.
   *
   **/
  
  void SplitVideo(HeatWaveImage ** imgs, SInt ilen);
  
  /**
   *
   * Join a video signal, interleaving the last images with first images.
   *
   * @param imgs The pointer to the video buffer.
   * @param ilen The number of images on buffer.
   *
   **/
  
  void JoinVideo(HeatWaveImage ** imgs, SInt ilen);

  /** Function prototype used with all lifting functions. **/

  typedef void (HeatWaveLift::*m_lftFunction)(Smpl *, Smpl *, SInt, SInt, 
                                              Bool) const;

  /**
   *
   * Fill a list of function pointers using certain criteria that can be
   * called to perform lifting.
   *
   * @param func [OUT] The array of function pointers. 
   * (Length of HEATWAVELIFTMAXSTEPS required) 
   * @param tran The transform type.
   * @param dirs The direction, true for forward else inverse.
   * @param upd Include update functions. (true by default).
   * @param prd Include predict functions. (true by defualt).
   * @return Number of functions pointers on list.
   *
   **/
  
  SInt GetFuncArray(HeatWaveLift::m_lftFunction * func, EnumTransform tran,
                    Bool dirs, Bool prd = True, Bool upd = True);

  /*@{*/
  /**
   *
   * Transform (1,1) base function.
   * 
   * @param a Sample a. 
   * @param b Sample b.
   *
   **/


  inline void Trn1_1PrdFwd(const Smpl & a, Smpl & b) const;
  inline void Trn1_1UpdFwd(Smpl & a, const Smpl & b) const;
  inline void Trn1_1UpdRev(Smpl & a, const Smpl & b) const;
  inline void Trn1_1PrdRev(const Smpl & a, Smpl & b) const;
  /*@}*/

  /*@{*/
  /**
   *
   * Transform (1,1)+PPP base function.
   * 
   * @param a Sample a. 
   * @param b Sample b.
   *
   **/


  inline void Trn1_1mPrdFwd(const Smpl & a, Smpl & b) const;
  inline void Trn1_1mUpdFwd(Smpl & a, const Smpl & b) const;
  inline void Trn1_1mUpdRev(Smpl & a, const Smpl & b) const;
  inline void Trn1_1mPrdRev(const Smpl & a, Smpl & b) const;
  /*@}*/
  
  /*@{*/
  /**
   *
   * Transform (2,2) base function.
   * 
   * @param a Sample a.
   * @param b Sample b.
   * @param c Sample c.
   *
   **/
  
  inline void Trn2_2PrdFwd(const Smpl & a, Smpl & b, const Smpl & c) const;
  inline void Trn2_2UpdFwd(const Smpl & a, Smpl & b, const Smpl & c) const;
  inline void Trn2_2UpdRev(const Smpl & a, Smpl & b, const Smpl & c) const;
  inline void Trn2_2PrdRev(const Smpl & a, Smpl & b, const Smpl & c) const;
  /*@}*/

  /*@{*/
  /**
   *
   * Transform (2+2,2) base function. Uses functions from (2,2).
   * 
   * @param a Sample a.
   * @param b Sample b.
   * @param c Sample c.
   * @param d Sample d.
   * @param e Sample e.
   *
   **/
  
  inline void Trn2p2_2PrdFwd(const Smpl & a, const Smpl & b, Smpl & c, 
                             const Smpl & d, const Smpl & e) const;
  inline void Trn2p2_2PrdRev(const Smpl & a, const Smpl & b, Smpl & c, 
                             const Smpl & d, const Smpl & e) const;
  /*@}*/
  
  /*@{*/
  /**
   *
   * Transform (4,4) base function.
   * 
   * @param a Sample a.
   * @param b Sample b.
   * @param c Sample c.
   * @param d Sample d.
   * @param e Sample e.
   *
   **/
  
  inline void Trn4_4PrdFwd(const Smpl & a, const Smpl & b, Smpl & c, 
                           const Smpl & d,const Smpl & e) const;
  inline void Trn4_4UpdFwd(const Smpl & a, const Smpl & b, Smpl & c, 
                           const Smpl & d,const Smpl & e) const;
  inline void Trn4_4UpdRev(const Smpl & a, const Smpl & b, Smpl & c, 
                           const Smpl & d,const Smpl & e) const;
  inline void Trn4_4PrdRev(const Smpl & a, const Smpl & b, Smpl & c, 
                           const Smpl & d,const Smpl & e) const;

  inline void Trn4_4BUpdFwd(const Smpl & a, const Smpl & b, Smpl & c, 
                            const Smpl & d,const Smpl & e) const;
  inline void Trn4_4BUpdRev(const Smpl & a, const Smpl & b, Smpl & c, 
                            const Smpl & d,const Smpl & e) const;
  /*@}*/
  
  /*@{*/
  /**
   *
   * Transform (6,6) base function.
   * 
   * @param a Sample a.
   * @param b Sample b.
   * @param c Sample c.
   * @param d Sample d.
   * @param e Sample e.
   * @param f Sample f.
   * @param g Sample g.
   *
   **/
  
  inline void Trn6_6PrdFwd(const Smpl & a, const Smpl & b, const Smpl & c, 
                           Smpl & d, const Smpl & e, const Smpl & f,
                           const Smpl & g) const;
  inline void Trn6_6UpdFwd(const Smpl & a, const Smpl & b, const Smpl & c, 
                           Smpl & d, const Smpl & e, const Smpl & f,
                           const Smpl & g) const;
  inline void Trn6_6UpdRev(const Smpl & a, const Smpl & b, const Smpl & c, 
                           Smpl & d, const Smpl & e, const Smpl & f,
                           const Smpl & g) const;
  inline void Trn6_6PrdRev(const Smpl & a, const Smpl & b, const Smpl & c, 
                           Smpl & d, const Smpl & e, const Smpl & f,
                           const Smpl & g) const;
  /*@}*/

  /*@{*/
  /**
   *
   * Transform (9-7) base function. 
   * <a href="http://www.ece.uvic.ca/~mdadams/publications/phdthesis.pdf">
   * (See page 89 of M. Adams's PhD for the reference.)</a>
   * 
   * @param a Sample a.
   * @param b Sample b.
   * @param c Sample c.
   *
   **/
  
  inline void Trn97_1PrdFwd(const Smpl & a, Smpl & b, const Smpl & c) const;
  inline void Trn97_1UpdFwd(const Smpl & a, Smpl & b, const Smpl & c) const;
  inline void Trn97_2PrdFwd(const Smpl & a, Smpl & b, const Smpl & c) const;
  inline void Trn97_2UpdFwd(const Smpl & a, Smpl & b, const Smpl & c) const;
  inline void Trn97_2UpdRev(const Smpl & a, Smpl & b, const Smpl & c) const;
  inline void Trn97_2PrdRev(const Smpl & a, Smpl & b, const Smpl & c) const;
  inline void Trn97_1UpdRev(const Smpl & a, Smpl & b, const Smpl & c) const;
  inline void Trn97_1PrdRev(const Smpl & a, Smpl & b, const Smpl & c) const;
  /*@}*/

  /*@{*/
  /**
   *
   * Transform D4 base function.
   * 
   * @param a Sample a.
   * @param b Sample b.
   *
   **/

  inline void TrnD4_1PrdFwd(const Smpl & a, Smpl & b) const;
  inline void TrnD4_2PrdFwd(const Smpl & a, Smpl & b) const;
  inline void TrnD4_2PrdRev(const Smpl & a, Smpl & b) const;
  inline void TrnD4_1PrdRev(const Smpl & a, Smpl & b) const;
  /*@}*/

  /*@{*/
  /**
   *
   * Transform D4 base function.
   * 
   * @param a Sample a.
   * @param b Sample b.
   * @param c Sample b.
   *
   **/
  inline void TrnD4_UpdFwd(const Smpl & a, Smpl & b, const Smpl & c) const;
  inline void TrnD4_UpdRev(const Smpl & a, Smpl & b, const Smpl & c) const;
  /*@}*/
   
  /**************************************************************************/
  /*@{*/
  /**
   *
   * Intra frame function.
   *
   * @param even The data pointer to the even samples.
   * @param odd The data pointer to the odd samples.
   * @param len The combined length of even and odd samples.
   * @param step The inter sample distance, must be 1 or greater.
   * @param forward Do forward transform else inverse.
   * 
   **/
  
  void Trn1_1Prd(Smpl * even, Smpl * odd, SInt len, SInt step, 
                 Bool forward) const;
  void Trn1_1Upd(Smpl * even, Smpl * odd, SInt len, SInt step, 
                 Bool forward) const;

  void Trn1_1mPrd(Smpl * even, Smpl * odd, SInt len, SInt step, 
                  Bool forward) const;
  void Trn1_1mUpd(Smpl * even, Smpl * odd, SInt len, SInt step, 
                  Bool forward) const;
  
  void Trn2_2Prd(Smpl * even, Smpl * odd, SInt len, SInt step, 
                 Bool forward) const;
  void Trn2_2Upd(Smpl * even, Smpl * odd, SInt len, SInt step, 
                 Bool forward) const;

  void Trn2p2_2Prd(Smpl * even, Smpl * odd, SInt len, SInt step, 
                   Bool forward) const;
  
  void Trn4_4Prd(Smpl * even, Smpl * odd, SInt len, SInt step, 
                 Bool forward) const;
  void Trn4_4Upd(Smpl * even, Smpl * odd, SInt len, SInt step, 
                 Bool forward) const;

  void Trn4_4BUpd(Smpl * even, Smpl * odd, SInt len, SInt step, 
                  Bool forward) const;
  
  void Trn6_6Prd(Smpl * even, Smpl * odd, SInt len, SInt step, 
                 Bool forward) const;
  void Trn6_6Upd(Smpl * even, Smpl * odd, SInt len, SInt step, 
                 Bool forward) const;

  void Trn97_1Prd(Smpl * even, Smpl * odd, SInt len, SInt step, 
                  Bool forward) const;
  void Trn97_1Upd(Smpl * even, Smpl * odd, SInt len, SInt step, 
                  Bool forward) const;
  void Trn97_2Prd(Smpl * even, Smpl * odd, SInt len, SInt step, 
                  Bool forward) const;
  void Trn97_2Upd(Smpl * even, Smpl * odd, SInt len, SInt step, 
                  Bool forward) const;

  void TrnD4_1Prd(Smpl * even, Smpl * odd, SInt len, SInt step, 
                  Bool forward) const;
  void TrnD4_Upd(Smpl * even, Smpl * odd, SInt len, SInt step, 
                 Bool forward) const;
  void TrnD4_2Prd(Smpl * even, Smpl * odd, SInt len, SInt step, 
                  Bool forward) const;
  /*@}*/

  /**
   *
   * Comparison operator.
   *
   * @param oth The other HeatWaveLift object to compare against.
   * @return If equal True else False.
   *
   **/

  Bool operator==(const HeatWaveLift & oth) const;

  /**
   *
   * Assignment operator
   *
   * @param oth The other HeatWaveLift object to copy.
   * @return A reference to this object.
   *
   **/

  HeatWaveLift & operator=(const HeatWaveLift & oth);
  
protected:

  /** 
   * 
   * Allocate the necessary resources. 
   *
   * @param len The lenght.
   *
   **/
  
  void DoAllocate(SInt len);

  /**
   *
   * Release the allocated resources.
   *
   **/

  void DoRelease();

  /** Used with mod operations filters (1_1m) **/
  SInt m_mod;

  /** Temp buffer. */
  SInt * m_buffer;
  
  /** Temp buffer lenght. */
  SInt m_bufferLen;
};

#endif //__HEATWAVELIFT_HPP__
