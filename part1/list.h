// lang::CwC
#pragma once

#include "str.h"
#include "object.h"
#include <cstring>
#include <cmath>

class StrList : public Object {
/* Class representing a list of Strings, uses a array to provide the
 * underlying storage and NULLs to signal the presence of Strings in the
 * list (does not consider them elements) */

 public:

  /* Array of Strings providing the underlying storage for this list,
 * every value should always be initalized to NULL or a String. */
  String **list_;

  /* How many elements are in this list */
  size_t size_ = 0;

  /* Whatis the size of the underlying array */
  size_t length_ = 10;

  StrList() : Object() {
      /* Creates an empty list of 10 nulls */
      list_ = this->create_null_array_(length_);
  };

  StrList(size_t length) : Object() {
      length_ = length;
      list_ = this->create_null_array_(length_);
  };

  StrList(String *start, size_t idx) : Object() {
      length_ = idx + 1;
      list_ = this->create_null_array_(length_);
      list_[idx] = start;
      up_size();
  };

  StrList(String *start) : Object() {
      StrList(start, 0);
  };

  ~StrList() {
      /* Deletes this list by deleting the underlying array */
      delete[] list_;
  };

  String **create_null_array_(size_t length) {
      /* Creates an array of Strings of the given list, where every element is
       * NULL to signal an empty array.
       * @param length: size of created array
       * @return: array of nullptrs of the given size */
      String **to_return = new String *[length];
      for (size_t i = 0; i < length; i = i + 1) to_return[i] = nullptr;
      return to_return;
  };

  void up_size() {
      /* Increments the size attribute, used to signal an element has been added
       * to a previously empty slot (ie NULL slot).
       * @return: None */
      size_++;
  };

  void down_size() {
      /* Decrements the size attribute, used to signal an element (non-NULL
       * String) has been removed. Error is thrown if there are no elements
       * in the list.
       * @return: None */
      // Should not be called if no elements added
      assert(size_ != 0);
      size_--;
  };

  size_t get_new_length() {
      return ceil(((this->length() * 3) / 2) + 1);
  }

  void expand_length() {
      this->expand_length(this->get_new_length());
  };

  void expand_length(size_t new_length) {
      assert(this->length() < new_length);
      size_t old_size = this->length();
      String **old_list = list_;
      length_ = new_length;
      list_ = this->create_null_array_(this->length() + 1);
      for (size_t i = 0; i < old_size; i = i + 1) {
          this->set_at_idx_(i, old_list[i]);
      }
      delete[] old_list;
  };

  void in_bounds_(size_t idx) {
      assert(idx < this->length());
  };

  bool is_assigned(size_t idx) {
      return this->get(idx) != nullptr;
  };

  virtual void push_back(String *e) {
      // Appends e to end
      this->up_size();
      this->expand_length(this->length() + 1);
      this->set_at_idx_(this->size() - 1, e);
  };

  virtual void add(size_t i, String *e) {
      /* Inserts the given String pointer at the given index, shifting all items
       * to the right of it up by one. If item is larger than list's current
       * capacity, list is expanded to fit the index. If given String is NULL,
       * doesn't count as adding a new element.
       * @param i: index to insert the given String
       * @param e: String to insert
       * @return: None */
      String **new_list = new String *[this->length() + 1];
      for (size_t idx = 0; idx < this->length(); idx = idx + 1) {
          if (idx >= i) {
              new_list[idx + 1] = this->get(idx);
          } else {
              new_list[idx] = this->get(idx);
          }
      }
      new_list[i] = e;
      this->reassign_list_(new_list);
      length_ = this->length() + 1;
      if (e != nullptr) this->up_size();
  };

