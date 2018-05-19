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

if [ -z $SOLVER_TEC ]; then
	echo -e "    \e[1;31mError: solver technology not specified\e[0m"
	exit
fi

echo -e "\e[1;4mRead\e[0m inputs from '$INPUT_DIR'"
if [ -z $INPUT_DIR ]; then
	echo -e "    \e[1;31mError: input directory not specified\e[0m"
	exit
fi
echo -e "\e[1;4mStore\e[0m outputs in '$OUTPUT_DIR'"
if [ -z $OUTPUT_DIR ]; then
	echo -e "    \e[1;31mError: output directory not specified\e[0m"
	exit
fi

if [ "$SOLVER_TEC" != "CP" ] && [ "$SOLVER_TEC" != "LP" ] && [ "$SOLVER_TEC" != "SAT" ]; then
	echo -e "\e[1;31mError invalid solver technology. Choose either CP, LP, or SAT\e[0m"
	exit
fi

EXE_FILE=$PROJ_DIR/$SOLVER_TEC/build-release/wrapping-boxes
SUFFIX="."$SOLVER_TEC

echo "Exe file used: $EXE_FILE"
if [ ! -f $EXE_FILE ]; then
	echo -e "\e[1;31mError: solver for $SOLVER_TEC does not exist\e[0m"
	echo -e "    \e[1;31mCheck corresponding *build-release* directory\e[0m"
	exit
else
	echo -e "\e[1;32mSolver for\e[0m \e[1;4;32m$SOLVER_TEC\e[0m \e[1;32mexists. Proceed to benchmarking...\e[0m"
fi

echo "" # blank line left intentionally

# create output directory
mkdir -p $OUTPUT_DIR
# create log directory (just in case we might need the output of the executions)
mkdir -p logs
# directory with optimal outputs
OPT_OUTPUT_DIR=$PROJ_DIR/outputs/hand-made

# number of instances solved optimally
n_optimal=0
# number of instances solved suboptimally
n_suboptimal=0
# total number of instances solved
n_total=0
# time so far (in milliseconds)
total_time_ms=0
# maximum number of threads in the system
max_threads=$(grep -c ^processor /proc/cpuinfo)

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
	
	echo $(date) >> logs/$OUT_LOG_FILE
	echo $(date) >> logs/$ERR_LOG_FILE
	
	echo "Executing $SOLVER_TEC solver with input file: $INPUT_DIR/$INFILE"
	
	# execute the exe file
	if [ "$SOLVER_TEC" == "CP" ]; then
	
		begin=$(date +%s%3N)
		./$EXE_FILE 					\
			-i $INPUT_DIR/$INFILE 		\
			-o $OUTPUT_DIR/$OUTFILE 	\
			--heuris-mix -Nr 5 -v 		\
			--stop-at 1 --stop-when 5	\
			--n-threads $max_threads	\
			>>  logs/$OUT_LOG_FILE		\
			2>> logs/$ERR_LOG_FILE
		end=$(date +%s%3N)
		msecs=$(($end - $begin))
		
	elif [ "$SOLVER_TEC" == "LP" ]; then
		
		begin=$(date +%s%3N)
		./$EXE_FILE 					\
			-i $INPUT_DIR/$INFILE 		\
			-o $OUTPUT_DIR/$OUTFILE 	\
			--optim -v					\
			--n-threads $max_threads	\
			--stop-when 45.0			\
			>>  logs/$OUT_LOG_FILE		\
			2>> logs/$ERR_LOG_FILE
		end=$(date +%s%3N)
		msecs=$(($end - $begin))
		
	elif [ "$SOLVER_TEC" == "SAT" ]; then
		echo -e "\e[1;31mCall to solver $SOLVER_TEC not implemented yet\e[0m" 
	fi
	
	n_total=$((n_total + 1))
		
	# if file with optimal solution exists check optimality
	if [ -f $OPT_OUTPUT_DIR/$OPT_FILE ] && [ -f $OUTPUT_DIR/$OUTFILE ]; then
		opt_length=$(head -n 1 $OPT_OUTPUT_DIR/$OPT_FILE)
		sol_length=$(head -n 1 $OUTPUT_DIR/$OUTFILE)
		
		if [ $opt_length == $sol_length ]; then
			echo -e "    \e[1;32mOptimal solution reached\e[0m"
			n_optimal=$((n_optimal + 1))
		elif [ $opt_length -lt $sol_length ]; then
			echo -e "    \e[1;33mSuboptimal solution:\e[0m"
			echo -e "        Optimal: \e[1;32m$opt_length\e[0m"
			printf "%+15s: " "$SOLVER_TEC"
			echo -e "\e[1;32m$sol_length\e[0m"
			n_suboptimal=$((n_suboptimal + 1))
		else
			echo -e "    \e[1;34mOoops: hand-made solution is worse than the solver's\e[0m"
			echo -e "        Optimal: \e[1;31m$opt_length\e[0m"
			printf "%+15s: " "$SOLVER_TEC"
			echo -e "\e[1;32m$sol_length\e[0m"
		fi
	fi
	
	solv_time_secs=$(echo "scale=3; $msecs/1000" | bc)
	echo "    In $solv_time_secs seconds"
	
	total_time_ms=$(($total_time_ms + $msecs))
	total_time_secs=$(echo "scale=3; $total_time_ms/1000" | bc)
	
	per_opt=$(echo "scale=2; (100*$n_optimal)/$n_total" | bc)
	per_sopt=$(echo "scale=2; (100*$n_suboptimal)/$n_total" | bc)
	echo "    Current progress:"
	echo "        Solved optimally    : $n_optimal / $n_total ( $per_opt% )"
	echo "        Solved sub-optimally: $n_suboptimal / $n_total ( $per_sopt% )"
	echo "        Total time elapsed  : $total_time_secs seconds"
done
