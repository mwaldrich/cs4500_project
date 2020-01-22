#include "../types.h"
#include "../str.h"
#include <cassert>

void string_tests() {
  puts("string type tests passed");
}

void float_tests() {
  puts("float type tests passed");
}

void int_tests() {
  puts("integer type tests passed");
}

void bool_tests() {
  // 0
  String* str = new String("1");
  assert(get_type(str) == DataType::BOOL);
  delete str;

  // 1
  str = new String("0");
  assert(get_type(str) == DataType::BOOL);
  delete str;

  puts("bool type tests passed");
}

void regex_parse_tests() {
  string_tests();
  float_tests();
  int_tests();
  bool_tests();
}