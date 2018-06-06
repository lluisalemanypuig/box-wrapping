#!/bin/bash

for enc in quadratic logarithmic heule; do
	for i in 3 4 5 6 7 8 9 11; do
		file="../inputs/material/bwp_"$i"_12_1.in"
		time ./exe-SAT.sh -i=$file -o=/tmp/dumb-solution --amo-encoder=$enc

		echo "Encoder: $enc"
		echo "    File $file"

		echo "    wait 5 seconds..."
		sleep 5
	done
done
