## ReadMe
### Ben Black, Brandon Finley, Matt Atteberry, Alex Grant

### IR Trees

All of our ir tree classes come in two flavors: `stm` and `exp`. `exp` produces a value, `stm` does not. All of the ir tree classes inherit from `stm` or `exp` which in turn inherit from an `IRTreeNode` class.  All of the IRtree nodes have a `toStr` class which will print out their sub tree, with each level of the tree getting indented more. Here are the types of IR tree nodes, they're mostly the same as the book but classes:

##### Const
Inherits from exp. Represents a literal word written in assembly as an integer.

##### Name
Inherits from exp. Represents an assembly label. Exacly like `Label` but it is an exp rather than a stm.

##### Temp
exp. Like a register, but there are infinite of them.

##### Binop
exp. Does some operation on two exps. Types of operations are: PLUS, MINUS, MUL, DIV, AND,  OR, XOR, Left SHIFT, Right SHIFT, Arithemtic Right SHIFT.

##### Mem
exp. Access or write to memory at the given address.

##### Call
exp. Call a function at the given label with the given arguments.

##### Eseq
exp. Two nodes joined together where the left one is a stm and the right is an exp. The value of the sequence is the value of the right

##### Move
stm. Put a value into a temp or into memory

##### Exp
stm. Points to a node of type exp, but does not return a value. If I want to make a node that doesn't do anything, I make a `Exp(Const(0))`

##### Jump
stm. Go to a label. Unlike the book, it does not take a labelist since we will not do dataflow analysis.

##### CJump
stm. Compare two exps and then jump to one label if the comparision is true and another if it's false.

##### Seq
stm. Like Eseq, but both nodes are stms

##### Label
stm. An assembly label, but with no expected value. So, you'd use Name before a function, but Label before a procedure.

### Temp
`temp.hh`/`.cc` Has functions `newlabel` and `newtemp` that get called by translate functions whenever you need to make a label or temp with a unique name. Makes sure names don't overlap.

### Frame
`frame.hh` has functions relating to the frame. It will generate an ir tree for getting the frame pointer for the translate functions.

### Translating

Each class of `ASTNode` has a `translate` function that returns a pointer to an IR tree. These functions are defined in `ast_translate.cc`. The main function in `tc.cc` calls the semantic checks on the root node, then calls translate on it. 


### Symbol tables

Symbol table header as well as helper classes are in `symbol_table.hh`. These symbol tables have separate tables for types and for variables/functions.

Type tables: Integer/String/Void/Nil can simply be represented with an Enum. But records and arrays cannot. Since record and array type expressions, not their string labels or structure are what make them unique, then they are represented as an integer label, called `value_id` in TypeExpr, which points to a location on a vector of such values on the type table.

So it looks more or less like this:

    BaseType = {INT,STR,ARRAY, ...}

    TypeExpr:
        BaseType
        value_id

    TypeTable:
        str -> TypeExpr
        value_id -> ArrayExpr
        value_id -> RecordExpr

Variables/functions are stored in the same table as each other, using a union-like structure to store them interchangeably.

The table as a whole is an inefficient functional style, using quadratic time and space to store redundant entries on the stack. Mutually recursive types and functions greatly complicate API and logic, but not data structures.

### Test cases

All the test cases code are in the `language_examples` folder. The appropriate error is documented in `test_semantics.cc`. To get location info run `parse_and_print` executable. Not all errors have associated location information, especially stuff to do with recursive types. These errors simply say they are on line -1. Unit tests things like types matching function arguments, scope in various places, mutually recursive types, recursive functions, etc.

Notably, the tests include the complex examples from the back of the tiger book.

### Non-passing tests

After adding in the tests from the book, now there are several non-passing tests, all dealing with either void or nil expressions.

These tests are:

* 10 (enforcing void while bodies)
* 15 (enforcing void if then bodies)
* 40 (enforcing void procedure returns)
* 45 (disallowing initializing nil vars not constrained by record type)
* 46 (using <> comparison on records)
