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
