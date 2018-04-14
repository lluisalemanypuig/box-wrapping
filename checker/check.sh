#!/bin/bash

PROJ_DIR=..
INPUTS_DIR=$PROJ_DIR/inputs
MATERIAL_IN=$INPUTS_DIR/material
CUSTOM_IN=$INPUTS_DIR/custom

OUTPUTS_DIR=$PROJ_DIR/outputs
HAND_MADE_OUT=$OUTPUTS_DIR/hand-made

OUTPUTS=(
	$HAND_MADE_OUT/bwp_3_1_1.out $HAND_MADE_OUT/bwp_3_2_1.out $HAND_MADE_OUT/bwp_3_3_2.out \
	$HAND_MADE_OUT/bwp_3_4_1.out $HAND_MADE_OUT/bwp_3_6_1.out $HAND_MADE_OUT/bwp_3_1_2.out \
	$HAND_MADE_OUT/bwp_3_3_1.out $HAND_MADE_OUT/bwp_3_3_3.out $HAND_MADE_OUT/bwp_3_5_1.out \
	$HAND_MADE_OUT/bwp_3_7_1.out
)
INPUTS=(
	$CUSTOM_IN/bwp_3_1_1.in $MATERIAL_IN/bwp_3_2_1.in $CUSTOM_IN/bwp_3_3_2.in \
	$MATERIAL_IN/bwp_3_4_1.in $MATERIAL_IN/bwp_3_6_1.in $CUSTOM_IN/bwp_3_1_2.in \
	$MATERIAL_IN/bwp_3_3_1.in $CUSTOM_IN/bwp_3_3_3.in $MATERIAL_IN/bwp_3_5_1.in \
	$MATERIAL_IN/bwp_3_7_1.in
)

for (( i = 0 ; i < ${#OUTPUTS[@]} ; i=$i+1 ));
do
	in=${INPUTS[${i}]}
	out=${OUTPUTS[${i}]}
	
	echo -e "\e[7;34m-----------------------------\e[0m"
	echo "in: $in, out: $out"
	./checker $in $out
	echo " "
	echo " "
done
