/*****************************************************************************
 *
 * @file SimpleCompressor.hpp
 * @brief Contains the SimpleCompressor class definition.
 * @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 *
 ****************************************************************************/

#ifndef __SIMPLECOMPRESSOR_HPP__
#define __SIMPLECOMPRESSOR_HPP__

#include "SimpleHuffTable.hpp"

/*****************************************************************************
 *
 * This class is responsible for compressing/decompressing data. It does this
 * by using a Huffman table, using the index numbers to find a binary
 * representation. Note this class will not constuct the Huffman table,
 * i.e. calculate frequencys. This is done so we can use one Huffman table
 * over and over again.
 *
 ****************************************************************************/

class SimpleCompressor
{
public:
  
  /**
   *
   * Default constructor.
   *
   **/
  
  SimpleCompressor();

  /**
   *
   * Parameterized constructor.
   *
   * @param table A Huffman table.
   * @param data The data vector.
   *
   * @note For every vector coeffecient there must be a matching index in the
   * Huffman table.
   * 
   **/

  SimpleCompressor(SimpleHuffTable & table, vector<int> data);

  /**
   *
   * Copy constructor.  
   *
   **/
  
  SimpleCompressor(SimpleCompressor & table);
  
  /**
   *
   * Virtual destructor.
   *
   **/

  virtual ~SimpleCompressor();

  /**
   *
   * Get the data vector.
   *
   * @return A reference to data vector.
   *
   **/

  vector<int> & GetData();

  /**
   *
   * Set the data vector.
   *
   * @param vec The data vector to copy.
   *
   **/

  void SetData(vector<int> & vec);

  /**
   *
   * Get the Huffman table.
   *
   * @return A reference to the Huffman table.
   *
   **/

  SimpleHuffTable & GetHuffman();

  /**
   *
   * Set the Huffman table.
   *
   * @param table The Huffman table to copy.
   *
   **/

  void SetHuffman(SimpleHuffTable & table);

  /**
   *
   * Compress the data, then externalise to a stream. Before calling this
   * function make sure that this object was passed a valid Huffman table
   * (SimpleHuffTable).
   * 
   * @param strm The strean to externalise to.
   *
   **/

  void Externalise(ofstream & strm);

  /**
   *
   * Internalise the data, then decompress data. Before calling this function
   * make sure that this object was passed a valid Huffman table
   * (SimpleHuffTable).
   *
   * @param strm The stream to internalise from.
   *
   **/

  void Internalise(ifstream & strm);

  /**
   *
   * Return the byte size of the data if it had to be externalised. Note this
   * includes 8 bytes for header.
   *
   * @return The data size in bytes if it had to be externalised.
   *
   **/

  int ExternSize();
  
  /**
   *
   * Equality test.
   *
   **/
  
  bool operator==(SimpleCompressor & rhs);

  /**
   *
   * Assignment operator.
   *
   * @param rhs The SimpleCompressor to copy settings from.
   * @return A reference to this object.
   *
   **/
  
  SimpleCompressor & operator=(SimpleCompressor & rhs);

protected:

  /**
   *
   * Construct a temp binary buffer from the data.
   *
   * @param temp The temporary buffer.
   *
   **/

  void ConstructTemp(vector<bool> & temp);

  /**
   *
   * Write the temp buffer out to the stream.
   *
   * @param temp The temporary buffer.
   * @param strm The stream to write out to.
   *
   **/

  void ExternTemp(vector<bool> & temp,ofstream & strm);

  /**
   *
   * Read the compressed data from a stream into a temporay buffer.
   *
   * @param temp The temporary buffer.
   * @param strm The stream to read from.
   * @param size The size of the compressed data in bytes plus padding bits.
   *
   **/
  
  void ReadTemp(vector<bool> & temp, ifstream & strm, int size);

  /**
   *
   * Convert the temporary buffer set into actual data set.
   *
   * @param temp The temporary buffer.
   * @param len The number of symbols to extract. The remaining symbols will
   * be regarded padding bytes and ignored.  
   *
   **/
  
  void ConstructData(vector<bool> & temp, int len);
  
  /**
   *
   * The (raw) uncompressed data.
   *
   **/

  vector<int> m_data;

  /**
   *
   * The Huffman table.
   *
   **/

  SimpleHuffTable m_table;

};

#endif
