#include "queue.h"
#include "string.h"

void string_queue_normal_usage() {
  /* Tests the normal usage of a Queue - adding, removing, peeking, and checking the size of the queue */
  Queue* string_queue = new Queue();

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
  String* popped = static_cast<String*>(string_queue->remove());
  assert(popped->equals(item));
  assert(string_queue->size() == 1);
  assert(string_queue->peek()->equals(item2));

  // remove another item
  popped = static_cast<String*>(string_queue->remove());
  assert(popped->equals(item2));
  assert(string_queue->size() == 0);

  delete string_queue;
  delete item;
  delete item2;
}

void string_queue_advanced_usage() {
  /* Tests more advanced / nicheusage of a Queue - concating Queues, equality, clearing, etc. */
  Queue* string_queue = new Queue();
  Queue* another_string_queue = new Queue();
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
  String* popped = static_cast<String*>(another_string_queue->remove());
  assert(popped->equals(string1));
  popped = static_cast<String*>(another_string_queue->remove());
  assert(popped->equals(string2));
  popped = static_cast<String*>(another_string_queue->remove());
  assert(popped->equals(string3));
  popped = static_cast<String*>(another_string_queue->remove());
  assert(popped->equals(string1));
  popped = static_cast<String*>(another_string_queue->remove());
  assert(popped->equals(string2));
  popped = static_cast<String*>(another_string_queue->remove());
  assert(popped->equals(string3));
  assert(another_string_queue->size() == 0);

  delete string_queue;
  delete another_string_queue;
  delete string1;
  delete string2;
  delete string3;
  // shouldn't delete `popped`, since it is aliased to string3.
  // if we tried to free it here, it would cause a double free bug
}

void string_queue_hashing_and_equality() {
  /* Testing hashing and equality of Queues */
  Queue* string_queue = new Queue();
  Queue* another_string_queue = new Queue();
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

  delete string_queue;
  delete another_string_queue;
  delete string1;
  delete string2;
  delete string3;
}

void object_queue_normal_usage() {
  /* Tests the normal usage of a Queue - adding, removing, peeking, and checking the size of the queue */
  Queue* object_queue = new Queue();

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
  String* popped = static_cast<String*>(object_queue->remove());
  assert(popped->equals(item));
  assert(object_queue->size() == 1);
  assert(object_queue->peek()->equals(item2));

  // remove another item
  popped = static_cast<String*>(object_queue->remove());
  assert(popped->equals(item2));
  assert(object_queue->size() == 0);

  delete object_queue;
  delete item;
  delete item2;
}

void object_queue_advanced_usage() {
  /* Tests more advanced / niche usage of a Queue - concating Queue, equality, clearing, etc. */
  Queue* p_object_queue =   new Queue();
  Queue* another_object_queue = new Queue();
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
  String* popped = static_cast<String*>(another_object_queue->remove());
  assert(popped->equals(string1));
  popped = static_cast<String*>(another_object_queue->remove());
  assert(popped->equals(string2));
  popped = static_cast<String*>(another_object_queue->remove());
  assert(popped->equals(string3));
  popped = static_cast<String*>(another_object_queue->remove());
  assert(popped->equals(string1));
  popped = static_cast<String*>(another_object_queue->remove());
  assert(popped->equals(string2));
  popped = static_cast<String*>(another_object_queue->remove());
  assert(popped->equals(string3));
  assert(another_object_queue->size() == 0);

  delete p_object_queue;
  delete another_object_queue;
  delete string1;
  delete string2;
  delete string3;
  // no need to delete `popped`, since it is aliased to string3.
}

void object_queue_hashing_and_equality() {
  /* Testing hashing and equality of Queues */
  Queue* object_queue = new Queue();
  Queue* another_object_queue = new Queue();
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
  delete string1;
  delete string2;
  delete string3;
}

