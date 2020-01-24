#include "object.h"
#include "str.h"

/* A queue of Objects, supporting O(1) insertion at the start of the queue and O(1) retrieval / removal from the end
 * of the queue. */
class ObjectQueue : public Object {

  /* Adds the given Object to the start of this Queue */
  void add(Object* o);

  /* Adds the given ObjectQueue to the start of this ObjectQueue */
  void add_all(ObjectQueue* o);

  /* Returns if given Object is the same as this ObjectQueue */
  bool equals(Object* o);

  /* Removes and returns the Object at the end of this ObjectQueue */
  Object* remove();

  /* Clears this ObjectQueue of all elements */
  void clear();

  /* Returns a reference to the Object at the end of this list */
  Object* peek();

  /* Returns the number of elements in this ObjectQueue */
  unsigned int size();
};

/* A queue of String, supporting O(1) insertion at the start of the queue and O(1) retrieval / removal from the end
 * of the queue. */
class StringQueue : public Object {

  /* Adds the given String to the start of this Queue */
  void add(String* o);

  /* Adds the given StringQueue to the start of this StringQueue */
  void add_all(StringQueue* o);

  /* Returns if given Object is the same as this StringQueue */
  bool equals(Object* o);

  /* Removes and returns the String at the end of this StringQueue */
  String* remove();

  /* Clears this StringQueue of all elements */
  void clear();

  /* Returns a reference to the String at the end of this list */
  Object* peek();

  /* Returns the number of elements in this StringQueue */
  unsigned int size();
};