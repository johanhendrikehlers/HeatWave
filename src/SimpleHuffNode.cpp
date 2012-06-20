/*****************************************************************************
 *
 * @file SimpleHuffNode.cpp
 * @brief Contains the SimpleHuffNode function definitions.
 * @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 *
 *****************************************************************************/

#include "SimpleHuffNode.hpp"

SimpleHuffNode::SimpleHuffNode()
{
  m_index = 0;
  m_frequency = 0;
  m_depth = 0;
  m_right = NULL;
  m_left = NULL;
  m_parent = NULL;
  m_bitset = false;
  m_sort_by_index = true;
}

SimpleHuffNode::SimpleHuffNode(SInt index, SInt frequency, 
                               SimpleHuffNode * child_left,
                               SimpleHuffNode * child_right,
                               SimpleHuffNode * parent,
                               Bool sort_by_index)
{
  m_left = child_left;
  m_right = child_right;
  m_parent = parent;
  m_sort_by_index = sort_by_index;
  
  if ( HasTwoChildren() )
    {
      m_index = 0;
      m_left->SetParent(this);
      m_right->SetParent(this);
      CalculateFrequency();
      AssertVariance();
    }
  else
    {
      ASSERT ( frequency >= 0 );
      m_index = index;
      m_frequency = frequency;
      m_depth = 0;
      m_bitset = false;
    }
}

SimpleHuffNode::SimpleHuffNode(const SimpleHuffNode & rhs)
{
  m_right = NULL;
  m_left = NULL;
  m_parent = NULL;
  m_depth = 0;
  m_bitset = false;
  
  Copy(rhs);
}
  
SimpleHuffNode::~SimpleHuffNode()
{
  delete m_right;
  m_right = NULL;
  delete m_left;
  m_left = NULL;
}

void 
SimpleHuffNode::SetIndex(SInt index)
{
  ASSERT ( !HasTwoChildren() );
  m_index = index;
}

SInt
SimpleHuffNode::GetIndex() const
{
  return m_index;
}

void 
SimpleHuffNode::SetFrequency(SInt freq)
{
  ASSERT ( freq >= 0 );
  ASSERT ( !HasTwoChildren() );
  m_frequency = freq;
}

SInt 
SimpleHuffNode::GetFrequency() const
{
  return m_frequency;
}

void 
SimpleHuffNode::CalculateFrequency()
{
  ASSERT ( HasTwoChildren() );
  
  m_frequency = 0;
  m_frequency += m_left->GetFrequency();
  m_frequency += m_right->GetFrequency();
}

SimpleHuffNode * 
SimpleHuffNode::GetLeftChild() const
{
  return m_left;
}

SimpleHuffNode * 
SimpleHuffNode::GetRightChild() const
{
  return m_right;
}

void 
SimpleHuffNode::SetParent(SimpleHuffNode * parent)
{
  m_parent = parent;
}

SimpleHuffNode * 
SimpleHuffNode::GetParent() const
{
  return m_parent;
}

void
SimpleHuffNode::BuildBinary(Bool root)
{
  if ( root ) {
    m_depth = 0;     
    
    if ( HasTwoChildren() ) {
      m_left->SetParent(this);
      m_right->SetParent(this);
      m_left->m_bitset = false;
      m_right->m_bitset = true;
      m_left->m_depth = 1;
      m_right->m_depth = 1; 
    } 
    else {
      ASSERT ( false );
      return; 
    } 
  }
  else {
    m_left->m_bitset = false;
    m_right->m_bitset = true;
    m_left->m_depth = m_depth+1;
    m_right->m_depth = m_depth+1;
    m_left->SetParent(this);
    m_right->SetParent(this);
  }
  
  if ( HasTwoChildren() ) {
    if ( m_left->HasTwoChildren() )
      m_left->BuildBinary(false);
    if ( m_right->HasTwoChildren() )
      m_right->BuildBinary(false); 
  }
}

void
SimpleHuffNode::DumpTree(multiset<SimpleHuffNode, 
                         greater<SimpleHuffNode> > & m_table)
{
  if ( HasTwoChildren() )
    {
      m_left->DumpTree(m_table);
      m_right->DumpTree(m_table);
    }
  else
    {
      m_table.insert(*this);
    }
}

void
SimpleHuffNode::PrintTable(const char * message)
{
  cout << "[" << m_depth << "]";
  for ( SInt i = 0; i < m_depth ; ++ i)
    cout << "  ";
  cout << "-";
  
  if ( HasTwoChildren() ) {
    cout << message << " node freq[" << m_frequency << "]";
    if ( m_bitset )
      cout << " bit[1]" << endl;
    else
      cout << " bit[0]" << endl;
      
    m_left->PrintTable("left");
    m_right->PrintTable("right");
  }
  else {
    cout << message << " leaf index[" << m_index << " freq[" 
         << m_frequency << "]" ;
    if ( m_bitset )
      cout << " bit[1] rep[";
    else
      cout << " bit[0] rep[";
      
    vector<Bool> bitset = GetBinary();
    
    for ( SInt i = 0 ; i < (SInt)bitset.size() ; ++ i ){
      if ( bitset[i] )
        cout << "1";
      else
        cout << "0";
    }
    
    cout << "]" << endl;
  }
}

