/**
 *
 * @file SimpleHuffTable.cpp
 * @brief Contain the SimpleHuffTable class's function definition.
 * @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 *
 **/

#include "SimpleHuffTable.hpp"
#include "MiscMathTool.hpp"

SimpleHuffTable::SimpleHuffTable()
{
  m_level = 0;
  m_subband = SubLL;
  m_theoretical = false;
}

SimpleHuffTable::SimpleHuffTable(const SimpleHuffTable & table)
{
  m_level = table.m_level;
  m_subband = table.m_subband;
  m_table = table.m_table;
  m_theoretical = table.m_theoretical;
  
  BuildBinary();
}

SimpleHuffTable::SimpleHuffTable(int lev, EnumSubband area)
{
  ASSERT ( lev >= 0 );
  
  m_level = lev;
  m_subband = area;
  m_theoretical = false;
}
  
SimpleHuffTable::~SimpleHuffTable()
{
}

int 
SimpleHuffTable::GetWaveLevel() const
{
  return m_level;
}

void 
SimpleHuffTable::SetWaveLevel(const int lev)
{
  ASSERT ( lev >= 0 );

  m_level = lev;
}

EnumSubband 
SimpleHuffTable::GetWaveSubband() const
{
  return m_subband;
}

void 
SimpleHuffTable::SetWaveSubband(const EnumSubband area)
{
  m_subband = area;
}

double
SimpleHuffTable::GetMean()
{
  int total_sum = 0;
  int total_freq = 0;
  int index = 0;
  int freq = 0;
  vector<bool> dummy;
  
  for ( Begin() ; GetCurrentRow(index, freq, dummy); Next() )
    {
      ASSERT ( freq > 0 );
      total_sum += (index*freq);
      total_freq += freq;
    }
  
  if (total_freq == 0)
    return 0;
  
  return (double)total_sum/(double)total_freq;
}

double
SimpleHuffTable::GetSigma()
{
  double sum = 0;
  int freq_total = 0;
  double mean = GetMean();
  
  int index = 0;
  int freq = 0;
  vector<bool> dummy;
  
  for ( Begin() ; GetCurrentRow(index, freq, dummy); Next() )
    {
      ASSERT ( freq > 0 );
      
      double index_dub = index;
      double to_add = pow(index_dub-mean,2);
      sum = sum + (to_add*freq);
      
      freq_total = freq_total + freq;
    }
  freq_total -= 1;
  
  if ( freq_total < 1)
    return 0;
  else
    return sqrt(sum/freq_total);
}

void
SimpleHuffTable::GetHighPoint(int & smbl, int & freq)
{
  smbl = -1;
  freq = -1;
  int tmp_index = 0;
  int tmp_freq = 0;
  vector<bool> tmp_dummy;
  
  for ( Begin() ; GetCurrentRow(tmp_index, tmp_freq, tmp_dummy); Next() )
    {
      ASSERT ( tmp_freq > 0 );
      
      if ( tmp_freq > freq )
        {
          freq = tmp_freq;
          smbl = tmp_index;
        }
    }
}

bool 
SimpleHuffTable::AddRow(int index, int freq)
{
  ASSERT ( freq >= 0 );
  
  SimpleHuffNode temp(index,freq);
  m_iter = m_table.find(temp);
  
  if ( m_iter != m_table.end() )
    {
      return false;
    }
  
  m_iter = m_table.insert(temp);
  return true;
}

bool 
SimpleHuffTable::FindRow(const int index,int & freq, vector<bool> & bin)
{
  int loc_index;
  int loc_freq;
  vector<bool> loc_bin;
  SimpleHuffNode temp(index,freq);

  m_iter = m_table.find(temp);
  
  if ( m_iter != m_table.end() )
    {
      freq = (*m_iter).GetFrequency();
      bin = (*m_iter).GetBinary();
      return true;
    }
  return false;
  
  for ( Begin(); GetCurrentRow(loc_index,loc_freq,loc_bin) ; Next() )
    {
      if (loc_index == index)
        {
          freq = loc_freq;
          bin = loc_bin;
          return true;
        }
    }
  
  return false;
}

