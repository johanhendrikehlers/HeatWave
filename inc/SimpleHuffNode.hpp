/*****************************************************************************
 *
 * @file SimpleHuffNode.hpp
 * @brief Contains the SimpleHuffNode class definition.
 * @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 *
 ****************************************************************************/

#ifndef __SIMPLEHUFFNODE_HPP__
#define __SIMPLEHUFFNODE_HPP__

#ifdef WIN32
// Turn off the warning regarding the identifier being truncated for the
// browse information
#pragma warning(disable:4786)
#endif

#include <exception>
#include <stdexcept>
#include <string>
#include <math.h> 
#include <vector>
#include <iostream>
#include <iomanip>
#include <set>
#include "CommonHeaders.hpp"

using namespace std;

/*****************************************************************************
 *
 * A Huffman node class. (Used internally from SimpleHuffTable class!)
 *
 ****************************************************************************/

class SimpleHuffNode
{
public:
  
  /**
   *
   * Default constuctor.
   *
   **/
  
  SimpleHuffNode();
  
  /**
   *
   * Parameterized constructor.
   *
   * @param index The nodes index.
   * @param frequency The nodes frequency.
   * @param child_left The left child to copy.
   * @param child_right The right child to copy.
   * @param parent The parent of this node.
   * @param sort_by_idx Sort by index, else by frequency. Index by default.
   *
   **/
  
  SimpleHuffNode(SInt index, SInt frequency, 
                 SimpleHuffNode * child_left = NULL,
                 SimpleHuffNode * child_right = NULL,
                 SimpleHuffNode * parent = NULL,
                 Bool sort_by_idx = true);

  
  /**
   *
   * Copy constructor.
   *
   * @param copy The SimpleHuffNode to copy.
   *
   **/
  
  SimpleHuffNode(const SimpleHuffNode & copy);
  
  /**
   *
   * Destructor.
   *
   **/

  virtual ~SimpleHuffNode();

  /**
   *
   * Set the index.
   *
   * @param index The index to set to.
   *
   **/
  
  void SetIndex(SInt index);
  
  /**
   *
   * Get the index.
   *
   * @return The index number.
   *
   **/
  
  SInt GetIndex() const;
  
  /**
   *
   * Set frequency.
   *
   * @param freq The frequency to set to.
   *
   **/

  void SetFrequency(SInt freq);

  /**
   *
   * @return The frequency of the leaf, or if a node the combined freaquancys.
   *
   **/

  SInt GetFrequency() const;

  /**
   *
   * @return True if node has two children, else False.
   *
   **/

  Bool HasTwoChildren() const; 

  /**
   *
   * @return A pointer to left child.
   *
   **/

  SimpleHuffNode * GetLeftChild() const;

  /**
   *
   * @return A pointer to right child.
   *
   **/

  SimpleHuffNode * GetRightChild() const;

  /**
   *
   * Set the parent of this node.
   *
   * @param parent The parent of the node.
   *
   **/

  void SetParent(SimpleHuffNode * parent);

  /**
   *
   * @return A pointer to the parent of this node.
   *
   **/

  SimpleHuffNode * GetParent() const;

  /**
   *
   * Build the binary/text representation.
   *
   * This function should only be called when the tree is compleet and all
   * nodes have two children or nun.
   * 
   * @param root Must be set to True, default is True.
   * @note do not change the patameter as this is a recursive function.
   *
   **/

  void BuildBinary(Bool root = true);

  /**
   *
   * Find an index using the binary representation. (Top Down)
   *
   * @param bin The list of binary numbers.
   * @param index (out) The index if found.
   * @param len (out) The binary lenght of index.
   * @param offset (out) The new offset in bin vector for next data.
   *
   **/
  
  Bool FindIndex(const vector<Bool> & bin, SInt & index, SInt & freq,
                 SInt & len, SInt offset);

  /**
   *
   * @return A binary representation. (Bottom Up)
   *
   **/

  vector<Bool> GetBinary() const;

  /**
   *
   * Dump leafs onto a table.
   *
   * @param table The table to dump to.
   *
   **/
  
  void DumpTree(multiset<SimpleHuffNode, greater<SimpleHuffNode> > & table);

  /**
   *
   * Print the leafs. ( I.E. those without any nodes below them. )
   *
   * @param message A message to print with root node.
   *
   **/

  void PrintTable(const char * message = NULL);

  /**
   *
   * Switch the effect of greater than (>), less then or eqaul (<=) and equal
   * to (==) operators to work on the index or frequency.
   *
   **/

  void SortByIndex(Bool sort_by_index = true);

  /**
   *
   * Greater than test, note if the frequency's are the same it will actualy
   * look at the index. This is used by the SimpleHuffTable so dont change.
   *
   **/

  Bool operator>(const SimpleHuffNode & rhs) const;

  /**
   *
   * Less than test, note if the frequency's are the same it will actualy
   * look at the index. This is used by the SimpleHuffTable so dont change.
   *
   **/
  
  Bool operator<(const SimpleHuffNode & rhs) const;

  /**
   *
   * Less than or equal test. This is used by SimpleHuffTree so dont change.
   *
   **/

  Bool operator<=(const SimpleHuffNode & rhs) const;

  /**
   *
   * Equality test.
   *
   **/

  Bool operator==(const SimpleHuffNode & rhs) const;

  /**
   *
   * Assignment operator.
   *
   **/

  SimpleHuffNode & operator=(const SimpleHuffNode & rhs);
    
protected:

  /**
   *
   * Calcuclate frequency.
   *
   **/

  void CalculateFrequency();

  /**
   *
   * Copy function.
   *
   **/
  
  void Copy(const SimpleHuffNode & rhs);

  /**
   *
   * Debug function only. Checks that the two children are not the same, or
   * even that the children isnt the parent or this etc. Used after setting
   * pointers.
   *
   **/

  void AssertVariance();
  
  /**
   *
   * The bitset.
   *
   **/

  Bool m_bitset;

  /**
   *
   * The level of this node. Where 0 is the root, and 1 for the two nodes
   * attached to the root etc.
   *
   **/

  SInt m_depth;

  /**
   *
   * The index number.
   *
   **/
  
  SInt m_index;

  /**
   *
   * The frequency number.
   *
   **/

  SInt m_frequency;

  /**
   *
   * Pointer to the right child.
   *
   **/

  SimpleHuffNode * m_right;

  /**
   *
   * Pointer to the left child.
   *
   **/

  SimpleHuffNode * m_left;

  /**
   *
   * Pointer to the parent node.
   *
   */
  
  SimpleHuffNode * m_parent;

  /**
   *
   * Sort on index.
   *
   **/
  
  Bool m_sort_by_index;
  
};

#endif
