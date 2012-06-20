/****************************************************************************/
/**
 ** @file   MiscTool.hpp
 ** @brief  Contains the class MiscTool.
 ** @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 **
 **/

#ifndef __MISCTOOL_HPP__
#define __MISCTOOL_HPP__

#include "MiscCmdLTool.hpp"
#include "MiscImageTool.hpp"
#include "SimpleCompressor.hpp"

/****************************************************************************/
/**
 ** Command Line Interface For HeatWave Library.
 ** 
 **/

class MiscTool : public MiscCmdLTool
{
public:

  /**************************************************************************/
  
  /**
   *
   * Parameterized Constuctor.
   *
   * See MiscCmdLTool::MiscCmdLTool().
   *
   **/
  
  MiscTool(const Char ** nams, const Char ** vers,
	       const Char ** hist, const Char ** auth,
	       const Char ** copy,
	       const Char * date, const Char * time,
	       SInt aMin, const Char * aNte, 
	       SInt cols = 78, Bool verb = False, 
	       const Char * stdI = NULL, const Char * stdO = NULL,
	       const Char * stdE = NULL);
  
  /**
   *
   * Destructor.
   *
   **/
  
  virtual ~MiscTool();

  /**
   *
   * @return The number of images loaded. 
   *
   **/

  SInt GetTotalImagesLoaded() const;

protected:

  /**************************************************************************/
  
  /**
   *
   * Argument registration function.
   *
   **/
  
  void DoGroupRegistration();
  
  /*@{*/
  /**
   *
   * Functions relating to image & video loading and saving.
   *
   * @param duty The reason for the call.
   * @param argc The number of arguments.
   * @param argv The value of arguments.
   * @return Negative value on error else result requested.
   * 
   **/
  
  SInt DoMainImgLoad(EnumFunctionDuty duty, SInt argc, const Char ** argv);
  SInt DoMainImgSave(EnumFunctionDuty duty, SInt argc, const Char ** argv);
  SInt DoMainImgVidL(EnumFunctionDuty duty, SInt argc, const Char ** argv);
  SInt DoMainImgHIII(EnumFunctionDuty duty, SInt argc, const Char ** argv);
  /*@}*/

  /*@{*/
  /**
   *
   * Functions relating to image & video transforms.
   *
   * @param duty The reason for the call.
   * @param argc The number of arguments.
   * @param argv The value of arguments.
   * @return Negative value on error else result requested.
   * 
   **/
  
  SInt DoMainImgSpat(EnumFunctionDuty duty, SInt argc, const Char ** argv);
  SInt DoMainImgTemp(EnumFunctionDuty duty, SInt argc, const Char ** argv);
  SInt DoMainImgHash(EnumFunctionDuty duty, SInt argc, const Char ** argv);
  SInt DoMainImgFuse(EnumFunctionDuty duty, SInt argc, const Char ** argv);
  SInt DoMainImgClrT(EnumFunctionDuty duty, SInt argc, const Char ** argv);
  SInt DoMainImgComp(EnumFunctionDuty duty, SInt argc, const Char ** argv);
  SInt DoMainImgHiEq(EnumFunctionDuty duty, SInt argc, const Char ** argv);
  SInt DoMainImgList(EnumFunctionDuty duty, SInt argc, const Char ** argv);
  SInt DoMainImgInfo(EnumFunctionDuty duty, SInt argc, const Char ** argv);
  SInt DoMainImgPlot(EnumFunctionDuty duty, SInt argc, const Char ** argv);
  /*@}*/
  
  /**
   *
   * Clean up.
   *
   **/

  void DoCleanUp();

  /**
   *
   * Check to see if a image number exists and if enough images on
   * list else print a message and return.
   *
   * @param inum The image number.
   * @param imin Minimum images required for this operation.
   * @return True if image number is ok, else false.
   *
   **/

  Bool CheckImgNum(SInt inum, SInt imin);

  /**
   *
   * Check to see if a image component exists.
   *
   * @param inum The image number.
   * @param icom The component number.
   * @return True if image and component number is ok, else false.
   *
   **/

  Bool CheckCmpNum(SInt inum, SInt cnum);

  /*@{*/
  /** 
   *
   * Alot of arguments share common feutures, such as selecting a component,
   * image or area. Argument handlers can call any one of the list to add this
   * to there lists.
   *
   * @param info The info struct to add this option too.
   * @param arg_offs The offset where this particular option should go.
   *
   **/

  void InfoUsesImgs(MiscArgInfo & info, SInt arg_offs);
  void InfoUsesCmps(MiscArgInfo & info, SInt arg_offs);
  void InfoUsesAreas(MiscArgInfo & info, SInt arg_offs);
  /*@}*/

  /**
   *
   * Decode the "image number(s)" string to a integer array. Usefull with
   * InfoUsesImgs.
   *
   * @param info The MiscArgInfo holding the string.
   * @param arg_offs The offset where the image option was set at.
   * @param min_img The minimum number of internal images or frames required
   * for this operation.  By default a minimum of 1.
   * @param frm_end In case where the default setting is used, return numbers
   * from end or start of array, always in a increasing order. By default from
   * end.

   * @return A valid list of image integers.
   *
   **/
  
  SIntA InfoAtoiImgs(MiscArgInfo & info, SInt arg_offs, 
                     SInt min_img = 1, Bool def_end = True);

  /**
   *
   * Decode the "component number(s)" string to a integer array. Usefull with
   * InfoUsesCmps.
   *
   * @param info The MiscArgInfo holding the string.
   * @param arg_offs The offset where the image option was set at.
   * @param imgn The image number currently being worked on.
   * @param img_offs The image or frame offset, in the internal array, number.
   * @return A valid list of image integers.
   *
   **/
  
  SIntA InfoAtoiCmps(MiscArgInfo & info, SInt arg_offs, SInt imgn, 
                     SInt min_cmp = 1, Bool frm_end = True);

  /**
   *
   * Decode the "area(s)" string to a array or rectangles. Usefull with
   * InfoUsesArea.
   *
   * @param info The MiscArgInfo holding the string.
   * @param arg_offs The offset where the image option was set at.
   * @param imgn The image number.
   * @param cmpn The component number.
   * @return A valid list of rectangular areas.
   *
   * @note The whole image area will be returned if non was specified.
   *
   **/
  
  RctA InfoAtoiRcts(MiscArgInfo & info, SInt arg_offs, SInt imgn, SInt cmpn);

  /**
   *
   * Add a integer array from a string while within upper and lower limits, else
   * print error message and False.
   *
   * @param strv The string containing the integer lists.
   * @param min The minimum number allowed.
   * @param max The maximum number allowed.
   * @param errm The error message to print in the form "%s bla bla bla %d is
   * out of bound %d and %d". (Note the 1 %s and 3 %d's).
   * @return True if all went ok, false otherwise.
   *
   **/

  Bool GetArrFromStr(SIntA & arr, const Char * strv, SInt min, SInt max, 
                     const Char * errm);
  
  /** Array of images. **/
  HeatWaveVideo m_images;

  /** Image tool. **/
  MiscImageTool m_imgTool;

};

#endif
