#include "../list.h"

void str_lst_init_tests() {
  // empty
  StrList* a = new StrList();
  assert(a->is_empty());

  // give a size
  int zero = 0;
  StrList* b = new StrList(zero);
  assert(b->is_empty());
  assert(b->equals(a));

  StrList* c = new StrList(10);
  assert(c->is_empty());

  //init from a string
  String* d = new String("foo");
  StrList* e = new StrList(d, 4);
  assert(!e->is_empty());
  assert(e->size() == 1);
  assert(e->get(4)->equals(d));

  delete a;
  delete b;
  delete c;
  delete d;
  delete e;

  puts("str list init tests passed");
};

void str_lst_equality_tests() {
  // empty vs empty
  StrList* a = new StrList();
  assert(a->equals(a));

  StrList* b = new StrList(10);
  assert(b->equals(b));
  assert(b->equals(a));

  // empty vs with items
  String* c = new String("foo");
  StrList* d = new StrList(3);
  d->set(0, c);
  assert(!a->equals(d));

  // full vs full same
  String* e = new String("bar");
  d->set(1, e);
  d->set(2, c);
  assert(e->equals(e));

  StrList* f = new StrList(3);
  String* g = new String("foo");
  String* h = new String("bar");
  f->set(0, g);
  f->set(1, h);
  f->set(2, c);
  assert(f->size() == d->size());
  assert(f->exists_at(0, d->get(0)));
  assert(f->exists_at(1, d->get(1)));
  assert(f->exists_at(2, d->get(2)));
  assert(f->equals(d));
  assert(d->equals(f));

  // diff lengths same item same idx
  StrList* i = new StrList(4);
  StrList* j = new StrList(6);
  i->set(2, e);
  j->set(2, e);
  assert(i->size() == j->size() == 1);
  //assert(!i->equals(j));
  //assert(!j->equals(i));

  delete a;
  delete b;
  delete c;
  delete d;
  delete e;
  delete f;
  delete g;
  delete h;
  delete i;
  delete j;

  puts("string list equality tests passed");

};

void str_lst_hash_tests() {
  // empty
  StrList* a = new StrList();
  StrList* b = new StrList();
  StrList* c = new StrList(5);
  assert(a->hash() == b->hash());
  assert(a->hash() == c->hash());

  // adding elements changes hash
  String* d = new String("foo");
  b->set(1, d);
  assert(b->hash() != a->hash());
  assert(b->hash() != c->hash());
  c->set(1, d);
  assert(b->hash() == c->hash());
  assert(c->hash() != a->hash());

  String* e = new String("bar");
  size_t old_hash = b->hash();
  b->set(0, e);
  assert(b->hash() != old_hash);

  delete a;
  delete b;
  delete c;
  delete d;
  delete e;

  puts("string list hash hash tests passed");
};

void str_lst_set_tests() {
  String* a = new String("foo");
  String* b = new String("bar");
  String* c = new String("gar");
  StrList* d = new StrList();

  d->set(0, a);
  d->set(1, b);
  d->set(2, c);
  assert(d->size() == 3);
  assert(d->get(2)->equals(c));

  // setting again does nothing
  d->set(2, a);
  assert(d->size() == 3);

  // setting assigned element to NULL decreased
  d->set(0, NULL);
  assert(d->size() == 2);

  delete a;
  delete b;
  delete c;
  delete d;
  puts("string list set tests passed");
};


void str_lst_add_tests() {
  String* a = new String("foo");
  String* b = new String("bar");
  String* c = new String("gar");
  StrList* d = new StrList();
  d->add(0, a);
  d->add(1, b);
  d->add(2, c);
  assert(d->size() == 3);
  assert(d->get(2)->equals(c));

  // adding shifts up
  String* e = new String("abc");
  d->add(1, e);
  assert(d->size() == 4);
  assert(d->get(1)->equals(e));
  assert(d->get(2)->equals(b));
  assert(d->get(3)->equals(c));

  // adding null doesn't increase size, but still shifts
  d->add(2, NULL);
  assert(d->size() == 4);
  assert(d->get(3)->equals(b));
  assert(d->get(4)->equals(c));

  delete a;
  delete b;
  delete c;
  delete d;
  delete e;
  puts("string list add tests passed");
};

