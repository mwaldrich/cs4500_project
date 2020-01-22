#pragma once
#include <regex.h>

enum class DataType {BOOL, INT, FLOAT, STRING};

DataType get_type(String *string) {
  const char *field = string->str_;
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

  DataType field_type;
  if (regexec(&string_regex, field, 0, nullptr, 0) == 0) {
    field_type = DataType::STRING;
  }
  else if (regexec(&float_regex, field, 0, nullptr, 0) == 0) {
    field_type = DataType ::FLOAT;
  }
  else if (regexec(&bool_regex, field, 0, nullptr, 0) == 0) {
    field_type = DataType::BOOL;
  }
  else {
    field_type = DataType::INT;
  }

  // free regexes
  regfree(&float_regex);
  regfree(&string_regex);
  regfree(&int_regex);
  regfree(&bool_regex);

  return field_type;
};