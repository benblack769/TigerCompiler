## ReadMe
### Ben Black, Brandon Finley


### AST design

The AST nodes are organized to match the parse tree as closely as reasonable. This makes it easy to reason about the AST, but gives it additional verbosity and depth.

All nodes inherit from ASTNode in `ast_node.hh`. This class only has an abstract `print` method, and stores location information.

The base classes, in `ast_interface.hh` are the classes which are referenced in other classes. They match the base types of the parse tree, for example expr, lvalue and declaration list.

The different types of expressions, lvalues, type expressions, etc, are in different files in the `ast_specifics` folder. Ideally, the only time these files should be included is when creating the tree (in this case, tiger.y).

Finally, the DeclarationList has additionally complex logic for recursive types, so it has its own implementation file, `ast_specifics/dec_list_impl.cc`. Since this logic needs the actual types of the declarations, there is some dynamic casting to the derived classes of Declaration.

### Symbol tables

Symbol table header as well as helper classes are in `symbol_table.hh`. These symbol tables have separate tables for types and for variables/functions.

The table as a whole is an inefficient functional style, using quadratic time and space to store the redundant entries on the stack. New scopes are created in let, for expressions and function declarations.

Type tables: Integer/String/Void/Nil can simply be represented with an Enum. But records and arrays cannot. Since record and array type expressions, not their string labels or structure, are what make them unique, they are represented as an integer label, called `value_id` in TypeExpr, which points to a location on a vector of such values on the type table.

So it looks more or less like this:

    BaseType = {INT,STR,ARRAY, ...}

    TypeExpr:
        BaseType
        value_id

    TypeTable:
        str -> TypeExpr
        value_id -> ArrayExpr
        value_id -> RecordExpr

Variables/functions are stored in the same table as each other, using a union style structure (but not an actual union) to store them interchangeably.


### Test cases

All the test cases code are in the `language_examples` folder. The appropriate error code documented in `test_semantics.cc`, and sometimes in a comment in the tiger test file. Unit tests things like types matching function arguments, scope in various places, mutually recursive types, recursive functions, etc.

Mutually recursive types: this test passes

    let type a = {x: d, y: c}
        type b = d
        type d = a
        type f = array of c
        type c = {x: a, y: c, z: f, w: b}
        var x : c := nil
    in
        x.x.x.y.z[3].x.y.w
    end

And this gives a special error code for cyclically dependent types:

    let type a = c
        type c = a
    in
        3
    end


Recursive call type is checked:

    /*error in recursive call*/
    let
        function f(a:int):int =
            if a = 0 then 0
            else f("hello")
    in
        f(10)
    end

Please go through the `language_examples` folder to see the 31 other tests and examples.

### Semantic error handling

To get location info run `parse_and_print` executable. Not all errors have associated location information, especially stuff to do with recursive types. These errors simply say they are on line -1.
Error handling prints out line numbers and error type. For example on this input:

    let
        var a:int := break
    in
        a + 2
    end

It will produce the following result:

    Error found in line:2:
    BAD_TYPE_MATCH

(This is a type error because the "break" expression returns a void type).
