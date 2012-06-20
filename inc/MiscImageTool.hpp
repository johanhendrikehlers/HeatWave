/****************************************************************************/
/**
 ** @file   MiscImageTool.hpp
 ** @brief  Contains the class MiscImageTool.
 ** @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 **
 **/

#ifndef __MISCIMAGETOOL_HPP__
#define __MISCIMAGETOOL_HPP__

#include "MiscIncludes.hpp"

/****************************************************************************/
/**
 ** Class for common image tools. These include reading and writing to and 
 ** from image files. Convertions between differen images classes from 
 ** different image librarys etc.
 ** 
 **/

class MiscImageTool
{
public:

  /**
   *
   * Groups of image librarys understood.
   *
   **/

  enum EnumLibrary {
    /** Jasper Image Library **/
    Lib_Jasper = 1,
    
    /** HeatWave Image Library **/
    Grp_HeatWave = 2,

    /** Herberts Image Library **/
    Grp_Herbert = 0xff
  };

  /**
   *
   * Default Constuctor.
   *
   **/

  MiscImageTool();
  
  /**
   *
   * Copy constructor.
   * 
   * @param oth The other MiscImageTool object to copy.
   *
   **/
  
  MiscImageTool(const MiscImageTool & oth);
  
  /**
   *
   * Destructor.
   *
   **/
  
  virtual ~MiscImageTool();
  
  /**
   *
   * @return A clone of this object.
   *
   **/

  MiscImageTool * GetClone() const;

  /**
   *
   * Converts images from Jasper format to HeatWave format.
   *
   * @param image The jasper image to convert.
   * @return HeatWave Image.
   *
   **/

  HeatWaveImage * GetHeatFromJasper(jas_image_t * image) const;

  /**
   *
   * Converts images from HeatWave format to Jasper format.
   *
   * @param image The HeatWave image to convet.
   * @return Jasper Image.
   *
   **/

  jas_image_t * GetJasperFromHeat(const HeatWaveImage * image) const;

  /**
   *
   * Convert Jasper color schema into HeatWave color schema.
   *
   * @param spc The jasper color schema ( space ).
   * @return HeatWave color schema.
   *
   **/
  
  EnumSpace GetSpaceHeatFromJasper(const jas_image_colorspc_t spce) const;
  
  /**
   *
   * Convert HeatWave color schema into Jasper color schema.
   *
   * @param schm The HeatWave color schema.
   * @return Jasper color schema.
   *
   **/
  
  jas_image_colorspc_t GetSpaceJasperFromHeat(const EnumSpace schm) const;

  /**
   *
   * Convert Jasper color type into HeatWave color type.
   *
   * @param spc The jasper color schema (space).
   * @param type The jasper color type.
   * @return HeatWave color type.
   *
   **/
  
  EnumColor GetColorHeatFromJasper(const jas_image_colorspc_t spc, 
				   const jas_image_smpltype_t type) const;

  /**
   *
   * Convert HeatWave color type into Jasper color type.
   *
   * @param type The HeatWave color type.
   * @return Jasper color type.
   *
   **/
  
  jas_image_smpltype_t GetColorJasperFromHeat(const EnumColor type) const;

  /**
   *
   * Get the last error message.
   *
   * @return The last error message.
   *
   **/

  Char * GetLastErrorMessage();

  /**
   *
   * Replace the last error message with a new copy.
   *
   * @param errm The new error message to copy.
   * @note Previous message will be deleted, and so will this one on
   * exit or replacement.
   *
   **/

  void CpyLastErrorMessage(const Char *);

  /**
   *
   * Replace the last error message with a new string.
   *
   * @param errm The new error message to own.
   * @note Previous message will be deleted, and so will this one on
   * exit or replacement.
   *
   **/

  void SetLastErrorMessage(Char *);

  /**
   *
   * Read a image from file or standard input.
   *
   * @param file The file name.
   * @param form The file format.
   * @param opts Options to parse to decoder.
   * @return Image on succes, else NULL.
   * @note Print last error for more information.
   *
   **/

  HeatWaveImage * ReadImage(const Char * file, const Char * form, 
			    const Char * opts);

  /**
   *
   * Write a image from to a file or to standard output.
   *
   * @oaram imgs The HeatWaveImage to externalise.
   * @param file The file name.
   * @param form The file format.
   * @param opts Options to parse to decoder.
   * @param pixl Pixelise data make sure its in range 0-255.
   * @return True on success, else False.
   * @note Print last error for more information.
   *
   **/

  Bool WriteImage(const HeatWaveImage & imgs, const Char * file, 
		  const Char * form, const Char * opts, Bool pixl);

  /**
   *
   * Assignment operator.
   *
   * @param rhs The other component to copy data and state from.
   * @return A reference to this object.
   *
   **/
  
  MiscImageTool & operator=(const MiscImageTool & oth);

  /**
   *
   * Equality comparison.
   *
   * @param rhs The other component to compare against.
   * @return True if equal, False if un-equal.
   *
   **/
  
  Bool operator==(const MiscImageTool & oth) const;
  
protected:
  
  /**
   *
   * Clean up any resources required.
   *
   **/

  void DoCleanUp();

  /** Error message. **/
  Char * m_error;
};

#endif //__MISCIMAGETOOL_HPP__
