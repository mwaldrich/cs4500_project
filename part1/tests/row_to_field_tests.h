#include <cassert>
#include "../str.h"
#include "../row_to_fields.h"

/* Module testing our SOR row to fields parser */

void single_fields() {
  String* row;
  String* output;
  StrList* fields = new StrList();

  // empty field
  row = new String("<>");
  output = new String("");
  fields = row_to_fields(row);
  assert(fields->size() == 1);
  assert(fields->get(0)->equals(output));
  delete row;
  delete output;
  delete fields;

  // string field
  row = new String("<foobar>");
  output = new String("foobar");
  fields = row_to_fields(row);
  assert(fields->size() == 1);
  assert(fields->get(0)->equals(output));
  delete row;
  delete output;
  delete fields;

  // number field
  row = new String("<123890>");
  output = new String("123890");
  fields = row_to_fields(row);
  assert(fields->size() == 1);
  assert(fields->get(0)->equals(output));
  delete row;
  delete output;
  delete fields;

  // field with quotes
  row = new String("<\"tyu3\">");
  output = new String("\"tyu3\"");
  fields = row_to_fields(row);
  assert(fields->size() == 1);
  assert(fields->get(0)->equals(output));
  delete row;
  delete output;
  delete fields;

  // single space around delimiter
  row = new String(" <tanks> ");
  output = new String("tanks");
  fields = row_to_fields(row);
  assert(fields->size() == 1);
  assert(fields->get(0)->equals(output));
  delete row;
  delete output;
  delete fields;

  // multiple spaces around delimiter
  row = new String("   <kthx>   ");
  output = new String("kthx");
  fields = row_to_fields(row);
  assert(fields->size() == 1);
  assert(fields->get(0)->equals(output));
  delete row;
  delete output;
  delete fields;

  // single spaces within delimiter
  row = new String("< kthx >");
  output = new String("kthx");
  fields = row_to_fields(row);
  assert(fields->size() == 1);
  assert(fields->get(0)->equals(output));
  delete row;
  delete output;
  delete fields;

  // single spaces within delimiter
  row = new String("< kthx >");
  output = new String("kthx");
  fields = row_to_fields(row);
  assert(fields->size() == 1);
  assert(fields->get(0)->equals(output));
  delete row;
  delete output;
  delete fields;

  // multiple spaces within delimiter
  row = new String(" <  kthx   >");
  output = new String("kthx");
  fields = row_to_fields(row);
  assert(fields->size() == 1);
  assert(fields->get(0)->equals(output));
  delete row;
  delete output;
  delete fields;

  // spaces within and around delimiter
  row = new String("    < kthx >    ");
  output = new String("kthx");
  fields = row_to_fields(row);
  assert(fields->size() == 1);
  assert(fields->get(0)->equals(output));
  delete row;
  delete output;
  delete fields;

  // spaces within and around delimiter with spaces in quotes
  row = new String("    <  \"  gha \"   >    ");
  output = new String("\"  gha \"");
  fields = row_to_fields(row);
  assert(fields->size() == 1);
  assert(fields->get(0)->equals(output));
  delete row;
  delete output;
  delete fields;

  // spaces within delimiter with quotes
  row = new String("<   \"kthxbi\"    >");
  output = new String("\"kthxbi\"");
  fields = row_to_fields(row);
  assert(fields->size() == 1);
  assert(fields->get(0)->equals(output));
  delete row;
  delete output;
  delete fields;

  // spaces within delimiter with spaces in quotes
  row = new String("<   \"   \"    >");
  output = new String("\"   \"");
  fields = row_to_fields(row);
  assert(fields->size() == 1);
  assert(fields->get(0)->equals(output));
  delete row;
  delete output;
  delete fields;

  // spaces around delimiter with spaces in quotes
  row = new String("   <\"   \">   ");
  output = new String("\"   \"");
  fields = row_to_fields(row);
  assert(fields->size() == 1);
  assert(fields->get(0)->equals(output));
  delete row;
  delete output;
  delete fields;

  // spaces around and within delimiter with spaces in quotes
  row = new String("   <  \"   \"    >   ");
  output = new String("\"   \"");
  fields = row_to_fields(row);
  assert(fields->size() == 1);
  assert(fields->get(0)->equals(output));
  delete row;
  delete output;
  delete fields;

  puts("single field tests passed");
}

