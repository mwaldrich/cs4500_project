#include "helper.h"
#include "str.h"
#include "list.h"
#include <cstring>
#include <getopt.h>
#include <unistd.h>

#define DEFAULT_FROM 0
#define DEFAULT_LEN 100
#define READ_SCHEMA_LINE_COUNT 500
#define START_BRACKET '<'
#define END_BRACKET '>'

static struct option arg_options[] = {{"f", required_argument, 0, 'f'},
                                      {"from", required_argument, 0, 'r'},
                                      {"len", required_argument, 0, 'l'},
                                      {"print_col_type", required_argument, 0, 't'},
                                      {0, 0, 0, 0}}; // must end in zeros

struct ArgVars {
  char* file_name;
  size_t from = DEFAULT_FROM;
  size_t len = DEFAULT_LEN;
  size_t col_type = -1;
  size_t col_idx [2];
  size_t missing_idx[2];
};

size_t char_array_to_uint(char* arg) {
  int to_return = atoi(arg);
  assert(to_return > -1);
  return to_return;
}

ArgVars *parse_arrays(int argc, char **argv) {
  // TODO note that int args are casted
  ArgVars* arg_vars = new ArgVars();
  bool file_given = false;
  int opt;
  while ((opt = getopt_long_only(argc, argv, "f:r:l:t:", arg_options, NULL)) != -1) {
    switch (opt) {
      case 'f':
        file_given = true;
        arg_vars->file_name = optarg;
        break;
      case 'r':
        arg_vars->from = char_array_to_uint(optarg);
        break;
      case 'l':
        arg_vars->len = char_array_to_uint(optarg);
        break;
      case 't':
        arg_vars->col_type = char_array_to_uint(optarg);
        break;
      default:
        printf("invalid flag: %c", optopt);
        exit(1);
    }
  }
  if (!file_given) {
    printf("must be given a file to read after -f flag");
    exit(1);
  }
  return arg_vars;
}

size_t get_col_count(FILE *stream) {
  size_t max_row_size = 0;
  size_t cur_row_size = 0;
  size_t line_count = 0;
  bool start_bracket_found = false;
  bool end_file = false;
  while (!end_file || line_count < READ_SCHEMA_LINE_COUNT) {
    char cur_char = fgetc(stream);
    if (cur_char == EOF) {
      end_file = true;
    }
    else if (cur_char == START_BRACKET) {
      // ignore multiple start brackets like <<>
      if (!start_bracket_found) start_bracket_found = true;
    }
    else if (cur_char == END_BRACKET) {
      if (start_bracket_found) {
        cur_row_size += 1;
        start_bracket_found = false;
      }
    }
    else if (cur_char == '\n') {
      if (cur_row_size > max_row_size) max_row_size = cur_row_size;
      cur_row_size = 0;
      line_count += 1;
      start_bracket_found = false;
    }
  }
  rewind(stream);
  return max_row_size;
}

StrList* row_to_fields(String* row_string) {
  StrList* fields = new StrList();
  size_t field_idx = 0;
  // indicies tracking start & end of a field
  // negative means currently unassigned
  int start_bracket_idx = -1;
  bool in_quotes = false;
  for (size_t idx = 1; idx < row_string->size(); idx += 1) {
    char cur_char = row_string->get(idx);
    if (cur_char == START_BRACKET) {
      if (start_bracket_idx < 0) start_bracket_idx = idx;
    }
    else if (cur_char == END_BRACKET || (in_quotes && cur_char == '"')) {
      if (start_bracket_idx >= 0) {
        fields->set(field_idx, row_string->get_slice(start_bracket_idx, idx));
        field_idx += 1;
        start_bracket_idx = -1;
        in_quotes = false;
      }
    }
    else if (cur_char == ' ' && start_bracket_idx >= 0 && !in_quotes) {
      // TODO handle case of space in field between non-space characters not in quotes
      start_bracket_idx += 1;
    }
    else if (cur_char == '"') {
      in_quotes = true;
    }
  }
  return fields;
}

StrList* buffer_to_string_rows(char* buffer, bool skip_first_and_last) {
  char delimiter[2] = "\n";
  StrList *token_list = new StrList(20);
  char *token = strtok(buffer, delimiter);
  size_t i = 0;
  while (token != nullptr) {
    if (!skip_first_and_last) {
      token_list->set(i, new String(token));
    }
    else {
      skip_first_and_last = false;
    }
    // TODO delete token
    delete token;
    token = strtok(nullptr, delimiter);
  }

  // remove last added row
  if (skip_first_and_last) {
    token_list->remove(i);
  }
  return token_list;
}

int main(int argc, char **argv) {
  ArgVars* arg_vars = parse_arrays(argc, argv);
  Sys *sys = new Sys();

  // read in file and tokenize
  FILE *sor_file = fopen(arg_vars->file_name, "r");
  if (sor_file == nullptr) {
    fputs("File error", stderr);
    exit(1);
  }
  size_t file_size = ftell(sor_file);
  rewind(sor_file);

  // Find row with the most number of fields
  size_t num_of_cols = get_col_count(sor_file);
  sys->p("Row with most fields: ").p(num_of_cols);

  // allocate memory to contain the num of bytes to read
  char *buffer = new char[arg_vars->len];

  // move file pointer start from start of file to from
  fseek(sor_file, arg_vars->from, SEEK_SET);
  // TODO check offset is offset correct?

  // copy the number of bytes in len into the buffer:
  size_t result = fread(buffer, 1, arg_vars->len, sor_file);
  // TODO how to handle result -> error?
  ferror(sor_file); // check for any non-result based errors

  bool skip_first_and_last = arg_vars->from != 0 || arg_vars->len < file_size;
  fclose(sor_file); // close file as soon as possible

  // split into rows based on newline
  StrList *row_str_list = buffer_to_string_rows(buffer, skip_first_and_last);

  for (size_t idx = 0; idx < row_str_list->size(); idx++) {
    String* to_print = row_str_list->get(idx);
    to_print->print();
  }

  // string rows -> list of fields as strings
  StrList** field_rows = new StrList*[row_str_list->size() + 1];
  for (size_t idx = 0; idx < row_str_list->size() + 1; idx += 1) {
    field_rows[idx] = row_to_fields(row_str_list->get(idx));
  }

  // TODO: Given a field -> Return the type of the field as an enum

  // TODO rows into columns

  // TODO operations on columns array

  // TODO set up testing bed

  // terminate
  delete[] field_rows;
  delete arg_vars;
  delete buffer;
  delete row_str_list;
  return 0;
}