// lang::CwC
#pragma once
#include <cstring>
#include <cassert>
#include "object.h"
#include "helper.h"

class String : public Object {

  size_t size();

  size_t hash();

  char get(int idx);

  String *get_slice(size_t start, size_t end);

  char get(size_t idx);

  String *reverse();

  int compare(String *str);

  virtual bool equals(Object *other);

  bool is_empty();

  String *concat(String *other);

  String *clone();

  virtual void print();
};
