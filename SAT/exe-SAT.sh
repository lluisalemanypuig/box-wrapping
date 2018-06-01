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
TIME_LIMIT=5.0
MAX_ITER=100

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
		
		--timeout=*)
		TIME_LIMIT="${i#*=}"
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

maxL=999999
UNSAT=0
TERMINATE=0
ITER=0

# keep iterating until:
#    the limit on iterations has been reached (unlikely)
#    the solver found an unsatisfiable formula (optimality)
#    the script had to terminate due to errors on the generation of the
#        clauses or of the solution
while [ $UNSAT -eq 0 ] && [ $TERMINATE -eq 0 ] && [ $ITER -le $MAX_ITER ]; do
	echo -e "    \e[1;34mTry max length:\e[0m $maxL"
	echo -e "    \e[1;34m    At iteration:\e[0m $ITER"
	
	# execute clause generator and store clauses
	CLAUSE_FILE=$OUT_DIR/box-wrapping.cnf
	$CLAUSE_GEN -i $IN_FILE -o $CLAUSE_FILE.rev --solver $SOLVER --amo-encoder $ENCODER --max-L $maxL
	
	if [ $? -eq 1 ]; then
		TERMINATE=1
		echo -e "    \e[1;31mError: clause generator failed\e[0m"
	else
		tac $CLAUSE_FILE.rev > $CLAUSE_FILE
		
		# execute lingeling
		SOLUTION_FILE=$OUT_DIR/box-wrapping.out
		SOLUTION_VARS=$OUT_DIR/box-wrapping.vars
		
		rm -f $SOLUTION_FILE
		/usr/bin/time --output=/dev/null timeout $TIME_LIMIT $LINGELING $CLAUSE_FILE > $SOLUTION_FILE
		N_LINES=$(cat $SOLUTION_FILE | wc -l)
		if [ $N_LINES -eq 0 ]; then
			# lingeling suffered was timeout-terminated.
			# The script must terminate because no lower values of maxL
			# will generate solvable CNFs in $TIME_LIMIT
			TERMINATE=1
			echo -e "    \e[1;31mlingeling was timeout-terminated\e[0m"
		else
			# lingeling terminated: is the CNF formula satisfiable?
			cat $SOLUTION_FILE |\
				grep -v -E "^c" |\
				tail --lines=+2 |\
				cut --delimiter=' ' --field=1 --complement > $SOLUTION_VARS
			
			N_LINES=$(cat $SOLUTION_VARS | wc -l)
			if [ $N_LINES -eq 0 ]; then
				# the formula is not satisfiable -> terminate
				UNSAT=1
				echo -e "    \e[1;31mCan't obtain a solution with length \e[0m $maxL"
			else
				# the formula is satisfiable: obtain solution in the roll
				
				output=$($SOL_GEN				\
					--boxes $IN_FILE			\
					--variables $SOLUTION_VARS	\
					-o $BOXES_SOLUTION			\
					--solver $SOLVER			\
					-v --max-L $maxL)
				
				outL=$?
				
				echo "$output" | sed 's/^/    /'
				
				# leave a blank line
				echo ""
				
				# check that the solution generator did not fail.
				if [ $outL -eq 0 ]; then
					# it failed...
					TERMINATE=1
					echo -e "    \e[1;31mError: solution generator failed\e[0m"
				else
					# it did not fail! gather roll length and
					# decrease by 1 so that in the next iteration
					# the solution is improved by 1
					maxL=$(($outL - 1))
				fi
			fi
		fi
	fi
	
	ITER=$(($ITER + 1))
done

if [ $UNSAT -eq 1 ]; then
	echo -e "    \e[1;32mOptimality reached!\e[0m"
fi

