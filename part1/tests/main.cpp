#pragma once
#include "object.h"
#include "helper.h"
#include "string.h"
#include "list.h"
#include <assert.h>

void object_tests() {
  // different hashes
  Object* a = new Object();
  Object* b = new Object();
  assert(a->hash() != b->hash());

  delete a;
  delete b;
  print("object tests passed");  
};


void test_1() {
  String * s = new String("Hello");
  String * t = new String("World");
  String * u = s->concat(t);
  assert(s->equals(s));
  assert(!s->equals(t));
  assert(!s->equals(u));
  print("test 1 passed");
}
 
void test_2() {
  String * s = new String("Hello");
  String * t = new String("World");
  String * u = s->concat(t);
  SortedStrList * l = new SortedStrList();
  l->sorted_add(s);
  l->sorted_add(t);
  l->sorted_add(u);
  assert(l->get(0)->equals(s));
  print("test 2 passed");
}
 
void test_3() {
  String * s = new String("Hello");
  String * t = new String("World");
  String * u = s->concat(t);
  SortedStrList * l = new SortedStrList();
  l->sorted_add(s);
  l->sorted_add(t);
  l->sorted_add(u);
  SortedStrList * l2 = new SortedStrList();
  l2->sorted_add(s);
  l2->sorted_add(t);
  l2->sorted_add(u);
  assert(l->equals(l2));
  assert(l2->equals(l));
  assert(l->hash() == l2->hash());
  print("test 3 passed");
}
 
void test_4() {
  String * s = new String("Hello");
  String * t = new String("World");
  String * u = s->concat(t);
  SortedStrList * l = new SortedStrList();
  l->sorted_add(s);
  l->sorted_add(t);
  l->sorted_add(u);
  SortedStrList * l2 = new SortedStrList();
  l2->add_all(0,l);
  assert(l->size() == l2->size());
  print("test 4 passed");
}


void misc_tests() {
  test_1();
  test_2();
  test_3();
  test_4();
  print("misc tests passed");
}

int main() {
  // do str hash tests
  object_tests();
  str_tests();
  str_lst_tests();
  sorted_list_tests();
  misc_tests();
  return 0;
};
