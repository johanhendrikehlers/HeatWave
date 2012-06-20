/****************************************************************************/
/**
 ** @file   MiscToolInfo.cpp
 ** @brief  Contains (some) handlers for MiscTool, relating to image 
 **         information feedback and some other minor ...
 ** @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 **
 **/

#include "MiscTool.hpp"

SInt
MiscTool::DoMainImgHash(EnumFunctionDuty duty, SInt argc, const Char ** argv)
{ 
  // set up a ArgInfo struct
  enum{ arg_img = 0, arg_cmp, arg_total};
  MiscArgInfo info(arg_total);
  info.singleName = "-sum";
  info.doubleName = "--checksum";
  info.description = "create a combined md5 checksum";
  /** @todo make is so we can use mutli checksums later. **/
  //info.descriptionLong = "available checksums include: CRC32, ADLER32, "
  //  "MD5 (default), MD4, SHA1/SHA256, HAVAL, RIPEMD160, TIGER, TIGER128, "
  //  "TIGER160 and GHOST";

  
  // perform the minor duty's
  if( duty != Dty_Perform ){
    return DoMinorDuty(duty, info, argc, argv);
  };
  
  // perform major duty
  SInt ret = DoArgInfoRecognition(info, argc, argv);
  if ( !CheckImgNum(0,1) ){
    return Err_Other;
  }
  
#ifndef WIN32
  MHASH td;
  UInt8 *hash;
  td = mhash_init(MHASH_MD5);
  if (td == MHASH_FAILED){
    fprintf(m_stdE,"%s mhash library failed (mhash_init())",ERR_M);
    return Err_Other;
  }
  for ( SInt i = 0 ; i < m_images.GetImageN() ; ++i ){
    HeatWaveImage & img = m_images.GetImage(i);
    for ( SInt c = 0 ; c < img.GetComponentN() ; ++c ){
      HeatWaveComponent & cmp = img.GetComponent(c);
      Char * data = (Char*)cmp.GetData();
      mhash(td,data,cmp.GetSize()*sizeof(Smpl));
    }
  }
  hash = (UInt8*)mhash_end(td);
  fprintf(m_stdO,"%s MD5 Hash: ",RES_M);
  for (SInt i = 0; i < (SInt) mhash_get_block_size(MHASH_MD5); i++) {
    fprintf(m_stdO,"%.2x", hash[i]);
  }
  fprintf(m_stdO,"\n");
  delete hash;
  return ret;
#else
  fprintf(m_stdO,"Option not yet supported on Windows\n");
  return Err_Other;
#endif 
}

SInt
MiscTool::DoMainImgList(EnumFunctionDuty duty, SInt argc, const Char ** argv)
{ 
  // set up a ArgInfo struct
  MiscArgInfo info(0);
  info.singleName = "-list";
  info.description = "list all the transforms that is supported";
  
  // perform the minor duty's
  if( duty != Dty_Perform ){
    return DoMinorDuty(duty, info, argc, argv);
  };
  
  // perform major duty
  SInt ret = DoArgInfoRecognition(info, argc, argv);
  fprintf(m_stdO,"%s transform list :",RES_M);
  for ( SInt i = Trn0_0 ; i < TrnTotal ; ++ i ){
    fprintf(m_stdO,"%s ",TransformName((EnumTransform)i));
  }
  fprintf(m_stdO,"\n");
  return ret;
}