void object_queue_big_usage() {
    /* Tests the normal usage of a Queue - adding, removing, peeking, and checking the size of the queue */
    Queue *object_queue = new Queue();
    String *item = new String("foobar");
    String *item2 = new String("tube");
    String *item3 = new String("cube");

    // empty queue size
    assert(object_queue->size() == 0);

    // add one item
    object_queue->add(item);
    assert(object_queue->size() == 1);
    assert(object_queue->peek()->equals(item));

    // add another item
    object_queue->add(item2);
    assert(object_queue->size() == 2);
    assert(object_queue->peek()->equals(item));

    // remove an item
    String *popped = static_cast<String *>(object_queue->remove());
    assert(popped->equals(item));
    assert(object_queue->size() == 1);
    assert(object_queue->peek()->equals(item2));

    // remove another item
    popped = static_cast<String *>(object_queue->remove());
    assert(popped->equals(item2));
    assert(object_queue->size() == 0);

    // queue should now be empty

    // add 10000 items
    for (int i = 0; i < 10000; i++) {
        // which item to add
        int itemIndex = i % 3;

        // add one of item, item2, or item3
        switch (itemIndex) {
            case 0:
                object_queue->add(item);
                break;
            case 1:
                object_queue->add(item2);
                break;
            case 2:
                object_queue->add(item3);
                break;
        }
    }

    // ensure length is 10000
    assert(object_queue->size() == 10000);

    // dequeue 2512 items
    for (int i = 0; i < 2512; i++) {
        object_queue->remove();
    }

    // ensure length is 10000 - 2512 = 7488
    assert(object_queue->size() == 7488);

    // dequeue 7481 items
    for (int i = 0; i < 7481; i++) {
        object_queue->remove();
    }

    // ensure length is 7488 - 7481 = 7
    assert(object_queue->size() == 7);

    // make sure next value in queue is (10000-7) % 3 = 0 --> item
    assert(object_queue->peek()->equals(item));

    // enqueue 1234 items
    for (int i = 0; i < 1234; i++) {
        object_queue->add(item2);
    }

    // ensure length is 7 + 1234 = 1241
    assert(object_queue->size() == 1241);

    // make new queue
    Queue *object_queue2 = new Queue();

    // add all elements from object_queue to object_queue2
    object_queue2->add_all(object_queue);

    // ensure they are equal
    assert(object_queue->size() == object_queue2->size());
    assert(object_queue->hash() == object_queue2->hash());
    assert(object_queue2->equals(object_queue));
    assert(object_queue->equals(object_queue2));

    // remove all elements 1241 elements from object_queue
    for (int i = 0; i < 1241; i++) {
        object_queue->remove();
    }

    // ensure object_queue is empty
    assert(object_queue->size() == 0);

    // ensure empty object_queue is equals() to a new empty queue
    Queue *object_queue3 = new Queue();
    assert(object_queue->size() == object_queue3->size());
    assert(object_queue->hash() == object_queue3->hash());
    assert(object_queue->equals(object_queue3));
    assert(object_queue3->equals(object_queue));

    // ensure object_queue (empty) is not equal to object_queue_2 (with 1241 elements)
    // ensure object_queue3 (empty) is not equal to object_queue_2 (with 1241 elements)
    assert(object_queue->size() != object_queue2->size());
    assert(!object_queue->equals(object_queue2));
    assert(!object_queue2->equals(object_queue));

    assert(object_queue3->size() != object_queue2->size());
    assert(!object_queue3->equals(object_queue2));
    assert(!object_queue2->equals(object_queue3));

    // add all remaining 1241 elements from object_queue2 into object_queue3
    for (int i = 0; i < 1241; i++) {
        object_queue3->add(object_queue2->remove());
    }

    // ensure object_queue2 is empty, and object_queue3 has 1241 items
    assert(object_queue2->size() == 0);
    assert(object_queue2->equals(object_queue));
    assert(object_queue->equals(object_queue2));

    assert(object_queue3->size() == 1241);

    // remove remaining 1241 items from object_queue3
    for (int i = 0; i < 1241; i++) {
        object_queue3->remove();
    }

    // ensure object_queue3 is emmpty
    assert(object_queue3->size() == 0);

    // ensure object_queue, object_queue2, and object_queue3 are all equal and empty.
    assert(object_queue->equals(object_queue2));
    assert(object_queue2->equals(object_queue3));
    assert(object_queue->equals(object_queue3));
    assert(object_queue->size() == 0);
    assert(object_queue2->size() == 0);
    assert(object_queue3->size() == 0);
    assert(object_queue->hash() == object_queue2->hash());
    assert(object_queue2->hash() == object_queue3->hash());

    delete object_queue;
    delete object_queue2;
    delete object_queue3;
    delete item;
    delete item2;
    delete item3;
}

int main() {
  string_queue_normal_usage();
  string_queue_advanced_usage();
  string_queue_hashing_and_equality();
  object_queue_normal_usage();
  object_queue_advanced_usage();
  object_queue_hashing_and_equality();
  object_queue_big_usage();
}
