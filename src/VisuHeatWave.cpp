/****************************************************************************/
/**
 *
 * @file   VisuHeatWave.cpp
 * @brief  Visualize certain feutures of HeatWave library.
 * @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 * 
 **/

#include <iostream>
#include <fstream>
#include "HeatWave.hpp"
using namespace std;

/** 
 *
 * all operations work on this geezer.
 *
 **/

ofstream fout;

/** color numbers **/
enum corn {Cred = 0, Cgreen, Cblue, Cblack, Cwhite, Cyellow, Ccyan, 
           Cmagenta, Ctotal};

/** get the color name **/
const char * name(const corn & cr){
  switch (cr){
  case Cred:
    return "red";
  case Cgreen:
    return "green";
  case Cblue:
    return "blue";
  case Cblack:
    return "black";
  case Cwhite:
    return "white";
  case Cyellow:
    return "yellow";
  case Ccyan:
    return "cyan";
  case Cmagenta:
    return "magenta";
  default:
    return "error"; 
  }
}

/** points for two boxes A & B **/
int Ax[Ctotal], Ay[Ctotal], Az[Ctotal];
int Bx[Ctotal], By[Ctotal], Bz[Ctotal];

/** constanst for box lengths **/
int range_len = 256;
int range_max = 255;

/**
 *
 * Setup plot file
 *
 **/

void plot_init(int Xmin, int Xmax, int Ymin, int Ymax, int Zmin, int Zmax,
               char * Xlabel = NULL, char * Ylabel = NULL, 
               char * Zlabel = NULL){
  fout << "reset" << endl;
  fout << "set isosample 50" << endl;
  fout << "set xrange [ " << Xmin << " to " << Xmax << " ]" << endl;
  fout << "set yrange [ " << Ymin << " to " << Ymax << " ]" << endl;
  fout << "set zrange [ " << Zmin << " to " << Zmax << " ]" << endl;
  fout << "unset tics" << endl;
  fout << "unset key" << endl;
  //fout << "set border 4095" << endl;
  fout << "set border 0" << endl;
  if ( Xlabel ){
    fout << "set xlabel \"" << Xlabel << "\"" << endl;
  }
  if ( Ylabel ){
    fout << "set ylabel \"" << Ylabel << "\"" << endl;
  }
  if ( Zlabel ){
    fout << "set zlabel \"" << Zlabel << "\"" << endl;
  }
}

/**
 *
 * Close plot file
 *
 **/

void plot_close(){
  fout << "splot -51" << endl;
  fout.flush();
  fout.close();
}

/**
 *
 * plot a arrow from A to B in 3 dimensions.
 *
 **/

void plot_arrow(int Ax, int Ay, int Az, int Bx, int By, int Bz){
  fout << "set arrow from " << Ax << "," << Ay << "," << Az << " to " 
       << Bx << "," << By << "," << Bz << " lw 0.5" << endl;
}

/**
 *
 * plot a line from A to B in 3 dimensions.
 *
 **/

void plot_line(int Ax, int Ay, int Az, int Bx, int By, int Bz){
  fout << "set arrow from " << Ax << "," << Ay << "," << Az << " to " 
       << Bx << "," << By << "," << Bz << " nohead lw 2" << endl;
}

/**
 *
 * plot a line from A to B in 3 dimensions.
 *
 **/

void plot_label(int x, int y, int z, char * lbl){
  fout << "set label \"" << lbl << "\" at " << x << "," << y << "," << z << endl; 
}

/**
 *
 * plot a box
 *
 **/
