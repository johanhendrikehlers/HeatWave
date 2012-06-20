/****************************************************************************/
/**
 *
 * @file   TestHeatWaveLift.cpp
 * @brief  A test fixture for the HeatWaveLift class.
 * @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 * 
 **/

#include <TestHeatWaveLift.hpp>
#include <iomanip>

CPPUNIT_TEST_SUITE_REGISTRATION (TestHeatWaveLift);

// local variables
#define TEST_NUMBER_DATA_INPUTS 5
#define LONGEST_LENGTH (19+1)
#define FULL_LENGTH (LONGEST_LENGTH+2)
#define PTR_OFFSET (1)
#define S (99) // place holder
#define E (-1) // out of array bound marker 
enum                {even, odd, evenS, oddS, oddX};
Smpl dataLength[] = {   8,   9,    16,   17,   19};
Smpl dataNext[]   = {   1,   1,     2,    2,    1};

Smpl  dataFull[TEST_NUMBER_DATA_INPUTS][FULL_LENGTH];
Smpl  spltFull[TEST_NUMBER_DATA_INPUTS][FULL_LENGTH];
Smpl * dataPtr[TEST_NUMBER_DATA_INPUTS];
Smpl * spltPtr[TEST_NUMBER_DATA_INPUTS];
const Smpl * dataOrg[TEST_NUMBER_DATA_INPUTS];
const Smpl * spltOrg[TEST_NUMBER_DATA_INPUTS];

const Smpl dataEven[]  = {0,1,2,3,4,5,6,7};
const Smpl spltEven[]  = {0,2,4,6,1,3,5,7};

const Smpl dataOdd[]   = {0,1,2,3,4,5,6,7,8};
const Smpl spltOdd[]   = {0,2,4,6,8,1,3,5,7};

const Smpl dataEvenS[] = {0,S,1,S,2,S,3,S,4,S,5,S,6,S,7,S};
const Smpl spltEvenS[] = {0,S,2,S,4,S,6,S,1,S,3,S,5,S,7,S};

const Smpl dataOddS[]  = {0,S,1,S,2,S,3,S,4,S,5,S,6,S,7,S,8};
const Smpl spltOddS[]  = {0,S,2,S,4,S,6,S,8,S,1,S,3,S,5,S,7};

const Smpl dataOddX[]  = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18};
const Smpl spltOddX[]  = {0,  2,  4,  6,  8,  10,   12,   14,   16,   18,
                            1,  3,  5,  7,  9,   11,   13,   15,   17   };

const Smpl lBndEven[]  = {0,2,4,6};
const Smpl hBndEven[]  = {1,3,5,7};
const Smpl lbndOdd[]   = {0,2,4,6,8};
const Smpl hBndOdd[]   = {1,3,5,7};

// init/reset data 
void
Reset_Data(){
  // original
  dataOrg[even] = dataEven;
  spltOrg[even] = spltEven;
  dataOrg[odd]  = dataOdd;
  spltOrg[odd]  = spltOdd;
  dataOrg[evenS] = dataEvenS;
  spltOrg[evenS] = spltEvenS;
  dataOrg[oddS]  = dataOddS;
  spltOrg[oddS]  = spltOddS;
  dataOrg[oddX]  = dataOddX;
  spltOrg[oddX]  = spltOddX;
  
  // reset
  for ( SInt a = 0 ; a < TEST_NUMBER_DATA_INPUTS ; ++a ){
    for ( SInt i = 0 ; i < FULL_LENGTH ; ++i ){
      dataFull[a][i] = E;
    }
  }
  for ( SInt a = 0 ; a < TEST_NUMBER_DATA_INPUTS ; ++a ){
    for ( SInt i = 0 ; i < dataLength[a] ; ++i ){
      dataFull[a][i+PTR_OFFSET] = (dataOrg[a])[i];
      spltFull[a][i+PTR_OFFSET] = (spltOrg[a])[i];
    }
  }
  for ( SInt a = 0 ; a < TEST_NUMBER_DATA_INPUTS ; ++a ){
    dataPtr[a] = &(dataFull[a][PTR_OFFSET]);
    spltPtr[a] = &(spltFull[a][PTR_OFFSET]);
  }
}

