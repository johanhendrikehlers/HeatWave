/****************************************************************************/
/**
 ** @File HeatWaveAVIReader.hpp
 ** @brief Contains the HeatWaveAVIReader class definition.
 ** @author Johan Hendrik Ehlers <https://github.com/johanhendrikehlers>
 **
 **/

#ifndef __HEATWAVEAVIREADER_HPP__
#define __HEATWAVEAVIREADER_HPP__

#include "HeatWaveEnums.hpp"
#include "HeatWaveComponent.hpp"
#include "HeatWaveImage.hpp"
#include "HeatWaveVideo.hpp"
#include "HeatWaveAVIStructs.hpp"
#include "HeatWaveAVIBase.hpp"

/****************************************************************************/
/**
 ** A AVI file handling class. This class works by going thoroug the file and
 ** then constucting a tree structure which is used later to locate specific
 ** data chunks. All Avi files are made up of a tree like system consisting of
 ** chunks and lists.
 ** Order of obtaining a video or a image is to:
 ** <ol>
 ** <li> Open the file (CHECK RETURNED VALUE!)</li>
 ** <li> Analyse the file (CHECK RETURNED VALUE!)</li>
 ** <li> Get the video or image! </li>
 ** <li> Close the file </li>
 ** </ol>
 **/

class HeatWaveAVIReader : public HeatWaveAVIBase
{
public:
  
  /**
   *
   * Default constructor.
   *
   **/
  
  HeatWaveAVIReader();

  /**
   *
   * Parameterised constructor.
   *
   * @param str The name of the file to open.
   * @param vid The pointer to the video (HeatWaveVideo) that will recieve read
   * data or contain the written data. If NULL then one will be created 
   * dynamicly.
   *
   **/
  
  HeatWaveAVIReader(const char * str, HeatWaveVideo * vid);

  /**
   *
   * Copy constuctor.
   *
   * @param oth The other HeatWaveAVIReader to copy.
   *
   **/

  HeatWaveAVIReader(const HeatWaveAVIReader & oth);
  
  /**
   *
   * Destructor.
   *
   **/
  
  virtual ~HeatWaveAVIReader();

  /**
   *
   * Open a AVI file.
   *
   * @param str The name of the file. If NULL previous name is used.
   * @return True if file is opened, False if not.
   *
   **/

  Bool OpenFile(const char * str = NULL);

  /**
   *
   * Analyse a open file. 
   * Extract all usefull headers and stream information.
   *
   * @return True if analyses succeeded, False if not.
   *
   **/

  Bool AnalyseFile();

  /**
   *
   * Load the video. (Loads entire video onto internal buffer, very costly!)
   *
   * @return True if video loaded ok, False if not.
   *
   **/

  HeatWaveVideo * LoadVideo();

  /**
   *
   * Return a single image.
   *
   * @param num The image number.
   * @return 
   *
   **/
  
  HeatWaveImage * LoadFrame(SInt num);

  /**
   *
   * Load the audio. (Loads entire audio track onto internal buffer.)
   *
   * @return True if loaded ok, False if not.
   * @todo Me! Does nothing for now!
   *
   **/

  Bool LoadAudio();

  /**
   *
   * Close the file.
   *
   * @return True if closed ok, False otherwise.
   *
   **/

  Bool CloseFile();

  /**
   *
   * Create a clone of this reader.
   *
   **/
  
  HeatWaveAVIReader * GetClone() const;
  
  /**
   *
   * Assignment operator.
   *
   * @param rhs The other reader to copy data and state from.
   * @return A reference to this object.
   *
   **/
  
  HeatWaveAVIReader& operator=(const HeatWaveAVIReader& rhs);
  
  /**
   *
   * Equality comparison.
   *
   * @param rhs The other reader to compare against.
   * @return A reference to this object.
   *
   **/
  
  Bool operator==(const HeatWaveAVIReader & rhs ) const;
  
protected:

  /* see later */
  struct DataChunk;

  /** 
   *
   * Analyse the underlying file.
   *
   * @param file_size The file size.
   * @return True if the file is an ok avi file, false otherwise.
   *
   **/

  Bool AnalyseFileInternal(SInt file_size);

  /**
   *
   * Analyse a certain chunk. Recursive function called from 
   * AnalyseFileInternal().
   *
   * @param chk The chunk to analyse.
   * @param max The maximum allowed size in bytes of chunk.
   *
   **/

  Bool AnalyseChunk(DataChunk * chk, SInt max);

  /**
   *
   * Return a pointer to a specific chunk. Do not delete it.
   *
   * @param str The chunk name or list type.
   * @param hop The amount of time to skip.
   * @param anl The datachunk to search in.
   * @return Datachunk if found else NULL.
   *
   **/

  DataChunk * LocateChunk(const char * str, SInt hop, DataChunk * anl = NULL);