  virtual void add_all(size_t i, StrList *c) {
      // Inserts all of elements in c into this list at i
      //
      if (c->is_empty()) return;

      size_t c_end = i + c->length();

      int space = i - this->length() - 1; // gap size if i > last idx
      if (space < 0) space = 0;

      size_t new_length = this->length() + c->length() + space;
      String **new_list = this->create_null_array_(new_length + 1);

      // elements appearing of this list appearing in i to end of the list
      // to be copied to end of inserted list
      int overlap_to_end_size = this->length() - i;
      if (overlap_to_end_size < 0) overlap_to_end_size = 0;
      String **overlap_to_end = this->create_null_array_(overlap_to_end_size);

      if (overlap_to_end_size > 0) {
          for (size_t idx = i; idx < this->length(); idx = idx + 1) {
              overlap_to_end[idx - i] = this->get(idx);
          }
      }

      // insert items before i
      size_t stop_i = i;
      if (this->length() < stop_i) stop_i = this->length();

      for (size_t idx = 0; idx < stop_i; idx = idx + 1) new_list[idx] = this->get(idx);

      // insert new items starting at i
      for (size_t idx = i; idx < c_end; idx = idx + 1) {
          new_list[idx] = c->get(idx - i);
          if (c->get(idx - i) != nullptr) up_size();
      }

      // insert any items perviously between i to end
      if (overlap_to_end_size > 0) {
          for (size_t idx = c_end; idx < new_length; idx = idx + 1) {
              new_list[idx] = overlap_to_end[idx - c_end];
          }
      }

      delete[] overlap_to_end;
      this->reassign_list_(new_list);
      length_ = new_length;
  };

  void reassign_list_(String **new_list) {
      delete[] list_;
      list_ = new_list;
  };

  void clear() {
      /* Removes all added elements from this list, resetting the size to 0.
       * Sets the size of the list to hold 10 items.
       * @return: None */
      this->reassign_list_(this->create_null_array_(10));
      length_ = 10;
      size_ = 0;
  };

  bool exists_at(size_t idx, String *str) {
      return this->get(idx)->equals(str);
  };

  virtual bool equals(Object *o) {
      /* Returns if the given object pointer is the same as this list of Strings
       * Attempts to cast the object to a list of Strings, then
       * @param o: object to compare for equality
       * @return: if given object is same as this list */
      // Compares o with this list for equality.
      StrList *casted_other = dynamic_cast<StrList *>(o);
      if (casted_other == nullptr) return false;
      if (this->is_empty() && casted_other->is_empty()) return true;
      if (this->size() != casted_other->size()) return false;
      if ((this->is_empty() && !casted_other->is_empty()) || (!this->is_empty() && casted_other->is_empty())) {
          return false;
      }

      StrList *shorter_lst = casted_other;
      StrList *longer_lst = this;
      size_t smaller_length = shorter_lst->length();

      if (this->length() < smaller_length) {
          shorter_lst = this;
          longer_lst = casted_other;
          smaller_length = this->length();
      }

      for (size_t i = 0; i < smaller_length; i = i + 1) {
          String *shorter_item = shorter_lst->get(i);
          String *longer_item = longer_lst->get(i);
          bool short_null = shorter_item == nullptr;
          bool long_null = longer_item == nullptr;

          if ((short_null || long_null)) {
              if (short_null != long_null) return false;
          } else if (!(shorter_item->equals(longer_item))) return false;
      }

      // check longer item doesn't have any non-null items
      if (longer_lst->length() > smaller_length) {
          for (size_t idx = smaller_length; idx < longer_lst->length(); idx = idx + 1) {
              if (longer_lst->get(idx) != nullptr) return false;
          }
      }
      return true;
  };

  String *get(size_t index) {
      /* Returns the item at the given index, either a String pointer or NULL
       * (empty slot). Error thrown if out of bounds.
       * @param index: index to retrieve item from
       * @return: item at the given index */
      this->in_bounds_(index);
      return list_[index];
  };

  size_t hash() {
      /* Hash function from StackOverflow from user Basile Starynkevitch at
       * https://stackoverflow.com/questions/8317508/hash-function-for-a-string */
      size_t hash = 379;
      size_t prime1 = 35051;
      size_t prime2 = 42227;
      size_t prime3 = 15101;
      for (size_t idx = 0; idx < this->length(); idx = idx + 1) {
          String *cur_item = this->get(idx);
          if (cur_item != nullptr) hash = (hash * prime1) ^ (cur_item->hash() * prime2);
      }
      return hash % prime3;
  }

