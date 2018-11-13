#!/bin/bash

unzip submissionsAll.zip
rm submissionsAll.zip 

cut -d , -f 1 CSE_322.csv | tr -d "\\t\"" > ids.txt
cut -d , -f 2 CSE_322.csv > names.txt

find -name "*.zip" | cut -d '_' -f 5 | cut -d '.' -f 1 > submitted2.txt
sort -n submitted2.txt> submitted.txt

ids=`cut -d , -f 1 CSE_322.csv | tr -d "\\t\""`
submissions=`find -name "*.zip" | cut -d '_' -f 5 | cut -d '.' -f 1`
#find -name "*.zip" | cut -d '_' -f 5 | cut -d '.' -f 1 > submitted2.txt
#sort -n submitted2.txt > submitted.txt
total=`find -name "*.zip" | cut -d '_' -f 5 | cut -d '.' -f 1 | wc -l`
#echo $total
for roll in $ids
do
    count=$((1))
    for i in $submissions
    do
        if [ $roll -eq $i ]; then
	        #echo "match $i $roll"
            break
        fi
        (( count++ ))
    done
    #echo $
    if [ $count -gt $total ]; then
    	echo "not match $roll"
        echo $roll >> absent.txt
        echo "$roll 0" >> marks.txt 
    fi
done
#while read -r line; do
#	bool=0
#	while read -r roll; do
#		#echo $roll
#		if [ $line=$roll ]; then
#			bool=1
#			echo $roll
#		fi
#	done < submitted.txt
#	if [ bool ]; then
#		echo "absent $line" 
#		#x=1
#	fi
#done < ids.txt
mkdir Output
mkdir Output/Extra
find -name "*.zip" | cut -d / -f 2 > file.txt
