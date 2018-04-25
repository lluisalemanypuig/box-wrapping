#!/bin/bash

PROJ_DIR=..

SOLVER_TEC=""	# either CP,LP,SAT
INPUT_DIR=""
OUTPUT_DIR=""

for i in "$@"; do
	case $i in
		
		--solver=*)
		SOLVER_TEC="${i#*=}"
		shift
		;;
		
		-i=*|--input=*)
		INPUT_DIR="${i#*=}"
		shift
		;;
		
		-o=*|--output=*)
		OUTPUT_DIR="${i#*=}"
		shift
		;;
		
		*)
		echo -e "\e[1;4;31mError:\e[0m" "Option $i unknown"
		;;
	esac
done

if [ "$SOLVER_TEC" == "" ]; then
	echo -e "    \e[1;31mError: solver technology not specified\e[0m"
	exit
fi

if [ "$SOLVER_TEC" != "CP" ] && [ "$SOLVER_TEC" != "LP" ] && [ "$SOLVER_TEC" != "SAT" ]; then
	echo -e "\e[1;31mError invalid solver technology. Choose either CP, LP, or SAT\e[0m"
	exit
fi

EXE_FILE=$PROJ_DIR/$SOLVER_TEC/bin-release/wrapping-boxes
SUFFIX="."$SOLVER_TEC

echo "Exe file used: $EXE_FILE"
if [ ! -f $EXE_FILE ]; then
	echo -e "    \e[1;31mError: solver for $SOLVER_TEC does not exist\e[0m"
else
	echo -e "    \e[1;32mSolver for $SOLVER_TEC exists. Proceed to benchmarking...\e[0m"
fi

echo -e "\e[1;4mRead\e[0m inputs from '$INPUT_DIR'"
if [ "$INPUT_DIR" == "" ]; then
	echo -e "    \e[1;31mError: input directory not specified\e[0m"
	exit
fi
echo -e "\e[1;4mStore\e[0m outputs in '$OUTPUT_DIR'"
if [ "$OUTPUT_DIR" == "" ]; then
	echo -e "    \e[1;31mError: output directory not specified\e[0m"
	exit
fi

# create output directory
mkdir -p $OUTPUT_DIR
# create log directory (just in case we might need the output of the executions)
mkdir -p logs
# directory with optimal outputs
OPT_OUTPUT_DIR=$PROJ_DIR/outputs/hand-made

for INFILE in $(ls -v $INPUT_DIR);
do
	# retrieve input identifier. E.g.: 8_10_11
	INFILE_LENGTH=${#INFILE}
	ID=${INFILE:4:($INFILE_LENGTH - 4 - 3)}
	
	# build output file name. Do not forget the suffix!
	BASE_OUTFILE="bwp_"$ID$SUFFIX
	OPT_FILE="bwp_"$ID.out
	OUT_LOG_FILE=$BASE_OUTFILE.log.out
	ERR_LOG_FILE=$BASE_OUTFILE.log.err
	OUTFILE=$BASE_OUTFILE.out
	
	echo $(date) >> logs/$OUTFILE.out
	echo $(date) >> logs/$OUTFILE.err
	
	# execute the exe file
	if [ "$SOLVER_TEC" == "CP" ]; then
		
		echo "Executing solver with input file: $INPUT_DIR/$INFILE"
		
		begin=$(date +%s%3N)
		./$EXE_FILE 						\
			-i $INPUT_DIR/$INFILE 			\
			-o $OUTPUT_DIR/$OUTFILE 		\
			--heuris-mix 					\
			--stop-at 5 --stop-when 2 -Nr 5	\
			>>  logs/$OUT_LOG_FILE			\
			2>> logs/$ERR_LOG_FILE
		end=$(date +%s%3N)
		
		# if file with optimal solution exists check optimality
		if [ -f $OPT_OUTPUT_DIR/$OPT_FILE ]; then
			opt_length=$(head -n 1 $OPT_OUTPUT_DIR/$OPT_FILE)
			sol_length=$(head -n 1 $OUTPUT_DIR/$OUTFILE)
			
			if [ $opt_length == $sol_length ]; then
				echo -e "    \e[1;32mOptimal solution reached at length: \e[0m"$opt_length
			elif [ $opt_length -gt $sol_length ]; then
				echo -e "    \e[1;34mOoops: hand-made solution is worse than the solver's\e[0m"
				echo    "        Optimal: $opt_length"
				echo    "        $SOLVER_TEC: $sol_length"
			else
				echo -e "    \e[1;33mSuboptimal solution:\e[0m"
				echo    "        Optimal: $opt_length"
				echo    "        $SOLVER_TEC: $sol_length"
			fi
		fi
		secs=$(($end - $begin))
		echo "    In $secs miliseconds."
		
	else
		echo -e "\e[1;31mCall to solver $SOLVER_TEC not implemented yet\e[0m" 
	fi
done
