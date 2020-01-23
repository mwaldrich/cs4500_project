#include "str.h"

StrList *buffer_to_string_rows(char *buffer, bool skip_first_and_last) {
  char delimiter[2] = "\n";
  StrList *token_list = new StrList(20);
  bool skip_first = skip_first_and_last;
  bool skip_last = skip_first_and_last;
  char *token = strtok(buffer, delimiter);
  size_t i = 0;
  while (token != nullptr) {
    if (!skip_first) {
      token_list->set(i, new String(token));
      i += 1;
    }
    else {
      skip_first = false;
    }
    token = strtok(nullptr, delimiter);
  }
  delete token;

  if (skip_last && i > 0) {
    token_list->remove(i-1);
  }
  return token_list;
}