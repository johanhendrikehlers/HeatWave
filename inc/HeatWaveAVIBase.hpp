/****************************************************************************/
/**
 ** @File HeatWaveAVIBase.hpp
 ** @brief Contains the HeatWaveAVIBase class definition.
 ** @author Johan Hendrik Ehlers <https://github.com/johanhendrikehlers>
 **
 **/

#ifndef __HEATWAVEAVIBASE_HPP__
#define __HEATWAVEAVIBASE_HPP__

#include "HeatWaveEnums.hpp"
#include "HeatWaveVideo.hpp"
#include "HeatWaveAVIStructs.hpp"

/** Constant list header size */
#define LIST_HEADER 12

/** Constant chunk header size */
#define CHUNK_HEADER 8

/** Constant four cc length */
#define FOUR_CC 4

/** Constant two cc length */
#define TWO_CC 2

/**
 *
 * A structure to associate a video type with a color space the color order,
 * and sampling ratios.
 *
 **/

typedef struct 
{
  EnumVideoType m_type;
  EnumSpace m_space;
  SInt m_colors;
  EnumColor m_order[MAXCOLORSINANYSPACE];
  SInt m_hSampling[MAXCOLORSINANYSPACE];
  SInt m_vSampling[MAXCOLORSINANYSPACE];
} StructCodingType;

/****************************************************************************/
/**
 ** A AVI file base class.
 **
 **/

class HeatWaveAVIBase
{
public:

  /**
   *
   * Constructor.
   *
   **/

  HeatWaveAVIBase();
  
  /**
   *
   * Destructor.
   *
   **/
  
  virtual ~HeatWaveAVIBase();

  /**
   *
   * @return The video file name.
   *
   **/
  
  const char * GetFileName() const;
  
  /**
   *
   * Set the video file name.
   *
   * @param str The video file name.
   *
   **/
  
  void SetFileName(char * str);

  /**
   *
   * Copy the file name.
   *
   * @param str The video file name.
   *
   **/
  
  void CpyFileName(const char * str);

  /**
   *
   * @return The video file handle.
   *
   **/
  
  FILE * GetFileHandle() const;

  /**
   *
   * Set the file handle.
   *
   * @param hnd The file handle.
   *
   **/

  void SetFileHandle(FILE * hnd);

  /**
   *
   * @return The a pointer to the video.
   *
   **/
  
  HeatWaveVideo * GetVideoPtr() const;

  /**
   *
   * Set the video pointer.
   *
   * @param ptr Pointer to video.
   *
   **/
  
  void SetVideoPtr(HeatWaveVideo * ptr);

  /**
   *
   * Copy the video pointer.
   *
   * @param ptr Pointer to video.
   *
   **/
  
  void CpyVideoPtr(const HeatWaveVideo * ptr);

  /**
   *
   * @return A reference to the video.
   *
   **/

  HeatWaveVideo & GetVideo() const;
  
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
   * Set the main avi header.
   *
   * @param hdr The header to point to.
   *
   **/

  void SetHeatWaveAVIHeader(HeatWaveAVIHeader * hdr);

  /**
   *
   * Copy the main avi header.
   *
   * @param hdr The header to copy.
   *
   **/
  
  void CpyHeatWaveAVIHeader(const HeatWaveAVIHeader * hdr);

  /**
   *
   * @return A pointer to the main avi header.
   *
   **/
  
  HeatWaveAVIHeader * GetHeatWaveAVIHeader() const;

  /**
   *
   * Set the video stream header.
   *
   * @param hdr The header to point to.
   *
   **/
  
  void SetVideoHeader(HeatWaveAVIStreamHeader * hdr);

  /**
   *
   * Copy the video stream header.
   *
   * @param hdr The header to copy.
   *
   **/
  
  void CpyVideoHeader(const HeatWaveAVIStreamHeader * hdr);

  /**
   *
   * @return A pointer to the video stream header.
   *
   **/
  
  HeatWaveAVIStreamHeader * GetVideoHeader() const;

  /**
   *
   * Set the video stream information.
   *
   * @param inf The information to point to.
   *
   **/
  
  void SetVideoInformation(HeatWaveAVIBitmapHeader * inf);
  
  /**
   *
   * Copy the video stream information.
   *
   * @param inf The information to copy.
   *
   **/
  
  void CpyVideoInformation(const HeatWaveAVIBitmapHeader * inf);

  /**
   *
   * @return A pointer to the video stream information.
   *
   **/
  
  HeatWaveAVIBitmapHeader * GetVideoInformation() const;

  /**
   *
   * @return Get the video stream name.
   *
   **/
  
  char * GetVideoName() const;
  
  /**
   *
   * Copy a new video stream name.
   *
   * @param str The new video stream name.
   *
   **/
  
  void CpyVideoName(const char * str);
  
  /**
   *
   * Set a new video stream name.
   *
   * @param str The new video stream name.
   *
   **/
  
  void SetVideoName(char * str);

  /**
   *
   * Get Video codec information. (Binary)
   *
   * @param size (OUT) The size of the data on the pointer.
   *
   **/

  char * GetVideoCodingInfo(SInt & size) const;

  /**
   *
   * Cpy the video codec information. (Binary)
   *
   * @param ptr The pointer to the codec information to copy.
   * @param size The amount of data to copy.
   *
   **/

  void CpyVideoCodingInfo(const char * ptr, SInt size);

  /**
   *
   * Set the video codec information. (Binary)
   *
   * @param ptr The pointer to the codec information.
   * @param size The amount of data pointing to.
   *
   **/