// vecA org vector
// vecB chk vestor
// vecBP chk vector plus left and right pad 
// length of vecA, vecB and (vecBO - 2)

Bool 
Compare_Vectors(const Smpl * vecA, const Smpl * vecB, const Smpl * vecAO, 
                SInt length)
{
  Bool check = True;
  for ( SInt i = 0 ; i < length ; ++i ){
    check &= (vecA[i] == vecB[i]);
  }
  if (!check){
    cerr << " Vectors did not match! " << endl;
    cerr << " ((1) What vector should have been.)" << endl;
    cerr << " ((2) What vector is.)" << endl;
    cerr << " ((3) What vector is plus left and right pad bytes.)" << endl;
    ios_base::fmtflags pre = cerr.flags();
    cerr.flags( ios::right | ios::dec | ios::showbase );
    streamsize size = cerr.width();
    const SInt width = 3;
    const Char * pad = " ";
    cerr << " (1)" << setw(width) << pad;
    for ( SInt i = 0 ; i < length ; ++i ){
      cerr << setw(3) << vecB[i];
    }
    cerr << setw(width) << pad << endl << " (2)" << setw(width) << pad;
    for ( SInt i = 0 ; i < length ; ++i ){
      cerr << setw(3) << vecA[i];
    }
    cerr << setw(width) << pad << endl << " (3)";
    for ( SInt i = 0 ; i < (length+2) ; ++i ){
      cerr << setw(3) << vecAO[i];
    }
    cerr << endl;
    cerr.flags(pre);
    cerr.width(size);
  }
  return check;
}

Bool 
Check_Range(SInt num, SInt low, SInt hgh, Char * nte)
{
  ASSERT ( low < hgh );
  if ( num < low || num > hgh ){
    if ( nte ){
      cerr << nte << endl;
    }
    cerr << " Number out of range! " << endl << " Number is : " << num 
         << " [" << low << "," << hgh << "]" << endl; 
    return False;
  }
  return True;
}

#define COMP_VEC_BEFORE_SPLIT(ID)                                       \
  Compare_Vectors(dataPtr[ID], dataOrg[ID], dataFull[ID], dataLength[ID])

#define COMP_VEC_AFTER_SPLIT(ID)                                        \
  Compare_Vectors(dataPtr[ID], spltOrg[ID], dataFull[ID], dataLength[ID])

#define COMP_VEC_AFTER_JOIN(ID)                                         \
  Compare_Vectors(dataPtr[ID], dataOrg[ID], dataFull[ID], dataLength[ID])

void 
TestHeatWaveLift::SplitAndJoin (void){
  // for each of the lifts and data sets ...
  for ( SInt l = 0 ; l < TEST_NUMBER_LIFTS ; ++l ){
    for ( SInt a = 0 ; a < TEST_NUMBER_DATA_INPUTS ; ++a ){
      Reset_Data();
      Smpl * even, * odd;
      CPPUNIT_ASSERT(COMP_VEC_BEFORE_SPLIT(a));
      liftList[l]->Split(dataPtr[a], dataLength[a], dataNext[a], even, odd);
      CPPUNIT_ASSERT(COMP_VEC_AFTER_SPLIT(a));
      liftList[l]->Join(dataPtr[a], dataLength[a], dataNext[a]);
      CPPUNIT_ASSERT(COMP_VEC_AFTER_JOIN(a));
      /*
      CPPUNIT_ASSERT(Compare_Vectors(even, even_only, 4));
      CPPUNIT_ASSERT(Compare_Vectors(odd, odd_only, 4));
      liftList[l]->Join(even_length,8,1);
      CPPUNIT_ASSERT(Compare_Vectors(even_length, even_check, 8));
      // check the split (with skip)
      liftList[l]->Split(even2_length,8,2,even,odd);
      CPPUNIT_ASSERT(Compare_Vectors(even2_length, even2_split, 8));
      liftList[l]->Join(even2_length,8,2);
      CPPUNIT_ASSERT(Compare_Vectors(even2_length, even2_check, 8));
      liftList[l]->Split(odd_length,7,1,even,odd);
      CPPUNIT_ASSERT(Compare_Vectors(odd_length, odd_split, 7));
      CPPUNIT_ASSERT(Compare_Vectors(even, even_only, 4));
      CPPUNIT_ASSERT(Compare_Vectors(odd, odd_only, 3));
      liftList[l]->Join(odd_length,7,1);
      CPPUNIT_ASSERT(Compare_Vectors(odd_length, odd_check, 7));
      */
    }
  } 
}

