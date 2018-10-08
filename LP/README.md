# Linear Programming

This part of the project uses the [Cplex](https://www.ibm.com/support/knowledgecenter/SS9UKU_12.7.0/com.ibm.cplex.zos.help/CPLEX/homepages/CPLEX_Z.html)
library (version 12.7.0) to solve the Box Wrapping Problem. See a detailed description of
the mathematical models used in the [report](https://github.com/lluisalemanypuig/box-wrapping/blob/master/report.pdf)

## Compilation

In order to compile the project, first open a terminal and go inside the root
directory of the project. Then, issue the following commands.

		cd LP/build-rules
		make -f Makefile release

## Execution

When in the root directory of the project, and after having compiled, issue
the following commands:

		cd LP/build-release
		./wrapping-boxes --help

The last command will display the usage of the program. This is the help
displayed:

		BOX WRAPPING PROBLEM - Linear Programming Solver
		
		This software uses the library CPLEX (tested under version 12.7)
		to solve the Box Wrapping Problem (BWP). It admits several options
		and are described below. Most of them are optional but it requires
		the specification of an input file containing the data, the solver
		that will be used, and its mode of execution, in case it has any.
		
		Parameters:
		    [-h, --help]: show the usage
		    [-i, --input]: specify the input file
		    [-o, --output]: specify the output file.
		        Only the first solution will be stored.
		    [-v, --verbose]: tell the CPLEX solver to output its progress.
		        Other messages will also be displayed.
		        Default: do not be verbose
		    --n-threads: use parallelisation to find solutions.
		        Default: 1 (sequential search)
		    --stop-when: specify the time limit (in seconds) of the execution.
		        The value must be positive. A negative value is interpreted as 'never stop'.
		        Default: never stop
		
		Solvers:
		    (1) --simple: initial version of a solver in Gecode for the BWP
		    (2) --rotate: simple solver extended to allow rotations
		    (3) --optim:  solver that tries to minimise roll length used.
		        The solutions found, however, may not be optimal.


