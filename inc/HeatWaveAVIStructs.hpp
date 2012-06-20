/****************************************************************************/
/**
 ** @file   HeatWaveAVIStructs.hpp
 ** @brief  Contains some structs and flags used in AVI files.
 ** @author Johan Hendrik Ehlers <https://github.com/johanhendrikehlers>
 ** 
 ** @note The following website is a pretty good source of information : 
 ** <a href="http://faydoc.tripod.com/formats/avi.htm">
 ** http://faydoc.tripod.com/formats/avi.htm</a>.
 **
 **/

#ifndef __HEATWAVEAVISTRUCTS_HPP__
#define __HEATWAVEAVISTRUCTS_HPP__

/*@{*/
/** flags for use in \<dwFlags\> in AVIFileHdr **/
#define AVIF_HASINDEX		0x00000010	
#define AVIF_MUSTUSEINDEX	0x00000020
#define AVIF_ISINTERLEAVED	0x00000100
#define AVIF_TRUSTCKTYPE	0x00000800	
#define AVIF_WASCAPTUREFILE	0x00010000
#define AVIF_COPYRIGHTED	0x00020000
/*@}*/

/** The AVI File Header LIST chunk should be padded to this size **/
#define AVI_HEADERSIZE  2048

/** The Main avi header. **/
typedef struct
{
  UInt	dwMicroSecPerFrame;	
  UInt	dwMaxBytesPerSec;	
  UInt	dwPaddingGranularity;	
  UInt	dwFlags;
  UInt	dwTotalFrames;	
  UInt	dwInitialFrames;
  UInt	dwStreams;
  UInt	dwSuggestedBufferSize;
  UInt	dwWidth;
  UInt	dwHeight;
  UInt	dwReserved[4];
} HeatWaveAVIHeader;

/** Rectangle struct. **/
typedef struct
{
  UInt left;
  UInt top;
  UInt right;
  UInt bottom;
} HeatWaveAVIRect;

/*@{*/
/** Data stream flags. **/
#define AVISF_DISABLED			0x00000001
#define AVISF_VIDEO_PALCHANGES		0x00010000
/*@}*/

/** AVI stream header. **/
typedef struct {
    char fccType[4];
    char fccHandler[4];
    UInt dwFlags;
    UInt16 wPriority;
    UInt16 wLanguage;
    UInt dwInitialFrames;
    UInt dwScale;	
    UInt dwRate;
    UInt dwStart;
    UInt dwLength;
    UInt dwSuggestedBufferSize;
    UInt dwQuality;
    UInt dwSampleSize;
    HeatWaveAVIRect rcFrame;
} HeatWaveAVIStreamHeader;

/*@{*/
/** Flags for index **/
#define AVIIF_LIST          0x00000001L 
#define AVIIF_KEYFRAME      0x00000010L 
#define AVIIF_NOTIME	    0x00000100L 
#define AVIIF_COMPUSE       0x0FFF0000L 
/*@}*/

/** Index entry. **/
typedef struct
{
    UInt ckid;
    UInt dwFlags;
    UInt dwChunkOffset;	
    UInt dwChunkLength;	
} HeatWaveAVIIndexEntry;

/** Palette entry **/
typedef struct
{
  UInt8 peRed;
  UInt8 peGreen;
  UInt8 peBlue;
  UInt8 peFlags;
} HeatWaveAVIPaletteEntry;

/** Palette change chunk. **/
typedef struct
{
    UInt8 bFirstEntry;
    UInt8 bNumEntries;
    UInt16 wFlags;
    HeatWaveAVIPaletteEntry peNew[1];
} HeatWaveAVIPaletteChange;

/** VIDS stream information (bitmap). **/
typedef struct 
{
  UInt biSize;
  UInt biWidth;
  UInt biHeight;
  UInt16 biPlanes;
  UInt16 biBitCount;
  UInt biCompression;
  UInt biSizeImage;
  UInt biXPelsPerMeter;
  UInt biYPelsPerMeter;
  UInt biClrUsed;
  UInt biClrImportant;
} HeatWaveAVIBitmapHeader;

/** AUDS stream information (wave audio). **/
typedef struct { 
  UInt16 wFormatTag; 
  UInt16 nChannels; 
  UInt nSamplesPerSec; 
  UInt nAvgBytesPerSec; 
  UInt16 nBlockAlign; 
  UInt16 wBitsPerSample; 
  UInt16 cbSize;
} HeatWaveAVIWaveHeader;

#endif /* _INC_AVIFMT */

