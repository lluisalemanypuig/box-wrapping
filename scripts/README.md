# Benchmarking

Use the [benchmark.sh](https://github.com/lluisalemanypuig/box-wrapping/blob/master/scripts/benchmark.sh)
script in order to execute a solver for all inputs in the input directory specified.

The parameters of this script are:
	
	--solver=	Specify the solver technology to be used.
			Use one of: CP,LP,SAT
			for Constraint Programming, Linear Programming and Satisfiability.
	
	[-i=,--input=]	Specify the input directory with the inputs.
	
	[o=,--output=]	Specify where to store the output of each input.
	