void multiple_fields() {
  String* row;
  String* output;
  String* output1;
  StrList* fields = new StrList();

  // two empty items
  row = new String("<><>");
  output = new String();
  fields = row_to_fields(row);
  assert(fields->size() == 2);
  assert(fields->get(0)->equals(output));
  assert(fields->get(1)->equals(output));
  delete row;
  delete output;
  delete fields;

  // two items
  row = new String("<foo><bar>");
  output = new String("foo");
  output1 = new String("bar");
  fields = row_to_fields(row);
  assert(fields->size() == 2);
  assert(fields->get(0)->equals(output));
  assert(fields->get(1)->equals(output1));
  delete row;
  delete output;
  delete output1;
  delete fields;

  // two items with spaces between
  row = new String("<foo>  <bar>");
  output = new String("foo");
  output1 = new String("bar");
  fields = row_to_fields(row);
  assert(fields->size() == 2);
  assert(fields->get(0)->equals(output));
  assert(fields->get(1)->equals(output1));
  delete row;
  delete output;
  delete output1;
  delete fields;

  // two items with spaces before
  row = new String("  <foo><bar>");
  output = new String("foo");
  output1 = new String("bar");
  fields = row_to_fields(row);
  assert(fields->size() == 2);
  assert(fields->get(0)->equals(output));
  assert(fields->get(1)->equals(output1));
  delete row;
  delete output;
  delete output1;
  delete fields;

  // two items with spaces after
  row = new String("<foo><bar>  ");
  output = new String("foo");
  output1 = new String("bar");
  fields = row_to_fields(row);
  assert(fields->size() == 2);
  assert(fields->get(0)->equals(output));
  assert(fields->get(1)->equals(output1));
  delete row;
  delete output;
  delete output1;
  delete fields;

  // two items with spaces all over
  row = new String("   <foo>   <bar>  ");
  output = new String("foo");
  output1 = new String("bar");
  fields = row_to_fields(row);
  assert(fields->size() == 2);
  assert(fields->get(0)->equals(output));
  assert(fields->get(1)->equals(output1));
  delete row;
  delete output;
  delete output1;
  delete fields;

  // two items with spaces within first
  row = new String("<  foo ><bar>");
  output = new String("foo");
  output1 = new String("bar");
  fields = row_to_fields(row);
  assert(fields->size() == 2);
  assert(fields->get(0)->equals(output));
  assert(fields->get(1)->equals(output1));
  delete row;
  delete output;
  delete output1;
  delete fields;

  // two items with spaces within second
  row = new String("<foo>< bar >");
  output = new String("foo");
  output1 = new String("bar");
  fields = row_to_fields(row);
  assert(fields->size() == 2);
  assert(fields->get(0)->equals(output));
  assert(fields->get(1)->equals(output1));
  delete row;
  delete output;
  delete output1;
  delete fields;

  // two items with spaces within both
  row = new String("<foo ><  bar  >");
  output = new String("foo");
  output1 = new String("bar");
  fields = row_to_fields(row);
  assert(fields->size() == 2);
  assert(fields->get(0)->equals(output));
  assert(fields->get(1)->equals(output1));
  delete row;
  delete output;
  delete output1;
  delete fields;

  // two items with spaces within and all around
  row = new String("  <  foo >  <  bar  >   ");
  output = new String("foo");
  output1 = new String("bar");
  fields = row_to_fields(row);
  assert(fields->size() == 2);
  assert(fields->get(0)->equals(output));
  assert(fields->get(1)->equals(output1));
  delete row;
  delete output;
  delete output1;
  delete fields;

  puts("multiple field tests passed");
}

void row_to_field_tests() {
  single_fields();
  multiple_fields();
}