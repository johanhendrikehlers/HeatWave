/****************************************************************************/
/**
 ** @file HeatWaveAVIReader.cpp
 ** @brief Contains class HeatWaveAVIReader's function definitions.
 ** @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 **
 **/

#include "HeatWaveAVIReader.hpp"

/** Error message to use on general I/O failure. **/
const char * IOERROR = "general I/O failure has occured";

HeatWaveAVIReader::HeatWaveAVIReader()
{
  memset((char*)this,0,sizeof(HeatWaveAVIReader));
  m_desMem = True;
  m_coder.m_type = VidUnknown;
}
  
HeatWaveAVIReader::HeatWaveAVIReader(const char * str, HeatWaveVideo * vid)
{
  memset((char*)this,0,sizeof(HeatWaveAVIReader));
  m_desMem = True;
  m_coder.m_type = VidUnknown;
  CpyFileName(str);
  SetVideoPtr(vid);
}

HeatWaveAVIReader::HeatWaveAVIReader(const HeatWaveAVIReader & oth)
{
  DoCopy(oth);
}
  
HeatWaveAVIReader::~HeatWaveAVIReader()
{
  DoDestroy();
}

Bool 
HeatWaveAVIReader::OpenFile(const char * str)
{
  if ( m_fileHandle ){
    const char * format = "file all ready open : %s";
    char * error = new char[strlen(m_fileName)+strlen(format)+10];
    sprintf(error,format,m_fileName);
    SetError(error);
    goto problem;
  }
  else{
    if ( str ){
      CpyFileName(str);
    }
    if ( !m_fileName ){
      CpyError("no file name given");
      goto problem;
    }
    m_fileHandle = fopen(m_fileName,"rb");
    if ( !m_fileHandle ){
      const char * format = "unable to open file : %s";
      char * error = new char[strlen(m_fileName)+strlen(format)+10];
      sprintf(error,format,m_fileName);
      SetError(error);
    }
  }
  
  return True;
 problem:
  return False;
}

Bool 
HeatWaveAVIReader::AnalyseFile()
{
  SInt max_chunk_size;
  if ( m_fileHandle == NULL ){
    CpyError("no file open to analyse");
    goto error;
  }
  max_chunk_size = GetFileSize();
  if ( max_chunk_size < 0 ){
    CpyError("error getting file size");
    goto error;
  }
  if (!AnalyseFileInternal(max_chunk_size)){
    goto error;
  }
  if (!LoadMetaData()){
    goto error;
  }
  if (!IsDecodable()){
    goto error;
  }
  
  return True;
 error:
  return False;
}

HeatWaveVideo *
HeatWaveAVIReader::LoadVideo()
{
  if ( (m_fileHandle == NULL) || (m_coder.m_type == VidUnknown) ){
    WARN_IF(True); // "forgot to check returned values" mistake
    return NULL;
  }

  HeatWaveVideo * ret = NULL;
  HeatWaveImage ** arr = new HeatWaveImage*[m_vids->dwLength];
  LEAVEONNULL(arr);
  memset((char*)arr,'\0',sizeof(HeatWaveImage*)*(m_vids->dwLength));

  for ( SInt i = 0 ; i < (SInt)(m_vids->dwLength) ; ++i ){
    arr[i] = LoadFrame(i);
    if ( arr[i] == NULL ){
      goto error;
    }
  }

  ret = new HeatWaveVideo(m_vids->rcFrame.right,m_vids->rcFrame.bottom,
			  m_coder.m_space,m_coder.m_hSampling,
			  m_coder.m_vSampling,m_vids->dwLength,
			  arr,True,False);
  LEAVEONNULL(ret);
  return ret;

error:
  for ( SInt i = 0 ; i < (SInt)(m_vids->dwLength) ; ++i ){
    delete [] arr[i];
  }
  delete arr;
  return NULL;
}

HeatWaveImage * 
HeatWaveAVIReader::LoadFrame(SInt num)
{
  if ( (m_fileHandle == NULL) || (m_coder.m_type == VidUnknown) ){
    WARN_IF(True); // "forgot to check returned values" mistake
    return NULL;
  }
  if ( (num < 0) || (num >= (SInt)(m_vids->dwLength)) ){
    char error[100];
    sprintf(error,"frame number %d does not exist! try range 0 to %d",num,
            m_vids->dwLength);
    CpyError(error);
    return NULL;
  }
  char frame_id[FOUR_CC+1];
  sprintf(frame_id,"%02ddb",m_vidsPriority);
  DataChunk * img_data = LocateChunk(frame_id,num);
  if ( img_data == NULL ){
    CpyError("file is missing frames!");
    return NULL;
  }
  if ( fseek ( m_fileHandle, img_data->m_offset+CHUNK_HEADER, SEEK_SET )){
    CpyError(IOERROR);
    return NULL;
  }

  return GetFrame(img_data->m_size);
}

