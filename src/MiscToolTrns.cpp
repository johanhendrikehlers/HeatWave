/****************************************************************************/
/**
 ** @file   MiscToolTrns.cpp
 ** @brief  Contains (some) argument handlers for MiscTool, relating to image 
 **         transforms and some other minor tasks.
 ** @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 **
 **/

#include "MiscTool.hpp"

SInt
MiscTool::DoMainImgFuse(EnumFunctionDuty duty, SInt argc, const Char ** argv)
{ 
  // set up a ArgInfo struct
  enum{ arg_fwd = 0, arg_rev, arg_level, arg_total};
  MiscArgInfo info(arg_total);
  info.singleName = "-f";
  info.doubleName = "--fuse";
  info.description = "create a fused image";
  
  // transform
  info.subName[arg_fwd] = "fwd-trns=";
  info.subDesc[arg_fwd] = "specify a forward wavelet transform";
  info.subFlag[arg_fwd] = Att_S|Att_TR|Att_SN;
  info.subStrDes[arg_fwd] = "name";
  info.subStrDef[arg_fwd] = "6_6";

  // transform
  info.subName[arg_rev] = "rev-trns=";
  info.subDesc[arg_rev] = "specify a reverse wavelet transform";
  info.subFlag[arg_rev] = Att_S|Att_TR|Att_SN;
  info.subStrDes[arg_rev] = "name";
  info.subStrDef[arg_rev] = "6_6";
  
  // level
  info.subName[arg_level] = "level=";
  info.subDesc[arg_level] = "specify a level to transform to";
  info.subFlag[arg_level] = Att_S|Att_TR|Att_IN;
  info.subStrDes[arg_level] = "int";
  info.subStrDef[arg_level] = "4";
  
  // perform the minor duty's
  if( duty != Dty_Perform ){
    return DoMinorDuty(duty, info, argc, argv);
  };
  
  // perform major duty
  SInt ret = DoArgInfoRecognition(info, argc, argv);
  if ( m_images.GetImageN() <= 1 ){
    fprintf(m_stdE,"%s need at least 2 images to fuse\n",ERR_M);
    return Err_Other;
  }
  if ( (TransformEnum(info.subStr[arg_fwd][0])==TrnUnknown) ){
    fprintf(m_stdE,"%s unrecognized transform \"%s\"\n",ERR_M,
            info.subStr[arg_fwd][0]);
    return Err_Other;
  }
  if ( (TransformEnum(info.subStr[arg_rev][0])==TrnUnknown) ){
    fprintf(m_stdE,"%s unrecognized transform \"%s\"\n",ERR_M,
            info.subStr[arg_rev][0]);
    return Err_Other;
  }
  SInt i = atoi(info.subStr[arg_level][0]);
  if ( i < 0 ){
    fprintf(m_stdE,"%s minimum transform level is 0\n",ERR_M);
    return Err_Other;
  }
  if ( !m_images.IsSpatiallyComparable() ){
    if ( i < 0 ){
      fprintf(m_stdE,"%s images is not compatible!\n",ERR_M);
      return Err_Other;
    }
  }
  
  m_images.DoSpatialTransform(TransformEnum(info.subStr[arg_fwd][0]),i);
  HeatWaveImage * tmp = m_images.GetFusedImage();
  ASSERT ( tmp != NULL );
  m_images.AddImage(tmp); 
  m_images.DoSpatialTransform(TransformEnum(info.subStr[arg_rev][0]),0,
                              False);
  if ( m_verbose ) {
    fprintf(m_stdE,"%s appending fused image to array\n", VRB_M);
    fprintf(m_stdE,"%s internal array has a total of %d images now\n",
            VRB_M, m_images.GetImageN());
  }
  /** @bug The following is required for images that does not have even
   ** number lenghts. **/
  tmp->DoCapData(0,255);
  return ret;
}

