#include "buffer_reader_tests.h"
#include "row_to_field_tests.h"
#include "type_regex_tests.h"

/* Main function to run all tests */

int main() {
  buffer_parse_tests();
  row_to_field_tests();
  regex_parse_tests();
}