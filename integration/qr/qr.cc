#include <qrencode.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>

// needs libqrencode
// Compile with gcc -o qr qr.c -lqrencode

static int rows()
{

  struct winsize w;
  if (ioctl(0, TIOCGWINSZ, &w) != -1) return w.ws_row;
  return -1;
}

int main(int argc, char *argv[])
{
  char const* url="http://familiekainz.at";
  if (argc == 2) url = argv[1];
  int r;
  
  QRcode* qr=0;
  
  int i,j;

  char const* t="\u2588\u2588";
  
  
  qr=QRcode_encodeString8bit(url,0,QR_ECLEVEL_H);
  if (qr->width+3 > rows()) {
    free(qr);
    qr=QRcode_encodeString8bit(url,0,QR_ECLEVEL_Q);
  }
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



