# Satisfiability

This part of the project uses the [lingeling](https://github.com/arminbiere/lingeling)
SAT solver to solve the Box Wrapping Problem. See a detailed description of
the mathematical models used in the [report](https://github.com/lluisalemanypuig/box-wrapping/blob/master/report.pdf)

## Compilation

In order to compile the project, first open a terminal and go inside the root
directory of the project. Then, issue the following commands.

	cd SAT/build-rules
	make -f Makefile release

## Execution

When in the root directory of the project, and after having compiled, change directory
into the SAT/ directory.

There, one can use the _clause-generator_ and the _solution-generator_ executable
files generated when compiling in order to:
- Generate the clauses that are to be fed to a SAT solver.
- Generate the "solution", that is, given the model answer of the SAT solver to the
clauses generated, this executable will generate a human-readable solution to the
instance.

Each of these executable files admit several parameters:

	./clause-generator --help
		
		BOX WRAPPING PROBLEM - Satisfiability (CNF) Solver

		This software uses SAT solver lingeling [1] to solve the Box Wrapping
		Problem (BWP). It admits several options and are described below. Most
		of them are optional but it requires the specification of an input file
		containing the data, the solver that will be used, and its mode of execution,
		in case it has any.
		This will only produce the clauses in conjunctive normal form required
		to solve the problem. The format of the clauses output were tested only
		for lingeling.
		The constraints are output to the standard output.
		[1]: https://github.com/arminbiere/lingeling

		Parameters:
			[-h, --help]: show the usage
			[-i, --input]: specify the input file
			[-o, --output]: specify where to store the clauses in CNF
			--max-L: give an upper bound on the roll's length
		Solvers:
			--solver: choose solver. Possible values are
				simple: do not consider rotations for boxes
				rotate: allow rotations for boxes
		Encoder:
			--amo-encoder: choose the encoder for "at most one" constraints. Values:
				quadratic (default)
				logarithmic
				heule
	
	./solution-generator --help
		
		BOX WRAPPING PROBLEM - Satisfiability (CNF) Solution builder

		This software takes as input the processed output of a SAT solver
		that consists of a list of Boolean variables, either set to true
		(positive integer) or to false (negative integer). Each variable
		is part of an instance of the Box Wrapping Problem (BWP) which must
		the input file of this builder. All these variables are read from
		the __standard input__

		Note: this program does not follow the standard for return values
		(0 success, 1 error, 2 misuse of shell builtins). Instead, the value 0
		is reinterpreted as general error, and any other positive value as the
		roll's length of the solution.

		Parameters:
			[-h, --help]: show the usage
			--boxes: specify file with BWP instance
			--variables: specify file with the Boolean assignment
			[-o, --output]: specify where to store the clauses in CNF
			[-v, --verbose]: output the solution read on standard output
			--max-L: give an upper bound on the roll's length
		Solvers:
			--solver: choose solver. Possible values are
				simple: do not consider rotations for boxes
				rotate: allow rotations for boxes

### Automatic execution

Alternatively, one can use the [exe-SAT.sh](https://github.com/lluisalemanypuig/box-wrapping/blob/master/SAT/exe-SAT.sh)
script for an automatic execution of the two. This script has the following parameters:

		[-i=,--input=] 		Specify the input file containing the instance of the
					problem to be solved.
						
		[-o=,--output=] 	Specify the output file where to store the solution 
					of the instance.
		
		[--solver=]		Specify the solver used in the _clause-generator_
					executable file. The valid values are the ones specified
					in that program's help.
		
		[--amo-encoder]		Specify the type of encoder used for the _at most one_
					constraints. The valid values are the ones specified
					in that program's help.
		
		[--solver-timeout]	Specify a time limit for the execution time of the SAT solver.
		
		[--script-timeout]	Specify a time limit for the total execution of the script.

A diagram illustrating how this script works in the Satisfiability section of the
[report](https://github.com/lluisalemanypuig/box-wrapping/blob/master/report.pdf).
