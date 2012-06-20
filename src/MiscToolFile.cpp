/****************************************************************************/
/**
 ** @file   MiscToolFile.cpp
 ** @brief  Contains (some) argument handlers for MiscTool, relating to file 
 **         handling.
 ** @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 **
 **/

#include "MiscTool.hpp"

SInt
MiscTool::DoMainImgLoad(EnumFunctionDuty duty, SInt argc, const Char ** argv)
{
  // set up a ArgInfo struct 
  enum{ arg_type = 0, arg_total};
  MiscArgInfo info(arg_total);
  info.singleName = "-l";
  info.doubleName = "--load-image";
  info.description = "Load image(s)";
  info.strDes = "file";
  info.flag = Att_FR|Att_SN|Att_Multi;
  
  info.subName[arg_type] = "type=";
  info.subDesc[arg_type] = "Specify a image format; bmp, jpg etc.";
  info.subFlag[arg_type] = Att_S|Att_TR;
  info.subStrDes[arg_type] = "fmt";
  
  // perform the minor duty's  
  if( duty != Dty_Perform ){
    return DoMinorDuty(duty, info, argc, argv);
  };
  
  // perform major duty 
  SInt ret = DoArgInfoRecognition(info, argc, argv);
  const Char * type = NULL;
  //SInt nxt_img = 0;
  HeatWaveImage * tmp = NULL;
  
  if ( info.subFlag[arg_type] & Att_Set ){
    type = info.subStr[arg_type][0];
  }

  for ( SInt i = 0 ; i < info.strNum ; ++i ){
    if ( m_verbose ){
      if ( strcmp (info.str[i],"stdin") == 0 ){
        fprintf(m_stdO,"%s reading image from standard input\n", VRB_M);
        info.str[i] = NULL;
      }
      else {
        fprintf(m_stdO,"%s reading image from file \"%s\"", VRB_M,
                info.str[i]);
      }
      if (type){
        fprintf(m_stdO," as type \"%s\"\n", type);
      }
      else{
        fprintf(m_stdO,"\n");
      }
    }
    tmp = m_imgTool.ReadImage(info.str[i], type, NULL);
    if ( !tmp ){
      fprintf(m_stdO,"%s loading image \"%s\" failed: %s\n", VRB_M,
              info.str[i], m_imgTool.GetLastErrorMessage());
      return Err_Other;
    }
    m_images.AddImage(tmp);
    ++i;
  }
  if ( m_verbose ){
    fprintf(m_stdO,"%s image(s) loaded ok, array lenght now %d\n", VRB_M,
            m_images.GetImageN());
  } 
  return ret;
}

SInt
MiscTool::DoMainImgVidL(EnumFunctionDuty duty, SInt argc, const Char ** argv)
{
  // set up a ArgInfo struct ...
  MiscArgInfo info(0);
  info.singleName = "-lv";
  info.doubleName = "--load-video";
  info.description = "load a video";
  info.strDes = "file";
  info.flag = Att_FR|Att_SN;
  
  // perform the minor duty's ...
  if( duty != Dty_Perform ){
    return DoMinorDuty(duty, info, argc, argv);
  };
  
  // perform major duty ...
  SInt ret = DoArgInfoRecognition(info, argc, argv);
  HeatWaveAVIReader reader;
  HeatWaveVideo * video = NULL;
  if ( !reader.OpenFile(info.str[0]) ){
    fprintf(m_stdE,"%s unable to open video \"%s\"\n", ERR_M, info.str[0]);
    return Err_Other;
  }
  if ( !reader.AnalyseFile() ){
    fprintf(m_stdE,"%s failed to anaylse file \"%s\"\n", ERR_M, info.str[0]);
    return Err_Other;
  }
  video = reader.LoadVideo();
  ASSERT ( video != NULL );
  reader.CloseFile();
  if ( m_verbose ){
    fprintf(m_stdE,"%s appending %d images to array from file \"%s\"\n",
            VRB_M, video->GetImageN(), info.str[0]);
  }
  m_images.DoAppendVideo(video);
  if ( m_verbose ){
    fprintf(m_stdE,"%s internal array has a total of %d images now\n",
            VRB_M, m_images.GetImageN());
  }
  ASSERT ( video == NULL );
  return ret;
}

