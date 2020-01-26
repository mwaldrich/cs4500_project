#include "object.h"
#include "str.h"

/* A queue of Objects, supporting O(1) insertion at the start of the queue and O(1) retrieval / removal from the end
 * of the queue. */
class Queue : public Object
{

  /* Adds the given Object to the start of this Queue */
  void add(Object *o);

  /* Adds the given Queue to the start of this Queue */
  void add_all(Queue *o);

  /* Returns if given Object is the same as this Queue */
  bool equals(Object *o);

  /* Removes and returns the Object at the end of this Queue */
  Object *remove();

  /* Clears this Queue of all elements */
  void clear();

  /* Returns a reference to the Object at the end of this list */
  Object *peek();

  /* Returns the number of elements in this Queue */
  unsigned int size();
};