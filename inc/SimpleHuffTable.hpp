/*****************************************************************************
 *
 * @file SimpleHuffTable.hpp
 * @brief Contains the SimpleHuffTable class definition.
 * @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 *
 ****************************************************************************/

#ifndef __SIMPLEHUFFTABLE_HPP__
#define __SIMPLEHUFFTABLE_HPP__

#ifdef WIN32
// Turn off the warning regarding the identifier being truncated for the
// browse information
#pragma warning(disable:4786)
#endif

#include <exception>
#include <stdexcept>
#include <vector>
#include <iterator>
#include <fstream>
#include <set>
#include "SimpleHuffNode.hpp"
#include "CommonHeaders.hpp"
#include "HeatWaveEnums.hpp"

using namespace std;

/*****************************************************************************
 *
 * A dynamic Hufman class. Works on any integer type data. This class is
 * responsible for creating a binary representation for any integer, using the
 * numbers frequency as a guide. This model takes advantage of the fact that
 * some values appear more than others. The underneath algorithms are based on
 * the "Huffman Compression". See the article by Joe Koss (Shades), describing
 * Al Steven source code from the Feb. 1991 article in Dr. Dobb's. This class
 * can be used on its own to get to a solution but it makes use of
 * SimpleHuffNode class. The wavelet information can be removed later to reuse
 * this class in different situtation as it is purely there for information
 * and does not affect the underlying operations.
 *
 ****************************************************************************/

class SimpleHuffTable
{
public:
  
  /**
   *
   * Default constructor.
   *
   **/
  
  SimpleHuffTable();

  /**
   *
   * Parameterized constructor.
   *
   * @param lev The wavelet level.
   * @param area The wavetet area.
   *
   **/
  
  SimpleHuffTable(int lev, EnumSubband area);
  
  /**
   *
   * Copy constructor.  
   *
   **/
  
  SimpleHuffTable(const SimpleHuffTable & table);
  
  /**
   *
   * Virtual destructor.
   *
   **/

  virtual ~SimpleHuffTable();

  /**
   *
   * The wavelet level of this Huffman Table.
   *
   * @return The wavelet level.
   *
   **/
  
  int GetWaveLevel() const;
  
  /**
   *
   * Set the wavelet level of this Huffmant Table.
   *
   * @param lev The wavelet level.
   *
   **/

  void SetWaveLevel(const int lev);

  /**
   *
   * The area to which this Huffman table relates to.
   *
   * @return The area to which this Huffman table relates to.
   *
   **/

  EnumSubband GetWaveSubband() const;

  /**
   *
   * Set the area to which this Huffman table relates to.
   *
   * @param area The area.
   *
   **/

  void SetWaveSubband(const EnumSubband area);

  /**
   *
   * Get the mean of the data.
   *
   * @return The mean of the data.
   *
   **/
  
  double GetMean();
  
  /**
   *
   * Get the sigma of the data.
   *
   * @return The sigma of the data.
   *
   **/
  
  double GetSigma();
  
  /**
   *
   * Gets the symbols with the maximum frequency value.
   *
   * @param smbl (out) The symbol.
   * @param freq (out) The frequency.
   *
   * @note If table is empty, both smbls and freq returns as -1;
   *
   **/
  
  void GetHighPoint(int & smbl, int & freq);
  
  /**
   *
   * Add a row to the table. Note that the row will only be added if the index
   * is unique.
   *
   * @param index The index number.
   * @param freq The frequency of the index number.
   * @return True if added, false otherwise.
   * @pre Frequency must be positive.
   *
   **/

  bool AddRow(int index, int freq);

  /**
   *
   * Find a row on the table using the index. Then return the frequency of
   * that index plus the binary representation using a vector<bool> to
   * represent binary data.
   *
   * @param index The index number.
   * @param freq (out) Return the frequency of the index number.
   * @param bin (out) The binary representation for the index number.
   * @return True if row was found, false otherwise.
   *
   **/

  bool FindRow(const int index,int & freq, vector<bool> & bin);

  /**
   *
   * Find a index on the table using the binary representation. If founded the
   * function will return the index plus frequency. Note this function matches
   * only the parts of the vectors that is shortest of the two. This lenght is
   * also returned.
   *
   * @param bin The binary representation for the index number.
   * @param index (out) The index number.
   * @param freq (out) Return the frequency of the index number.
   * @param len (out) The length of the match.
   * @param offset The offset from the start of the binary representation
   * (parameter bin) to start the search from.
   * @return True if row was found, false otherwise.
   *
   **/