  /**
   *
   * Load the meta data. (File header plus stream headers and information.)
   *
   * @return True if no error, False otherwise.
   *
   **/

  Bool LoadMetaData();

  /**
   *
   * Is the file decodable.
   *
   * @return True if the file is decodable, False otherwise.
   *
   **/

  Bool IsDecodable();

  /**
   *
   * Get a number of characters from the underlying video file.
   *
   * @param len The number of characters to get.
   * @param str The string large enough to hold the characters.
   * @return True if ok, false otherwise.
   *
   **/

  Bool GetCharacters(SInt len, char * str);

  /**
   *
   * Get a frame from the file. Note the file should be set to read first
   * byte of underlying frame.
   *
   * @param actSize The actual size of the underlying data vector.
   * @return True if all went ok, False otherwise.
   *
   **/

  HeatWaveImage * GetFrame(SInt actSize);

  /** 
   *
   * Load a stream information and its appropraite information.
   *
   * @param no The stream number.
   * @return True if loaded ok, false otherwise.
   *
   **/

  Bool LoadStream(SInt no);

  /**
   *
   * Load a chunk.
   *
   * @param chk The chunk to load.
   * @param hdr The data to skip from offset.
   * @param len The lenght of array. (Max to load!)
   * @param ptr The pointer to the array that will recieve the data.
   * @return The actual butes loaded. Negative number if error.
   * @note This function will empty the list on ptr first. 
   *
   **/

  SInt LoadChunk(const DataChunk & chk, SInt hdr, SInt len, char * ptr);

  /**
   *
   * Calculate the file size. (note the file must be open.)
   *
   * @return The file size if opened else a negative number.
   *
   **/
  
  SInt GetFileSize() const;

  /**
   *
   * See if a string is in a list of strings. 
   *
   * @param str The string to check.
   * @param lst The list of strings to check.
   * @param len The length of strings in both str and lst. (Default is 4)
   * @return True if it is, False otherwise.
   *
   **/
  
  Bool SeeIfInList(const char * str,const char ** lst, SInt len = 4) const;

  /**
   *
   * AVI files are made up of chunks.
   * @note Do not change the order of members as this will cause failure
   * when reading or writing avi files.
   *
   **/
  
  struct DataChunk
  {
    /** 4 character word to identufy chunk. **/
    char m_name[4];

    /** The size of the chunk (not including first 8 bytes. ) **/
    SInt m_size;

    /** The list identity **/
    char m_type[4];

    /** True if list type, false otherwise. **/
    Bool m_list;

    /** Offset to name of chunk in file. **/
    SInt m_offset;

    /** Pointer to next chunk. **/
    DataChunk * m_nxt;

    /** Pointer to first sub chunk. **/
    DataChunk * m_sub;
    
    /**
     *
     * Constructor.
     *
     **/

    DataChunk()
    {
      for ( SInt i = 0; i < 4 ; ++i ){
        m_name[i]=0;
        m_type[i]=0;
      }
      m_size = 0;
      m_list = True;
      m_offset = 0;
      m_nxt = NULL;
      m_sub = NULL;
    }

    /**
     *
     * Copy Constructor.
     *
     * @param oth The other DataChunk to copy. 
     * @param nxt Copy the DataChunk tree on m_nxt member.(True by def.)
     * @param sub Copy the DataChunk tree on m_sub member.(True by def.)
     * @note The data pointers are not copyed and initiated to NULL.
     *
     **/

    DataChunk(const DataChunk & oth, Bool nxt = True, Bool sub = True)
    {
      for ( SInt i = 0; i < 4 ; ++i ){
        m_name[i]=oth.m_name[i];
        m_type[i]=oth.m_type[i];
      }
      m_size = oth.m_size;
      m_list = oth.m_list;
      m_offset = oth.m_offset;
      m_nxt = NULL;
      m_sub = NULL;
      if ( (oth.m_nxt) && nxt ){
        m_nxt = new DataChunk(*oth.m_nxt,True,True);
        LEAVEONNULL(m_nxt);
      }
      if ( (oth.m_sub) && sub ){
        m_sub = new DataChunk(*oth.m_sub,True,True);
        LEAVEONNULL(m_sub);
      }
    }

    /**
     *
     * Destructor.
     *
     **/

    ~DataChunk()
    {
      if ( m_sub ){
        delete m_sub;
        m_sub = NULL;
      }
      if ( m_nxt ){
        delete m_nxt;
        m_nxt = NULL;
      }
    }
  };
  
  /**
   *
   * Copy from another HeatWaveAVIReader. 
   *
   * @param rhs The other HeatWaveAVIReader to copy.
   *
   **/
  
  void DoCopy(const HeatWaveAVIReader & rhs);
  
  /** Main data chunk. 
   ** @note Internal member only, no external access. **/
  DataChunk m_chunk;

  /** Video decoder options **/
  StructCodingType m_coder;
};

#endif // __HEATWAVEAVIREADER_HPP__