  void SetVideoCodingInfo(char * ptr, SInt size);
 
  /**
   *
   * Set the audio stream header.
   *
   * @param hdr The header to point to.
   *
   **/
  
  void SetAudioHeader(HeatWaveAVIStreamHeader * hdr);

  /**
   *
   * Copy the audio stream header.
   *
   * @param hdr The header to copy.
   *
   **/
  
  void CpyAudioHeader(const HeatWaveAVIStreamHeader * hdr);

  /**
   *
   * @return A pointer to the audio stream header.
   *
   **/
  
  HeatWaveAVIStreamHeader * GetAudioHeader() const;

  /**
   *
   * Set the audio stream information.
   *
   * @param inf The information to point to.
   *
   **/
  
  void SetAudioInformation(HeatWaveAVIWaveHeader * inf);
  
  /**
   *
   * Copy the audio stream information.
   *
   * @param inf The information to copy.
   *
   **/
  
  void CpyAudioInformation(const HeatWaveAVIWaveHeader * inf);

  /**
   *
   * @return A pointer to the audio stream information.
   *
   **/
  
  HeatWaveAVIWaveHeader * GetAudioInformation() const;

  /**
   *
   * @return Get the audio stream name.
   *
   **/
  
  char * GetAudioName() const;
  
  /**
   *
   * Copy a new audio stream name.
   *
   * @param str The new audio stream name.
   *
   **/
  
  void CpyAudioName(const char * str);
  
  /**
   *
   * Set a new audio stream name.
   *
   * @param str The new audio stream name.
   *
   **/
  
  void SetAudioName(char * str);

  /**
   *
   * Get Audio codec information. (Binary)
   *
   * @param size (OUT) The size of the data on the pointer.
   *
   **/

  char * GetAudioCodingInfo(SInt & size) const;

  /**
   *
   * Cpy the audio codec information. (Binary)
   *
   * @param ptr The pointer to the codec information to copy.
   * @param size The amount of data to copy.
   *
   **/

  void CpyAudioCodingInfo(const char * ptr, SInt size);

  /**
   *
   * Set the audio codec information. (Binary)
   *
   * @param ptr The pointer to the codec information.
   * @param size The amount of data pointing to.
   *
   **/

  void SetAudioCodingInfo(char * ptr, SInt size);

  /**
   *
   * @return The last error message.
   *
   **/
  
  char * GetError() const;
  
protected:

  /**
   *
   * Copy a new last error message.
   *
   * @param str The new last error message.
   *
   **/
  
  void CpyError(const char * str);

  /**
   *
   * Set a new last error message.
   *
   * @param str The new last error message.
   *
   **/

  void SetError(char * str);

  /**
   *
   * Destroy all memory pointers and close the file.
   * @note If m_desMem if false nothing happens.
   *
   **/

  void DoDestroy();
  
  /** File name. **/
  char * m_fileName;
  
  /** File handle. **/
  FILE * m_fileHandle;

  /** Video buffer. **/
  HeatWaveVideo * m_video;  
  
  /** Destroy memory pointers on OBJECT delete or assignment. 
   ** @note No affect on individual object member assignment! **/
  Bool m_desMem;

  /** AVI header. **/
  HeatWaveAVIHeader * m_avih;
  
  /** Video Stream Header. **/
  HeatWaveAVIStreamHeader * m_vids;

  /** Video Stream Information. **/
  HeatWaveAVIBitmapHeader * m_vidsInfo;

  /** Video Stream Name. **/
  char * m_vidsName;
  
  /** Video Stream Coding Information. **/
  char * m_vidsCodingInfo;
  
  /** Video Stream Coding Information size. **/
  SInt m_vidsCodingInfoSize;

  /** Video data priority. **/
  SInt m_vidsPriority;

  /** Audio Stream Header. **/
  HeatWaveAVIStreamHeader * m_auds;
  
  /** Audio Stream Information. **/
  HeatWaveAVIWaveHeader * m_audsInfo;

  /** Audio Stream Name. **/
  char * m_audsName;
  
  /** Audio Stream Coding Information. **/
  char * m_audsCodingInfo;
  
  /** Audio Stream Coding Information size. **/
  SInt m_audsCodingInfoSize;

  /** Audio data priority. **/
  SInt m_audsPriority;

  /** Error message. **/
  char * m_error;
};

extern const char LIST_RIFF[];
extern const char LIST_LIST[];
extern const char * LIST_NAMES[];
extern const char LIST_AVI_[];
extern const char LIST_STRL[];
extern const char LIST_HDRL[];
extern const char LIST_MOVI[];
extern const char LIST_REC_[];
extern const char * LIST_TYPES[];
extern const char CHUNK_AVIH[];
extern const char CHUNK_STRH[];
extern const char CHUNK_STRF[];
extern const char CHUNK_STRD[];
extern const char CHUNK_STRN[];
extern const char CHUNK_IDX1[];
extern const char CHUNK_JUNK[];
extern const char CHUNK_DB[];
extern const char CHUNK_DC[];
extern const char CHUNK_PC[];
extern const char CHUNK_WB[];
extern const char * CHUNK_NAMES[];
extern const char STREAM_VIDS[];
extern const char STREAM_AUDS[];
extern const char STREAM_MIDS[];
extern const char STREAM_TXTS[];
extern const char * STREAM_TYPES[];
extern const StructCodingType CODEING_TYPES[];

#endif // __HEATWAVEAVIBASE_HPP__