void plot_a_box()
{
  plot_line(Ax[Cblack],Ay[Cblack],Az[Cblack],
            Ax[Cred],Ay[Cred],Az[Cred]);
  plot_line(Ax[Cblack],Ay[Cblack],Az[Cblack],
            Ax[Cgreen],Ay[Cgreen],Az[Cgreen]);
  plot_line(Ax[Cblack],Ay[Cblack],Az[Cblack],
            Ax[Cblue],Ay[Cblue],Az[Cblue]);
  plot_line(Ax[Cwhite],Ay[Cwhite],Az[Cwhite],
            Ax[Cmagenta],Ay[Cmagenta],Az[Cmagenta]);
  plot_line(Ax[Cwhite],Ay[Cwhite],Az[Cwhite],
            Ax[Ccyan],Ay[Ccyan],Az[Ccyan]);
  plot_line(Ax[Cwhite],Ay[Cwhite],Az[Cwhite],
            Ax[Cyellow],Ay[Cyellow],Az[Cyellow]);
  plot_line(Ax[Cblue],Ay[Cblue],Az[Cblue],
            Ax[Ccyan],Ay[Ccyan],Az[Ccyan]);
  plot_line(Ax[Cblue],Ay[Cblue],Az[Cblue],
            Ax[Cmagenta],Ay[Cmagenta],Az[Cmagenta]);
  plot_line(Ax[Cgreen],Ay[Cgreen],Az[Cgreen],
            Ax[Ccyan],Ay[Ccyan],Az[Ccyan]);
  plot_line(Ax[Cgreen],Ay[Cgreen],Az[Cgreen],
            Ax[Cyellow],Ay[Cyellow],Az[Cyellow]);
  plot_line(Ax[Cred],Ay[Cred],Az[Cred],
            Ax[Cyellow],Ay[Cyellow],Az[Cyellow]);
  plot_line(Ax[Cred],Ay[Cred],Az[Cred],
            Ax[Cmagenta],Ay[Cmagenta],Az[Cmagenta]);
}

void plot_b_box()
{
  plot_line(Bx[Cblack],By[Cblack],Bz[Cblack],
            Bx[Cred],By[Cred],Bz[Cred]);
  plot_line(Bx[Cblack],By[Cblack],Bz[Cblack],
            Bx[Cgreen],By[Cgreen],Bz[Cgreen]);
  plot_line(Bx[Cblack],By[Cblack],Bz[Cblack],
            Bx[Cblue],By[Cblue],Bz[Cblue]);
  plot_line(Bx[Cwhite],By[Cwhite],Bz[Cwhite],
            Bx[Cmagenta],By[Cmagenta],Bz[Cmagenta]);
  plot_line(Bx[Cwhite],By[Cwhite],Bz[Cwhite],
            Bx[Ccyan],By[Ccyan],Bz[Ccyan]);
  plot_line(Bx[Cwhite],By[Cwhite],Bz[Cwhite],
            Bx[Cyellow],By[Cyellow],Bz[Cyellow]);
  plot_line(Bx[Cblue],By[Cblue],Bz[Cblue],
            Bx[Ccyan],By[Ccyan],Bz[Ccyan]);
  plot_line(Bx[Cblue],By[Cblue],Bz[Cblue],
            Bx[Cmagenta],By[Cmagenta],Bz[Cmagenta]);
  plot_line(Bx[Cgreen],By[Cgreen],Bz[Cgreen],
            Bx[Ccyan],By[Ccyan],Bz[Ccyan]);
  plot_line(Bx[Cgreen],By[Cgreen],Bz[Cgreen],
            Bx[Cyellow],By[Cyellow],Bz[Cyellow]);
  plot_line(Bx[Cred],By[Cred],Bz[Cred],
            Bx[Cyellow],By[Cyellow],Bz[Cyellow]);
  plot_line(Bx[Cred],By[Cred],Bz[Cred],
            Bx[Cmagenta],By[Cmagenta],Bz[Cmagenta]);
}

/**
 *
 * place marks inside box
 *
 **/
void mark_boxes()
{
  char tmp_org[100];
  char tmp_new[100];
  for ( int c = 0 ; c < Ctotal ; ++c ){
    tmp_org[0] = '\0';
    tmp_new[0] = '\0';
    sprintf(tmp_org,"str_org_%s",name((corn)c));
    sprintf(tmp_new,"str_new_%s",name((corn)c));
    plot_label(Ax[c],Ay[c],Az[c],tmp_org);
    plot_label(Bx[c],By[c],Bz[c],tmp_new);    
  }
}

/**
 *
 * Set a box to RGB corners
 *
 **/

