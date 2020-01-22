#include "../list.h"
#include "../buffer_reader.h"
#include "../str.h"
#include <cassert>
#include <cstring>

/* Module for testing our function for converting a char array into a StrList of fields */

void test_split_no_skip() {
  char buffer[100];
  StrList* str_list;

  // single word
  String* word = new String("foobar");
  strcpy(buffer, word->str_);
  str_list = buffer_to_string_rows(buffer, false);
  assert(str_list->size() == 1);
  assert(str_list->get(0)->equals(word));
  delete str_list;

  // two words
  String* word2 = new String("foobar\ngahtar");
  String* word3 = new String("foobar");
  String* word4 = new String("gahtar");
  strcpy(buffer, word2->str_);
  str_list = buffer_to_string_rows(buffer, false);
  assert(str_list->size() == 2);
  assert(str_list->get(0)->equals(word3));
  assert(str_list->get(1)->equals(word4));
  delete str_list;

  // five words
  String* word5 = new String("foobar\ngahtar\n643\nKLIO\nping pong");
  String* word6 = new String("foobar");
  String* word7 = new String("gahtar");
  String* word8 = new String("643");
  String* word9 = new String("KLIO");
  String* word10 = new String("ping pong");
  strcpy(buffer, word5->str_);
  str_list = buffer_to_string_rows(buffer, false);
  assert(str_list->size() == 5);
  assert(str_list->get(0)->equals(word6));
  assert(str_list->get(1)->equals(word7));
  assert(str_list->get(2)->equals(word8));
  assert(str_list->get(3)->equals(word9));
  assert(str_list->get(4)->equals(word10));
  delete str_list;

  puts("buffer reader tests no skip passed");
}

void test_split_skip() {
  char buffer[100];
  StrList* str_list;

  // single word
  String* word = new String("barfoo");
  strcpy(buffer, word->str_);
  str_list = buffer_to_string_rows(buffer, true);
  assert(str_list->size() == 0);
  delete str_list;

  // two words
  String* word2 = new String("foobar\ngahtar");
  strcpy(buffer, word2->str_);
  str_list = buffer_to_string_rows(buffer, true);
  assert(str_list->size() == 0);
  delete str_list;

  // five words
  String* word5 = new String("foobar\ngahtar\n643\nKLIO\nping pong");
  String* word7 = new String("gahtar");
  String* word8 = new String("643");
  String* word9 = new String("KLIO");
  strcpy(buffer, word5->str_);
  str_list = buffer_to_string_rows(buffer, true);
  assert(str_list->size() == 3);
  assert(str_list->get(0)->equals(word7));
  assert(str_list->get(1)->equals(word8));
  assert(str_list->get(2)->equals(word9));
  delete str_list;

  puts("buffer reader tests skip passed");
}