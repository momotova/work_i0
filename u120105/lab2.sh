#!/bin/bash
echo $1
# echo \a "Hello world!"
cd $1
pathOk='ok'
pathFail='failed'
rmdir $pathOk
rmdir $pathFail
mkdir $pathOk
mkdir $pathFail
for i in $(ls)
do
	cd $i
	for fname in *.cir
	do
		# if [ "$fname" == "" ]; then
		# 	cd ..
		# 	continue
		# fi
		echo "LS=$fname"
		if [ $fname != "*.cir" ];then
			symspice -i $fname -o ${fname} -format apb > {$fname}log.txt
			num=$(cat ${fname}.lis | grep "Simulation finished")
			echo $num
			if [ "$num" != "" ]
				then
					file=`ls *.apb`
					echo "FILE=$file"
					echo 'Ok'
					cp $file ../$pathOk
				else
					echo 'No'
					file=`ls *.lis`
					echo $file
					cp {$fname}log.txt ../$pathFail
					
			# echo $fname
			fi
		fi
	done	
	cd ..
done
# read fname   
# if [ -e $fname ]; then
# 	echo "$fname is a file"
# else
# 	echo "$fname is not a file"
# fi
