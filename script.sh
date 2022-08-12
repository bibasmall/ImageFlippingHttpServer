#!/bin/bash

for filename in "$1"/*; do
	curl -s --data-binary @"$filename" "$2":"$3" -o "${filename%.*}_mirr.jpg"
    echo "$filename" 
    echo "${filename%.*}_mirr.jpg"
done
