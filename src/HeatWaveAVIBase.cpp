/****************************************************************************/
/**
 ** @file HeatWaveAVIBase.cpp
 ** @brief Contains class HeatWaveAVIBase's function definitions plus 
 **        constants.
 ** @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 **
 **/

#include "HeatWaveAVIBase.hpp"

/*@{*/
/** List name */
const char LIST_RIFF[] = "RIFF";
const char LIST_LIST[] = "LIST";
const char * LIST_NAMES[] = {
  LIST_RIFF,LIST_LIST,NULL
};
/*@}*/

/*@{*/
/** List type */
const char LIST_AVI_[] = "AVI ";
const char LIST_STRL[] = "strl";
const char LIST_HDRL[] = "hdrl";
const char LIST_MOVI[] = "movi";
const char LIST_REC_[] = "rec ";
const char * LIST_TYPES[] = {
  LIST_AVI_,LIST_STRL,LIST_HDRL,LIST_MOVI,LIST_REC_,NULL
};
/*@}*/

/*@{*/
/** Chunk name. */
const char CHUNK_AVIH[] = "avih";
const char CHUNK_STRH[] = "strh";
const char CHUNK_STRF[] = "strf";
const char CHUNK_STRD[] = "strd";
const char CHUNK_STRN[] = "strn";
const char CHUNK_IDX1[] = "idx1";
const char CHUNK_JUNK[] = "JUNK";
const char CHUNK_DB[] = "**db";
const char CHUNK_DC[] = "**dc";
const char CHUNK_PC[] = "**pc";
const char CHUNK_WB[] = "**wb";
const char * CHUNK_NAMES[] = {
  CHUNK_AVIH,CHUNK_STRH,CHUNK_STRF,CHUNK_STRD,CHUNK_STRN,CHUNK_IDX1,
  CHUNK_JUNK,CHUNK_DB,CHUNK_DC,CHUNK_PC,CHUNK_WB,NULL
};
/*@}*/

/*@{*/
/** Stream type. */
const char STREAM_VIDS[] = "vids";
const char STREAM_AUDS[] = "auds";
const char STREAM_MIDS[] = "mids";
const char STREAM_TXTS[] = "txts";
const char * STREAM_TYPES[] = {
  "vids","auds","mids","txts",NULL
};
/*@}*/

/**
 * 
 * A array of coding types.
 *
 **/

const StructCodingType CODEING_TYPES[] = {
  {VidIYUV,SpcYUV,3,{ClrY,ClrU,ClrV},{1,2,2},{1,2,2}},
  {VidGREY,SpcGrey,1,{ClrY,ClrUnknown,ClrUnknown},{1,1,1},{1,1,1}},
};

HeatWaveAVIBase::HeatWaveAVIBase()
{
  ASSERT ( VidTotal == sizeof(CODEING_TYPES)/sizeof(StructCodingType));
  memset(this,0,sizeof(HeatWaveAVIBase));
  m_desMem = True;
}

HeatWaveAVIBase::~HeatWaveAVIBase()
{
  DoDestroy();
}

const char * 
HeatWaveAVIBase::GetFileName() const
{
  return m_fileName;
}

void 
HeatWaveAVIBase::SetFileName(char * str)
{
  WARN_IF(m_fileName != NULL);
  m_fileName = str;
}
  
void 
HeatWaveAVIBase::CpyFileName(const char * str)
{
  if ( m_fileName ){
    delete [] m_fileName;
    m_fileName = NULL;
  }
  m_fileName = new char[strlen(str)+1];
  LEAVEONNULL(m_fileName);
  strcpy(m_fileName,str);
}

FILE *
HeatWaveAVIBase::GetFileHandle() const
{
  return m_fileHandle;
}

void 
HeatWaveAVIBase::SetFileHandle(FILE * hnd)
{
  WARN_IF(m_fileHandle!=NULL);
  m_fileHandle = hnd;
}
  
HeatWaveVideo *
HeatWaveAVIBase::GetVideoPtr() const
{
  return m_video;
}
  
void 
HeatWaveAVIBase::SetVideoPtr(HeatWaveVideo * ptr)
{
  WARN_IF(m_video!=NULL);
  m_video = ptr;
}

