/****************************************************************************/
/**
 *
 * @file   IIIGet.c
 * @brief  A command line application for "getting" values from a 
 *         intermediate integer image (iii).
 * @author Johan Hendrik Ehlers <johanhendrikehlers@gmail.com>
 * 
 **/

#include "IIICommon.h"

int 
main(int argc, char ** argv)
{
  int i,x,y,file_size;
  int xy_error = 0;
  int width = 0, height = 0, precision = 0, count = 0;
  char color_name[10], is_signed[10]; 
  FILE * file = NULL;
  char * file_name = NULL;
  char sample[IIIDSIZE+1];
  memset(sample, 0, IIIDSIZE+1);
  
  if ( ! ((argc == IIIGETCMDMODE) || (argc == IIIGETENVMODE)) ){
    goto usage;
  }
  
  if ( argc == IIIGETCMDMODE ){
    file_name = argv[IIIARGGETFILE];
  }
  else{
    file_name = getenv(IIIENVNAME);
  }
  
  if ( ! file_name ){
    file = stdin;
  }
  else{
    file = fopen(file_name,"rb");
  }
  
  if ( ! file ){
    printf (IIIERRFILEOPEN,file_name);
    goto usage;
  }
  
  if ( file != stdin ){
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    if ( file_size < IIIHSIZE ){
      printf ( IIIERRFILESMALL, file_name);
      goto error;
    }
  }
  
  count = fscanf(file, IIIHEADER, &width, &height, 
                 color_name, &precision, is_signed);
  if ( count != 5 ){
    printf ( IIIERRFILEINFO, file_name);
    goto error;
  }
  
  x = atoi ( argv[IIIARGX] );
  y = atoi ( argv[IIIARGY] );
  
  if ( (x < 0) || (x >= width) ){
    printf (IIIERRCOORD,"x",x,width);
    xy_error++;
  }
  
  if ( (y < 0) || (y >= height) ){
    printf (IIIERRCOORD,"y",y,height);
    xy_error++;
  }
  
  if ( xy_error ){
    goto coord_error;
  }
  
  count = IIIHSIZE+(IIIDSIZE*x)+(IIIDSIZE*width*y);
  if ( file != stdin ){
    count = fseek (file, count, SEEK_SET);
  }
  else{
    for ( i = 0 ; i < (count-IIIHSIZE) ; ++i ){
      if ( getc (file) == EOF ){
        printf ( IIIERRFILESMALL, "stdin");
        goto error;
      }
    }
    count = 0;
  }     
  
  if ( count ){
    printf ( IIIERRFILESMALL, file_name);
    goto error;
  }
  
  count = fread(sample, IIIDSIZE, 1, file);
  if ( count != 1 ){
    printf ( IIIERRFILEINFO, file_name);
    goto error;
  }
  
  if ( file != stdin ){
    fclose(file);
  }
  
  printf(sample);
  exit(0);
  
 error:
  perror ("more error info");
  exit(1);
  
 coord_error:
  exit(1);
  
 usage:
  printf(IIIUSAGEGET);
  exit(1);
}

