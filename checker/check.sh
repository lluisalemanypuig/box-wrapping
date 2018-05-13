#!/bin/bash

function make_input_filename() {
	OUTFILE=$1
	OUTFILE_LENGTH=${#OUTFILE}
	i=$(($OUTFILE_LENGTH - 4))	# place iterator at last dot
	
	# place iterator at second dot, if exists
	j=$(($i - 1))
	while [ $j -gt 0 ] && [ ${OUTFILE:$j:1} != "." ]; do
		j=$(($j - 1))
	done
	
	if [ $j -eq 0 ]; then
		# if no 'second' dot exists
		echo ${OUTFILE:0:$i}.in
	else
		# if a 'second' dot exists
		echo ${OUTFILE:0:$j}.in
	fi
}

CHECKER_PROG=checker

if [ -f $CHECKER_PROG ];
then
	echo -e "\e[1;32mChecker program exists. Proceed to check.\e[0m"
else
	echo -e "\e[1;31mChecker program does not exist\e[0m"
	echo -e "\e[1;31mEither:\e[0m"
	echo -e "    \e[3;31mModify variable 'CHECKER_PROG'\e[0m"
	echo -e "    \e[3;31mDid you compile the source file?\e[0m"
	exit
fi

ALL_IN_DIRS=()
OUT_DIR=""
SUFFIX=""

for i in "$@"; do
	case $i in
		-i=*|--input=*)
		ALL_IN_DIRS+=("${i#*=}")
		shift
		;;
		
		-o=*|--output=*)
		OUT_DIR="${i#*=}"
		shift
		;;
		
		-s=*|--suffix=*)
		SUFFIX="${i#*=}"
		shift
		;;
		
		*)
		echo -e "\e[1;4;31mError:\e[0m" "Option $i unknown"
		;;
	esac
done

if [ "$SUFFIX" != "" ]; then
	SUFFIX=.$SUFFIX
fi

if [ -z $ALL_IN_DIRS ]; then
	echo -e "\e[1;31mNo input directories specified\e[0m"
	echo -e "    \e[3;31mPass them using -i=|--input=\e[0m"
	exit
fi

if [ -z $OUT_DIR ]; then
	echo -e "\e[1;31mOutput directory not specified\e[0m"
	echo -e "    \e[3;31mPass it using -o=|--output=\e[0m"
	exit
fi

for OUTFILE in $(ls -v $OUT_DIR);
do
	# retrieve input file:
	#     bwp_8_13_1.out    -> bwp_8_13_1.in
	#     bwp_8_13_1.LP.out -> bwp_8_13_1.in
	INFILE=$(make_input_filename $OUTFILE)
	
	# if the output file exists (because it may not...) then run the checker
	for IN_DIR in ${ALL_IN_DIRS[@]}; do
		if [ -f $IN_DIR/$INFILE ];
		then
			echo -e "\e[1;34m---------------------------------\e[0m"
			echo -e "\e[1;32mExecuting checker on:\e[0m"
			echo -e "\e[1;33m     Input file:\e[0m \e[3;34m$IN_DIR/$INFILE\e[0m"
			echo -e "\e[1;33m    Output file:\e[0m \e[3;34m$OUT_DIR/$OUTFILE\e[0m"
			
			./$CHECKER_PROG $IN_DIR/$INFILE $OUT_DIR/$OUTFILE
			if [ $? -eq 1 ];
			then
				echo -e "\e[1;31m----------------------\e[0m"
				echo -e "\e[1;31m  **  CHECK FAILED  **\e[0m"
				echo -e "\e[1;31m----------------------\e[0m"
				exit
			fi
			
			# no need to keep iterating through the input directories
			break
		else
			echo -e "\e[3;31m---------------------------------\e[0m"
			echo -e "\e[1;31mWarning: input file '$INFILE' does not exist in directory $IN_DIR\e[0m"
			echo -e "    \e[3;31mSkipping...\e[0m"
		fi
	done
done

echo
echo -e "\e[1;34m-------------------------------------\e[0m"
echo -e "\e[1;32mAll checks succeded. Congratulations!\e[0m"
