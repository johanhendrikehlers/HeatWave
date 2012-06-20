/*****************************************************************************
 *
 * @file SimpleCompressor.cpp
 * @brief Contain the SimpleCompressorclass's function definition.
 * @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 *
 ****************************************************************************/

#include "SimpleCompressor.hpp"

SimpleCompressor::SimpleCompressor()
{
}

SimpleCompressor::SimpleCompressor(SimpleHuffTable & table, vector<int> data)
{
  m_table = table;
  m_data = data;
}

SimpleCompressor::SimpleCompressor(SimpleCompressor& com)
{
  m_data = com.m_data;
  m_table = com.m_table;
}
  
SimpleCompressor::~SimpleCompressor()
{
  // nothing to do
}

void
SimpleCompressor::SetData(vector<int> & vec)
{
  m_data.clear();
  m_data = vec;
}

vector<int> &
SimpleCompressor::GetData()
{
  return m_data;
}

SimpleHuffTable &
SimpleCompressor::GetHuffman()
{
  return m_table;
}

void
SimpleCompressor::SetHuffman(SimpleHuffTable & table)
{
  m_table = table;
}

void 
SimpleCompressor::Externalise(ofstream & strm)
{
  int size = ExternSize();
  int vec_length = m_data.size();

  ASSERT ( (int)m_data.size() == (int)vec_length ); 
  
  strm.write ((char*)&size,4);
  strm.write ((char*)&vec_length,4);
  
  if ( m_data.size() > 0 )
    {
      vector<bool> temp;
      ConstructTemp(temp);
      ExternTemp(temp,strm);
    } 
}

void 
SimpleCompressor::ConstructTemp(vector<bool> & temp)
{
  int total_bits = 0;
  
  for ( int i = 0 ; i < (int)m_data.size() ; ++i)
    {
      int freq_ignored = 0;
      int index = m_data.at(i);
      vector<bool> bin;
      if ( !m_table.FindRow(index,freq_ignored,bin)){
	ASSERT ( false );
      }
      
      ASSERT ( bin.size() > 0);
      
      total_bits += bin.size();

      for ( int n = 0 ; n < (int)bin.size() ; ++n)
        {
          temp.push_back(bin.at(n));
        }
    }

  int pad_bytes = total_bits%8;

  if ( pad_bytes != 0 )
    {
      for ( int i = 0 ; i < (8-pad_bytes); ++i)
        {
          ++total_bits;
          temp.push_back(false);
        }
    }

  ASSERT( (total_bits%8)==0 );
}

void 
SimpleCompressor::ExternTemp(vector<bool> & temp,ofstream & strm)
{
  ASSERT ( temp.size() > 0 );
  ASSERT ( (temp.size())%8 == 0 );

  int bytes_written;
  bytes_written = 0; // for unused warning
  
  unsigned char char_bits = 0;
  
  for ( int i = 0; i < (int)temp.size(); ++i )
    { 
      char_bits = char_bits << 1;
      char_bits = char_bits | (unsigned char)temp.at(i);
      
      if ( ( ((i+1) % 8) == 0) &&
           ( i > 0 ) )
        {
          ASSERT ( ++bytes_written );
          strm.write((char*)&char_bits,1);
          char_bits = 0;
        }
    }

  ASSERT ((int)bytes_written == (int)(temp.size()/8));
}

void
SimpleCompressor::Internalise(ifstream & strm)
{
  m_data.clear();
  
  int size;
  int vec_size;
  strm.read((char*)&size,4);
  strm.read((char*)&vec_size,4);
  
  ASSERT (size > 8);
  
  vector<bool> temp;
  ReadTemp(temp,strm,size-8);
  ConstructData(temp,vec_size);
}

void 
SimpleCompressor::ReadTemp(vector<bool> & temp, ifstream & strm, int size)
{
  int bytes_read;
  bytes_read = 0; 
  int bits_converted;
  bits_converted = 0;
  
  for ( int i = 0; i < size ; ++i )
    {
      unsigned char comp = 0;
      strm.read((char*)&comp,1);
      ASSERT (++bytes_read);
      
      for ( int b = 0; b < 8 ; ++b )
        {
          ASSERT (++bits_converted);
          
          if (comp & 0x80)
            {
              temp.push_back(true);
            }
          else
            {
              temp.push_back(false);
            }
          comp = comp << 1;
        }
    }
  
  ASSERT ( (bytes_read*8) == bits_converted );
  ASSERT ( bytes_read == size);
}

void 
SimpleCompressor::ConstructData(vector<bool> & temp, int len) 
{
  m_data.clear();
  
  int offset = 0;
  int index = -1;
  int frequency = -1; // ignored
  int match_length = -1;
  
  for ( int i = 0 ; i < len ; ++i )
    {
      if ( !m_table.FindIndex(temp,index,frequency,match_length,offset)){
	ASSERT ( false );
      }
      
      ASSERT ( match_length != -1 );
      
      offset += match_length;
      m_data.push_back(index);
    }
  
  ASSERT ( (int)m_data.size() == len );
}

int 
SimpleCompressor::ExternSize()
{
  int size = 8; //header byte size
  int bitsize = 0;
  
  for ( int i = 0 ; i < (int)m_data.size() ; ++i)
    {
      int freq_ignored = 0;
      int index = m_data.at(i);
      vector<bool> bin;
      bool found = m_table.FindRow(index,freq_ignored,bin);
      bitsize += bin.size();
      // ASSERT ( found == true )
      if ( found != true )
        {
          // debug code
          cout << " index not found = " << index << endl;
          cout << " i = " << i << " m_data.size() = " << m_data.size() << endl;
        }
      ASSERT (bin.size() > 0);
    }
  
  int byte_size = bitsize/8;
  
  if ( (bitsize%8) != 0)
    {
      ++byte_size;
    }

  /** 
   * @todo If these are never false then consider making the externsize a
   * unsigned short. 
   **/
  
  //ASSERT ( (size+byte_size) >= 0 );
  //ASSERT ( (size+byte_size) <= 65535 );
  
  return size+byte_size;
}

bool 
SimpleCompressor::operator==(SimpleCompressor& rhs)
{
  bool ret = true;
  ret = ret && (m_data == rhs.m_data);
  ret = ret && (m_table == rhs.m_table);
  return ret;
}

SimpleCompressor& 
SimpleCompressor::operator=(SimpleCompressor& rhs)
{
  m_table = rhs.m_table;
  m_data.clear();
  m_data = rhs.m_data;
  return (*this);
}