SInt
MiscTool::DoMainImgSpat(EnumFunctionDuty duty, SInt argc, const Char ** argv)
{ 
  // set up a ArgInfo struct
  enum{ arg_trns = 0, arg_inv, arg_total};
  MiscArgInfo info(arg_total);
  info.singleName = "-st";
  info.doubleName = "--spatial-transform";
  info.description = "spatial transform all images";
  info.strDes = "level";
  info.strDef = "4";
  info.flag = Att_FO|Att_IN;

  info.subName[arg_trns] = "transform=";
  info.subDesc[arg_trns] = "specify a transform type";
  info.subFlag[arg_trns] = Att_S|Att_TR|Att_SN;
  info.subStrDes[arg_trns] = "transform";
  info.subStrDef[arg_trns] = "6_6";
  
  info.subName[arg_inv] = "inverse";
  info.subDesc[arg_inv] = "do inverse transform";
  info.subFlag[arg_inv] = Att_S;
    
  // perform the minor duty's
  if( duty != Dty_Perform ){
    return DoMinorDuty(duty, info, argc, argv);
  };
  
  // perform major duty
  SInt ret = DoArgInfoRecognition(info,argc,argv);
  if ( !CheckImgNum(0,1) ){
    return Err_Other;
  }
  if ( (TransformEnum(info.subStr[arg_trns][0])==TrnUnknown) ){
    fprintf(m_stdE,"%s unrecognized transform \"%s\"\n",ERR_M,
            info.subStr[arg_trns][0]);
    return Err_Other;
  }
  SInt level = atoi(info.str[0]);
  Bool fwd = True;
  if ( info.subFlag[arg_inv] & Att_Set ){
    fwd = False;
  }
  if ( level< 0 ){
    fprintf(m_stdE,"%s minimum transform level is 0\n",ERR_M);
    return Err_Other;
  }
  m_images.DoSpatialTransform(TransformEnum(info.subStr[arg_trns][0]),
                              level,fwd);
  return ret;
}

SInt
MiscTool::DoMainImgTemp(EnumFunctionDuty duty, SInt argc, const Char ** argv)
{ 
  // set up a ArgInfo struct#
  enum{ arg_trns = 0, arg_inv, arg_total};
  MiscArgInfo info(arg_total);
  info.singleName = "-tt";
  info.doubleName = "--temporal-transform";
  info.description = "temporal transform across all images";
  info.strDes = "level";
  info.strDef = "4";
  info.flag = Att_FO|Att_IN;
  
  info.subName[arg_trns] = "transform=";
  info.subDesc[arg_trns] = "specify a transform type";
  info.subFlag[arg_trns] = Att_S|Att_TR|Att_SN;
  info.subStrDes[arg_trns] = "transform";
  info.subStrDef[arg_trns] = "6_6";
  
  info.subName[arg_inv] = "inverse";
  info.subDesc[arg_inv] = "do inverse transform";
  info.subFlag[arg_inv] = Att_S;
    
  // perform the minor duty's
  if( duty != Dty_Perform ){
    return DoMinorDuty(duty, info, argc, argv);
  };
  
  // perform major duty
  SInt ret = DoArgInfoRecognition(info,argc,argv);
  if ( !CheckImgNum(0,1) ){
    return Err_Other;
  }
  if ( (TransformEnum(info.subStr[arg_trns][0])==TrnUnknown) ){
    fprintf(m_stdE,"%s unrecognized transform \"%s\"\n",ERR_M,
            info.subStr[arg_trns][0]);
    return Err_Other;
  }
  SInt level = atoi(info.str[0]);
  Bool fwd = True;
  if ( info.subFlag[arg_inv] & Att_Set ){
    fwd = False;
  }
  if ( level< 0 ){
    fprintf(m_stdE,"%s minimum transform level is 0\n",ERR_M);
    return Err_Other;
  }
  m_images.DoTemporalTransform(TransformEnum(info.subStr[arg_trns][0]),
                               level,fwd);
  m_images.SetMinPrecSgn();
  return ret;
}