void str_lst_add_all_tests() {
  String* a = new String("new");
  String* b = new String("old");
  String* c = new String("bob");
  StrList* lst = new StrList();
  lst->set(0, a);
  lst->set(1, b);
  lst->set(2, c);
  assert(lst->size() == 3);

  // adding empty at start
  StrList* empty = new StrList();
  lst->add_all(0, empty);
  assert(lst->exists_at(0, a));
  assert(lst->exists_at(1, b));
  assert(lst->exists_at(2, c));
  assert(lst->size() == 3);

  // add empty at inner index
  lst->add_all(2, empty);
  assert(lst->exists_at(0, a));
  assert(lst->exists_at(1, b));
  assert(lst->exists_at(2, c));
  assert(lst->size() == 3);

  // add empty at out of bounds index
  lst->add_all(5, empty);
  assert(lst->exists_at(0, a));
  assert(lst->exists_at(1, b));
  assert(lst->exists_at(2, c));
  assert(lst->size() == 3);

  // adding to empty at start
  empty->add_all(0, lst);
  assert(empty->size() == lst->size());
  assert(empty->equals(lst));

  // adding to empty at out of bounds index
  StrList* empty2 = new StrList();
  empty2->add_all(2, lst);
  assert(empty2->get(0) == NULL);
  assert(empty2->get(1) == NULL);
  assert(empty2->exists_at(2, lst->get(0)));
  assert(empty2->exists_at(3, lst->get(1)));
  assert(empty2->exists_at(4, lst->get(2)));

  // adding after end
  StrList* empty3 = new StrList();
  empty3->add_all(12, lst);
  assert(empty3->get(3) == NULL);
  assert(empty3->get(8) == NULL);
  assert(empty3->exists_at(12, lst->get(0)));
  assert(empty3->exists_at(13, lst->get(1)));
  assert(empty3->exists_at(14, lst->get(2)));

  // adding with overlap
  StrList* e = new StrList();
  StrList* f = new StrList();
  for (size_t idx = 0; idx < e->length(); idx = idx + 1) {
    e->set(idx, a);
    f->set(idx, b);
  }

  StrList* e_and_f = new StrList(20);
  for (size_t idx = 0; idx < e_and_f->length(); idx = idx + 1) {
    if (idx < 5 || 14 < idx) {
      e_and_f->set(idx, b);
    }
    else {
      e_and_f->set(idx, a);
    }
  }

  e->add_all(5, f);
  //assert(e_and_f->equals(e));

  delete a;
  delete b;
  delete c;
  delete lst;
  delete empty;
  delete empty2;
  delete empty3;
  delete e;
  delete f;
  delete e_and_f;

  puts("string list add all tests passed");
};

void str_lst_push_back_tests() {
  // empty
  StrList* a = new StrList();
  String* b = new String("foo");
  assert(a->size() == 0);
  assert(a->length() == 10);
  a->push_back(b);
  assert(a->size() == 1);
  assert(a->length() == 11);
  assert(a->exists_at(10, b));
  a->push_back(b);
  assert(a->size() == 2);
  assert(a->length() == 12);
  assert(a->exists_at(11, b));

  // with full list
  StrList* c = new StrList(2);
  String* d = new String("gah");
  c->set(0, b);
  c->set(1, b);
  assert(c->size() == 2);
  assert(c->length() == 2);
  c->push_back(d);
  assert(c->size() == 3);
  assert(c->length() == 3);
  assert(c->exists_at(2, d));

  // set past initial length of list
  StrList* e = new StrList(10);
  assert(e->is_empty());
  assert(e->length() == 10);
  e->set(15, d);
  assert(e->size() == 1);
  assert(e->length() > 10);
  assert(e->exists_at(15, d));

  delete a;
  delete b;
  delete c;
  delete d;
  delete e;

  puts("string list push back tests passed");
};

void str_lst_index_of_tests() {
  StrList* a = new StrList();
  String* b = new String("bar");
  assert(a->index_of(b) > a->size());
  a->set(3, b);
  assert(a->size() == 1);
  assert(a->index_of(b) == 3);
  a->set(5, b);
  assert(a->size() == 2);
  assert(a->index_of(b) == 3);
  a->remove(3);  // shifts down 1
  assert(a->index_of(b) == 4);

  delete a;
  delete b;

  puts("string list index of tests passed");
};

void str_lst_get_tests() {
  StrList* a = new StrList();
  String* b = new String("goe");
  String* c = new String("foo");

  // with add
  a->add(1, b);
  assert(a->get(1)->equals(b));

  // with set
  a->set(5, c);
  assert(a->get(5)->equals(c));

  delete a;
  delete b;
  delete c;

  puts("string list get tests completed");
};

void str_lst_clear_tests() {
  // on empty list
  StrList* a = new StrList();
  assert(a->size() == 0);
  a->clear();
  assert(a->size() == 0);

  // on list with items
  String* b = new String("foo");
  a->set(2, b);
  a->set(5, b);
  a->set(7, b);
  assert(a->size() == 3);
  a->clear();
  assert(a->size() == 0);

  delete a;
  delete b;

  puts("string list clear tests passed");
};

void str_lst_remove_tests() {
  StrList* a = new StrList();
  String* b = new String("foo");
  assert(a->size() == 0);
  a->set(3, b);
  a->set(6, b);
  assert(a->size() == 2);
  a->remove(3);
  assert(a->size() == 1);
  assert(a->get(5)->equals(b)); // shifted down
  a->remove(5);
  assert(a->size() == 0);

  // removing null keeps same size
  a->set(4, b);
  assert(a->size() == 1);
  assert(a->get(7) == NULL);
  assert(a->size() == 1);

  // removing null before item shifts down
  a->remove(2);
  assert(a->size() == 1);
  assert(a->get(3)->equals(b));

  delete a;
  delete b;

  puts("string list remove tests passed");
};


int str_list_tests() {
  str_lst_init_tests();
  str_lst_equality_tests();
  str_lst_hash_tests();
  str_lst_get_tests();
  str_lst_add_tests();
  str_lst_add_all_tests();
  str_lst_push_back_tests();
  str_lst_clear_tests();
  str_lst_set_tests();
  str_lst_index_of_tests();
  str_lst_remove_tests();
};