void set_a_box_to_RGB()
{
  Ax[Cred] = range_max; Ay[Cred] = 0; Az[Cred] = 0;
  Ax[Cgreen] = 0; Ay[Cgreen] = range_max; Az[Cgreen] = 0;
  Ax[Cblue] = 0; Ay[Cblue] = 0; Az[Cblue] = range_max;     
  Ax[Cblack] = 0; Ay[Cblack] = 0; Az[Cblack] = 0; Ax[Cwhite] = range_max; 
  Ay[Cwhite] = range_max; Az[Cwhite] = range_max;
  Ax[Cyellow] = range_max; Ay[Cyellow] = range_max; Az[Cyellow] = 0;
  Ax[Ccyan] = 0; Ay[Ccyan] = range_max; Az[Ccyan] = range_max;
  Ax[Cmagenta] = range_max; Ay[Cmagenta] = 0; Az[Cmagenta] = range_max;
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
  /* Visualise the YUV space in the RGB */
  {
    const char * file_to_use = "vis_yuv_in_rgb_using_ict.plot";
    fout.open(file_to_use);
    if ( !fout ){
      cerr << " Error : unable to open \"" << file_to_use << "\"!" << endl;
    }
    plot_init(0,range_max+1,0,range_max+1,0,range_max+1);
    //plot_init(0,range_max+1,0,range_max+1,0,range_max+1,"R","G","B");
    //plot_init(-50,range_max,-50,range_max,-50,range_max,"R","G","B");
    
    HeatWaveImage img(0, 0, 1, 1, SpcRGB, 3);
    HeatWaveComponent ** cmp;

    enum {red = 0, green, blue};
    enum {Y = 0, U, V};
    int R, G ,B;
    int c_y, c_u , c_v;
    int gran = 255;
    
    for ( R = 0; R < range_len ; R += gran ){
      for ( G = 0; G < range_len ; G += gran ){
        for ( B = 0; B < range_len ; B += gran ){
          img.SetSpace(SpcRGB, True);
          cmp = img.GetComponentA(); 
          cmp[red]->SetSmpl(0,0,R);
          cmp[green]->SetSmpl(0,0,G);
          cmp[blue]->SetSmpl(0,0,B);	  	
          img.SetSpace(SpcYUV, False, -1, False);
          c_y = cmp[Y]->GetSmpl(0,0);
          c_u = cmp[U]->GetSmpl(0,0);
          c_v = cmp[V]->GetSmpl(0,0);
          plot_arrow(R,G,B,c_y,c_u,c_v);
        }
      }
    }
    
    //box
    set_a_box_to_RGB();
    int point = 0;
    for ( int c = 0 ; c < Ctotal ; ++c ){
      img.SetSpace(SpcRGB, True);
      cmp = img.GetComponentA(); 
      cmp[red]->SetSmpl(0,0,Ax[c]);
      cmp[green]->SetSmpl(0,0,Ay[c]);
      cmp[blue]->SetSmpl(0,0,Az[c]);	  	
      img.SetSpace(SpcYUV, False, -1, False);
      Bx[c]= cmp[Y]->GetSmpl(0,0);
      By[c]= cmp[U]->GetSmpl(0,0);
      Bz[c]= cmp[V]->GetSmpl(0,0);
    }
    plot_a_box();
    plot_b_box();
    mark_boxes();
    
    plot_arrow(Bx[Cblack],By[Cblack],Bz[Cblack],
               Bx[Cwhite],By[Cwhite],Bz[Cwhite]);
    plot_label((Bx[Cwhite]+Bx[Cblack])>>1,(By[Cwhite]+By[Cblack])>>1,
               (Bz[Cwhite]+Bz[Cblack])>>1,"Y");
    /*
      
      plot_arrow(Bx[Cblack],By[Cblack],Bz[Cblack],0,-50,255);
      plot_label(0,-50,255,"U");
      
      plot_arrow(Bx[Cblack],By[Cblack],Bz[Cblack],255,-130,0);
      plot_label(255,-130,0,"V");
    */
    
    // points
    /*
      for ( SInt R = 0; R < range_len ; R += gran ){
      for ( SInt G = 0; G < range_len ; G += gran ){
      for ( SInt B = 0; B < range_len ; B += gran ){
      // RGB to YUV (Testing the corners of the color cube)
      img.SetSpace(SpcRGB, True);
      cmp = img.GetComponentA(); 
      cmp[red]->SetSmpl(0,0,R);
      cmp[green]->SetSmpl(0,0,G);
      cmp[blue]->SetSmpl(0,0,B);	  	
      img.SetSpace(SpcYUV, False, -1, False);
      SInt c_y, c_u , c_v;
      c_y = cmp[Y]->GetSmpl(0,0);
      c_u = cmp[U]->GetSmpl(0,0);
      c_v = cmp[V]->GetSmpl(0,0);
      plot_label(c_y,c_u,c_v,"O");
      }
      }
      }
    */

    plot_close();
  }/* end of visu */
  
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

