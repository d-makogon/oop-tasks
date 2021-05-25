#!/bin/bash

[ -e task2 ] || exit

for dir in ./*;
do
	[ -d $dir ] && [ -e "$dir/workflow.txt" ] || continue
	if [ -e "$dir/out.txt" ]
	then
		rm "$dir/out.txt"
	fi
	
	echo "$dir: "
	
	cd $dir
	
	execStr="../task2 workflow.txt "
	
	if ! grep -q readfile "workflow.txt";
	then
		execStr="${execStr} -i in.txt"
	fi
	if ! grep -q writefile "workflow.txt";
	then
		execStr="${execStr} -o out.txt"
	fi
	
	eval $execStr
	
	cd ..
	
	printf "\n"
done