  bool FindIndex(const vector<bool> &bin, int & index, int & freq, int & len,
                 int offset);

  /**
   *
   * Get the current row.
   *
   **/
  
  bool GetCurrentRow(int & index, int & freq, vector<bool> & bin);

  /**
   *
   * @return The number of rows on the table.
   *
   **/

  int TotalRows();

  /**
   *
   * Set the row iterator the start row.
   *
   **/

  void Begin();

  /**
   *
   * Set the iterator to next row.
   *
   **/

  void Next();

  /**
   *
   * See if the iterator is on a valid row.
   *
   * @return True if iterator is pointing to a valid row, false otherwise.
   *
   **/

  bool Valid();
  
  /**
   *
   * Build the binary representation for each row. This should be done after
   * adding all the rows to the table.
   *
   **/

  void BuildBinary();

  /**
   *
   * Clear the table of all rows.
   *
   **/

  void ClearTable();

  /**
   *
   * Externalise the Huffman table to a file.
   * 
   * @param strm The strean to externalise to.
   *
   **/

  void Externalise(ofstream & strm);

  /**
   *
   * Internalise the Huffman table from a stream.
   *
   * @param strm The stream to internalise from.
   *
   **/

  void Internalise(ifstream & strm);

  /**
   *
   * Return the byte size of the Huffman table if it had to be
   * externalised. Note this includes 8 bytes for header.
   *
   * @return The table size in bytes if it had to be externalised.
   *
   **/

  int ExternSize();

  /**
   *
   * Change this table to a theoretical table.
   *
   **/

  void SetTheoretical(bool set, int start, int end);

  bool GetTheoretical();

  void PrintTable();

  /**
   *
   * Return a laplacian probability frequency using a x offset.
   *
   * @param sigma The sigma of the data range.
   * @param mean The mean of the data range.
   * @param value The data value (x axis).
   * @return The probability (y axis).
   *
   **/

  SFloat64 PDF(SFloat64 sigma, SFloat64 mean, SFloat64 value);

  /**
   *
   * Equality test.
   *
   * @param rhs The other SimpleHuffTable to compare this object to.
   * @return True if equal, false otherwise.
   *
   **/

  bool operator==(SimpleHuffTable & rhs);

  /**
   *
   * Assignment operator.
   *
   * @param rhs The object to copy values from.
   * @return A reference to this.
   *
   **/

  SimpleHuffTable & operator=(const SimpleHuffTable & rhs);

protected:

  /**
   *
   * Clears the temporary table before copying the main table onto it, then
   * clears the main table.
   *
   **/

  void CopyTable();
  
  /**
   *
   * The entry row set, or holding bay before building the tree. 
   * Sorts by index.
   *
   **/
  
  multiset<SimpleHuffNode, greater<SimpleHuffNode> > m_table;
  
  /**
   *
   * The entry row set iterator, or holding bay iterator.
   *
   **/

  multiset<SimpleHuffNode, greater<SimpleHuffNode> >::iterator m_iter;
  
  /**
   *
   * The (temporary natured) tree building row set.
   * Sorts by frequency.
   *
   **/
  
  multiset<SimpleHuffNode, less_equal<SimpleHuffNode> > m_temp;
  
  /**
   *
   * The (temopary natured) tree building row set iterator.
   * Sorts by frequency.
   *
   **/
  
  multiset<SimpleHuffNode, less_equal<SimpleHuffNode> >::iterator m_tempIter;

  /**
   *
   * The last build tree.
   *
   **/
  
  SimpleHuffNode m_tree;
  
  /**
   *
   * The wavelet level.
   *
   **/
  
  int m_level;
  
  /**
   *
   * The wavelet area.
   *
   **/
  
  EnumSubband m_subband;

  /**
   *
   * Is theoretical.
   *
   **/

  bool m_theoretical;

  /**
   *
   * Theoretical sigma.
   *
   **/

  int m_sigma;

  /**
   *
   * Theoretical mean.
   *
   **/

  int m_mean;

  /**
   *
   * Theoretical start.
   *
   **/

  short m_start;

  /**
   *
   * Theoretical end.
   *
   **/

  short m_end;
  
};

#endif