SInt
MiscTool::DoMainImgClrT(EnumFunctionDuty duty, SInt argc, const Char ** argv)
{ 
  // set up a ArgInfo struct
  enum{ arg_ct = 0, arg_total};
  MiscArgInfo info(arg_total);
  info.singleName = "-ct";
  info.doubleName = "--color-transform";
  info.description = "color transform images";
  info.descriptionLong = "color transform images between following color" 
    " spaces: \"YUV\" (YCbCr), \"RGB\" and \"grey\". When converting between" 
    " RGB and YUV there is a choice of two convertion operations, a fully" 
    " reversible operation (RCT) as is specified for JPEG2000 and a"
    " non-reversible (traditional) color transform (ICT) as is specified for"
    " JPEG.";

  info.subName[arg_ct] = "method=";
  info.subDesc[arg_ct] = "One of \"RCT\" or \"ICT\"";
  info.subFlag[arg_ct] = Att_S|Att_TR|Att_SN;
  info.subStrDes[arg_ct] = "transform";
  info.subStrDef[arg_ct] = "RCT"; 
  
  info.flag = Att_FR|Att_SN;
  info.strDes = "clr";
  
  // perform the minor duty's
  if( duty != Dty_Perform ){
    return DoMinorDuty(duty, info, argc, argv);
  };
  
  // perform major duty
  SInt ret = DoArgInfoRecognition(info, argc, argv);
  if ( !CheckImgNum(0,1) ){
    return Err_Other;
  }
  
  const Char * clr_spc = info.GetStr();
  EnumSpace space = SpaceEnum(clr_spc);
  if ( space == SpcUnknown ){
    fprintf(m_stdE,"%s \"%s\" is not a recognized color space!\n",ERR_M,
            clr_spc);
    return Err_Other;
  }
  
  const Char * clr_trn = info.GetStr(arg_ct);
  EnumCT trn;
  if ( !clr_trn ){
    trn = RCT;
  }
  else{
    trn = CTEnum(clr_trn);
    if ( trn == UnknownCT ){
      fprintf(m_stdE,"%s \"%s\" is not a recognized color transform!\n",ERR_M,
              clr_trn);
      return Err_Other;
    }
  }

  Bool lsls = True;
  if ( trn == ICT ){
    lsls = False;
  }     
  
  if ( !(m_images.SetSpace(space,False,-1,lsls)) ){
    fprintf(m_stdE,"%s color transform is not reversible\n", VRB_M);
  }
  return ret;
}