void 
TestHeatWaveLift::SplitAndJoinOpt (void){
  // for each of the lifts and data sets ...
  for ( SInt l = 0 ; l < TEST_NUMBER_LIFTS ; ++l ){
    for ( SInt a = 0 ; a < TEST_NUMBER_DATA_INPUTS ; ++a ){
      Reset_Data();
      Smpl * even, * odd;
      CPPUNIT_ASSERT(COMP_VEC_BEFORE_SPLIT(a));
      liftList[l]->SplitOpt(dataPtr[a], dataLength[a], dataNext[a], even, odd);
      CPPUNIT_ASSERT(COMP_VEC_AFTER_SPLIT(a));
      liftList[l]->JoinOpt(dataPtr[a], dataLength[a], dataNext[a]);
      CPPUNIT_ASSERT(COMP_VEC_AFTER_JOIN(a));
    }
  } 
}

void 
TestHeatWaveLift::SplitAndJoinZ (void){
  // for each of the lifts and data sets ...
  for ( SInt l = 0 ; l < TEST_NUMBER_LIFTS ; ++l ){
    for ( SInt a = 0 ; a < TEST_NUMBER_DATA_INPUTS ; ++a ){
      Reset_Data();
      Smpl * even, * odd;
      CPPUNIT_ASSERT(COMP_VEC_BEFORE_SPLIT(a));
      liftList[l]->SplitZ(dataPtr[a], dataLength[a], dataNext[a], even, odd);
      CPPUNIT_ASSERT(COMP_VEC_AFTER_SPLIT(a));
      liftList[l]->JoinZ(dataPtr[a], dataLength[a], dataNext[a]);
      CPPUNIT_ASSERT(COMP_VEC_AFTER_JOIN(a));
      /*
      CPPUNIT_ASSERT(Compare_Vectors(even, even_only, 4));
      CPPUNIT_ASSERT(Compare_Vectors(odd, odd_only, 4));
      liftList[l]->Join(even_length,8,1);
      CPPUNIT_ASSERT(Compare_Vectors(even_length, even_check, 8));
      // check the split (with skip)
      liftList[l]->Split(even2_length,8,2,even,odd);
      CPPUNIT_ASSERT(Compare_Vectors(even2_length, even2_split, 8));
      liftList[l]->Join(even2_length,8,2);
      CPPUNIT_ASSERT(Compare_Vectors(even2_length, even2_check, 8));
      liftList[l]->Split(odd_length,7,1,even,odd);
      CPPUNIT_ASSERT(Compare_Vectors(odd_length, odd_split, 7));
      CPPUNIT_ASSERT(Compare_Vectors(even, even_only, 4));
      CPPUNIT_ASSERT(Compare_Vectors(odd, odd_only, 3));
      liftList[l]->Join(odd_length,7,1);
      CPPUNIT_ASSERT(Compare_Vectors(odd_length, odd_check, 7));
      */
    }
  } 
}

void 
TestHeatWaveLift::SplitAndJoinN (void){
  for ( SInt l = 0 ; l < TEST_NUMBER_LIFTS ; ++l ){
    for ( SInt a = 0 ; a < TEST_NUMBER_DATA_INPUTS ; ++a ){
      Reset_Data();
      Smpl * even, * odd;
      CPPUNIT_ASSERT(COMP_VEC_BEFORE_SPLIT(a));
      liftList[l]->SplitN(dataPtr[a], dataLength[a], dataNext[a], even, odd);
      CPPUNIT_ASSERT(COMP_VEC_AFTER_SPLIT(a));
      liftList[l]->JoinN(dataPtr[a], dataLength[a], dataNext[a]);
      CPPUNIT_ASSERT(COMP_VEC_AFTER_JOIN(a));
    }
  }
}