SInt
MiscTool::DoMainImgSave(EnumFunctionDuty duty, SInt argc, const Char ** argv)
{ 
  // set up a ArgInfo struct ...
  enum{ arg_type = 0, arg_no, arg_codec, arg_pixel, arg_all, arg_total};
  MiscArgInfo info(arg_total);
  info.singleName = "-s";
  info.doubleName = "--save-image";
  info.description = "Save an image to a file.";
  info.descriptionLong = 
    "To write directly to standard out, use \"stdout\" as a file name.";
  info.strDes = "file";
  info.flag = Att_FR|Att_SN;
  
  info.subName[arg_type] = "type=";
  info.subDesc[arg_type] = "Specify a image format; bmp, jpg etc.";
  info.subFlag[arg_type] = Att_S|Att_TR|Att_SN;
  info.subStrDes[arg_type] = "fmt";
  
  info.subName[arg_no] = "no=";
  info.subDesc[arg_no] = 
    "Specify a specific image from video array, else last image is saved.";
  info.subFlag[arg_no] = Att_S|Att_TR|Att_IN;
  info.subStrDes[arg_no] = "int";
  
  info.subName[arg_codec] = "codec=";
  info.subDesc[arg_codec] = 
    "Pass information on to coder. (see jasper library information)";
  info.subFlag[arg_codec] = Att_S|Att_TR|Att_SN;
  info.subStrDes[arg_codec] = "string";
  
  info.subName[arg_pixel] = "pixelise";
  info.subDesc[arg_pixel] = 
    "Pixelise image before saving. (usefull for viewing transforms)";
  info.subFlag[arg_pixel] = Att_S;
  info.subName[arg_all] = "all";
  info.subDesc[arg_all] = 
    "Save all the images on the array (use %d in your file name to "
    "save the frame number position!)";
  info.subFlag[arg_all] = Att_S;
  
  // perform the minor duty's
  if( duty != Dty_Perform ){
    return DoMinorDuty(duty, info, argc, argv);
  };

  // perform major duty
  SInt ret = DoArgInfoRecognition(info, argc, argv);
  
  SInt num;
  Bool pixl = False;
  const Char * type = NULL;
  const Char * codec = NULL;
  if ( info.subStrNum[arg_type] )  { type = info.subStr[arg_type][0];}
  if ( info.subStrNum[arg_codec] ) { codec = info.subStr[arg_codec][0];}
  if ( info.subFlag[arg_pixel] & Att_Set ) {pixl = True;}

  if ( m_verbose ){
    fprintf(m_stdO,"%s preparing to save a image\n",VRB_M);
  }
  
  if ( info.subFlag[arg_all] & Att_Set ){

    if ( m_verbose ){
      fprintf(m_stdO,"%s request is to save all images\n",VRB_M);
    }
    /** @todo use regexp here %d is not enough can be %02d etc... **/
    /*if ( strstr(info.str,"%d") == NULL ){
      fprintf(m_stdE,"%s please use \"%%d\" in file name when option "
      "`all` is used\n", ERR_M);
      return Err_Other;
      }
    */
    if ( !CheckImgNum(0,1) ){
      return Err_Other;
    }

    ASSERT ( info.str );
    ASSERT ( info.str[0] );

    Char * tmp = new char[strlen(info.str[0])+10];
    LEAVEONNULL(tmp);
    for ( SInt i = 0 ; i < m_images.GetImageN() ; ++i ){
      sprintf(tmp, info.str[0], i);
      if( m_verbose ){
        fprintf(m_stdO,"%s preparing to save image \"%s\"\n",VRB_M, tmp);
      }
      if(!m_imgTool.WriteImage(m_images.GetImage(i), tmp, type, codec, pixl)){
        fprintf(m_stdE,"%s saving image \"%s\" failed: %s\n",ERR_M,tmp,
                m_imgTool.GetLastErrorMessage());
        return Err_Other;
      }
      if( m_verbose ){
        fprintf(m_stdO,"%s saved image \"%s\"\n",VRB_M, tmp);
      }
    }
    delete tmp;
    return ret;
  }
  
  
  if ( strcmp(info.str[0],"stdout") == 0 ) info.str[0] = NULL;
  if ( info.subFlag[arg_no] & Att_Set ) {
    num = atoi(info.subStr[arg_no][0]);
  }
  else{
    num = (m_images.GetImageN()-1);
  }
  if ( !CheckImgNum(num,1) ){
    return Err_Other;
  }
  
  if ( !m_imgTool.WriteImage(m_images.GetImage(num),info.str[0],
                             info.subStr[arg_type][0], info.subStr[arg_codec][0],
                             pixl) ){
    fprintf(m_stdE,"%s saving image \"%s\" failed: %s",ERR_M,info.str[0],
            m_imgTool.GetLastErrorMessage());
    return Err_Other;
  }
  if ( m_verbose ) {
    fprintf(m_stdE,"%s saved image %d on array to \"%s\"\n", VRB_M,
            num,info.str[0]);
  }
  return ret;
}

