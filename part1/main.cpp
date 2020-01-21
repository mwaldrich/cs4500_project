#include "helper.h"
#include "object.h"
#include "str.h"
#include "list.h"
#include <cstring>
#include <unistd.h>
#include <regex.h>

Column::data_type get_type(char* field) {
  regex_t string_regex;
  regex_t float_regex;
  regex_t bool_regex;
  regex_t int_regex;

  int string_regex_compiled = regcomp(&string_regex, "[a-zA-Z]+", REG_EXTENDED);
  int float_regex_compiled = regcomp(&float_regex, "[-+]?[0-9]*\\.+[0-9]+", REG_EXTENDED);
  int bool_regex_compiled = regcomp(&bool_regex, "^[0|1]$", REG_EXTENDED);
  int int_regex_compiled = regcomp(&int_regex, "[-+]?[0-9]+", REG_EXTENDED);

  assert(string_regex_compiled==0);
  assert(float_regex_compiled==0);
  assert(bool_regex_compiled==0);
  assert(int_regex_compiled==0);

  Column::data_type field_type;
  if(regexec(&string_regex, field, 0, NULL, 0) == 0) {
    field_type = Column::STRING;
  } else if(regexec(&float_regex, field, 0, NULL, 0) == 0) {
    field_type = Column::FLOAT;
  } else if(regexec(&bool_regex, field, 0, NULL, 0) == 0) {
    field_type = Column::BOOL;
  } else {
    field_type = Column::INT;
  }

  // free regexes
  regfree(&float_regex);
  regfree(&string_regex);
  regfree(&int_regex);
  regfree(&bool_regex);

  return field_type;
};

int main(int argc, char** argv) {

  // parser
  Sys* sys = new Sys();
  char *fvalue = const_cast<char *>("data.sor");
  int from = 0;
  int len = 100;
  int cli_opt;

  FILE * pFile;
  char * buffer;
  size_t result;

  // read in file and tokenize
  pFile = fopen ( fvalue , "r" );
  if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

  // Find row with the most number of fields
  char* line;
  char* row_with_most_fields = new char();
  int most_num_fields;
  while (fgets(line , INT_MAX , pFile) != NULL) {
    int num_fields = 0;
    for(size_t i = 0; i < strlen(line) - 1; i++) {
      if(line[i] == '<' && line[i + 1] != '>') {
        num_fields+= 1;
      }
    }

    if (num_fields > most_num_fields) {
      delete row_with_most_fields;
      row_with_most_fields = new char(strlen(line));
      strcpy(row_with_most_fields, line);
      most_num_fields = num_fields;
    } 
  }
  sys->p("Row with most Fields: ").p(row_with_most_fields);

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