# Constraint Programming

This part of the project uses the [Gecode](http://www.gecode.org/doc-latest/reference/index.html)
library (version 6.0.0) to solve the Box Wrapping Problem. See a detailed description of
the mathematical models used in the [report](https://github.com/lluisalemanypuig/box-wrapping/blob/master/report.pdf)

## Compilation

In order to compile the project, first open a terminal and go inside the root
directory of the project. Then, issue the following commands.

		cd CP/build-rules
		make -f Makefile release

## Execution

When in the root directory of the project, and after having compiled, issue
the following commands:

		cd CP/build-release
		./wrapping-boxes --help

The last command will display the usage of the program. This is the help
displayed:

		BOX WRAPPING PROBLEM - Constraint Programming Solver

		This software uses the library Gecode (tested under version 6.0.0)
		to solve the Box Wrapping Problem (BWP). It admits several options
		and are described below. Most of them are optional but it requires
		the specification of an input file containing the data, the solver
		that will be used, and its mode of execution, in case it has any.

		Parameters:
			[-h, --help]: show the usage
			[-i, --input]: specify the input file
			[-o, --output]: specify the output file.
				Only the first solution will be stored.

		Solvers:
			(1) --simple: initial version of a solver in Gecode for the BWP
			(2) --rotate: simple solver extended to allow rotations
			(3) --optim:  solver that tries to minimise roll length used.
				The solutions found, however, may not be optimal.
			(4) --heuris-incr: launches the solver that minimises roll length with different
				preprocessing strategies of the input data:
				  -> area-increasingly sorted (boxes increasingly sorted by area)
				  -> width-increasingly sorted (boxes increasingly sorted by width)
			(5) --heuris-decr: launches the solver that minimises roll length with different
				preprocessing strategies of the input data:
				  -> area-decreasingly sorted (boxes decreasingly sorted by area)
				  -> width-decreasingly sorted (boxes decreasingly sorted by width)
			(6) --heuris-rand: launches the solver that minimises roll length with different
				preprocessing strategies of the input data:
				  -> randomly permuted input (boxes randomly permuted)
				When one execution is completed, the next will try to find solutions
				with shorter roll length than the best found in the previous.
				This strategy will be executed as many times as specified in the parameter
				-N (see options for solvers (6), (7)
			(7) --heuris-mix: launches the solver that minimises roll length with different
				preprocessing strategies of the input data:
				  -> area-increasingly sorted (boxes increasingly sorted by area)
				  -> area-decreasingly sorted (boxes decreasingly sorted by area)
				  -> width-increasingly sorted (boxes increasingly sorted by width)
				  -> width-decreasingly sorted (boxes decreasingly sorted by width)
				  -> randomly permuted input (boxes randomly permuted)
				When one strategy is completed, the next will try to find solutions
				with shorter roll length than the best found in the previous.
				The first four strategies will be executed only once, while the last (random)
				will be executed as many times as specified in the parameter -N (see options
				for solvers (6), (7)

		Options for solvers (1), (2), (3)
			* Modes of execution for solvers
				--enumerate: print the solutions found
					This is the default mode of execution
				--count-only: do not print the solutions

			* Terminating options
				--stop-at: stop finding solutions after n solutions were found
					Default: find all solutions
				--stop-when: stop finding solutions after s seconds have passed
					Default: never stop

			* Input data preprocessing
				--scramble: makes a random permutation of the input boxes
				--seed: seeds the random number generator to avoid repeating
					permutations between independent executions of the program

		Option for solvers (4), (5)
			--stop-at: specify the amount of solutions per execution each solver
				is allowed to find. Once the amount is reached, the solver terminates.
				Default: find all solutions
			--stop-when: specify the amount of time, in seconds, each
				execution can last up to
				Default: never stop
			[-v, --verbose]: display progress of heuristic.
				Default: do not display progress

		Option for solvers (6), (7)
			* the options for solvers (4), (5)
			[-Nr, --n-rand]: specify the number of times of the execution of the
				solver when using random permutations of the input data.
				Default: 10

		Options for all solvers
			--n-threads: use parallelisation to find solutions.
				Default: 1 (sequential search)


		

