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
from the course's webpage during the second semester of academic year 2017-2018. A report explaining the contents of
this repository can be found [here](https://github.com/lluisalemanypuig/box-wrapping/blob/master/report.pdf).

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

## Dependencies

Apart from the tools and libraries mentioned above, this project has other dependencies
that have to be taken into account.

In order to compile each part of the project, one needs to have the [make](https://www.gnu.org/software/make/)
tool installed. It is mandatory to have a compiler that supports C++11. Also, for the Satisfiability part,
a _bash_ interpreter is needed to run the scripts.

### Tools

Compiling the [box-wrapper](https://github.com/lluisalemanypuig/box-wrapping/tree/master/box-wrapper) interface
requires the use of either the _qmake_ tool, or the _Qt creator_ IDE.

### Libraries

The interface [box-wrapper](https://github.com/lluisalemanypuig/box-wrapping/tree/master/box-wrapper) needs the
[Qt](https://www.qt.io/) library. Bear in mind that, since it uses OpenGL, it is also needed to have installed the
<QtOpenGL> corresponding modules of the Qt library.