void
HeatWaveAVIBase::CpyVideoPtr(const HeatWaveVideo * ptr)
{
  WARN_IF(m_video!=NULL);
  ASSERT(ptr != NULL);
  m_video = new HeatWaveVideo(*ptr);
  LEAVEONNULL(m_video);
}

HeatWaveVideo &
HeatWaveAVIBase::GetVideo() const
{
  ASSERT(m_video!=NULL);
  return (*m_video);
}
  
void 
HeatWaveAVIBase::SetDesMem(Bool des)
{
  m_desMem = des;
}

Bool
HeatWaveAVIBase::GetDesMem() const
{
  return  m_desMem;
}

HeatWaveAVIHeader * 
HeatWaveAVIBase::GetHeatWaveAVIHeader() const
{
  return m_avih;
}

void
HeatWaveAVIBase::SetHeatWaveAVIHeader(HeatWaveAVIHeader * hdr)
{
  ASSERT(hdr != NULL);
  m_avih = hdr;
}

void
HeatWaveAVIBase::CpyHeatWaveAVIHeader(const HeatWaveAVIHeader * hdr)
{
  WARN_IF(m_avih != NULL);
  ASSERT(hdr != NULL);
  m_avih = new HeatWaveAVIHeader;
  LEAVEONNULL(m_avih);
  memmove(m_avih,hdr,sizeof(HeatWaveAVIHeader));
}

HeatWaveAVIStreamHeader * 
HeatWaveAVIBase::GetVideoHeader() const
{
  return m_vids;
}

void 
HeatWaveAVIBase::SetVideoHeader(HeatWaveAVIStreamHeader * hdr)
{
  WARN_IF(m_vids != NULL);
  m_vids = hdr;
}

void 
HeatWaveAVIBase::CpyVideoHeader(const HeatWaveAVIStreamHeader * hdr)
{
  WARN_IF(m_vids != NULL);
  ASSERT(hdr != NULL);
  m_vids = new HeatWaveAVIStreamHeader;
  LEAVEONNULL(m_vids);
  memmove(m_vids,hdr,sizeof(HeatWaveAVIStreamHeader));
}

HeatWaveAVIBitmapHeader * 
HeatWaveAVIBase::GetVideoInformation() const
{
  return m_vidsInfo;
}

void 
HeatWaveAVIBase::SetVideoInformation(HeatWaveAVIBitmapHeader * inf)
{
  WARN_IF(m_vidsInfo != NULL);
  m_vidsInfo = inf;
}

void 
HeatWaveAVIBase::CpyVideoInformation(const HeatWaveAVIBitmapHeader * inf)
{
  WARN_IF(m_vidsInfo != NULL);
  ASSERT(inf != NULL);
  m_vidsInfo = new HeatWaveAVIBitmapHeader;
  LEAVEONNULL(m_vidsInfo);
  memmove(m_vidsInfo,inf,sizeof(HeatWaveAVIBitmapHeader));
}

char * 
HeatWaveAVIBase::GetVideoName() const
{
  return m_vidsName;
}
   
void 
HeatWaveAVIBase::CpyVideoName(const char * str)
{
  if ( m_vidsName ){
    delete [] m_vidsName;
    m_vidsName = NULL;
  }
  m_vidsName = new char[strlen(str)+1];
  LEAVEONNULL(m_vidsName);
  strcpy(m_vidsName,str);
}
  
void 
HeatWaveAVIBase::SetVideoName(char * str)
{
  WARN_IF(m_vidsName != NULL);
  m_vidsName = str;
}

char * 
HeatWaveAVIBase::GetVideoCodingInfo(SInt & size) const
{
  size = m_vidsCodingInfoSize;
  return m_vidsCodingInfo;
}

void 
HeatWaveAVIBase::CpyVideoCodingInfo(const char * ptr, SInt size)
{
  if ( m_vidsCodingInfo ){
    delete [] m_vidsCodingInfo;
    m_vidsCodingInfo = NULL;
  }
  m_vidsCodingInfo = new char[size];
  LEAVEONNULL(m_vidsCodingInfo);
  strncpy(m_vidsCodingInfo,ptr,size);
  m_vidsCodingInfoSize = size;
}

void 
HeatWaveAVIBase::SetVideoCodingInfo(char * ptr, SInt size)
{
  WARN_IF(m_vidsCodingInfo != NULL);
  m_vidsCodingInfo = ptr;
  m_vidsCodingInfoSize = size;
}

