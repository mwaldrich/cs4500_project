#include "helper.h"
#include <unistd.h>

int main(int argc, char** argv) {
  Sys* sys = new Sys();
  char *fvalue = const_cast<char *>("data.sor");
  int from = 1;
  int len = 10;
  int cli_opt;

  FILE * pFile;
  char * buffer;
  size_t result;

  pFile = fopen ( fvalue , "r" );
  if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

  // allocate memory to contain the whole file:
  buffer = new char[len];
  if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

  fseek ( pFile , from , SEEK_SET );
  // copy the number of bytes in len into the buffer:
  result = fread (buffer,1,len,pFile);

  sys->pln(buffer);
  sys->pln(result);
  // terminate
  fclose (pFile);
  free (buffer);
  return 0;

}