SInt
MiscTool::DoMainImgInfo(EnumFunctionDuty duty, SInt argc, const Char ** argv)
{ 
  // set up a ArgInfo struct
  MiscArgInfo info(0);
  info.singleName = "-i";
  info.doubleName = "--info";
  info.description = "return various statistics for each component";
  
  // perform the minor duty's
  if( duty != Dty_Perform ){
    return DoMinorDuty(duty, info, argc, argv);
  };
  
  // perform major duty
  SInt ret = DoArgInfoRecognition(info, argc, argv);
  if ( !CheckImgNum(0,1) ){
    return Err_Other;
  }
  
  // return results for each component in each image. 
  for ( SInt i = 0 ; i < m_images.GetImageN() ; ++i ){
	HeatWaveImage & img = m_images.GetImage(i);
    fprintf(m_stdO,"%s Image[%d] tlx: %d\n",RES_M,i,img.GetTLX());
    fprintf(m_stdO,"%s Image[%d] tly: %d\n",RES_M,i,img.GetTLY());
    fprintf(m_stdO,"%s Image[%d] width: %d\n",RES_M,i,img.GetWidth());
    fprintf(m_stdO,"%s Image[%d] height: %d\n",RES_M,i,img.GetHeight());
    fprintf(m_stdO,"%s Image[%d] color space: %s\n",RES_M,i,
            SpaceName(img.GetSpace()));
    fprintf(m_stdO,"%s Image[%d] components: %d\n",RES_M,i,
            img.GetComponentN());
    for ( SInt c = 0 ; c < img.GetComponentN() ; ++c ){
      HeatWaveComponent & cmp = img.GetComponent(c);
      Smpl min, max, total;
      SFloat64 mean, std_dev, std_err, rms;
      cmp.GetStats(min,max,total,mean,std_dev,std_err,rms);
      fprintf(m_stdO,"%s Image[%d,%d] color name: %s\n",RES_M,i,c,
              ColorName(cmp.GetColor()));
      fprintf(m_stdO,"%s Image[%d,%d] tlx: %d\n",RES_M,i,c,cmp.GetTLX());
      fprintf(m_stdO,"%s Image[%d,%d] tly: %d\n",RES_M,i,c,cmp.GetTLY());
      fprintf(m_stdO,"%s Image[%d,%d] width: %d\n",RES_M,i,c,cmp.GetWidth());
      fprintf(m_stdO,"%s Image[%d,%d] height: %d\n",RES_M,i,c,cmp.GetHeight());
      fprintf(m_stdO,"%s Image[%d,%d] h step: %d\n",RES_M,i,c,cmp.GetHStep());
      fprintf(m_stdO,"%s Image[%d,%d] v step: %d\n",RES_M,i,c,cmp.GetVStep());
      fprintf(m_stdO,"%s Image[%d,%d] size: %d\n",RES_M,i,c,cmp.GetSize());
      fprintf(m_stdO,"%s Image[%d,%d] precision: %d (%s)\n",RES_M,i,c,
              cmp.GetPrec(), cmp.GetSgnd() ? "signed" : "unsigned");
      fprintf(m_stdO,"%s Image[%d,%d] minimum: %d\n",RES_M,i,c,min);
      fprintf(m_stdO,"%s Image[%d,%d] maximum: %d\n",RES_M,i,c,max);
      fprintf(m_stdO,"%s Image[%d,%d] sum: %d\n",RES_M,i,c,total);
      fprintf(m_stdO,"%s Image[%d,%d] mean: %f\n",RES_M,i,c,mean);
      fprintf(m_stdO,"%s Image[%d,%d] standard deviation: %f\n",RES_M,i,c,
              std_dev);
      fprintf(m_stdO,"%s Image[%d,%d] standard error: %f\n",RES_M,i,c,
              std_err);
      fprintf(m_stdO,"%s Image[%d,%d] root mean square: %f\n",RES_M,i,c,
              rms);
      fprintf(m_stdO,"%s Image[%d,%d] Shannons entropy: %5f bpp\n",RES_M,
              i,c,cmp.GetEntropy());
    }
  }
  return ret;
}

SInt
MiscTool::DoMainImgPlot(EnumFunctionDuty duty, SInt argc, const Char ** argv)
{ 
  // prepare the MiscArgInfo struct ... 
  SInt ret = 0;
  enum{ arg_img, arg_cmp, arg_area, arg_hist, arg_total};  
  MiscArgInfo info(arg_total);
  info.singleName = "-gp";
  info.doubleName = "--gnuplot";
  info.description = "get plot information (suitable for \"gnuplot\")";
  InfoUsesImgs(info, arg_img);
  InfoUsesCmps(info, arg_cmp);
  InfoUsesAreas(info, arg_area);

  info.subName[arg_hist] = "histogram";
  info.subDesc[arg_hist] = "print the histogram";
  info.subFlag[arg_hist] = Att_F;

  // include null values or not (another option ...) ...
  
  // exit on minor duty's ...
  if( duty != Dty_Perform ){
    return DoMinorDuty(duty, info, argc, argv);
  }
  else{
    ret = DoArgInfoRecognition(info, argc, argv);
  }

  // open a file if requested ...
  FILE * out = m_stdO;
  SIntA imgA = InfoAtoiImgs(info, arg_img, 1);
  if ( !(imgA.Size()) ){
    return Err_Other;
  }
  // IMAGE
  for ( SInt i = 0 ; i < imgA.Size() ; ++i ){
    SInt img_n = imgA.Get(i);
    if (m_verbose){
      fprintf(m_stdO,"%s entering image %d\n", VRB_M, img_n);
    }
    // COMPONENT
    SIntA cmpA = InfoAtoiCmps(info, arg_cmp, img_n);
    for ( SInt c = 0 ; c < cmpA.Size() ; ++c ){
      SInt cmp_n = cmpA.Get(c);
      if (m_verbose){
        fprintf(m_stdO,"%s entering component %d\n", VRB_M,cmp_n);
      }
      // AREA
      RctA rctA = InfoAtoiRcts(info, arg_cmp, img_n, cmp_n);
      for ( SInt a = 0 ; a < rctA.Size() ; ++a ){
        Rct area_n = rctA.Get(a);
        if (m_verbose){
          fprintf(m_stdO,"%s entering area %d : (%d,%d) %dw x %dh\n", 
                  VRB_M, a, area_n.x, area_n.y, area_n.w, area_n.h);
        }
        // WORK IT ...
        //Bool GetHistogram(SInt tlx, SInt tly, SInt width, SInt height, Smpl *& hist,
        //SInt & range, SInt & shift) const;
        Smpl * hist = NULL;
        SInt range = 0, shift = 0;
        if ( !m_images.GetImage(img_n).
             GetComponent(cmp_n).
             GetHistogram(area_n.x, area_n.y, area_n.w, area_n.h, hist, range, shift) ){
          fprintf(m_stdE,"%s histogram failed, possible bad area",ERR_M);
          return Err_Other;
        }
        for ( SInt h = 0 ; h < range ; ++h ){
          fprintf(out,"%d\t%d\n",h-shift,hist[h]);
        }
        DEL_ARRAY(hist);
        // ... TILL HERE
      }
    }
  }
  return ret;
}