#define COMP_ORG_AND_HLF()                              \
  Compare_Vectors(org_ptr, hlf_ptr, test_data_org, len)

#define COMP_ORG_AND_DUP()                              \
  Compare_Vectors(org_ptr, dup_ptr, test_data_org, len)

void 
TestHeatWaveLift::SplitAndJoinAll (void){
  // virtual array len and steps between data (step must be 1 or more)
  const SInt vir_max_len = 128;
  const SInt vir_max_stp = 16;
  const SInt tru_max_len = ((vir_max_len)*vir_max_stp)+2;

  Smpl test_data_org[tru_max_len];
  Smpl test_data_hlf[tru_max_len];
  Smpl test_data_dup[tru_max_len];
  Smpl * org_ptr = &(test_data_org[1]);
  Smpl * dup_ptr = &(test_data_dup[1]);
  Smpl * hlf_ptr = &(test_data_hlf[1]);
  Smpl * even = NULL, * odd = NULL;
  
  for ( SInt stp = 2; stp <= vir_max_stp ; ++stp ){
    for ( SInt virLen = 1 ; virLen <= vir_max_len ; ++virLen ){
      
      for ( SInt i = 0 ; i < tru_max_len ; ++i ){
        test_data_org[i] = test_data_dup[i] = test_data_hlf[i] = E;    
      }
      
      for (SInt i = 0 ; i < virLen ; ++i ){
        org_ptr[i*stp] = dup_ptr[i*stp] = hlf_ptr[i*stp] = (Smpl)(i%SMPL_MAX);
        ASSERT((i*stp)<tru_max_len);
      }
      
      SInt len = virLen*stp;
      
      // we assume "Split" works
      liftList[0]->Split(hlf_ptr, len, stp, even, odd);
      for ( SInt i = 0 ; i < len ; ++i ){                               
        org_ptr[i] = hlf_ptr[i];                                        
      }
      CPPUNIT_ASSERT((COMP_ORG_AND_HLF()));
      liftList[0]->Join(org_ptr, len, stp);
      CPPUNIT_ASSERT((COMP_ORG_AND_DUP()));

      // test "Opt"
      liftList[0]->SplitOpt(org_ptr, len, stp, even, odd);
      CPPUNIT_ASSERT((COMP_ORG_AND_HLF()));
      liftList[0]->JoinOpt(org_ptr, len, stp);
      CPPUNIT_ASSERT((COMP_ORG_AND_DUP()));
      
      // test "Z"
      liftList[0]->SplitZ(org_ptr, len, stp, even, odd);
      CPPUNIT_ASSERT((COMP_ORG_AND_HLF()));
      liftList[0]->JoinZ(org_ptr, len, stp);
      CPPUNIT_ASSERT((COMP_ORG_AND_DUP()));
      
      // test "N"
      liftList[0]->SplitN(org_ptr, len, stp, even, odd);
      CPPUNIT_ASSERT((COMP_ORG_AND_HLF()));
      liftList[0]->JoinN(org_ptr, len, stp);
      CPPUNIT_ASSERT((COMP_ORG_AND_DUP()));

      // done
      ASSERT(test_data_hlf[0] == E);
      ASSERT(test_data_hlf[len+1] == E);
      ASSERT(test_data_org[0] == E);
      ASSERT(test_data_org[len+1] == E);
      ASSERT(test_data_dup[0] == E);
      ASSERT(test_data_dup[len+1] == E);
    }
  }
}

// setUp/tearDown functions
void 
TestHeatWaveLift::setUp (){  
  liftList[0] = liftA = new HeatWaveLift(-1);
  liftList[1] = liftB = new HeatWaveLift(0);
  liftList[2] = liftC = new HeatWaveLift(1);
}

void 
TestHeatWaveLift::tearDown (){
  delete liftA;
  delete liftB;
  delete liftC;  
}
