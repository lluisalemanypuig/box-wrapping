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

if [ "$SOLVER_TEC" != "CP" ] && [ "$SOLVER_TEC" != "LP" ] && [ "$SOLVER_TEC" != "SAT" ]; then
	echo -e "\e[1;31mError invalid solver technology. Choose either CP, LP, or SAT\e[0m"
	exit
fi

EXE_CP=$PROJ_DIR/CP/bin-release/wrapping-boxes
EXE_LP=$PROJ_DIR/LP/bin-release/wrapping-boxes
EXE_SAT=$PROJ_DIR/SAT/bin-release/wrapping-boxes

if [ "$SOLVER_TEC" == "CP" ]; then
	EXE_FILE=$EXE_CP
	SUFFIX=".CP"
elif [ "$SOLVER_TEC" == "LP" ]; then
	EXE_FILE=$EXE_LP
	SUFFIX=".LP"
elif [ "$SOLVER_TEC" == "SAT" ]; then
	EXE_FILE=$EXE_SAT
	SUFFIX=".SAT"
fi

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

for INFILE in $(ls $INPUT_DIR);
do
	# retrieve input identifier. E.g.: 8_10_11
	INFILE_LENGTH=${#INFILE}
	ID=${INFILE:4:($INFILE_LENGTH - 4 - 3)}
	
	# build output file name. Do not forget the suffix!
	BASE_OUTFILE="bwp_"$ID$SUFFIX
	OUT_LOG_FILE=$BASE_OUTFILE.log.out
	ERR_LOG_FILE=$BASE_OUTFILE.log.err
	OUTFILE=$BASE_OUTFILE.out
	
	echo $(date) >> logs/$OUTFILE.out
	echo $(date) >> logs/$OUTFILE.err
	
	echo "Solve for file $INPUT_DIR/$INFILE"
	
	# execute the exe file
	if [ "$SOLVER_TEC" == "CP" ]; then
		
		./$EXE_FILE 							\
			-i $INPUT_DIR/$INFILE 				\
			-o $OUTPUT_DIR/$OUTFILE 			\
			--heuris-rand 						\
			--stop-at 3 --stop-when 2 -Nr 2		\
			>>  logs/$OUT_LOG_FILE				\
			2>> logs/$ERR_LOG_FILE
	else
		echo -e "\e[1;31mCall to solver $SOLVER_TEC not implemented yet\e[0m" 
	fi
done
