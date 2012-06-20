/****************************************************************************/
/**
 ** @file   MiscTextTool.hpp
 ** @brief  Contains the class MiscTextTool.
 ** @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 **
 **/

#ifndef __MISCTEXTTOOL_HPP__
#define __MISCTEXTTOOL_HPP__

#include "MiscIncludes.hpp"
#include "MiscDataTypes.hpp"
#include "MiscDefines.hpp"

/****************************************************************************/
/**
 ** A class for handling various miscalanious text tasks.
 **
 **/

class MiscTextTool
{

public:
  
  /**
   *
   * Default constructor.
   *
   **/
  
  MiscTextTool();

  /**
   *
   * Parameterised constructor.
   *
   * @param max Maximum internal buffer to use in bytes, negative value means
   * no limit.
   *
   **/
  
  MiscTextTool(SInt max);
  
  /**
   *
   * Copy constuctor.
   *
   * @param oth The other MiscTextTool to copy.
   *
   **/

  MiscTextTool(const MiscTextTool & oth);
  
  /**
   *
   * Destructor.
   *
   **/
  
  ~MiscTextTool();
  
  /**
   *
   * @return Maximum buffer size in bytes.
   *
   **/
  
  SInt GetBufferSizeMax() const;
  
  /**
   *
   * Set the maximum buffer size in bytes.
   *
   * @param size The maximum size.
   *
   **/
  
  void SetBufferSizeMax( SInt size );

  /**
   *
   * Return a pointer to the last entry entry on a text vector before NULL
   * pointer. If vector is completely empty then a default can be specified.
   *
   * @param vctr The vector of strings.
   * @param dflt The default string if vector is empty.
   * @return A pointer to last string on vector or default.
   *
   **/

  const Char * GetVectorEnd(const Char ** vctr, const Char * dflt) const;

  /**
   *
   * Add a referance(pointer) to a string at the end of a string vector, if
   * the vector is not large enough it will be grown by a specified size.
   *
   * @param rvec The reference string vector.
   * @param rstr The string to reference. (Point to.)
   * @param vecs The reference string size.
   * @param vecg The growth granularity of the reference string vector.
   * @return The new reference string vector size.
   * 
   **/

  SInt AddPtr(const Char **& rvec, const Char * rstr, SInt vecs, SInt vecg);

  /**
   *
   * Limit the length of a string.
   *
   * @param guil The guilty string to be limited.
   * @param leng The maximum length the string should be.
   * @param tail The tail to end the guilty string by if given else only NULL
   * will be used.
   *
   **/

  void Chop(Char * guil, SInt leng, const Char * tail);

  /**
   *
   * Put n characters of a certain type.
   *
   * @param type Char type to put.
   * @param numb The bumber of chars to put.
   * @param strm The stream to write to.
   * @note If numb is 0 or less nothing happens.
   *
   **/

  void PutCTimes(const Char type, SInt numb, FILE * strm); 

  /**
   *
   * Put n characters of a string.
   *
   * @param strp String pointer.
   * @param numb The bumber of chars to put.
   * @param strm The stream to write to.
   * @note If numb is 0 or less nothing happens.
   *
   **/

  void PutSTimes(const Char * strp, SInt numb, FILE * strm); 

  /**
   *
   * Format a message to fit inside a box or column. Optionally tell the
   * function what to use as a left and right border. Further more the
   * function can be told where to brake the sentance (for example at spaces
   * or tabs) and what to use if there is no suitable break point found. Note
   * that new lines is still respected and that no new line is placed after
   * final row.
   *
   * @param mesg The message to print.
   * @param wdth The width of the box. (not including the left and right 
   * spaceing.)
   * @param hght The height of the box. (ignored if negative)
   * @param strm The stream to write to.
   * @param left [OPTIONAL] Left padding. (ignored if NULL)
   * @param rght [OPTIONAL] Right padding. (ignored if NULL)
   * @param brak [OPTIONAL] Break after these characters. (ignored if NULL)
   * @param wcon [OPTIONAL] Connect broken words with these. (ignored if NULL)
   * @param skip [OPTIONAL] These characters will be skipped if first on row.
   * @param prnl [OPTIONAL] Print a new line after last row, True by default.
   *
   */

  SInt PutBox(const Char * mesg, SInt wdth, SInt hght, FILE * strm, 
              const Char * left, const Char * rght, const Char * brak, 
              const Char * wcon, const Char * skip, Bool prnl = True);

  /**
   *
   * Check n bytes of a string for (a) certain character(s). Either return a
   * pointer to the first or last occurance.
   *
   * @param srch A pointer to a character string to be searched.
   * @param find A pointer to a character string that should be matched.
   * @param lnth The maximum length to be search. If 0 or less all will be 
   * searched.
   * @param offs The offset to do the search at. If 
   * @param frst Do a forward search.
   *
   **/

  const Char * StrChr(const Char * srch, const Char * find, SInt lnth, 
                      SInt offs = 0, Bool frst = True);

  /**
   *
   * See if a character belongs to a group.
   *
   * @param test The character to test.
   * @param grup The group the character might belong too.
   * @return True if test character was found in grup.
   *
   **/
  
  Bool ChkCGrp(const Char test, const Char * grup);

  /**
   *
   * Turn a special formatted string into a list of integers.
   *
   * This string should be seperated by commans, as in "1,2,3" alternatively a
   * range could be specified as such "1:5", which would be the same as
   * "1,2,3,4,5". Other option is place a step increase size, as such "1:10@2"
   * which eqautes to "1,3,5,7,9". Another usefull option is the random
   * option, which is used as follow "1~10" which will return a random number
   * between 1 and 10. Integers must be in deximal format.
   *
   * @param strn The string to reformat.
   * @return A list of integers, if found else none.
   *
   **/

  SIntA AToSIntA( const Char * strn);
  
  /**
   *
   * Create a clone of this text tool.
   *
   **/

  MiscTextTool * GetClone() const;

  /**
   *
   * Assignment operator.
   *
   * @param rhs The other MiscTextTool object to copy.
   * @return A reference to this object.
   *
   **/
  
  MiscTextTool& operator=(const MiscTextTool& rhs);
  
  /**
   *
   * Equality comparison.
   *
   * @param rhs The other MiscTextTool object to compare against.
   * @return True if equal, false otherwise.
   *
   **/
  
  Bool operator==(const MiscTextTool & rhs ) const;

protected:
  
  /**
   *
   * Copy from another MiscTextTool. 
   *
   * @param rhs The other MiscTextTool to copy.
   *
   **/

  void DoCopy(const MiscTextTool & rhs);
  
  /**
   *
   * Destroy allocated memory.
   *
   **/

  void DoDestroy();
  
  /** Max buffer size **/
  SInt m_bufferSize;

  /** Heap buffer **/
  Char * m_buffer;  
};

#endif  //__MISCTEXTTOOL_HPP__