HeatWaveImage * 
HeatWaveAVIReader::GetFrame(SInt actSize)
{
  HeatWaveImage * ret = NULL;
  HeatWaveComponent ** cmp = new HeatWaveComponent*[m_coder.m_colors];
  LEAVEONNULL(cmp);
  for ( SInt c = 0 ; c < m_coder.m_colors ; ++c ){
    SInt width = m_vidsInfo->biWidth;
    SInt height = m_vidsInfo->biHeight;
    SInt hStep = m_coder.m_hSampling[c];
    SInt vStep = m_coder.m_vSampling[c];
    if ( (width % hStep) || (height % vStep) ){
      CpyError("unable to handle video image size!");
      goto error;
    }
    width = width / hStep;
    height = height / vStep;
    SInt size = height * width;
    actSize -= size;
    if ( actSize < 0 ){
      CpyError("frame chunk is missing information!");
      goto error;
    }
    cmp[c] = new HeatWaveComponent(0,0,hStep,vStep,width,height,False,8,
                                 m_coder.m_order[c]);
    LEAVEONNULL(cmp[c]);
    Smpl * vect = cmp[c]->GetData();
    SInt in;
    for ( SInt i = 0 ; i < size ; ++i ){
      if ( (in = fgetc(m_fileHandle)) == EOF ){
        CpyError(IOERROR);
        goto error;
      }
      else{
        vect[i] = (Smpl)in;
      }
    }
  }
  ret = new HeatWaveImage(0,0,m_vidsInfo->biWidth,m_vidsInfo->biHeight,
                        m_coder.m_space,m_coder.m_colors,cmp,True,False);
  LEAVEONNULL(ret);
  return ret;
  
 error:
  delete [] cmp;
  return NULL;
}

Bool 
HeatWaveAVIReader::LoadAudio()
{
  CpyError("audio feuture not yet implemented");
  return False;
}

Bool 
HeatWaveAVIReader::CloseFile()
{
  if ( m_fileHandle == NULL ){
    CpyError("no file open");
    goto error;
  }
  if ( fclose ( m_fileHandle )== EOF ){
    CpyError("failed to close file");
    goto error;
  }
  m_fileHandle = NULL;
  
  return True;
 error:
  return False;
}

HeatWaveAVIReader *
HeatWaveAVIReader::GetClone() const
{
  return NULL;
}

Bool 
HeatWaveAVIReader::AnalyseFileInternal(SInt file_size)
{
  if ( fseek ( m_fileHandle, 0 , SEEK_SET )){
    CpyError(IOERROR);
    goto problem;
  }
  if ( file_size < LIST_HEADER ){
    CpyError("file is to small, not suitable for usage");
    goto problem;
  }
  else if ( !GetCharacters(LIST_HEADER,(char*)&m_chunk)){
    CpyError("file has errors, not suitable for usage");
    goto problem;
  }
  if (( strncmp(m_chunk.m_name,LIST_NAMES[0],FOUR_CC) != 0 ) ||
      ( strncmp(m_chunk.m_type,LIST_TYPES[0],FOUR_CC) != 0 ) ||
      ( m_chunk.m_size > (file_size-CHUNK_HEADER))){
    CpyError("file is not in RIFF format, i.e. not a AVI file");
    goto problem;
  }
  else {
    if ( m_chunk.m_sub ){
      delete m_chunk.m_sub;
      m_chunk.m_sub = NULL;
    }
    m_chunk.m_list = True;
    m_chunk.m_sub = new DataChunk();
    LEAVEONNULL(m_chunk.m_sub);
    if (!AnalyseChunk(m_chunk.m_sub,m_chunk.m_size)){
      goto problem;
    }
  }
  
  return True;
 problem:
  return False;
}