SInt
MiscTool::DoMainImgComp(EnumFunctionDuty duty, SInt argc, const Char ** argv)
{ 
  // set up a ArgInfo structure ...
  enum{ arg_code = 0, arg_total};  
  MiscArgInfo info(arg_total);
  info.singleName = "-z";
  info.doubleName = "--compress";
  info.description = "calculates average lossless compression ratio";

  info.subName[arg_code] = "coding=";
  info.subDesc[arg_code] = "specify a compression type";
  info.subFlag[arg_code] = Att_S|Att_TR|Att_SN;
  info.subStrDes[arg_code] = "huffman, arithmetic or theoretical";
  info.subStrDef[arg_code] = "huffman";
  
  // perform the minor duty's
  if( duty != Dty_Perform ){
    return DoMinorDuty(duty, info, argc, argv);
  };
  
  // perform major duty
  SInt ret = DoArgInfoRecognition(info, argc, argv);
  if ( !CheckImgNum(0,1) ){
    return Err_Other;
  }

  // see if coding type is recognized
  if ( !((strcmp(info.subStr[arg_code][0],"huffman") == 0) ||
         (strcmp(info.subStr[arg_code][0],"arithmetic") == 0) ||
         (strcmp(info.subStr[arg_code][0],"theoretical") == 0)) ){
    fprintf(m_stdE,"%s \"%s\" is not a recognized coding style!", 
            ERR_M , info.subStr[arg_code][0]);
    return Err_Other;
  }

  // fixed for now
  const Smpl sample_sizeof = 1;
  const Smpl cmp_base_size = m_images.GetImage(0).GetComponentN();
  SInt avg_imgs_size[MAXCOLORSINANYSPACE];
  SInt avg_comp_size[MAXCOLORSINANYSPACE];
  memset((Char*)avg_imgs_size,'\0',(MAXCOLORSINANYSPACE)*sizeof(SInt));
  memset((Char*)avg_comp_size,'\0',(MAXCOLORSINANYSPACE)*sizeof(SInt));

  // Open a temporary name.
  for ( SInt i = 0 ; i < m_images.GetImageN() ; ++i ){
    HeatWaveImage & img = m_images.GetImage(i);
    for ( SInt c = 0 ; c < img.GetComponentN() ; ++c ){
      HeatWaveComponent & cmp = img.GetComponent(c);
      Char * tempFile = tmpnam(NULL);
      SInt size = cmp.GetSize();
      SInt file_size = 0;
      fprintf(m_stdE,"%s compressing image %d %s component",RES_M,
              i,ColorName(cmp.GetColor()));
      fflush(m_stdO);
      
      if ( strcmp(info.subStr[arg_code][0], "arithmetic") == 0){
        /** @todo change the jasper lib to be able to bypass spatial 
         ** transform **/
        HeatWaveImage tmpImg(img);
        img.SetSpace(SpcGrey,True,c);
        if (!(m_imgTool.WriteImage(tmpImg,tempFile,"jp2","numrlvls=0",
                                   False))){
          fprintf(m_stdE,"%s failed to write temp jp2 file \"%s\"!", 
                  ERR_M , tempFile);
          return Err_Other;
        }
      }
      
      else if ( strcmp(info.subStr[arg_code][0], "theoretical") == 0) {
        SFloat64 entropy = cmp.GetEntropy();
        entropy /= (sample_sizeof*8.0);
        // fprintf(m_stdE,"%s entropy is %f\n",ERR_M,entropy);
        file_size = (SInt)((SFloat64)size*entropy);
      }
      
      else { 
        ASSERT ( strcmp(info.subStr[arg_code][0],"huffman") == 0);
        ofstream fout;
        fout.open(tempFile,ofstream::out|ofstream::trunc|ofstream::binary);
        if ( !fout ){
          fprintf(m_stdE,"%s unable to open temp file \"%s\"\n",ERR_M,
                  tempFile);
          return Err_Other;
        }
        vector<Smpl> vec;
        SimpleCompressor comp;
        SimpleHuffTable table;
        Smpl * data = cmp.GetData();
        SInt * histogram = NULL;
        SInt hist_range = 0;
        SInt hist_offset = 0;
        if(!cmp.GetHistogram(histogram, hist_range, hist_offset)){
          fprintf(m_stdE,"%s skipping component due to histogram error\n",
                  ERR_M);
          continue;
        }
        SInt min = (0-hist_offset);
        SInt max = (hist_range-hist_offset);
        for (SInt i = min; i < max; ++ i){
          if ( histogram[i+hist_offset] ){
            table.AddRow(i,histogram[i+hist_offset]);
          }
        }
        delete [] histogram;
        table.BuildBinary();
        vec.reserve(size);
        for ( SInt i = 0 ; i < (size) ; ++ i){
          vec.push_back(data[i]);
        }
        comp.SetData(vec);
        comp.SetHuffman(table);
        comp.Externalise(fout);
        fout.flush();
        fout.close();
      }
      
      // get the file size back if not theoretical
      if ( strcmp(info.subStr[arg_code][0],"theoretical") != 0) {
        ifstream fin;
        fin.open(tempFile,ofstream::binary);
        if ( !fin ){
          fprintf(m_stdE,"%s unable to open temp file \"%s\"\n",ERR_M,tempFile);
          return Err_Other;
        }
        fin.seekg(0,ios_base::end);
        file_size = fin.tellg();
        // fprintf(m_stdE,"%s file size %d\n",ERR_M,file_size);
        fin.close();
        if(remove(tempFile)){
          fprintf(m_stdE,"%s unable to delete temp file \"%s\"\n",ERR_M,
                  tempFile);
          continue;
        }
      }

      avg_imgs_size[c] += size;
      avg_comp_size[c] += file_size;
      
      // todo check
      // end clear screan
      fprintf(m_stdE,"\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b"
              "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b"
              "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
    }
  }

  HeatWaveImage & img = m_images.GetImage(0);
  for ( SInt i = 0 ; i < cmp_base_size ; ++i ){
    HeatWaveComponent & cmp = img.GetComponent(i);
    ASSERT( avg_imgs_size[i] != 0 );
    ASSERT( avg_comp_size[i] != 0 );
    ASSERT( sample_sizeof > 0 );
    avg_imgs_size[i] *= sample_sizeof;
    double dbl = (double)avg_comp_size[i]/(double)avg_imgs_size[i];
    fprintf(m_stdO,"%s %f compression ratio (%s component)\n",RES_M,dbl,
            ColorName(cmp.GetColor()));
  }
  
  /*
  // Get a histogram 
  if ( m_verbose == 5 ){
  INFOM << "** construct a histogram ** " << endl;
  }
  arr = m_image[img]->GetComponentA();


  // Build the Huffman table 
  if ( m_verbose == 5 ){
  INFOM << "** building Huffman tree ** " << endl;
  }
  min = (0-hist_offset);
  max = (hist_range-hist_offset);
      
  for (SInt i = min; i < max; ++ i){
  if ( histogram[i+hist_offset] ){
  table.AddRow(i,histogram[i+hist_offset]);
  }
  }
  delete [] histogram;
  table.BuildBinary();
    
  // Construct the vector.
  if ( m_verbose == 5 ){
  INFOM << "** contruct data vector from image ** " << endl;
  }
  data_vec = arr[cmp]->GetVector(tlx,tly,width,height);
  ASSERT ( data_vec != NULL );
  vec.reserve(width*height);
  for ( SInt i = 0 ; i < (width*height) ; ++ i){
  vec.push_back(data_vec[i]);
  }

  // Write out to file
  if ( m_verbose == 5 ){
  INFOM << "** externalise data ** " << endl;
  }
  comp.SetData(vec);
  comp.SetHuffman(table);
  comp.Externalise(fout);
  fout.seekp(0,ios_base::end);
  file_size = fout.tellp();
  fout.close();
      
  // write the result! 
  INFOM << "compression result : "
  << (file_size-8) << endl;
      
  // Read back the data for assurance and check
  if ( chk ){
  if ( m_verbose == 5 ){
  INFOM << "** checking data integrity **" << endl;
  }       
  fin.open(data,ofstream::binary|ofstream::in);
  if (!fin){
  FERR << "unable to (re-)open the file " << data 
  << " for checking." << endl;
  goto error;
  }

  comp.Internalise(fin);
  fin.close();
  vec_ref = comp.GetData();
  if ( (((SInt)vec_ref.size() == (width*height))) ){
  for ( SInt i = 0 ; i < (width*height) ; ++ i){
  if (vec_ref[i] != data_vec[i]){
  FERR << "returned data, at pos [" << i << "], has value of "
  << vec_ref[i] << " and should be " << data_vec[i] << endl;
  goto error;
  }
  }
  }
  else{
  FERR << "returned vector length is " << vec_ref.size()
  << " and should be " << (width*height) << endl;
  goto error;
  }
  }

  // delete file if need be
  if ( del ){
  if(remove(data)){
  FERR << "unable to delete temp file";
  perror(" ");
  goto error;
  }
  } 
  }
  }

  if ( m_verbose ){
  OKM << "compression went OK" << endl;
  }
  */
  
  return ret;
}

SInt
MiscTool::DoMainImgHiEq(EnumFunctionDuty duty, SInt argc, const Char ** argv)
{ 
  // set up a ArgInfo struct
  MiscArgInfo info(0);
  info.singleName = "-he";
  info.doubleName = "--hist-eqaul";
  info.description = "perform a histogram equalisation";
  
  // perform the minor duty's
  if( duty != Dty_Perform ){
    return DoMinorDuty(duty, info, argc, argv);
  };
  
  // perform major duty
  SInt ret = DoArgInfoRecognition(info, argc, argv);
  if ( !CheckImgNum(0,1) ){
    return Err_Other;
  }
  m_images.DoHE();
  return ret;
}

