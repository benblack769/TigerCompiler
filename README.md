## ReadMe
### Ben Black, Brandon Finley


### Symbol tables

Symbol table header as well as helper classes are in `symbol_table.hh`. These symbol tables have separate tables for types and for variables/functions.

Type tables: Integer/String/Void/Nil can simply be represented with an Enum. But records and arrays cannot. Since record and array type expressions, not their string labels or structure are what make them unique, then they are represented as an integer label, called `value_id` in TypeExpr, which points to a location on a vector of such values on the type table.

Variables/functions are stored in the same table as each other, using a union-like structure to store them interchangeably.

The table as a whole is an inefficient functional style, using quadratic time and space to store redundant entries on the stack. Mutually recursive types and functions greatly complicate API and logic, but not data structures.

### Test cases

All the test cases code are in the `language_examples` folder. The appropriate error is documented in `test_semantics.cc`. To get location info run `parse_and_print` executable. Not all errors have associated location information, especially stuff to do with recursive types. These errors simply say they are on line -1. Unit tests things like types matching function arguments, scope in various places, mutually recursive types, recursive functions, etc.

Notably, the tests include the complex examples from the back of the tiger book.
