#!/bin/bash
pathOk='ok'
pathFail='failed'


DIR=$1


#read -p "Enter directory " work_dir
if [[ -n $DIR ]]
then
if [[ -d $DIR ]]
then
cd $1
rm -rf $pathOk
rm -rf $pathFail
mkdir $pathOk
mkdir $pathFail
	LIST=`find . -iname '*.cir'`
for ONE_OF_LIST in $LIST
do
	echo $ONE_OF_LIST
	symspice -i $ONE_OF_LIST -o $ONE_OF_LIST -format apb > $ONE_OF_LIST.log
	file_analise=$(cat ${ONE_OF_LIST}.lis | grep "Simulation finished")
	echo $file_analise


if [[ "$file_analise" != "" ]]
				then
								
					
					find -regex '.*\(apb\|ap3\).*'| xargs -n 1 -I  % cp -r --parents  "%" ./$pathOk
					
					echo $file_apb
					echo 'Simulation Ok.'
					mv $ONE_OF_LIST.log  ./$pathOk
					mv ${ONE_OF_LIST}.lis ./$pathOk
					
					#mv $ONE_OF_LIST.log $work_dir/$pathOk
				else
					echo 'Simulation abort. No simulation files'
					#file_lis="$1 *.lis"
					#echo $ONE_OF_LIST
					mv ${ONE_OF_LIST}.lis ./$pathFail
					cp $ONE_OF_LIST ./$pathFail
					mv $ONE_OF_LIST.log ./$pathFail
					
			# echo $fname
			fi
done
else
	echo "No directory"
	mkdir $DIR
	exit 0
fi
else
	echo "No arguments input"
	exit 0
fi
