#include "helper.h"
#include "object.h"
#include "str.h"
#include "list.h"
#include <cstring>
#include <unistd.h>

int main(int argc, char** argv) {
  Sys* sys = new Sys();
  char *fvalue = const_cast<char *>("data.sor");
  int from = 0;
  int len = 100;
  int cli_opt;

  FILE * pFile;
  char * buffer;
  size_t result;

  pFile = fopen ( fvalue , "r" );
  if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

  // allocate memory to contain the whole file:
  buffer = new char[len];
  if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

  // move to from start to "from"
  fseek ( pFile , from , SEEK_SET );
  // copy the number of bytes in len into the buffer:
  result = fread (buffer,1,len,pFile);

  bool skip_first = from == 0;
  bool skip_last = len < ftell(pFile);
  rewind(pFile); // reset after getting size

  char delimiter[2] = "\n";
  StrList* token_list = new StrList();
  char* token = strtok(buffer, delimiter);
  
  while (token != NULL) {
    token_list->push_back(new String(token));
    token = strtok(NULL, delimiter);
  }

  for (size_t i = 0; i < token_list->size(); i++) {
    String* to_print = token_list->get(i);
    to_print->print();
  }

  // TODO: Make f, from, and len cli arguments

  // TODO: Get rid of first and/or last rows

  // TODO: Row as a string -> List of fields as strings

  // TODO: Given a field -> Return the type of the field as an enum

  // terminate
  fclose (pFile);
  delete buffer;
  delete token_list;
  return 0;

}