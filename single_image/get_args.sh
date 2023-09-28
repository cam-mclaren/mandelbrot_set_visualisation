#!/bin/sh

value=`cat image_parameters.txt`
OLD_IFS=$IFS
IFS=$'\n'
lines=($value)
IFS=$OLD_IFS

for line in "${lines[@]}"; do 
	echo "$line"
done

