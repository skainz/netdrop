#include <qrencode.h>
#include <stdio.h>
#include<stdlib.h>

// needs libqrencode
// Compile with gcc -o qr qr.c -lqrencode

int main()
{
  
  char* url="foo";
  int r;
  
  QRcode* qr=0;
  
  int i,j;

  char* t="██";
  
  
  qr=QRcode_encodeString8bit(url,0,QR_ECLEVEL_H);
  
  for (i=0;i<qr->width+2;i++)
    {
      printf("%s",t);
    }
  printf("\n");

  for (i=0;i<qr->width;i++)
    {
      printf("%s",t);
      for (j = qr->width - 1; j >= 0; j--) 
	{
	  if ((qr->data[j*qr->width + i] & 0x1) == 0)
	    {
	      printf("%s",t); 
	    }
	  else { printf("  ");}
	}
      printf("%s\n",t);
    }
    

  for (i=0;i<qr->width+2;i++)
    {
      printf("%s",t);
    }
  printf("\n");



  free(qr);
  return 0;
    
}