HeatWaveAVIStreamHeader * 
HeatWaveAVIBase::GetAudioHeader() const
{
  return m_auds;
}

void 
HeatWaveAVIBase::SetAudioHeader(HeatWaveAVIStreamHeader * hdr)
{
  WARN_IF(m_auds != NULL);
  m_auds = hdr;
}

void 
HeatWaveAVIBase::CpyAudioHeader(const HeatWaveAVIStreamHeader * hdr)
{
  WARN_IF(m_auds != NULL);
  ASSERT(hdr != NULL);
  m_auds = new HeatWaveAVIStreamHeader;
  LEAVEONNULL(m_auds);
  memmove(m_auds,hdr,sizeof(HeatWaveAVIStreamHeader));
}

HeatWaveAVIWaveHeader * 
HeatWaveAVIBase::GetAudioInformation() const
{
  return m_audsInfo;
}

void 
HeatWaveAVIBase::SetAudioInformation(HeatWaveAVIWaveHeader * inf)
{
  WARN_IF(m_audsInfo != NULL);
  m_audsInfo = inf;
}

void 
HeatWaveAVIBase::CpyAudioInformation(const HeatWaveAVIWaveHeader * inf)
{
  WARN_IF(m_audsInfo != NULL);
  ASSERT(inf != NULL);
  m_audsInfo = new HeatWaveAVIWaveHeader;
  LEAVEONNULL(m_audsInfo);
  memmove(m_audsInfo,inf,sizeof(HeatWaveAVIWaveHeader));
}

char * 
HeatWaveAVIBase::GetAudioName() const
{
  return m_audsName;
}
   
void 
HeatWaveAVIBase::CpyAudioName(const char * str)
{
  if ( m_audsName ){
    delete [] m_audsName;
    m_audsName = NULL;
  }
  m_audsName = new char[strlen(str)+1];
  LEAVEONNULL(m_audsName);
  strcpy(m_audsName,str);
}
  
void 
HeatWaveAVIBase::SetAudioName(char * str)
{
  WARN_IF(m_audsName != NULL);
  m_audsName = str;
}

char * 
HeatWaveAVIBase::GetAudioCodingInfo(SInt & size) const
{
  size = m_audsCodingInfoSize;
  return m_audsCodingInfo;
}

void 
HeatWaveAVIBase::CpyAudioCodingInfo(const char * ptr, SInt size)
{
  if ( m_audsCodingInfo ){
    delete [] m_audsCodingInfo;
    m_audsCodingInfo = NULL;
  }
  m_audsCodingInfo = new char[size];
  LEAVEONNULL(m_audsCodingInfo);
  strncpy(m_audsCodingInfo,ptr,size);
  m_audsCodingInfoSize = size;
}

void 
HeatWaveAVIBase::SetAudioCodingInfo(char * ptr, SInt size)
{
  WARN_IF(m_audsCodingInfo != NULL);
  m_audsCodingInfo = ptr;
  m_audsCodingInfoSize = size;
}

void
HeatWaveAVIBase::DoDestroy()
{
  if ( m_desMem ){
    delete [] m_fileName;
    if ( m_fileHandle ){
      if ( fclose ( m_fileHandle )== EOF ){
        WARN_IF ( False ); // Not drastic but take note 
                           // if happening regularly!
      }
    }
    delete m_video;
    delete m_avih;
    delete m_vids;
    delete m_vidsInfo;
    delete m_vidsName;
    delete m_vidsCodingInfo;
    delete m_auds;
    delete m_audsInfo;
    delete m_audsName;
    delete m_audsCodingInfo;
    delete m_error;
    memset(this,0,sizeof(HeatWaveAVIBase));
  }
}

char * 
HeatWaveAVIBase::GetError() const
{
  return m_error;
}

void 
HeatWaveAVIBase::CpyError(const char * str)
{
  if ( m_error ){
    delete [] m_error;
    m_error = NULL;
  }
  m_error = new char[strlen(str)+1];
  LEAVEONNULL(m_error);
  strcpy(m_error,str);
}

void 
HeatWaveAVIBase::SetError(char * str)
{
  if ( m_error ){
    delete [] m_error;
    m_error = NULL;
  }
  m_error = str;
}
