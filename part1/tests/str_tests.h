#include "../str.h"

void str_init_tests() {
  // empty init
  String* a = new String();
  assert(a->size() == 0);
  delete a;

  String* b = new String("");
  assert(b->size() == 0);
  delete b;

  String* c = new String("foobar");
  assert(c->size() == 6);
  delete c;

  puts("string init tests passed");
};

void str_equality_tests() {
  // empty and empty
  String* a = new String();
  String* b = new String();
  String* c = new String("");
  assert(a->equals(b));
  assert(b->equals(a));
  assert(b->equals(c));
  assert(c->equals(b));
  delete a;
  delete b;
  delete c;

  // self equality
  String* d = new String("foo");
  assert(d->equals(d));

  // string equality same item
  String* e = new String("foo");
  assert(e->equals(d));
  assert(d->equals(e));

  // string inequality same size
  String* f = new String("bar");
  assert(!f->equals(e));

  // string equality diff size
  String* g = new String("boozo");
  assert(!e->equals(g));

  delete d;
  delete e;
  delete f;
  delete g;

  puts("string equality tests passed");
};

void str_hash_tests() {
  // empty
  String* a = new String();
  String* b = new String();
  assert(a->hash() == b->hash());

  String* c = new String("foo");
  String* d = new String("bar");
  String* e = new String("foo");
  assert(c->hash() != d->hash());
  assert(c->hash() == e->hash());

  delete a;
  delete b;
  delete c;
  delete d;
  delete e;

  puts("string hash tests passed");
};

void str_compare_tests() {
  // empty vs empty
  String* a = new String();
  assert(a->compare(a) == 0);

  // empty vs string
  String* b = new String("foo");
  assert(a->compare(b) < 0);
  assert(b->compare(a) > 0);

  // string vs string equal
  assert(b->compare(b) == 0);

  // string vs string same size
  String* c = new String("bar");
  assert(c->compare(b) < 0);
  assert(b->compare(c) > 0);

  // string vs string diff size
  String* d = new String("kiplo");
  assert(d->compare(b) > 0);
  assert(b->compare(d) < 0);

  // string vs string same start
  String* e = new String("kip");
  assert(e->compare(d) < 0);
  assert(d->compare(e) > 0);

  delete a;
  delete b;
  delete c;
  delete d;
  delete e;

  puts("string compare tests finished");
};



void str_slice_tests() {

};

void str_get_tests() {
  // positive get
  String* a = new String("foo");
  assert(a->get(0) == 'f');
  assert(a->get(1) == 'o');
  assert(a->get(2) == 'o');

  // negative get
  assert(a->get(-1) == 'o');
  assert(a->get(-2) == 'o');
  assert(a->get(-3) == 'f');

  assert(a->get(0) == a->get(-3));
  assert(a->get(1) == a->get(-2));
  assert(a->get(2) == a->get(-1));

  delete a;
  puts("string get tests passed");
};

void str_reverse_tests() {
  // reverse empty
  String* a = new String();
  String* rev_a = a->reverse();
  assert(a->equals(rev_a));
  assert(rev_a->equals(a));
  delete a;
  delete rev_a;

  // reverse string
  String* b = new String("foo");
  String* rev_b = new String("oof");
  //assert(b->reverse()->equals(rev_b));
  //assert(rev_b->reverse()->equals(b));
  delete b;
  delete rev_b;

  puts("string reverse tests passed");
};

void str_concat_tests() {
  String* a = new String();

  // empty + empty
  String* b = new String();
  String* a_and_b = new String();
  String* a_and_a = new String();
  assert(a->equals(a_and_b));
  assert(a->equals(a_and_a));

  String* d = new String("foo");
  // string + empty
  //assert(d->equals(d->concat(a)));

  // empty + string
  //assert(d->equals(a->concat(d)));

  // string + string
  String* e = new String("bar");
  String* f = new String("foobar");
  String* d_and_e = d->concat(e);
  assert(f->equals(d_and_e));

  // self + self
  String* g = new String("foofoo");
  String* d_and_d = d->concat(d);
  assert(g->equals(d_and_d));

  delete a;
  delete b;
  delete a_and_a;
  delete a_and_b;
  delete d;
  delete d_and_e;
  delete d_and_d;
  delete e;
  delete f;
  delete g;

  puts("string concat tests passing");
};


void str_clone_tests() {
  // clone empty
  String* a = new String();
  String* a_clone = a->clone();
  assert(a->equals(a_clone));

  // clone string
  String* b = new String("foobar");
  String* b_clone = b->clone();
  assert(b->equals(b_clone));

  delete a;
  delete a_clone;
  delete b;
  delete b_clone;

  puts("string clone tests passed");
};

int str_tests() {
  str_init_tests();
  str_equality_tests();
  str_hash_tests();
  str_get_tests();
  str_reverse_tests();
  str_clone_tests();
  str_slice_tests();
  str_concat_tests();
  str_compare_tests();
  puts("all string tests passed");
};