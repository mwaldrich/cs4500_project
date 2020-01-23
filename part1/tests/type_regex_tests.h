#include "../types.h"
#include "../str.h"
#include <cassert>

void string_tests() {
  String* str = new String("a");
  assert(get_type(str) == DataType::STRING);
  delete str;

  str = new String("abc");
  assert(get_type(str) == DataType::STRING);
  delete str;

  str = new String("123abc");
  assert(get_type(str) == DataType::STRING);
  delete str;

  puts("string type tests passed");
}

void float_tests() {
  String* str = new String("0.4");
  assert(get_type(str) == DataType::FLOAT);
  delete str;

  str = new String(".5");
  assert(get_type(str) == DataType::FLOAT);
  delete str;

  str = new String("1.2");
  assert(get_type(str) == DataType::FLOAT);
  delete str;

  puts("float type tests passed");
}

void int_tests() {
  String* str = new String("10");
  assert(get_type(str) == DataType::INT);
  delete str;

  str = new String("0001");
  assert(get_type(str) == DataType::INT);
  delete str;

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