# IOS 17

## Chapter 19 Preprocessing directives

### 19.3 Macro replacement [cpp.replace]

A preprocessing directive of the form

    # define identifier replacement-list new-line

defines an _object-like_ macro... The replacement list is then re-scanned for
more macro names.

A preprocessing directive of the form

    # define identifier lparen replacement-list_opt ) new-line

defines a _function-like_ macro with parameters.

#### 19.3.1 Argument substitution [cpp.subst]

A parameter in the replacement list, unless preceded by a `#` or `##`
preprocessing token or followed by a `##` preprocessing token, is replaced by
the corresponding argument **after** all macros contained therein have been
expanded.

#### 19.3.3 The `##` operator [cpp.concat]

If, in the replacement list of a function-like macro, a parameter is immediately
preceded or followed by a `##` preprocessing token, the parameter is replaced by
the corresponding argument's preprocessing token sequence.

> Personal note: This is the reason why to expand `__COUNTER__`, we need two
> macros, rather than one.

    #include <iostream>

    // The following code does not work.
    //
    //    #define CONCAT(x, y) x##y
    #define CONCAT(x, y) CONCAT_IMPL(x, y)
    #define CONCAT_IMPL(x, y) x##y

    #define MKSTR(x) # x
    #define NAME(x) MKSTR(x)

    #define UNIQUE_NAME(x) NAME(CONCAT(x, __COUNTER__))
    #define PRINT_NEW_NAME

    int main(int argc, char** argv) {
      std::cout << UNIQUE_NAME(h) << "\n";
      std::cout << UNIQUE_NAME(h) << "\n";
      return 0;
    }

> End of Personal note.
