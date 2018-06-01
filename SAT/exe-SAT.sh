#!/bin/bash

# ----------------------------
# MODIFY ONLY THESE VARIABLES
LINGELING=~/Documents/software/lingeling/lingeling
OUT_DIR=/tmp
# ----------------------------

mode=debug

# generator compilation and selection
cd build-rules
make -f Makefile $mode > /dev/null
cd ..
CLAUSE_GEN=build-$mode/clause-generator
SOL_GEN=build-$mode/solution-generator
CHECKER=../checker/checker

# argument parsing
SOLVER="rotate"
ENCODER="quadratic"
IN_FILE=""
BOXES_SOLUTION=""

for i in "$@"; do
	case $i in
		
		-i=*|--input=*)
		IN_FILE="${i#*=}"
		shift
		;;
		
		-o=*|--output=*)
		BOXES_SOLUTION="${i#*=}"
		shift
		;;
		
		--solver=*)
		SOLVER="${i#*=}"
		shift
		;;
		
		--amo-encoder=*)
		ENCODER="${i#*=}"
		shift
		;;
		
		*)
		echo -e "\e[1;4;31mError:\e[0m" "Option $i unknown"
		;;
	esac
done

if [ -z $IN_FILE ]; then
	echo -e "    \e[1;31mError: input file containing the description of the\e[0m"
	echo -e "    \e[1;31mbox wrapping problem instance was not specified\e[0m"
	exit
fi

if [ -z $BOXES_SOLUTION ]; then
	echo -e "    \e[1;31mError: output file that should contain the solution to the\e[0m"
	echo -e "    \e[1;31mbox wrapping problem was not specified\e[0m"
	exit
fi

# execute clause generator and store clauses
CLAUSE_FILE=$OUT_DIR/box-wrapping.cnf
$CLAUSE_GEN -i $IN_FILE -o $CLAUSE_FILE.rev --solver $SOLVER --amo-encoder $ENCODER

if [ $? -eq 1 ]; then
	echo -e "    \e[1;31mError: clause generator failed\e[0m"
	exit
fi

tac $CLAUSE_FILE.rev > $CLAUSE_FILE

# execute lingeling
SOLUTION_FILE=$OUT_DIR/box-wrapping.out
SOLUTION_VARS=$OUT_DIR/box-wrapping.vars
$LINGELING $CLAUSE_FILE > $SOLUTION_FILE
cat $SOLUTION_FILE | grep -v -E "^c" | tail --lines=+2 | cut --delimiter=' ' --field=1 --complement > $SOLUTION_VARS

N_LINES=$(cat $SOLUTION_VARS | wc -l)
if [ $N_LINES -eq 0 ]; then
	echo -e "    \e[1;31mUNSATISFIABLE!\e[0m"
else 
	# if CNF not unsatisfiable then obtain solution in the roll
	$SOL_GEN --boxes $IN_FILE --variables $SOLUTION_VARS -o $BOXES_SOLUTION --solver $SOLVER -v
fi



