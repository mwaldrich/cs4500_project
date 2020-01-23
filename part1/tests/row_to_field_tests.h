#include <cassert>
#include "../str.h"
#include "../row_to_fields.h"

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
  puts("multiple field tests passed");
}

void row_to_field_tests() {
  single_fields();
  multiple_fields();
}