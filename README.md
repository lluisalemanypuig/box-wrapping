# Box Wrapping Problem (BWP)

This repository contains 3 different approaches to solve the Box Wrapping Problem:
* [Constraint Programming](https://github.com/lluisalemanypuig/box-wrapping/tree/master/CP)
* [Linear Programming](https://github.com/lluisalemanypuig/box-wrapping/tree/master/LP)
* [Satisfiability](https://github.com/lluisalemanypuig/box-wrapping/tree/master/SAT)

This project was developed during the second semester of the academic year 2017-2018 as part
of the [Combinatorial Problem solving](http://www.cs.upc.edu/~erodri/webpage/cps/cps.html)
course project from the Master in Innovation and Research in Informatics (MIRI).
The complete specification of the problem (input and output format, exact goals, ..) can be found
[in this pdf file](https://github.com/lluisalemanypuig/box-wrapping/blob/master/statement.pdf), which could be downloaded
from the course's webpage during the second semester of academic year 2017-2018. The report explaining the contents of
this repository can be found [here](https://github.com/lluisalemanypuig/box-wrapping/blob/master/report.pdf).

This project was graded with:
* Constraint Programming: 10/10
* Linear Programming: 9/10
* Satisfiability: 10/10

## Constraint programming (CP)

The Constraint Programming approach consists on implementing a mathematical model using
[Gecode](http://www.gecode.org/doc-latest/reference/index.html) (v. 6.0.0).

## Linear Programming (LP)

The Linear Programming approach consists on implementing a mathematical model using
[Cplex](https://www.ibm.com/support/knowledgecenter/SS9UKU_12.7.0/com.ibm.cplex.zos.help/CPLEX/homepages/CPLEX_Z.html)
(v. 12.7.0).

## Satisfiability (SAT)

The Satisfiablity approach consists on generating a Boolean formula in conjunctive normal
form (CNF) so that a SAT solver can solve it. The SAT solver used is [lingeling](https://github.com/arminbiere/lingeling).
