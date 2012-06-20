/****************************************************************************/
/**
 *
 * @file   TestHeatWave.cpp
 * @brief  A static compliance test for the HeatWave library.
 * @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 * 
 **/

#include <iostream>
#include <HeatWave.hpp>

#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/TestListener.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/ui/qt/TestRunner.h>

using namespace std;

int
main(int argc, char ** argv)
{
  // create a test listener
  CPPUNIT_NS :: BriefTestProgressListener testprogress;
  
  // informs test-listener about testresults
  CPPUNIT_NS :: TestResult testresult;
  
  // register listener for collecting the test-results
  CPPUNIT_NS :: TestResultCollector collectedresults;
  testresult.addListener (&collectedresults);
  testresult.addListener (&testprogress);
  
  // insert test-suite at test-runner by registry
  CPPUNIT_NS :: TestRunner testrunner;
  // CPPUNIT_NS::QtUi::TestRunner testrunner;
  testrunner.addTest (CPPUNIT_NS::TestFactoryRegistry::getRegistry().
                      makeTest());
  testrunner.run (testresult);
  //testrunner.run ();
  
  // output results in compiler-format
  CPPUNIT_NS :: CompilerOutputter 
    compileroutputter (&collectedresults, std::cerr);
 
  compileroutputter.write ();
  
  // return 0 if tests were successful
  return collectedresults.wasSuccessful () ? 0 : 1;
}
