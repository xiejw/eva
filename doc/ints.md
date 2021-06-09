Ints
====

C has many, indeed many, types for the integers, e.g., `char`, `short`, etc.

| Types                 | Header File | Note |
|-----------------------|-------------| ---- |
|`[u]int(8/16/32/64)_t` | `stdint.h`  | Exactly 8, 16, 32 and 64 bits |
|`[u]intptr_t`          | `stdint.h`  | Integer type capable of holding a pointer |
|`[s]size_t`            | `stddef.h`  | Integral type of the result of the sizeof operator|
|`ptrdiff_t`            | `stddef.h`  | Signed integral type of the result of subtracting two pointers|
