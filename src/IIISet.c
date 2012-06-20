/****************************************************************************/
/**
 *
 * @file   IIISet.c
 * @brief  A command line application for "getting" values from a 
 *         intermediate integer image (iii).
 * @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 * 
 **/

#include "IIICommon.h"

int 
main(int argc, char ** argv)
{
  int x, y, file_size;
  int xy_error = 0;
  int width = 0, height = 0, precision = 0, count = 0;
  char color_name[10], is_signed[10]; 
  FILE * file = NULL;
  char * file_name = NULL;
  int sample, min_sample = 0, max_sample = 0;
  
  if ( ! ((argc == IIISETCMDMODE) || (argc == IIISETENVMODE)) ){
    goto usage;
  }
  
  if ( argc == IIISETCMDMODE ){
    file_name = argv[IIIARGSETFILE];
  }
  else{
    file_name = getenv(IIIENVNAME);
  }
  
  if ( file_name ){
    file = fopen(file_name,"rb+");
  }
  if ( ! file ){
    printf (IIIERRFILENONE);
    goto usage;
  }
  
  fseek(file, 0, SEEK_END);
  file_size = ftell(file);
  fseek(file, 0, SEEK_SET);
  /*printf("file size : %d", file_size);*/
  
  if ( file_size < IIIHSIZE ){
    printf ( IIIERRFILESMALL, file_name);
    goto error;
  }
  
  count = fscanf(file, IIIHEADER, &width, &height, 
		 color_name, &precision, is_signed);
  if ( count != 5 ){
    printf ( IIIERRFILEINFO, file_name);
    goto error;
  }
  /*printf("read header ok");*/
  
  count = IIIHSIZE+(IIIDSIZE*width*height);
  if ( file_size < count ){
    printf ( IIIERRFILESMALL, file_name);
    goto error;
  }

  max_sample = (1 << precision);
  if ( strcmp(is_signed,IIITRUE) == 0 ){
    min_sample -= (max_sample/2);
  }
  max_sample -= 1;
  /*printf(" max sample : %d",max_sample);*/
  
  x = atoi ( argv[IIIARGX] );
  y = atoi ( argv[IIIARGY] );
  sample = atoi ( argv[IIIARGSETSAMPLE] );
  
  if ( (x < 0) || (x >= width) ){
    printf (IIIERRCOORD,"x",x,width);
    xy_error++;
  }
  
  if ( (y < 0) || (y >= height) ){
    printf (IIIERRCOORD,"y",y,height);
    xy_error++;
  }

  if ( (sample < min_sample) || (sample > max_sample) ){
    printf (IIIERRSAMPLE,sample,min_sample,max_sample);
    xy_error++;
  }
  
  if ( xy_error ){
    goto coord_error;
  }
			
  count = IIIHSIZE+(IIIDSIZE*x)+(IIIDSIZE*width*y);
  count = fseek (file, count, SEEK_SET);
  /*printf("seek ok: %d", count);*/
  
  if ( count ){
    printf (IIIERRFILESMALL, file_name);
    goto error;
  }
  
  if ( fprintf(file, IIISAMPLEOUT, sample) != IIIDSIZE ){
    printf (IIIERRFILEWRITE, file_name);
    goto error;
  }
  
  fclose(file);
  exit(0);
  
 error:
  perror (IIIPERROR);
  exit(1);
  
 coord_error:
  exit(1);
  
 usage:
  printf (IIIUSAGESET);
  exit(1);
}

