#include "queue.h"

void string_queue_normal_usage() {
  /* Tests the normal usage of a StringQueue - adding, removing, peeking, and checking the size of the queue */
  StringQueue* string_queue = new StringQueue();

  // empty queue size
  assert(string_queue->size() == 0);

  // add one item
  String* item = new String("foobar");
  string_queue->add(item);
  assert(string_queue->size() == 1);
  assert(string_queue->peek()->equals(item));

  // add another item
  String* item2 = new String("tube");
  string_queue->add(item2);
  assert(string_queue->size() == 2);
  assert(string_queue->peek()->equals(item));

  // remove an item
  String* popped = string_queue->remove();
  assert(popped->equals(item));
  assert(string_queue->size() == 1);
  assert(string_queue->peek()->equals(item2));

  // remove another item
  popped = string_queue->remove();
  assert(popped->equals(item2));
  assert(string_queue->size() == 0);

  delete string_queue;
}

void string_queue_advanced_usage() {
  /* Tests more advanced / nicheusage of a StringQueue - concating StringQueues, equality, clearing, etc. */
  StringQueue* string_queue = new StringQueue();
  StringQueue* another_string_queue = new StringQueue();
  String* string1 = new String("software_dev");
  String* string2 = new String("is very fun");
  String* string3 = new String("seriously!");

  // adding empty self to empty self
  assert(string_queue->size() == 0);
  string_queue->add_all(string_queue);
  assert(string_queue->size() == 0);

  // adding empty to empty
  assert(string_queue->size() == 0);
  string_queue->add_all(another_string_queue);
  assert(string_queue->size() == 0);

  // adding empty to non-empty
  string_queue->add(string1);
  string_queue->add(string2);
  string_queue->add(string3);
  assert(string_queue->size() == 3);

  assert(another_string_queue->size() == 0);
  string_queue->add_all(another_string_queue);
  assert(string_queue->size() == 3);

  // adding non-empty to empty
  assert(another_string_queue->size() == 0);
  assert(string_queue->size() == 3);
  another_string_queue->add_all(string_queue);
  assert(string_queue->size() == 3);
  assert(another_string_queue->size() == 3);
  assert(string_queue->equals(another_string_queue));

  // clearing a previously added string queue has no effect on the string queue that was added to
  assert(string_queue->size() == 3);
  assert(another_string_queue->size() == 3);
  string_queue->clear();
  assert(string_queue->size() == 0);
  assert(another_string_queue->size() == 3);

  // add non-empty self to self
  another_string_queue->add_all(another_string_queue);
  assert(another_string_queue->size() == 6);
  String* popped = another_string_queue->remove();
  assert(popped->equals(string1));
  String* popped = another_string_queue->remove();
  assert(popped->equals(string2));
  String* popped = another_string_queue->remove();
  assert(popped->equals(string3));
  String* popped = another_string_queue->remove();
  assert(popped->equals(string1));
  String* popped = another_string_queue->remove();
  assert(popped->equals(string2));
  String* popped = another_string_queue->remove();
  assert(popped->equals(string3));
  assert(another_string_queue->size() == 0);

  delete string_queue;
  delete another_string_queue;
  delete string1;
  delete popped;
  delete string2;
  delete string3;
}

void string_queue_hashing_and_equality() {
  /* Testing hashing and equality of StringQueues */
  StringQueue* string_queue = new StringQueue();
  StringQueue* another_string_queue = new StringQueue();
  String* string1 = new String("hofamnn");
  String* string2 = new String("banana");
  String* string3 = new String("foobar");

  // empty
  assert(string_queue->equals(another_string_queue));
  assert(string_queue->hash() == another_string_queue->hash());

  // add item
  string_queue->add(string1);
  assert(!string_queue->equals(another_string_queue));

  // add item to other queue
  another_string_queue->add(string1);
  assert(string_queue->equals(another_string_queue));
  assert(string_queue->hash() == another_string_queue->hash());

  // add more items
  string_queue->add(string2);
  another_string_queue->add(string2);
  string_queue->add(string3);
  another_string_queue->add(string3);
  string_queue->add(string1);
  another_string_queue->add(string1);
  assert(string_queue->equals(another_string_queue));
  assert(string_queue->hash() == another_string_queue->hash());

  // clear and add
  string_queue->clear();
  another_string_queue->clear();
  assert(string_queue->equals(another_string_queue));
  assert(string_queue->hash() == another_string_queue->hash());
}

