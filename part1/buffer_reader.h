#include "str.h"

StrList *buffer_to_string_rows(char *buffer, bool skip_first_and_last) {
  /* Given a SOR file read in a char buffer, splits the file along the new line delimiter '\n' and returns the rows
   * as Strings in a StrList.
   * @param buffer: SOR file as char buffer to break apart
   * @param skip_first_and_last: whether or not to skip the first & last rows of the read file
   * @return: rows of the given SOR file as Strings in a StrList */
  char delimiter[2] = "\n";
  StrList *token_list = new StrList(0);
  bool skip_first = skip_first_and_last;
  bool skip_last = skip_first_and_last;
  char *token = strtok(buffer, delimiter);
  size_t idx = 0;
  while (token != nullptr) {
    if (!skip_first) {
      token_list->push_back(new String(token));
      idx += 1;
    }
    else {
      skip_first = false;
    }
    token = strtok(nullptr, delimiter);
  }
  delete token;
  if (skip_last && idx > 0) token_list->remove(idx - 1);
  return token_list;
}