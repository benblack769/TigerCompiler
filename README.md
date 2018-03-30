## ReadMe
### Ben Black, Brandon Finley


### Symbol tables

Pretty boring. Inefficient functional style, quadratic time and space. Mutually recursive types and functions greatly complicate API and logic, but not data structures.

### Test cases

All the test cases are in the `language_examples` folder. The appropriate error is documented in `test_semantics.cc`. To get location info run `parse_and_print` executable. Not all errors have associated type information, especially stuff to do with recursive types. Unit tests things like types matching function arguments, scope in various places, mutually recursive types.

Notably, the tests include the complex examples from the back of the tiger book.