Bool
SimpleHuffNode::FindIndex(const vector<Bool> & bin, SInt & index, SInt & freq,
                          SInt & len, SInt offset)
{
  if ( (SInt)bin.size() < (offset+m_depth) )
    return false;
  
  if ( HasTwoChildren() )
    { 
      ASSERT ( m_left != NULL );
      ASSERT ( m_right != NULL );
      ASSERT ( m_right->m_bitset != m_left->m_bitset );
      ASSERT ( ((int)bin.size()) >= (offset+m_depth) );
      
      // cout << "offset+mdepth" << (offset+m_depth);
      // cin.ignore();
      
      if ( m_left->m_bitset == bin[offset+m_depth] )
        {
          // cout << "left!";
          return m_left->FindIndex(bin,index,freq,len,offset);
        }
      else
        {
          // cout << "right!";
          return m_right->FindIndex(bin,index,freq,len,offset);
        }
    }
  else
    {
      if ( bin[offset+(m_depth-1)] == m_bitset )
        {
          index = m_index;
          freq = m_frequency;
          len = m_depth;
          return true;
        }
      return false;
    }
}

vector<Bool>
SimpleHuffNode::GetBinary() const
{
  vector<Bool> ret;
  const SimpleHuffNode * ptr = this;
  
  for ( SInt i = 0; i < m_depth ; ++i )
    {
      ASSERT ( ptr != NULL );
      if ( ptr == NULL )
        {
          cout << " i == " << i << " m_depth == " << m_depth << endl;
        }
      ret.push_back(ptr->m_bitset);
      ptr = ptr->GetParent();
    }
  
  vector<Bool> temp;
  
  for ( SInt i = ((ret.size())-1); i >= 0 ; --i )
    {
      temp.push_back(ret[i]);
    }
  
  return temp;
}

void
SimpleHuffNode::SortByIndex(Bool sort_by_index)
{
  m_sort_by_index = sort_by_index;
}

Bool 
SimpleHuffNode::operator>(const SimpleHuffNode & rhs) const
{
  //  if ( m_index == rhs.m_index )
  //   return m_frequency < rhs.m_frequency;
  
  return m_index > rhs.m_index;
}

Bool 
SimpleHuffNode::operator<(const SimpleHuffNode & rhs) const
{
  return m_index < rhs.m_index;
}

Bool
SimpleHuffNode::operator<=(const SimpleHuffNode & rhs) const
{
  return (m_frequency <= rhs.m_frequency);
}

SimpleHuffNode & 
SimpleHuffNode::operator=(const SimpleHuffNode & rhs)
{
  Copy(rhs);
  return *this;
}

Bool 
SimpleHuffNode::operator==(const SimpleHuffNode & rhs) const
{
  return (m_index == rhs.m_index);
  
  /*
   Bool ret = true;
   ret = ret && (m_index == rhs.m_index);
   ret = ret && (m_frequency == rhs.m_frequency);
   ret = ret && (m_frequency == rhs.m_frequency);
   ret = ret && (HasTwoChildren() == rhs.HasTwoChildren());
   ret = ret && (m_depth == rhs.m_depth);
   ret = ret && (m_sort_by_index == rhs.m_sort_by_index);
   
   if ( !ret )
   return ret;
   
   if ( HasTwoChildren() )
    {
    ret = ret && ((*GetLeftChild()) == (*rhs.GetLeftChild()));
    ret = ret && ((*GetRightChild()) == (*rhs.GetRightChild()));
    }
    
    return ret;
  */
}

void
SimpleHuffNode::Copy(const SimpleHuffNode & rhs)
{
  delete m_left;
  delete m_right;
  m_left = m_right = NULL;
  
  m_index = rhs.m_index;
  m_frequency = rhs.m_frequency;
  m_bitset = rhs.m_bitset;
  m_parent = rhs.m_parent;
  m_depth = rhs.m_depth;
  m_sort_by_index = rhs.m_sort_by_index;
  
  if ( rhs.HasTwoChildren() )
    {
      m_left = new SimpleHuffNode(*rhs.GetLeftChild());
      m_right = new SimpleHuffNode(*rhs.GetRightChild());
    }
  
  AssertVariance();
}

Bool
SimpleHuffNode::HasTwoChildren() const
{
  Bool ret = true;
  ret = ret && (m_right != NULL);
  ret = ret && (m_left != NULL);
  return ret;
}

void
SimpleHuffNode::AssertVariance()
{
  ASSERT ( this != m_right );
  ASSERT ( this != m_left );
  
  if (m_right != NULL)
    {
      ASSERT ( m_right != m_left );
    }
  
  if (m_parent != NULL)
    {
      ASSERT ( m_right != m_parent );
      ASSERT ( m_left != m_parent );
      ASSERT ( this != m_parent );
    }
}

