#include "list.h"

void sorted_str_lst_sorted_add_lst() {
  SortedStrList* a = new SortedStrList(2);
  String* b = new String("foo");
  String* c = new String("gah");
  String* d = new String("nod");

  assert(a->size() == 0);
  assert(a->length() == 2);

  a->sorted_add(c);
  assert(a->size() == 1);
  assert(a->length() == 2);
  assert(a->exists_at(0, c));

  a->sorted_add(c);
  assert(a->size() == 2);
  assert(a->length() == 2);
  assert(a->exists_at(0, c));
  assert(a->exists_at(1, c));

  a->sorted_add(b);
  assert(a->size() == 3);
  assert(a->length() == 3);
  assert(a->exists_at(0, b));
  assert(a->exists_at(1, c));

  a->sorted_add(d);
  assert(a->size() == 4);
  assert(a->length() == 4);
  assert(a->exists_at(3, d));

  delete a;
  delete b;
  delete c;
  delete d;

  puts("sorted string list sorted add tests passed");
};

int main() {
  sorted_str_lst_sorted_add_lst();
  puts("sorted list tests passed");
};