#include "buffer_reader_tests.h"
#include "row_to_field_tests.h"
#include "type_regex_tests.h"
#include "str_list_tests.h"
#include "str_tests.h"
#include "sorted_str_list_tests.h"

/* Main function to run all tests */

int main() {
  string_tests();
  str_list_tests();
  sorted_str_list_tests();
  buffer_parse_tests();
  row_to_field_tests();
  regex_parse_tests();
}