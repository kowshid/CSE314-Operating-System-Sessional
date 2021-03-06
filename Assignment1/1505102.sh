#!/bin/bash

unzip submissionsAll.zip
rm submissionsAll.zip 

cut -d , -f 1 CSE_322.csv | tr -d "\\t\"" > ids.txt
cut -d , -f 2 CSE_322.csv > names.txt

find -name "*.zip" | cut -d '_' -f 5 | cut -d '.' -f 1 > submitted2.txt
sort -n submitted2.txt > submitted.txt
rm submitted2.txt

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
        echo $roll >> absents.txt
        echo "$roll 0" >> marks.txt 
    fi
done
#while read -r line; do
#	bool="false"
#	while read -r roll; do
#		#echo $roll
#		if [ $line=$roll ]; then
#			bool="true"
#			echo $roll
#		fi
#	done < submitted.txt
#	if [ bool="false" ]; then
#		echo "absent $line"
#	fi
#done < ids.txt
mkdir Output
mkdir Output/Extra
find -name "*.zip" | cut -d / -f 2 > file.txt

while read -r line; do
	mkdir Output/temp 
	unzip "$line" -d Output/temp
	cd Output/temp
	count=$((1))
	for i in $ls
	do
		(( count++ ))
	done
	folder=`ls`
	cd ..
	cd ..
	if [ $count -eq 1 ] ; then
		echo "n = $count"			
		if grep -q "$folder" CSE_322.csv; then
			#echo "I'm in mark"
			echo "$folder 10" >> marks.txt
			cp -r Output/temp/"$folder" Output/"$folder"
		fi
		
	else
		echo "$folder 0" >> marks.txt
		cp -r Output/temp/"$folder" Output/Extra/"$folder"
	fi

	#cd ..
	#cd ..
	rm -r Output/temp
	rm -r "$line"
done < file.txt

sort marks.txt > Marks.txt
sort absents.txt > Absents.txt
rm marks.txt
rm absents.txt