Bool 
HeatWaveAVIReader::AnalyseChunk(DataChunk * chk, SInt max)
{
  if ( max < CHUNK_HEADER ){
    goto structure_problem;
  }
  chk->m_offset = ftell(m_fileHandle);
  if ( chk->m_offset < 0 ){
    goto io_problem;
  }
  if ( !GetCharacters(CHUNK_HEADER,(char*)chk)){
    goto io_problem;
  }
  if ( SeeIfInList(chk->m_name,LIST_NAMES,FOUR_CC) ){
    chk->m_list = True;
    if ( !GetCharacters(FOUR_CC,chk->m_type) ){
      goto io_problem;
    }
    if ( !SeeIfInList(chk->m_type,LIST_TYPES,FOUR_CC) ){
      goto structure_problem;
    }
    if ( (chk->m_size-FOUR_CC) >= CHUNK_HEADER){
      chk->m_sub = new DataChunk();
      LEAVEONNULL(chk->m_sub);
      if (!AnalyseChunk(chk->m_sub,(chk->m_size-FOUR_CC))){
        goto structure_problem;
      }
    }
    if ( (max-((chk->m_size)+CHUNK_HEADER)) >= CHUNK_HEADER){
      if (fseek(m_fileHandle,(chk->m_offset)+CHUNK_HEADER+(chk->m_size),
                SEEK_SET)){
        goto io_problem;
      }
      chk->m_nxt = new DataChunk();
      LEAVEONNULL(chk->m_sub);
      if (!AnalyseChunk((chk->m_nxt),(max-((chk->m_size)+CHUNK_HEADER)))){
        goto structure_problem;
      }
    }
    return True;
  }
  else if ( SeeIfInList(chk->m_name,CHUNK_NAMES,FOUR_CC) ){
    chk->m_list = False;
    if ( (max-((chk->m_size)+CHUNK_HEADER)) >= CHUNK_HEADER){
      if (fseek(m_fileHandle,(chk->m_offset)+CHUNK_HEADER+(chk->m_size),
                SEEK_SET)){
        return False;
      }
      chk->m_nxt = new DataChunk();
      LEAVEONNULL(chk->m_nxt);
      if (!AnalyseChunk((chk->m_nxt),(max-((chk->m_size)+CHUNK_HEADER)))){
        goto structure_problem;
      }
    }
    return True;
  }
  else{
    goto structure_problem;
  }
  
  return True;
 io_problem:
  CpyError(IOERROR);
  return False;
 structure_problem:
  CpyError("file structure error, contact author");
  return False;
}

Bool 
HeatWaveAVIReader::LoadMetaData()
{
  DataChunk * avih = LocateChunk(CHUNK_AVIH,0);
  DataChunk * idx1;
  if ( avih == NULL ){
    CpyError("unable to locate a avi header");
    goto error;
  }
  
  if ( avih->m_list ){
    CpyError("avi file used avi header as a list, this is a error!");
    goto error;
  }
  
  m_avih = new HeatWaveAVIHeader;
  LEAVEONNULL(m_avih);
  LoadChunk(*avih,CHUNK_HEADER,sizeof(HeatWaveAVIHeader),(char*)m_avih);

  if ( m_avih->dwStreams < 1 ){
    CpyError("avi file has no data streams");
    goto error;
  }

  if ( (m_avih->dwFlags & AVIF_HASINDEX) ){
    idx1 = LocateChunk(CHUNK_IDX1,0);
    if ( idx1 == NULL ){
      CpyError("avi file has no index but AVIF_HASINDEX flag is set");
      goto error;
    }
  }
  
  for ( SInt i = 0 ; i < (SInt)(m_avih->dwStreams) ; ++i ){
    if ( !LoadStream(i) ){
      goto error;
    }
  }
  
  return True;
 error:
  return False;
}

