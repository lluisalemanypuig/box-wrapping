#!/bin/bash

cd report

for i in $@; do
	if [ "$i" == "c" ]; then
		pdflatex main.tex
	elif [ "$i" == "b" ]; then
		biber main
	elif [ "$i" == "s" ]; then
		evince main.pdf &
	fi
done

cd ..
