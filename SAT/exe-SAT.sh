#!/bin/bash

# ----------------------------
# MODIFY ONLY THESE VARIABLES
LINGELING=~/Documents/software/lingeling/lingeling
OUT_DIR=/tmp
# ----------------------------

# generator compilation and selection
mode=release
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
SOLVER_TIME=5.0
SCRIPT_TIME=120
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
		
		--solver-timeout=*)
		SOLVER_TIME="${i#*=}"
		shift
		;;
		
		--script-timeout=*)
		SCRIPT_TIME="${i#*=}"
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
SOLVER_TIMEOUT=0
SCRIPT_TIMEOUT=0
ITER=0
msecs=0
SCRIPT_TIME=$((1000*$SCRIPT_TIME))

# keep iterating until:
#    the limit on iterations has been reached (unlikely)
#    the solver found an unsatisfiable formula (optimality)
#    the solver was timeout terminated
#    the script had to terminate due to errors on the generation of the
#        clauses or of the solution
#    the script reached its time limit
while
	[ $UNSAT -eq 0 ] && [ $TERMINATE -eq 0 ] &&
	[ $SOLVER_TIMEOUT -eq 0 ] && [ $SCRIPT_TIMEOUT -eq 0 ] &&
	[ $ITER -le $MAX_ITER ];
do
	begin=$(date +%s%3N)
	
	echo -e "    \e[1;34mTry max length:\e[0m $maxL"
	echo -e "    \e[1;34m    At iteration:\e[0m $ITER"
	
	# execute clause generator and store clauses
	CLAUSE_FILE=$OUT_DIR/box-wrapping.cnf
	$CLAUSE_GEN -i $IN_FILE -o $CLAUSE_FILE.rev --solver $SOLVER --amo-encoder $ENCODER --max-L $maxL
	
	if [ $? -eq 1 ]; then
		TERMINATE=1
		echo -e "        \e[1;31mError: clause generator failed\e[0m"
	else
		tac $CLAUSE_FILE.rev > $CLAUSE_FILE
		
		# execute lingeling
		SOLUTION_FILE=$OUT_DIR/box-wrapping.out
		SOLUTION_VARS=$OUT_DIR/box-wrapping.vars
		
		rm -f $SOLUTION_FILE
		/usr/bin/time --output=/dev/null 				\
			timeout $SOLVER_TIME						\
			$LINGELING $CLAUSE_FILE > $SOLUTION_FILE
		
		TIMEOUT_FILE=$OUT_DIR/lingeling-timeout
		cat $SOLUTION_FILE | grep SIGNAL > $TIMEOUT_FILE
		UNSAT_FILE=$OUT_DIR/lingeling-unsat
		cat $SOLUTION_FILE | grep UNSATISFIABLE > $UNSAT_FILE
		
		# lingeling terminated: time to determine if it was due a timeout
		# or due unsatisfiability
		
		if [ $(cat $TIMEOUT_FILE | wc -l) -gt 0 ]; then
			# lingeling was timeout-terminated.
			# The script must terminate because no lower values of maxL
			# will generate solvable CNFs in $TIME_LIMIT
			SOLVER_TIMEOUT=1
			echo -e "        \e[3;31mlingeling was timeout-terminated\e[0m"
		elif [ $(cat $UNSAT_FILE | wc -l) -gt 0 ]; then
			# lingeling determined unsatisfiablity
			UNSAT=1
			echo -e "        \e[3;31mlingeling determined unsatisfiability of the Boolean formula\e[0m"
		else
			# the formula is satisfiable: obtain solution in the roll
			# from the truth assignment generated by lingeling
			
			cat $SOLUTION_FILE |\
				grep -v -E "^c" |\
				tail --lines=+2 |\
				cut --delimiter=' ' --field=1 --complement > $SOLUTION_VARS
			
			output=$($SOL_GEN				\
				--boxes $IN_FILE			\
				--variables $SOLUTION_VARS	\
				-o $BOXES_SOLUTION			\
				--solver $SOLVER			\
				-v --max-L $maxL)
			
			outL=$?
			
			# output generated by generation solver is indented
			# so as to make the output of this script nicer
			echo "$output" | sed 's/^/    /'
			
			# leave a blank line
			echo ""
			
			# check that the solution generator did not fail.
			if [ $outL -eq 0 ]; then
				# it failed...
				TERMINATE=1
				echo -e "        \e[1;31mError: solution generator failed\e[0m"
			else
				# it did not fail! gather roll length and
				# decrease by 1 so that in the next iteration
				# the solution is improved by 1
				maxL=$(($outL - 1))
			fi
		fi
	fi
	ITER=$(($ITER + 1))
	
	end=$(date +%s%3N)
	msecs=$(($msecs + $end - $begin))
	
	if [ $msecs -gt $SCRIPT_TIME ]; then
		# the script reached its time limit
		SCRIPT_TIMEOUT=1
	fi
done

if [ $SCRIPT_TIMEOUT -eq 0 ]; then
	if [ $UNSAT -eq 1 ]; then
		echo -e "        \e[1;32mOptimality reached!\e[0m"
	fi
	if [ $SOLVER_TIMEOUT -eq 1 ]; then
		echo "        However, it is likely that this happened"
		echo "        trying to determine unsatisfiablity"
	fi
else
	echo -e "        \e[1;31mScript reached time limit\e[0m"
fi

