/****************************************************************************/
/**
 *
 * @file   StatHeatWave.cpp
 * @brief  Return some statistics for the HeatWave library.
 * @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 * 
 **/

#include <iostream>
#include "HeatWave.hpp"
using namespace std;

/**
 *
 * Compare two vectors
 * 
 * @param bm Bench mark to compare against
 * @param oth The other vector to check.
 * @param length The length of both vectors. 
 * @return True if equal, false otherwise. 
 *
 **/

Bool 
Compare_Vectors(Smpl * bm, Smpl * oth, SInt length)
{
  Bool check = True;
  for ( SInt i = 0 ; i < length ; ++i ){
    check &= (bm[i] == oth[i]);
  }
  if (!check){
    cerr << " Vectors did not match! " << endl;
    cerr << " Vector is : ";
    for ( SInt i = 0 ; i < length ; ++i ){
      cout << bm[i] << " ";
    }
    cout << endl << " Should be : ";
    for ( SInt i = 0 ; i < length ; ++i ){
      cout << oth[i] << " ";
    }
    cout << endl;
  }
  return check;
}

/**
 *
 * Check that a number is in certain range.
 * 
 * @param num Number to check
 * @param low Lower end of range.
 * @param hgh Higher end of range.
 * @param nte Note to print out on error.
 * @return True if in range, false otherwise. 
 *
 **/

Bool 
Check_Range(SInt num, SInt low, SInt hgh, Char * nte)
{
  ASSERT ( low < hgh );
  Bool check = True;
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

/****************************************************************************/
/** 
 *
 * The main application function
 * @param argc Number of command line arguments.
 * @param argv Pointers to each command line argument. 
 * @return 0 on success, 1 to 255 on errors.
 * 
 **/

SInt
AppMain(SInt argc, Char ** argv)
{
  /**************************************************************************/
  /* Step 1 : HeatWaveImage Color Transform Test ICT                        */
  {
#define SHIFTr (1 << 16)
#define SHIFTg (1 <<  8)
#define SHIFTb (1 <<  0)
#define MAX_CLRS (1 << 24)
 
    cout << "Step 1 : Color Transform Stats - RGB to YUV to RGB" << endl;
    HeatWaveImage img(0, 0, 1, MAX_CLRS, SpcRGB, 3);
    HeatWaveComponent ** cmp;
    enum {red = 0, green, blue};
    enum {Y = 0, U, V};
    
    // setup a test RGB image where each pixel value is used only once ...
    cout << "       : Setup a RGB test image 1 by 16,777,216 ..." << endl;
    img.SetSpace(SpcRGB, True);
    cmp = img.GetComponentA();
    for ( SInt R = 0; R < 256 ; R += 1 ){
      for ( SInt G = 0; G < 256 ; G += 1 ){
        for ( SInt B = 0; B < 256 ; B += 1 ){
          SInt w = (R*SHIFTr)+(G*SHIFTg)+(B*SHIFTb);
          ASSERT ( w >= 0 );
          ASSERT ( w < MAX_CLRS );
          cmp[red  ]->SetSmpl(0,w,R);
          cmp[green]->SetSmpl(0,w,G);
		  cmp[blue ]->SetSmpl(0,w,B);
        }
      }
    }
    
    // convert the RGB image to YUV ...
    cout << "       : Convert to YUV - " << endl;
    
    img.SetSpace(SpcYUV, False, -1, False);

    // convert the YUV image back to RGB ...
    img.SetSpace(SpcRGB, False, -1, False);
    cmp = img.GetComponentA();

    // create a array we will use to cound how many RGB pixels were "hit" ...
    SInt * hit_cntr = new SInt[MAX_CLRS];
    for ( SInt w = 0; w < (MAX_CLRS) ; ++w ){
      ASSERT ( w >= 0 );
      ASSERT ( w < MAX_CLRS );
      hit_cntr[w] = 0;
    }
    SInt Br, Bg, Bb;
    for ( SInt w = 0; w < (MAX_CLRS) ; ++w ){
      Br = cmp[red  ]->GetSmpl(0,w);
      Bg = cmp[green]->GetSmpl(0,w);
      Bb = cmp[blue ]->GetSmpl(0,w);
      SInt offset = (Br*SHIFTr)+(Bg*SHIFTg)+(Bb*SHIFTb); 
      ASSERT ( offset >= 0 );
      ASSERT ( offset < MAX_CLRS );
      hit_cntr[offset] += 1;
    }
    SInt total_hit = 0;
    for ( SInt w = 0; w < (MAX_CLRS) ; ++w ){
      if (hit_cntr[w]){
        total_hit ++;
      }
    }

    SFloat64 ratio = total_hit;
    ratio /= (SFloat64)(MAX_CLRS);
    cout << "Transform is 1 to " << ratio << endl;
    delete hit_cntr;
  }
  
  /**************************************************************************/
  return 0;
}

/****************************************************************************/
/** 
 ** The process entry. 
 ** @param argc Number of command line arguments.
 ** @param argv Pointers to each command line argument. 
 ** @return 0 on success, 1 to 255 on errors.
 **
 **/

#include <iostream>
using namespace std;

int
main (SInt argc, Char ** argv)
{
  try
    {
      return AppMain(argc, argv);
    }
  catch (const exception e)
    {
      cerr << "Exit : Very Bad!\n"
           << "Standard exeption caught: " << e.what() << endl;
      return 2;
    }
  catch (...)
    {
      cerr << "Exit : Extremely Bad!\n"
           << "Unknown exeption caught!" << endl;
      return 3;
    }
  return 0;
}

