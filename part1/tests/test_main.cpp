#include "buffer_reader_tests.h"
#include "row_to_field_tests.h"

int main() {
  test_split_no_skip();
  test_split_skip();
  single_fields();
  multiple_fields();
}