// lang::CwC
#pragma once
#include <cstring>
#include <cassert>
#include "object.h"
#include "helper.h"

class String : public Object {
 public:
  /* Underlying char array of this String */
  const char *str_;

  /* Size of this String, or the number chars in the underlying array */
  size_t size_;

  String() : Object() {
      size_ = 0;
      char *mock_str = create_char_str_array_(0);
      str_ = mock_str;
  };

  String(const char *str) : Object() {
      size_ = strlen(str);
      char *mock_str = create_char_str_array_(size_);
      for (size_t i = 0; i < size_; i = i + 1) mock_str[i] = str[i];
      str_ = mock_str;
  };

  ~String() {
      delete[] str_;
  };

  size_t size() {
      return size_;
  };

  size_t hash() {
      /* Hash function from StackOverflow from user Basile Starynkevitch at
       * https://stackoverflow.com/questions/8317508/hash-function-for-a-string */
      size_t hash = 17;
      size_t prime1 = 21559;
      size_t prime2 = 32831;
      size_t prime3 = 3061;
      for (size_t idx = 0; idx < this->size(); idx = idx + 1) hash = (hash * prime1) ^ (this->get(idx) * prime2);
      return hash % prime3;
  }

  char get(int idx) {
      int casted_size = static_cast<int>(size_);
      assert(idx < casted_size);
      int a = -size_;
      assert(a <= idx);
      if (idx < 0) idx = casted_size + idx;
      return str_[idx];
  };

  String *get_slice(size_t start, size_t end) {
    /* Creates a new String of all the chars between start to end - 1. Start and end must be within the size of this
     * String. If start equals end a empty String is returned.
     * @param start: index to start slice
     * @param end: index to end slice minus one
     * @return: chars of this string between start to end - 1 */
      assert(start <= this->size() && end <= this->size());
      if (start == end) return new String();
      char *slice = create_char_str_array_(end - start);
      for (size_t i = start; i < end; i++) slice[i - start] = this->get(i);
      String* to_return = new String(slice);
      delete[] slice;
      return to_return;
  };

  char get(size_t idx) {
    assert(0 <= idx < this->size_);
    return this->str_[idx];
  }

  String *reverse() {
      if (this->size() == 0) return new String();
      char *rev_str = create_char_str_array_(this->size());
      for (size_t i = 0; i < this->size(); i = i + 1) rev_str[i] = this->get(this->size() - 1 - i);
      String* to_return = new String(rev_str);
      delete[] rev_str;
      return to_return;
  };

  int compare(String *str) {
      assert(str != nullptr);
      if (this->equals(str)) return 0;
      if (this->is_empty()) return -1;
      if (str->is_empty()) return 1;

      size_t min_size = str->size() < this->size() ? str->size() : this->size();
      for (size_t idx = 0; idx < min_size; idx = idx + 1) {
          char this_char = this->get(idx);
          char other_char = str->get(idx);
          if (this_char < other_char) return -1;
          if (this_char > other_char) return 1;
      }

      // equal up to min_size, return -1 if this is shorter than str else 1
      if (this->size() < str->size()) return -1;
      return 1;
  }

  virtual bool equals(Object *other) {
      String *casted_other = dynamic_cast<String *>(other);
      if (other == nullptr || this->size() != casted_other->size()) return false;

      for (size_t i = 0; i < this->size(); i = i + 1) {
          if (this->get(i) != casted_other->get(i)) return false;
      }
      return true;
  };

  bool is_empty() {
      return this->size() == 0;
  };

  String *concat(String *other) {
      assert(other != nullptr);
      if (this->is_empty() && other->is_empty()) return new String();
      if (this->is_empty()) return other->clone();
      if (other->is_empty()) return this->clone();

      size_t new_size = this->size() + other->size();
      char *new_str = create_char_str_array_(new_size);
      for (size_t i = 0; i < new_size; i = i + 1) {
          new_str[i] = i < this->size() ? this->get(i) : other->get(i - this->size());
      }
      String* to_return = new String(new_str);
      delete[] new_str;
      return to_return;
  };

  char* create_char_str_array_(size_t size) {
      /* Creates a char array of the given size plus one, with the additional allocated index containing the null
       * character to designate the char array as a null-terminated string.
       * @param size: size of the char array to create
       * @return: char array of the given size as a null-terminated string */
      char* to_return = new char[size + 1];
      to_return[size] = '\0';
      return to_return;
  };

  String *clone() {
      if (this->size() == 0) return new String();
      char *clone_str = create_char_str_array_(this->size());
      for (size_t i = 0; i < this->size(); i = i + 1) clone_str[i] = this->get(i);
      String* to_return = new String(clone_str);
      delete[] clone_str;
      return to_return;
  };

  virtual void print() {
      pln(str_);
  }
};