Bool 
HeatWaveAVIReader::LoadStream(SInt no)
{
  HeatWaveAVIStreamHeader * stream_hdr;
  DataChunk * strh;
  DataChunk * strf;
  DataChunk * strn;
  DataChunk * strd;
  DataChunk * strl;
  strl = LocateChunk(LIST_STRL,no);
  if ( strl == NULL ){
    CpyError("file has a missing stream");
    goto error;
  }
  strh = LocateChunk(CHUNK_STRH,0,strl);
  strf = LocateChunk(CHUNK_STRF,0,strl);
  strn = LocateChunk(CHUNK_STRN,0,strl); // currently being ignored
  strd = LocateChunk(CHUNK_STRD,0,strl); // currently being ignored
  if ((strh == 0) || (strf == 0)){
    CpyError("stream is missing header information");
    goto error;
  }
  stream_hdr = new HeatWaveAVIStreamHeader;
  LEAVEONNULL(stream_hdr);
  LoadChunk(*strh, CHUNK_HEADER, sizeof(HeatWaveAVIStreamHeader),
            (char*)stream_hdr);
  if ( strncmp(stream_hdr->fccType,STREAM_VIDS,FOUR_CC) == 0 ){
    if ( m_vids || m_vidsInfo ){
      CpyError("second video stream found");
      goto error;
    }
    m_vids = stream_hdr;
    m_vidsInfo = new HeatWaveAVIBitmapHeader;
    LEAVEONNULL(m_vidsInfo);
    LoadChunk(*strf,CHUNK_HEADER,sizeof(HeatWaveAVIBitmapHeader),
              (char*)m_vidsInfo);
    if ( strn ){
      if ( strn->m_size > 0 ){
        m_vidsName = new char[(strn->m_size)+1];
        LEAVEONNULL(m_vidsName);
        LoadChunk(*strn,CHUNK_HEADER,strn->m_size,
                  m_vidsName);
        m_vidsName[strn->m_size] = '\0';
      }
    }
    if ( strd ){
      if ( strd->m_size > 0 ){
        m_vidsCodingInfo = new char[(strd->m_size)];
        LEAVEONNULL(m_vidsCodingInfo);
        LoadChunk(*strd,CHUNK_HEADER,strd->m_size,
                  m_vidsCodingInfo);
        m_vidsCodingInfoSize = strd->m_size;
      }
    }
    m_vidsPriority = no;
  }
  else if ( strncmp(stream_hdr->fccType,STREAM_AUDS,FOUR_CC) == 0 ){
    if ( m_auds || m_audsInfo ){
      CpyError("second audio stream found");
      goto error;
    }
    m_auds = stream_hdr;
    m_audsInfo = new HeatWaveAVIWaveHeader;
    LEAVEONNULL(m_vidsInfo);
    LoadChunk(*strf,CHUNK_HEADER,sizeof(HeatWaveAVIWaveHeader),
              (char*)m_audsInfo);
    if ( strn ){
      if ( strn->m_size > 0 ){
        m_audsName = new char[(strn->m_size)+1];
        LEAVEONNULL(m_audsName);
        LoadChunk(*strn,CHUNK_HEADER,strn->m_size,
                  m_audsName);
        m_audsName[strn->m_size] = '\0';
      }
    }
    if ( strd ){
      if ( strd->m_size > 0 ){
        m_audsCodingInfo = new char[(strd->m_size)];
        LEAVEONNULL(m_audsCodingInfo);
        LoadChunk(*strd,CHUNK_HEADER,strd->m_size,
                  m_audsCodingInfo);
        m_audsCodingInfoSize = strd->m_size;
      }
    }
    m_audsPriority = no;
  }
  else{
    char tmp[100];
    char fcc[] = {"____"};
    strncpy(fcc,stream_hdr->fccType,FOUR_CC);
    fcc[FOUR_CC] = '\0';
    sprintf(tmp,"cant handle fcc type : %s",fcc);
    CpyError(tmp);
    goto error;
  }
  
  return True;
 error:
  return False;
}

SInt 
HeatWaveAVIReader::LoadChunk(const DataChunk & chk, SInt hdr, SInt len, 
                           char * ptr)
{
  for ( SInt i = 0; i < len; ++i ){
    ptr[i] = 0;
  }
  ASSERT(hdr>=0);
  if (fseek(m_fileHandle,(chk.m_offset)+(hdr),SEEK_SET)){
    return -1;
  }
  SInt min_size = chk.m_size;
  min_size = HeatWaveMath::Min(len,min_size);
  if (!GetCharacters(min_size,ptr)){
    return -1;
  }
  return min_size;
}

