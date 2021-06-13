# Volume 4

## Horn Clause and Formula

A Horn clause is a clause (a disjunction of literals) with at most one positive
literal.  A Horn formula is a propositional formula formed by conjunction of
Horn clauses.

### Definite Horn Formula

The definite Horn formula must satisfy

```
f(1, 1, ..., 1) = 1
```

The core of the definite Horn formula is set the variables which must be true
whenever `f` is true.

### Algorithrm C



### Indefinite Horn Formula

Exercise 48 provides the steps to test satisfiability of Horn formula in
general. The idea is quite simple

1. Introduce a new variable `lambda`, and convert all indefinite causes to
   definite cause. For example, the following indefinite Horn clause

        not a or not b

   will be converted as

        not a or not b or lambda.

2. Apply Algorithrm C to the new definite Horn formula. The original Horn
   formula is satisfiable if and only if `lambda` is not in the core of the new
   definite Horn formula.