  size_t index_of(Object *o) {
      /* Returns the index of the first occurance of this object if it is
       * present in the list, else returns a number larger than the size of
       * the list to signal a failure in search. Does not search if object is
       * NULL.
       * @param o: object to return the index of
       * @return: index of the first occurance of the object of int larger than
       *          size of list */
      for (size_t i = 0; i < this->length(); i = i + 1) {
          String *cur_item = this->get(i);
          if (cur_item != nullptr && cur_item->equals(o)) return i;
      }
      return this->length() * 2;
  };

  String *remove(size_t i) {
      /* Removes the item at the given index from the list, shifting all
       * elements to the right of it down by one. Throws an error if i
       * is out of bounds. Returns the item that was present at the index,
       * either NULL or a String pointer.
       * @param i: index of item to remove
       * @return: item that was present at the index */
      String *to_return = this->get(i);
      for (size_t idx = i; idx < this->length() - 1; idx = idx + 1) {
          this->set_at_idx_(idx, this->get(idx + 1));
      }
      this->set_at_idx_(this->length() - 1, nullptr);
      if (to_return != nullptr) this->down_size();
      return to_return;
  };

  void set_at_idx_(size_t idx, String *str) {
      this->in_bounds_(idx);
      list_[idx] = str;
  };

  virtual String *set(int i, String *e) {
      /* Sets the given index of this list to the given Sting. If given index is
       * below the list's current capacity, list is expanded to accomdate it.
       * If a non-null String is assigned to a currently NULL index, counts
       * as adding an element. Assigning NULL to a currently non-NULL index
       * counts as removing an element */
      assert(0 <= i);
      String *old_e = nullptr;
      if (i < this->length()) {
          old_e = this->get(i);
          if (old_e == nullptr && e != nullptr) {
              this->up_size();
          } else if (old_e != nullptr && e == nullptr) {
              this->down_size();
          }
          this->set_at_idx_(i, e);
      } else {
          this->expand_length(i * 2);
      }
      return old_e;
  };

  bool is_empty() {
      /* Returns if this list has no elements in it.
       * @return: if this list is has any elements */
      return this->size() == 0;
  };

  size_t length() {
      /* Returns the size of the underlying String array.
       * @return: size of underlying String array */
      return length_;
  };

  size_t size() {
      /* Returns the number of elements (non-null indices) in this list.
       * @return: number of elements in this list*/
      return size_;
  };
};

class SortedStrList : public StrList {
 public:

  SortedStrList() : StrList() {};

  SortedStrList(size_t length) : StrList(length) {};

  void sorted_add(String *str) {
      if (str == nullptr) return;

      size_t insert_idx = this->length();  // default to end of list
      bool item_inserted = false;
      for (size_t idx = 0; idx < this->length(); idx = idx + 1) {
          String *cur_item = this->get(idx);
          if (cur_item == nullptr) {
              StrList::set(idx, str);
              item_inserted = true;
              break;
          } else if (str->compare(this->get(idx)) < 0) {
              StrList::add(idx, str);
              item_inserted = true;
              break;
          }
      }
      if (!item_inserted) StrList::add(this->length(), str);
  };

  void add_all(size_t idx, StrList *lst) {
      this->add_all(lst);
  }

  void add_all(StrList *lst) {
      for (size_t idx = 0; idx < lst->length(); idx = idx + 1) this->sorted_add(lst->get(idx));
  }

  void add(size_t idx, String *lst) {
      assert(false);
  }

  virtual void push_back(String *str) {
      assert(false);
  };

  virtual String *set(int i, String *e) {
      assert(false);
  };
};


class Column : public StrList {
    public:
    enum data_type {Float, Int, Bool, Str} data_type_;
    
    Column(data_type data_type) : StrList() {
        data_type_ = data_type;
    };

    data_type colum_type() {
        return data_type_;
    }
};