bool 
SimpleHuffTable::FindIndex(const vector<bool> & bin, int & index, int & freq, 
                           int & len, int offset)
{
  return m_tree.FindIndex(bin,index,freq,len,offset);
}

bool 
SimpleHuffTable::GetCurrentRow(int & index, int & freq, vector<bool> & bin)
{
  if ( !Valid() )
    return false;
  
  index = (*m_iter).GetIndex();
  freq = (*m_iter).GetFrequency();
  bin = (*m_iter).GetBinary();
  return true;
}

int 
SimpleHuffTable::TotalRows()
{
  return m_table.size();
}

void 
SimpleHuffTable::Begin()
{
  m_iter = m_table.begin();
}

void 
SimpleHuffTable::Next()
{
  ++m_iter;
}

bool 
SimpleHuffTable::Valid()
{
  if ( m_iter == m_table.end() )
    return false;
  else
    return true;
}
  
void 
SimpleHuffTable::BuildBinary()
{
  int org_size;
  
  if ( m_table.empty() )
    return;
  else {
    org_size = m_table.size();
    CopyTable();
  }
  
  while ( m_temp.size() > 1) {
    m_tempIter = m_temp.begin();
    SimpleHuffNode * left = new SimpleHuffNode(*m_tempIter); 
    m_temp.erase(m_tempIter);
    m_tempIter = m_temp.begin();
    SimpleHuffNode * right = new SimpleHuffNode(*m_tempIter);
    m_temp.erase(m_tempIter);
    SimpleHuffNode parent(0,0,left,right);
    m_temp.insert(parent);
  }
  
  ASSERT( m_temp.size() == 1);
  m_tempIter = m_temp.begin();
  m_tree = *m_tempIter; 
  m_temp.erase(m_tempIter);
  m_tree.BuildBinary();
  m_tree.DumpTree(m_table);
  //m_tree.PrintTable("root");
  m_temp.clear();

  ASSERT ( (int)m_table.size() == org_size );
}

void 
SimpleHuffTable::ClearTable()
{
  m_table.clear();
}


void 
SimpleHuffTable::Externalise(ofstream & strm)
{
  int size = ExternSize();
  unsigned char level = (unsigned char)GetWaveLevel();
  unsigned char area = (unsigned char)GetWaveSubband();
  
  strm.write ((char*)&size,4);
  strm.write ((char*)&level,1);
  strm.write ((char*)&area,1);
  strm.write ((char*)&m_theoretical,1);

  //cout << " size : " << size << endl;
  //cout << " level : " << level << endl;
  //cout << " area : " << area << endl;
  //cout << " theoretical : " << (int)m_theoretical << endl;
  
  if ( m_theoretical )
    {
      strm.write((char*)&m_sigma,sizeof(m_sigma));
      strm.write((char*)&m_mean,sizeof(m_mean));
      strm.write((char*)&m_start,2);
      strm.write((char*)&m_end,2);
      return;
    }
 
  int loc_index;
  int loc_freq;
  vector<bool> loc_bin;
  
  for ( Begin(); GetCurrentRow(loc_index,loc_freq,loc_bin) ; Next() )
    {
      signed short small_index = (signed short)loc_index;
      //signed short small_freq = (signed short)loc_freq;
      int small_freq = loc_freq;

      ASSERT ( loc_index == (int)small_index );
      
#ifdef DEBUG
      // debug
      if ( loc_index != small_index )
        {
          cout << "loc_index = " << loc_index << " and small_index = "
               << small_index << endl;
        }
      // end debug
#endif
      
      ASSERT ( loc_freq == (int)small_freq );
      ASSERT ( loc_freq > 0 );
      
      strm.write((char*)&small_index,2);
      strm.write((char*)&small_freq,4);
    }
}

