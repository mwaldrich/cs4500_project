#pragma once
//lang::CwC

/* A class that representing the top of a object hierarchy */
class Object
{
  
  // Empty object constructor
  Object();
  
  // Virtual object destructor; able to be overriden by subclasses
  virtual ~Object();

  /* Returns this object's hash value. Two objects that are equal should have the same hash; object's that are not
   * may or may not have the same hash. */
  virtual size_t hash();

  /* Trivial hash function. */
  virtual size_t hash_me_();

  /* Returns if other Object is the same as this Object */
  virtual bool equals(Object *other);

  /* Returns a C style string describing this Object */
  virtual char *to_string();

  /* Prints a representation of this object to stdout */
  virtual void print();
};