SInt
MiscTool::DoMainImgHIII(EnumFunctionDuty duty, SInt argc, const Char ** argv)
{ 
  // set up a ArgInfo struct
  enum{ arg_read = 0, arg_img, arg_cmp, arg_total};  
  MiscArgInfo info(arg_total);
  info.singleName = "-iii";
  info.description = "write or read a intermediate integer image";
  info.flag = Att_FR|Att_SN;
  info.strDes = "std|file";
  info.strDef = "std";

  info.subName[arg_read] = "read";
  info.subDesc[arg_read] = "read a iii image";
  info.subFlag[arg_read] = Att_S;
  info.subStrDes[arg_read] = "read";
  
  info.subName[arg_img] = "img=";
  info.subDesc[arg_img] = "image number to read|write";
  info.subFlag[arg_img] = Att_TR|Att_IN;
  
  info.subName[arg_cmp] = "cmp=";
  info.subDesc[arg_cmp] = "component number to read|write";
  info.subFlag[arg_cmp] = Att_TR|Att_IN;
  
  
  // perform the minor duty's
  if( duty != Dty_Perform ){
    return DoMinorDuty(duty, info, argc, argv);
  };
  
  // perform major duty
  SInt ret = DoArgInfoRecognition(info, argc, argv);
  
  // Read a iii image
  if ( info.subFlag[arg_read] & Att_Set ) {
    SInt img = -1;
    SInt cmp = -1;
    if ( info.subFlag[arg_img] & Att_Set ){
      img = atoi(info.subStr[arg_img][0]);
    }
    if ( info.subFlag[arg_cmp] & Att_Set ){
      cmp = atoi(info.subStr[arg_cmp][0]);
    }
    if ( m_verbose ){
      fprintf(m_stdE,"%s reading a iii image\n", VRB_M);
    }
    HeatWaveImage * img_ptr = NULL;
    
    // If image & component is non existend we add to end of list
    if ( ! m_images.CheckComponent(img, cmp) ){
      if ( m_verbose ){
        fprintf(m_stdE,"%s creating a new image on array\n",
                VRB_M);
      }
      img_ptr = new HeatWaveImage(0,0,1,1,SpcGrey,1);
      LEAVEONNULL(img_ptr);
      m_images.AddImage(img_ptr);
      if ( m_verbose ){
        fprintf(m_stdE,"%s internal array has a total of %d images now\n",
                VRB_M, m_images.GetImageN());
      }
      img = m_images.GetImageN()-1;
      cmp = 0;
    }

    else{
      if ( m_verbose ){
        fprintf(m_stdE,"%s using existing image %d and component %d\n",
                VRB_M, img, cmp);
      }
    }
    HeatWaveImage & img_ref = m_images.GetImage(img);
    HeatWaveComponent & cmp_ref = img_ref.GetComponent(cmp);
    
    // from standard input
    if ( strcmp("std",info.str[0]) == 0) {   
      if ( m_verbose ){
        fprintf(m_stdE,"%s reading image from standard input\n",
                VRB_M);
      }
      if ( ! cmp_ref.ReadIII(NULL,m_stdI) ) {
        fprintf(m_stdE, "%s unable to read iii image (stdin)\n", ERR_M);
        return Err_Other;
      }
    }
    else {
      if ( m_verbose ){
        fprintf(m_stdE,"%s reading image from \"%s\"\n",
                VRB_M, info.str[0]);
      }
      FILE * tmp = cmp_ref.ReadIII(info.str[0],NULL);
      if ( !tmp ){
        fprintf(m_stdE, "%s unable to read iii image \"%s\"\n", 
                ERR_M, info.str[0] );
        return Err_Other;
      }
      fclose(tmp);
      if ( img_ptr ){
        cmp_ref.SetColor(ClrGrey);
      }
    }

    // If we added a new image set its values to the components values
    if ( img_ptr ){
      img_ref.SetWidth(cmp_ref.GetWidth());
      img_ref.SetHeight(cmp_ref.GetHeight());
      img_ref.SetSpace(SpcGrey);
    }
  }
 
  // Write a iii image
  else{
    SInt img = (m_images.GetImageN()-1);
    SInt cmp = 0;
    if ( info.subFlag[arg_img] & Att_Set ){
      img = atoi(info.subStr[arg_img][0]);
    }
    if ( info.subFlag[arg_cmp] & Att_Set ){
      cmp = atoi(info.subStr[arg_cmp][0]);
    }
    if ( !CheckImgNum(img,1) ){
      return Err_Other;
    }
    if ( !CheckCmpNum(img,cmp) ){
      return Err_Other;
    }
    HeatWaveComponent & cmp_ref = 
      m_images.GetImage(img).GetComponent(cmp);
    
    // to standard input
    if ( strcmp("std",info.str[0]) == 0) {
      if ( m_verbose ){
        fprintf(m_stdE,"%s writing iii to standard output\n",
                VRB_M);
      }
      if ( ! cmp_ref.WriteIII(NULL,m_stdO) ) {
        fprintf(m_stdE, "%s unable to write iii to standard output\n", ERR_M);
        return Err_Other;
      }
    }
    else {
      if ( m_verbose ){
        fprintf(m_stdE,"%s writing iii to \"%s\"\n",
                VRB_M, info.str[0]);
      }
      FILE * tmp = cmp_ref.WriteIII(info.str[0],NULL);
      if ( !tmp ){
        fprintf(m_stdE, "%s unable to write iii file \"%s\"\n", 
                ERR_M, info.str[0] );
        return Err_Other;
      }
      fclose(tmp);
    }
  }
  return ret;
}
