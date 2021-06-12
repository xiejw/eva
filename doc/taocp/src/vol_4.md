# Volume 4

## Horn

### Definite Horn

### Indefinite Horn

Exercise 48 provides the steps to test satisfiability of Horn. The idea is
simple

1. Introduce a new variable `lambda`, and convert all indefinite causes to
   definite cause. For example, `not a and not b` will be converted as `not a
   and not b and lambda`.
2. Apply Algorithrm C to the new definite Horn. The original Horn is
   satisfiable if and only if `lambda` is not in the core of the new definite
   Horn.
