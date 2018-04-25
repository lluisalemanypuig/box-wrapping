#!/bin/bash

CHECKER_PROG=checker

if [ -f $CHECKER_PROG ];
then
	echo -e "\e[1;32mChecker program exists. Proceed to check.\e[0m"
else
	echo -e "\e[1;31mChecker program does not exist\e[0m"
	echo -e "\e[1;31mModify variable 'CHECKER_PROG'\e[0m"
	exit
fi

PROJ_DIR=..
IN_DIR=$1	# input directory
OUT_DIR=$2	# where to find the corresponding outputs
SUFFIX=.$3	# an optional suffix, like .CP, .LP, .SAT,

for INFILE in $(ls $IN_DIR);
do
	# retrieve input identifier. E.g.: 8_10_11
	INFILE_LENGTH=${#INFILE}
	ID=${INFILE:4:($INFILE_LENGTH - 4 - 3)}
	
	# build output file name. Do not forget the suffix!
	OUTFILE="bwp_"$ID$SUFFIX".out"
	
	# if the output file exists (because it may not...)
	# then run the checker
	if [ -f $OUT_DIR/$OUTFILE ];
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
	else
		echo -e "\e[1;31mWarning: output file '$OUT_DIR/$OUTFILE' does not exist\e[0m"
		echo -e "    \e[1;31mSkipping...\e[0m"
	fi
done

echo
echo -e "\e[1;34m-------------------------------------\e[0m"
echo -e "\e[1;32mAll checks succeded. Congratulations!\e[0m"
