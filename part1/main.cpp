#include "helper.h"
#include "str.h"
#include "list.h"
#include <cstring>
#include <getopt.h>
#include <unistd.h>
#include <regex.h>

#define DEFAULT_FROM 0
#define DEFAULT_LEN 100
#define READ_SCHEMA_LINE_COUNT 500
#define START_BRACKET '<'
#define END_BRACKET '>'

Column::data_type get_type(char *field) {
  regex_t string_regex;
  regex_t float_regex;
  regex_t bool_regex;
  regex_t int_regex;

  int string_regex_compiled = regcomp(&string_regex, "[a-zA-Z]+", REG_EXTENDED);
  int float_regex_compiled = regcomp(&float_regex, "[-+]?[0-9]*\\.+[0-9]+", REG_EXTENDED);
  int bool_regex_compiled = regcomp(&bool_regex, "^[0|1]$", REG_EXTENDED);
  int int_regex_compiled = regcomp(&int_regex, "[-+]?[0-9]+", REG_EXTENDED);

  assert(string_regex_compiled == 0);
  assert(float_regex_compiled == 0);
  assert(bool_regex_compiled == 0);
  assert(int_regex_compiled == 0);

  Column::data_type field_type;
  if (regexec(&string_regex, field, 0, nullptr, 0) == 0) {
    field_type = Column::STRING;
  } else if (regexec(&float_regex, field, 0, nullptr, 0) == 0) {
    field_type = Column::FLOAT;
  } else if (regexec(&bool_regex, field, 0, nullptr, 0) == 0) {
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

static struct option arg_options[] = {{"f", required_argument, 0, 'f'},
                                      {"from", required_argument, 0, 'r'},
                                      {"len", required_argument, 0, 'l'},
                                      {"print_col_type", required_argument, 0, 't'},
                                      {0, 0, 0, 0}}; // must end in zeros

class ArgVars {
 public:
  char *file_name;
  size_t from = DEFAULT_FROM;
  size_t len = DEFAULT_LEN;
  size_t col_type = -1;
  size_t col_idx[2];
  size_t missing_idx[2];
};

size_t char_array_to_uint(char *arg) {
  int to_return = atoi(arg);
  assert(to_return > -1);
  return to_return;
}

ArgVars *parse_arrays(int argc, char **argv) {
  // TODO note that int args are casted
  ArgVars *arg_vars = new ArgVars();
  bool file_given = false;
  int opt;
  while ((opt = getopt_long_only(argc, argv, "f:r:l:t:", arg_options, NULL)) != -1) {
    switch (opt) {
      case 'f':file_given = true;
        arg_vars->file_name = optarg;
        break;
      case 'r':arg_vars->from = char_array_to_uint(optarg);
        break;
      case 'l':arg_vars->len = char_array_to_uint(optarg);
        break;
      case 't':arg_vars->col_type = char_array_to_uint(optarg);
        break;
      default:printf("invalid flag: %c", optopt);
        exit(1);
    }
  }
  if (!file_given) {
    printf("must be given a file to read after -f flag");
    exit(1);
  }
  return arg_vars;
}

void set_position(FILE *stream, fpos_t pos) {
  int result = fgetpos(stream, &pos);
  if (result != 0) {
    printf("error in setting position from file");
    exit(1);
  }
}

String *get_largest_row(FILE *stream) {
  size_t largest_row_size = 0;
  fpos_t largest_row_start;
  size_t largest_row_len = 0;

  size_t cur_row_size = 0;
  fpos_t cur_row_start;
  size_t cur_row_len = 0;

  size_t line_count = 0;
  bool start_bracket_found = false;
  bool end_file = false;

  while (!end_file && line_count < READ_SCHEMA_LINE_COUNT) {
    char cur_char = fgetc(stream);
    cur_row_len += 1;
    if (cur_char == EOF) {
      end_file = true;
    } else if (cur_char == START_BRACKET) {
      // ignore multiple start brackets like <<>
      if (!start_bracket_found) {
        start_bracket_found = true;
        set_position(stream, cur_row_start);
      }
    } else if (cur_char == END_BRACKET) {
      if (start_bracket_found) {
        cur_row_size += 1;
        start_bracket_found = false;
      }
    } else if (cur_char == '\n') {
      if (cur_row_size > largest_row_size) {
        largest_row_size = cur_row_size;
        largest_row_start = cur_row_start;
        largest_row_len = cur_row_len - 1;
      }
      cur_row_len = 0;
      cur_row_size = 0;
      line_count += 1;
      start_bracket_found = false;
    }
  }

  // reset to largest row & read into string
  int result = fsetpos(stream, &largest_row_start);
  if (result != 0) perror("error setting position");

  char *largest_row = new char[largest_row_len + 1];
  fread(largest_row, sizeof(char), largest_row_len, stream);
  largest_row[largest_row_len] = '\0';
  String *largest_row_str = new String(largest_row);

  delete[] largest_row;
  rewind(stream);

  return largest_row_str;
}

StrList *row_to_fields(String *row_string) {
  StrList *fields = new StrList();
  size_t field_idx = 0;
  // indices tracking start & end of a field, negative means currently unassigned
  int start_bracket_idx = -1;
  bool in_quotes = false;
  Sys * sys = new Sys();
  for (size_t idx = 1; idx < row_string->size(); idx += 1) {
    char cur_char = row_string->get(idx);
    if (cur_char == START_BRACKET) {
      if (start_bracket_idx < 0) start_bracket_idx = idx;
    } else if (cur_char == END_BRACKET || (in_quotes && cur_char == '"')) {
      if (start_bracket_idx >= 0) {
        fields->set(field_idx, row_string->get_slice(start_bracket_idx, idx));
        field_idx += 1;
        start_bracket_idx = -1;
        in_quotes = false;
      }
    } else if (cur_char == ' ' && start_bracket_idx >= 0 && !in_quotes) {
      // TODO handle case of space in field between non-space characters not in quotes
      start_bracket_idx += 1;
    } else if (cur_char == '"') {
      in_quotes = true;
    }
  }
  return fields;
}

StrList *buffer_to_string_rows(char *buffer, bool skip_first_and_last) {
  char delimiter[2] = "\n";
  StrList *token_list = new StrList(20);
  char *token = strtok(buffer, delimiter);
  size_t i = 0;
  Sys* sys = new Sys();
  while (token != nullptr) {
    if (!skip_first_and_last) {
      token_list->set(i, new String(token));
      i += 1;
    } else {
      skip_first_and_last = false;
    }
    token = strtok(nullptr, delimiter);
  }
  delete token;
  if (skip_first_and_last) token_list->remove(i);
  return token_list;
}

int main(int argc, char **argv) {
  ArgVars *arg_vars = parse_arrays(argc, argv);
  Sys *sys = new Sys();

  // read in file and tokenize
  FILE *sor_file = fopen(arg_vars->file_name, "r");
  if (sor_file == nullptr) {
    fputs("File error", stderr);
    exit(1);
  }

  // go to end, get file size, rewind to start
  fseek(sor_file, 0L, SEEK_END);
  size_t file_size = ftell(sor_file);
  rewind(sor_file);

  // Find row with the most fields
  String *largest_row = get_largest_row(sor_file);
  sys->p("Row with most fields: ").p(largest_row->str_);

  // move file pointer start from start of file to from
  fseek(sor_file, arg_vars->from, SEEK_SET);
  sys->p(arg_vars->from).pln();
  // TODO check offset is offset correct?

  size_t buffer_len = arg_vars->len;
  if (file_size - arg_vars->from) buffer_len = file_size - arg_vars->from;

  // copy len number of bytes into a buffer
  char *sor_buffer = new char[buffer_len + 1];
  size_t result = fread(sor_buffer, 1, buffer_len, sor_file);
  sor_buffer[buffer_len] = '\0';
  fclose(sor_file); // close file as soon as possible
  if (result != 0) ferror(sor_file);

  // split file buffer into rows based on newline
  bool skip_first_and_last = arg_vars->from != 0 || arg_vars->len < file_size;
  puts(sor_buffer);
  StrList *row_str_list = buffer_to_string_rows(sor_buffer, skip_first_and_last);

  // string rows -> list of fields as strings
  StrList **field_rows = new StrList *[row_str_list->size()];
  for (size_t idx = 0; idx < row_str_list->size(); idx += 1) {
    field_rows[idx] = row_to_fields(row_str_list->get(idx));
  }

  // largest row into array of fields for
  

  // TODO Given a field, return the type of the field as an enum

  // TODO largest col count to largest column as string, column count with row to fields

  // TODO rows into columns

  // TODO operations on columns array

  // TODO set up testing bed

  // terminate
  delete[] field_rows;
  delete arg_vars;
  delete largest_row;
  delete[] sor_buffer;
  delete row_str_list;
  return 0;
}