void object_queue_normal_usage() {
  /* Tests the normal usage of a StringQueue - adding, removing, peeking, and checking the size of the queue */
  ObjectQueue* object_queue = new ObjectQueue();

  // empty queue size
  assert(object_queue->size() == 0);

  // add one item
  String* item = new String("foobar");
  object_queue->add(item);
  assert(object_queue->size() == 1);
  assert(object_queue->peek()->equals(item));

  // add another item
  String* item2 = new String("tube");
  object_queue->add(item2);
  assert(object_queue->size() == 2);
  assert(object_queue->peek()->equals(item));

  // remove an item
  String* popped = object_queue->remove();
  assert(popped->equals(item));
  assert(object_queue->size() == 1);
  assert(object_queue->peek()->equals(item2));

  // remove another item
  popped = object_queue->remove();
  assert(popped->equals(item2));
  assert(object_queue->size() == 0);

  delete object_queue;
}

void object_queue_advanced_usage() {
  /* Tests more advanced / niche usage of a StringQueue - concating ObjectQueue, equality, clearing, etc. */
  ObjectQueue* p_object_queue =   new ObjectQueue();
  ObjectQueue* another_object_queue = new ObjectQueue();
  String* string1 = new String("software_dev");
  String* string2 = new String("is very fun");
  String* string3 = new String("seriously!");

  // adding empty self to empty self
  assert(p_object_queue->size() == 0);
  p_object_queue->add_all(p_object_queue);
  assert(p_object_queue->size() == 0);

  // adding empty to empty
  assert(p_object_queue->size() == 0);
  p_object_queue->add_all(another_object_queue);
  assert(p_object_queue->size() == 0);

  // adding empty to non-empty
  p_object_queue->add(string1);
  p_object_queue->add(string2);
  p_object_queue->add(string3);
  assert(p_object_queue->size() == 3);

  assert(another_object_queue->size() == 0);
  p_object_queue->add_all(another_object_queue);
  assert(p_object_queue->size() == 3);

  // adding non-empty to empty
  assert(another_object_queue->size() == 0);
  assert(p_object_queue->size() == 3);
  another_object_queue->add_all(p_object_queue);
  assert(p_object_queue->size() == 3);
  assert(another_object_queue->size() == 3);
  assert(p_object_queue->equals(another_object_queue));

  // clearing a previously added string queue has no effect on the string queue that was added to
  assert(p_object_queue->size() == 3);
  assert(another_object_queue->size() == 3);
  p_object_queue->clear();
  assert(p_object_queue->size() == 0);
  assert(another_object_queue->size() == 3);

  // add non-empty self to self
  another_object_queue->add_all(another_object_queue);
  assert(another_object_queue->size() == 6);
  String* popped = another_object_queue->remove();
  assert(popped->equals(string1));
  popped = another_object_queue->remove();
  assert(popped->equals(string2));
  popped = another_object_queue->remove();
  assert(popped->equals(string3));
  popped = another_object_queue->remove();
  assert(popped->equals(string1));
  popped = another_object_queue->remove();
  assert(popped->equals(string2));
  popped = another_object_queue->remove();
  assert(popped->equals(string3));
  assert(another_object_queue->size() == 0);

  delete p_object_queue;
  delete another_object_queue;
  delete string1;
  delete popped;
  delete string2;
  delete string3;
}

void object_queue_hashing_and_equality() {
  /* Testing hashing and equality of StringQueues */
  ObjectQueue* object_queue = new ObjectQueue();
  ObjectQueue* another_object_queue = new ObjectQueue();
  String* string1 = new String("hofamnn");
  String* string2 = new String("banana");
  String* string3 = new String("foobar");

  // empty
  assert(object_queue->equals(another_object_queue));
  assert(object_queue->hash() == another_object_queue->hash());

  // add item
  object_queue->add(string1);
  assert(!object_queue->equals(another_object_queue));

  // add item to other queue
  another_object_queue->add(string1);
  assert(object_queue->equals(another_object_queue));
  assert(object_queue->hash() == another_object_queue->hash());

  // add more items
  object_queue->add(string2);
  another_object_queue->add(string2);
  object_queue->add(string3);
  another_object_queue->add(string3);
  object_queue->add(string1);
  another_object_queue->add(string1);
  assert(object_queue->equals(another_object_queue));
  assert(object_queue->hash() == another_object_queue->hash());

  // clear and add
  object_queue->clear();
  another_object_queue->clear();
  assert(object_queue->equals(another_object_queue));
  assert(object_queue->hash() == another_object_queue->hash());

  delete object_queue;
  delete another_object_queue;
}

int main() {
  string_queue_normal_usage();
  string_queue_advanced_usage();
  string_queue_hashing_and_equality();
  object_queue_normal_usage();
  object_queue_advanced_usage();
  object_queue_hashing_and_equality();
}