void 
SimpleHuffTable::Internalise(ifstream & strm)
{
  int size;
  unsigned char level,area;
  
  strm.read((char*)&size,4);
  strm.read((char*)&level,1);
  strm.read((char*)&area,1);
  strm.read((char*)&m_theoretical,1);

  SetWaveLevel((int)level);
  SetWaveSubband((EnumSubband) area);
  
  if ( m_theoretical ) {
    ASSERT ( size == 27 );
    strm.read((char*)&m_sigma,sizeof(m_sigma));
    strm.read((char*)&m_mean,sizeof(m_mean));
    strm.read((char*)&m_start,2);
    strm.read((char*)&m_end,2);
    for ( int i = m_start; i <= m_end ; ++ i ) {
      double pdf = PDF(m_sigma, m_mean, (double)i);
      pdf = pdf * 10000000;
      bool add = AddRow(i,(int)pdf);
      ASSERT ( add==true );
    }
    return;
  }
  
  size = size - 7;
  size = size/6;
  
  signed short index = 0;
  // unsigned short frequency = 0;
  int frequency = 0;
  
  for ( int i = 0; i < size ; ++i )
    {
      strm.read((char*)&index,2);
      strm.read((char*)&frequency,4);
      ASSERT ( frequency > 0 );
      
      bool check = AddRow((int)index,(int)frequency);
      ASSERT (check);
    }

  BuildBinary();
}

int
SimpleHuffTable::ExternSize()
{
  if ( m_theoretical )
    {
      return 27;
    }
  else
    {
      int size = 7 + (m_table.size()*6);
      return size;
    }
}

void
SimpleHuffTable::SetTheoretical(bool set, int start, int end)
{
  if ( m_theoretical )
    return;
  
  ASSERT ( m_theoretical == false );
  ASSERT ( m_subband != SubLL );
  
  m_mean = (int)GetMean();
  m_sigma = (int)GetSigma();
  m_start = start;
  m_end = end;
  
  ClearTable();
  
  for ( int i = start; i <= end ; ++ i )
    {
      double pdf = PDF(m_sigma, m_mean, (double)i);
      pdf = pdf * 10000000;
      //cout << " Mean = " << m_mean;
      //cout << " sigma = " << m_sigma;
      //cout << " i = " << i << " pdf = " << (int)pdf << endl
      bool add = AddRow(i,(int)pdf);
      ASSERT ( add );
    }
  
  m_theoretical = true;
}

bool 
SimpleHuffTable::GetTheoretical()
{
  return m_theoretical;
}

void 
SimpleHuffTable::PrintTable()
{
  BuildBinary();
  m_tree.PrintTable("root");
}

bool 
SimpleHuffTable::operator==(SimpleHuffTable & rhs)
{
  bool ret = true;
  ret = ret && (m_table == rhs.m_table);
  ret = ret && (m_level == rhs.m_level);
  ret = ret && (m_subband == rhs.m_subband);
  return ret;
}

SimpleHuffTable & 
SimpleHuffTable::operator=(const SimpleHuffTable & rhs)
{
  m_table.clear();
  m_table = rhs.m_table;
  m_level = rhs.m_level;
  m_subband = rhs.m_subband;
  
  BuildBinary();
  return (*this);
}

void 
SimpleHuffTable::CopyTable()
{
  m_temp.clear();
  int loc_index;
  int loc_freq;
  vector<bool> loc_bin;
  
  for ( Begin(); GetCurrentRow(loc_index,loc_freq,loc_bin) ; Next() )
    {
      SimpleHuffNode temp(loc_index,loc_freq,NULL,NULL,NULL,true);
      m_temp.insert(temp);
    }
  
  m_table.clear();
}

SFloat64
SimpleHuffTable::PDF(SFloat64 sigma, SFloat64 mean, SFloat64 value)
{
  SFloat64 B = sigma/sqrt(2.0L);
  return exp(-fabs(value-mean)/B)/2/B;
}
