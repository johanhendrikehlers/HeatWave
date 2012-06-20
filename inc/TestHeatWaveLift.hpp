/****************************************************************************/
/**
 *
 * @file   TestHeatWaveLift.hpp
 * @brief  A test fixture for the HeatWaveLift class.
 * @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 * 
 **/

#ifndef __TESTHEATWAVELIFT_HPP__
#define __TESTHEATWAVELIFT_HPP__

#define TEST_NUMBER_LIFTS 3

#include <HeatWaveLift.hpp>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

using namespace std;

class TestHeatWaveLift : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE (TestHeatWaveLift);
  CPPUNIT_TEST (SplitAndJoin);
  CPPUNIT_TEST (SplitAndJoinOpt);
  CPPUNIT_TEST (SplitAndJoinZ);
  CPPUNIT_TEST (SplitAndJoinN);
  CPPUNIT_TEST (SplitAndJoinAll);
  CPPUNIT_TEST_SUITE_END ();

public:
  void setUp (void);
  void tearDown (void);
  
protected:
  void SplitAndJoin   (void);
  void SplitAndJoinOpt(void);
  void SplitAndJoinZ  (void);
  void SplitAndJoinN  (void);
  void SplitAndJoinAll(void);

private:
  HeatWaveLift * liftA, * liftB, * liftC;
  HeatWaveLift * liftList[TEST_NUMBER_LIFTS];
};

#endif
