# IOS 17

## Chapter 19 Preprocessing directives

### 19.3 Macro replacement [cpp.replace]

The identifiers `__VA_ARGS__` and `__VA_OPT__` shall occur only in the
replacement-list of a function-like macro that uses the ellipsis notation in the
parameters.

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

An identifier `__VA_ARGS__` that occurs in the replacement list shall be treated
as if it were a parameter, and the variable arguments shall form the
preprocessing tokens used to replace it.

The token sequence `__VA_OPT__`(content) shall be treated as if it were a
parameter, and the preprocessing tokens used to replace it are defined as
follows. If the variable arguments consist of no tokens, the replacement
consists of a single placemarker preprocessing token.  Otherwise, the
replacement consists of the results of the expansion of content as the
replacement list of the current function-like macro before rescanning and
further replacement.

#### 19.3.3 The `#` operator [cpp.stringize]

Each `#` preprocessing token in the replacement list for a function-like macro
shall we followed by a parameter as next preprocessing token in the replacement
list.

If, in the replacement list, a parameter is immediately preceded by a `#`
preprocessing token, both are are replaced by a single character string literal
preprocessing token that contains the spelling of the preprocessing token
sequence for the corresponding argument.



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

#### 19.3.4 Rescanning and further replacement [cpp.rescan]

After all parameters in the replacement list have been substituted and `#` and
`##` processing has taken place...Then the resulting preprocessing token
sequence is rescanned, along with all subsequent preprocessing token of the
source file.

