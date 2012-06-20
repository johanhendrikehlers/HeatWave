/****************************************************************************/
/**
 ** @file   CommonDataVectors.hpp
 ** @brief  Fast vector descriptors for basic data types.
 ** @author Johan Hendrik Ehlers <https://github.com/johanhendrikehlers>
 ** 
 **/

#include "CommonDataTypes.hpp"
#include "CommonDefines.hpp"

#ifndef __COMMONDATAVECTORS_HPP__
#define __COMMONDATAVECTORS_HPP__

/***********************************************************************/
#define CREATE_A_VECTOR_DESCRIPTOR(name,type)                           \
  class name                                                            \
{                                                                       \
                                                                        \
 public:                                                                \
                                                                        \
  name(SInt size = COMMON_GRANULARITY){                                 \
    memset((Char*)this,'\0',sizeof(name));                              \
    if ( size <= 0 ){                                                   \
      return;                                                           \
    }                                                                   \
    else{                                                               \
      memSize = size;                                                   \
      NEW_ARRAY(ptr,type,size);                                         \
    }                                                                   \
  }                                                                     \
                                                                        \
  name(const name & oth){                                               \
    memset((Char*)this,'\0',sizeof(name));                              \
    CopyOther(oth);                                                     \
  }                                                                     \
                                                                        \
  ~name(){                                                              \
    delete [] ptr;                                                      \
  }                                                                     \
                                                                        \
  inline type Get(const SInt offset) const{                             \
    ASSERT( offset >= 0 );                                              \
    ASSERT( offset < memUsed );                                         \
    return ptr[offset];                                                 \
  }                                                                     \
                                                                        \
  inline type * GetPtr() const{                                         \
    return ptr;                                                         \
  }                                                                     \
                                                                        \
  inline SInt Size() const{                                             \
    return memUsed;                                                     \
  }                                                                     \
                                                                        \
  inline void PushBack(type value){                                     \
    if ( memUsed == memSize ){                                          \
      type * tmp;                                                       \
      NEW_ARRAY(tmp, type, memSize + COMMON_GRANULARITY);               \
      if ( memSize ){                                                   \
        memcpy(tmp,ptr,memSize*sizeof(type));                           \
        DEL_ARRAY(ptr);                                                 \
      }                                                                 \
      ASSERT ( ptr == NULL );                                           \
      ptr = tmp;                                                        \
      memSize += COMMON_GRANULARITY;                                    \
    }                                                                   \
    ptr[memUsed]=value;                                                 \
    ++memUsed;                                                          \
  }                                                                     \
                                                                        \
  inline void Clear(){                                                  \
    memUsed = 0;                                                        \
  }                                                                     \
                                                                        \
  inline name & operator=(const name & oth){                            \
    CopyOther(oth);                                                     \
    return (*this);                                                     \
  }                                                                     \
                                                                        \
  inline Bool operator==(const name & oth) const{                       \
    if ( memUsed == oth.memUsed ){                                      \
      if ( memcmp((char*)ptr,(Char*)oth.ptr, memUsed*sizeof(type)) ){   \
        return True;                                                    \
      }                                                                 \
    }                                                                   \
    return False;                                                       \
  }                                                                     \
                                                                        \
 protected:                                                             \
                                                                        \
  void CopyOther (const name & oth){                                    \
    if ( this != &oth ){                                                \
      Clear();                                                          \
      if ( oth.Size() ){                                                \
        DEL_ARRAY(ptr);                                                 \
        NEW_ARRAY(ptr, type, oth.memSize*sizeof(type));                 \
        memcpy(ptr, oth.ptr, oth.memUsed*sizeof(type));                 \
        memSize = oth.memSize;                                          \
        memUsed = oth.memUsed;                                          \
      }                                                                 \
    }                                                                   \
  }                                                                     \
                                                                        \
  SInt memSize;                                                         \
  SInt memUsed;                                                         \
  type * ptr;                                                           \
};                                                                      

CREATE_A_VECTOR_DESCRIPTOR(SFloat64A, SFloat64)
CREATE_A_VECTOR_DESCRIPTOR(SFloat32A, SFloat32)
CREATE_A_VECTOR_DESCRIPTOR(SIntA, SInt)
CREATE_A_VECTOR_DESCRIPTOR(UIntA, UInt)
CREATE_A_VECTOR_DESCRIPTOR(SInt16A, SInt16)
CREATE_A_VECTOR_DESCRIPTOR(UInt16A, UInt16)
CREATE_A_VECTOR_DESCRIPTOR(SInt8A, SInt8)
CREATE_A_VECTOR_DESCRIPTOR(UInt8A, UInt8)
                                        
#endif //__COMMONDATAVECTORS_HPP__

