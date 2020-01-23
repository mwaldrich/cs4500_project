#include "helper.h"
#include "str.h"
#include "list.h"
#include <cstring>
#include <getopt.h>
#include "buffer_reader.h"
#include "row_to_fields.h"

Column **infer_schema(StrList *row_fields) {
  Column **columns = new Column *[row_fields->size()];
  for (int i = 0; i < row_fields->size(); i++) {
    columns[i] = new Column(get_type(row_fields->get(i)));
  }
  return columns;
}

#define DEFAULT_FROM 0
#define DEFAULT_LEN 100
#define READ_SCHEMA_LINE_COUNT 500
#define START_BRACKET '<'
#define END_BRACKET '>'

/* Set of support command line arguments */
static struct option arg_options[] = {{"f", required_argument, 0, 'f'},
                                      {"from", required_argument, 0, 'r'},
                                      {"len", required_argument, 0, 'l'},
                                      {"print_col_type", required_argument, 0, 't'},
                                      {"print_col_idx", required_argument, 0, 'i'},
                                      {"is_missing_idx", required_argument, 0, 'm'},
                                      {0, 0, 0, 0}}; // must end in zeros

class ArgVars : public Object {
  /* Helper class containing the arguments to command line flags this program supports. Every flag is considered
   * optional except for the file name, non-required arguments are represented by a default value or negative number
   * to represent that the argument was never instantiated. */
 public:
  /* Name of SOR file to read from */
  char *file_name;

  /* Byte in the listed file to start reading from */
  size_t from;

  /* Number of bytes in listed file to read, starting at the value of 'from'*/
  size_t len;

  /* The column index whose type will be read */
  int col_type;

  /* Column index & row index whose value will be read */
  int col_idx[2];

  /* Column index & row index to read  */
  int missing_idx[2];

  ArgVars() : Object() {
    file_name = nullptr;
    from = DEFAULT_FROM;
    len = DEFAULT_LEN;
    col_type = -1;
    col_idx[0] = -1;
    col_idx[1] = -1;
    missing_idx[0] = -1;
    missing_idx[1] = -1;
  }

  virtual bool equals(Object *other) {
    ArgVars *casted_other = dynamic_cast<ArgVars *>(other);
    if (other == nullptr) return false;
    return this->from == casted_other->from &&
        this->len == casted_other->len &&
        this->col_type == casted_other->col_type &&
        this->col_idx[0] == casted_other->col_idx[0] &&
        this->col_idx[1] == casted_other->col_idx[1] &&
        this->missing_idx[0] == casted_other->missing_idx[0] &&
        this->missing_idx[1] == casted_other->missing_idx[1];
  }

  bool missing_idx_assigned() {
    /* Returns if the missing index variables of this ArgVars have been instantiated, i.e. are the column index & row
     * index are non-negative.
     * @return: if the variables of the missing index attribute have been set */
    return this->missing_idx[0] > -1 || this->missing_idx[1] > -1;
  }

  bool col_idx_assigned() {
    /* Returns if the column index variables of this ArgVars have been instantiated, i.e. are the column index & row
     * index are non-negative.
     * @return: if the values of the column index attribute have been set */
    return this->col_idx[0] > -1 || this->col_idx[1] > -1;
  }
};

size_t char_array_to_uint(const char *arg) {
  /* Intended to converted a array of integers as chars (like "1934") into a an actual positive integer (like 1934).
   * If the char array represents a negative number or other set of non-integer characters, an error is thrown.
   * Zero is allowed but only in the form of "0".
   * @param arg: character array to convert to a positive integer
   * @return: positive integer form of given character array*/
  if (strlen(arg) == 1 && arg[0] == '0') return 0;
  int to_return = atoi(arg);
  assert(to_return > -1);
  return to_return;
}

ArgVars *parse_cl_args(int argc, char **argv) {
  /* Parses the given command line arguments from main into a ArgVars based off of the supported argument flags for this
   * program. Arguments are required to at least have the name of a SOR file to read, follow the "-f" flag.
   * @param argc: number of command line arguments given
   * @param argv: command line arguments from main
   * @return: ArgVars representing the argument flags & their following arguments found in the given command line
   * arguments */
  ArgVars *arg_vars = new ArgVars();
  int opt;
  while ((opt = getopt_long_only(argc, argv, "f:r:l:t:i:m:", arg_options, nullptr)) != -1) {
    switch (opt) {
      case 'f':
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
      case 'i':
        // TODO error checking on next item? explain
        optind--;
        arg_vars->col_idx[0] = char_array_to_uint(argv[optind]);
        optind++;

        if (argv[optind][0] == '-') {
          puts("print col index flag requires two arguments");
          exit(1);
        }

        arg_vars->col_idx[1] = char_array_to_uint(argv[optind]);
        break;
      case 'm':
        optind--;
        arg_vars->missing_idx[0] = char_array_to_uint(argv[optind]);
        optind++;

        if (argv[optind][0] == '-') {
          puts("missing col index flag requires two arguments");
          exit(1);
        }

        arg_vars->missing_idx[1] = char_array_to_uint(argv[optind]);
        break;
      default:
        printf("invalid flag: %c", optopt);
        exit(1);
    }
  }
  if (arg_vars->file_name == nullptr) {
    printf("must be given a file to read after -f flag");
    exit(1);
  }
  return arg_vars;
}

