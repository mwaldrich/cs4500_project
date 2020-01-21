#include "helper.h"
#include "str.h"
#include "list.h"
#include <cstring>
#include <getopt.h>
#include <unistd.h>

#define DEFAULT_FROM 0
#define DEFAULT_LEN 100

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
        printf("invalid command: %c", optopt);
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
  bool start_bracket_found = false;
  bool end_file = false;

  while (!end_file) {
    char cur_char = fgetc(stream);

    if (cur_char == EOF) {
      end_file = true;
    }
    else if (cur_char == '<') {
      // ignore multiple start brackets like <<>
      if (!start_bracket_found) start_bracket_found = true;
    }
    else if (cur_char == '>') {
      if (start_bracket_found) {
        cur_row_size += 1;
        start_bracket_found = false;
      }
    }
    else if (cur_char == '\n') {
      if (cur_row_size > max_row_size) max_row_size = cur_row_size;
      cur_row_size = 0;
      start_bracket_found = false;
    }
  }
  rewind(stream);
  return max_row_size;
}

int main(int argc, char **argv) {
  ArgVars* arg_vars = parse_arrays(argc, argv);
  Sys *sys = new Sys();

  FILE *sor_file;
  char *buffer;

  // read in file and tokenize
  sor_file = fopen(arg_vars->file_name, "r");
  if (sor_file == nullptr) {
    fputs("File error", stderr);
    exit(1);
  }

  // Find row with the most number of fields
  size_t num_of_cols = get_col_count(sor_file);

  sys->p("Row with most fields: ").p(num_of_cols);

  // allocate memory to contain the num of bytes to read
  buffer = new char[arg_vars->len];
  if (buffer == NULL) {
    fputs("Memory error", stderr);
    exit(2);
  }

  // move file pointer start from start of file to from
  fseek(sor_file, arg_vars->from, SEEK_SET);
  // TODO offset is offset correct?

  // copy the number of bytes in len into the buffer:
  size_t result = fread(buffer, 1, arg_vars->len, sor_file);
  // TODO how to handle result -> error?
  ferror(sor_file); // check for any non-result based errors

  bool skip_first = arg_vars->from != 0;
  bool skip_last = arg_vars->len < ftell(sor_file);
  rewind(sor_file); // reset after getting size

  // split into rows based on newline
  char delimiter[2] = "\n";
  StrList *token_list = new StrList(20);
  char *token = strtok(buffer, delimiter);
  size_t i = 0;
  while (token != nullptr) {
    if (!skip_first) {
      token_list->set(i, new String(token));
    }
    else {
      skip_first = false;
    }
    // TODO delete token
    delete token;
    token = strtok(nullptr, delimiter);
  }

  // remove last added row
  if (skip_last) {
    token_list->remove(i);
  }

  for (size_t idx = 0; idx < token_list->size(); idx++) {
    String* to_print = token_list->get(idx);
    to_print->print();
  }

  // TODO: Row as a string -> List of fields as strings

  // TODO: Given a field -> Return the type of the field as an enum

  // terminate
  fclose(sor_file);
  delete arg_vars;
  delete buffer;
  delete token_list;
  return 0;
}