// unusaul for avi files to have more than 4 depth levels
// also for stack size reasons we dont use recursive functions
// (for Windows CE and Symbian)
HeatWaveAVIReader::DataChunk * 
HeatWaveAVIReader::LocateChunk(const char * str, SInt hop, DataChunk * anl)
{
  ASSERT ( hop >= 0 );
  ASSERT ( strlen(str) == FOUR_CC );
  const SInt max_depth = 10; 
  DataChunk * split[max_depth];
  SInt current_level = 0;
  DataChunk * current;
  if ( anl ){
    current = anl;
  }
  else{
    current = m_chunk.m_sub;
  }
  while ( (current!=NULL) ){
    if (( strncmp(current->m_name,str,FOUR_CC) == 0 ) ||
        ( strncmp(current->m_type,str,FOUR_CC) == 0 ) ){
      if ( hop == 0 ){
        return current;
      }
      else{
        --hop;
      }
    }
    if ( current->m_sub ){
      split[current_level] = current;
      ++current_level;
      if ( current_level >= max_depth ){
        CpyError("avi file has to many depths! Possibly corrupted."
                 "(HeatWave solution increase max_depth in "__FILE__
                 " function LocatChunk()");
        return NULL;
      }
      current = current->m_sub;
      continue;
    }
    if ( current->m_nxt ){
      current = current->m_nxt;
      continue;
    }
    current = NULL;
    for ( SInt i = (current_level-1); i >= 0 ; --i ){
      if (split[i]->m_nxt){
        current = split[i]->m_nxt;
        current_level = i;
        break;
      }
    }
  }
  return NULL;
}

Bool HeatWaveAVIReader::IsDecodable()
{

  ASSERT(m_vids != NULL);
  char tmp[FOUR_CC+1];
  strncpy(tmp,m_vids->fccHandler,FOUR_CC);
  tmp[FOUR_CC] = '\0';
  m_coder.m_type = VideoTypeEnum(tmp);
  if ( m_coder.m_type == VidUnknown ){
    char error[100];
    sprintf(error,"unknown video stream type : %s",tmp);
    CpyError(error);
    return False;
  }
  else{
    SInt offset = (SInt) m_coder.m_type;
    ASSERT ( m_coder.m_type == CODEING_TYPES[offset].m_type );
    m_coder.m_space = CODEING_TYPES[offset].m_space;
    m_coder.m_colors = CODEING_TYPES[offset].m_colors;
    for ( SInt i = 0 ; i < MAXCOLORSINANYSPACE ; ++i ){
      m_coder.m_order[i] = CODEING_TYPES[offset].m_order[i];
      m_coder.m_hSampling[i] = CODEING_TYPES[offset].m_hSampling[i];
      m_coder.m_vSampling[i] = CODEING_TYPES[offset].m_vSampling[i];
    }
  }
  return True;
}

Bool 
HeatWaveAVIReader::GetCharacters(SInt len, char * str)
{
  SInt in;
  for ( SInt i = 0 ; i < len ; ++i ){
    if ( (in = fgetc(m_fileHandle)) == EOF ){
      return False;
    }
    else{
      str[i] = (char)in;
    }
  }
  return True;
}

SInt 
HeatWaveAVIReader::GetFileSize() const
{
  fpos_t current;
  SInt size = -1;
  ASSERT(m_fileHandle != NULL);
  if ( fgetpos(m_fileHandle, &current) ){
    return -1;
  }
  if (fseek(m_fileHandle, 0, SEEK_END)){
    return -1;
  }
  size = ftell(m_fileHandle);
  if ( fsetpos(m_fileHandle, &current) ){
    return -1;
  }
  return size;
}

HeatWaveAVIReader& 
HeatWaveAVIReader::operator=(const HeatWaveAVIReader& rhs)
{
  return (*this);
}
  
Bool 
HeatWaveAVIReader::operator==(const HeatWaveAVIReader & rhs ) const
{
  return False;
}

void 
HeatWaveAVIReader::DoCopy(const HeatWaveAVIReader & rhs)
{
  DoDestroy();
  SetFileName(rhs.m_fileName);
  if ( rhs.m_video ){
    m_video = rhs.m_video->GetClone();
  }
}
  
Bool 
HeatWaveAVIReader::SeeIfInList(const char * str,const char ** lst,
                             SInt len) const
{
  ASSERT ( str != NULL );
  ASSERT ( lst != NULL);
  ASSERT ( len >= 0 );
  if ( strlen(str) < (UInt)len ){
    WARN_IF ( False );
    return False;
  }
  SInt i = 0;
  while ( lst[i] != NULL ){
    if ( strlen(lst[i]) < (UInt)len ){
      WARN_IF ( False );
      return False;
    }
    ++i;
  }
  
  i = 0;
  while ( lst[i] != NULL ){
    for ( SInt c = 0 ; c < len ; ++c ){
      if (( lst[i][c] == '*' ) ||
          ( lst[i][c] == str[c] )){
        if ( (c+1) == len ){
          return True;
        }
      }
      else{
        ++i;
        break; // out of for loop
      }
    }
  }
  return False;
}