void set_position(FILE *stream, fpos_t pos) {
  /* Sets the given file position to the current file position of the given file stream throws an error if there is an
   * issue doing so.
   * @param stream: file whose current position will be read
   * @param pos: file position to be set
   * @return: None */
  int result = fgetpos(stream, &pos);
  if (result != 0) {
    printf("error in setting position from file");
    exit(1);
  }
}

String *get_largest_row(FILE *sor_stream) {
  /* Returns the largest row (by field count) of the given SOR file stream as a String. Rewinds the file after reading.
   * @param sor_stream: SOR file to read
   * @return: largest row given SOR file */
  rewind(sor_stream);
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
    char cur_char = fgetc(sor_stream);
    cur_row_len += 1;
    if (cur_char == EOF) {
      end_file = true;
    }
    else if (cur_char == START_BRACKET && !start_bracket_found) {
      start_bracket_found = true;
      set_position(sor_stream, cur_row_start);
    }
    else if (start_bracket_found && cur_char == END_BRACKET) {
      cur_row_size += 1;
      start_bracket_found = false;
    }
    else if (cur_char == '\n') {
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
  int result = fsetpos(sor_stream, &largest_row_start);
  if (result != 0) perror("error setting position");

  char *largest_row = new char[largest_row_len + 1];
  size_t read_result = fread(largest_row, sizeof(char), largest_row_len, sor_stream);
  largest_row[largest_row_len] = '\0';
  String *largest_row_str = new String(largest_row);

  delete[] largest_row;
  rewind(sor_stream);
  return largest_row_str;
}

int main(int argc, char **argv) {
  ArgVars *arg_vars = parse_cl_args(argc, argv);
  Sys *sys = new Sys();

  // read in file and tokenize
  FILE *sor_file = fopen(arg_vars->file_name, "r");
  if (sor_file == nullptr) {
    fputs("error reading in given SOR file name", stderr);
    exit(1);
  }

  // get size of the file
  fseek(sor_file, 0L, SEEK_END);
  size_t file_size = ftell(sor_file);
  rewind(sor_file);

  // find row with the most fields, turn into schema for columns
  String *largest_row = get_largest_row(sor_file);
  StrList *largest_row_fields = row_to_fields(largest_row);

  // move file pointer from bytes over from start
  fseek(sor_file, arg_vars->from, SEEK_SET);
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
  StrList *row_str_list = buffer_to_string_rows(sor_buffer, skip_first_and_last);

  // turn each string row into a list of fields (as string)
  StrList **field_rows = new StrList *[row_str_list->size()];
  for (size_t idx = 0; idx < row_str_list->size(); idx += 1) field_rows[idx] = row_to_fields(row_str_list->get(idx));

  // transform rows of fields in array of columns
  Column **columns = infer_schema(largest_row_fields);
  size_t num_rows = row_str_list->size();
  size_t num_columns = largest_row_fields->size();
  StrList *current_row;
  for (size_t i = 0; i < num_columns; i++) {
    for (size_t j = 0; j < num_rows; j++) {
      current_row = field_rows[j];
      String* to_add = current_row->size() <= i ? new String() : current_row->get(i);
      columns[i]->push_back(to_add);
    }
  }

  // print output based on arg vars & created columns array
  if (arg_vars->col_type >= 0) {
    assert(arg_vars->col_type < num_columns);
    print_data_type(columns[arg_vars->col_type]->get_column_type());
  }

  if (arg_vars->missing_idx_assigned()) {
    int column_idx = arg_vars->missing_idx[0];
    int row_idx = arg_vars->missing_idx[1];
    assert(column_idx < num_columns);
    assert(row_idx < num_rows);
    printf("%d\n", columns[column_idx]->get(row_idx)->is_empty());
  }

  if (arg_vars->col_idx_assigned()) {
    int column_idx = arg_vars->col_idx[0];
    int row_idx = arg_vars->col_idx[1];
    assert(column_idx < num_columns);
    assert(row_idx < num_rows);
    columns[column_idx]->get(row_idx)->print();
  }

  // clean up
  delete[] columns;
  delete[] field_rows;
  delete arg_vars;
  delete largest_row;
  delete largest_row_fields;
  delete[] sor_buffer;
  delete row_str_list;
  delete sys;
  return 0;
}