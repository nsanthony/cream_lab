#! /bin/bash

# 2564 channels
#inputs are -d -> detector, -t -> time,-l -> layer


while [[ $# -gt 1 ]]
do
key="$1"

case $key in
	-p|--path)
	PAT="$2" #DO NOT NAME PATH will cause issues with bash since PATH is environment variable
	shift # past argument (shifting key by 1 so $1 -> $2 etc.)
	;;
	-d|--detector)
    DETECTOR="$2"
    shift 
    ;;
    -t|--time)
    TIME="$2"
    shift 
    ;;
    -l|--layer)
    LAYER="$2"
    shift 
    ;;
    --default)
    DEFAULT=YES
    ;;
    *)
            unknown option
    ;;
esac
shift
done

echo DETECOTR  = "${DETECTOR}"
echo TIME     = "${TIME}"
echo LAYER    = "${LAYER}"
echo PATH 	  = "${PAT}"



path="${PAT}"
cd $path/scripts

if [[ ${DETECTOR} = "cal" ]]; then
    masked_list.bash cal #gets the mask list
    end="c.spi" #end of the file name DATA-TIMEc.spi
    file=${TIME}$end
    echo FILE = $file
elif [[ ${DETECTOR} = "scd" ]]; then
	masked_list.bash scd
	if [[ ${LAYER} = "1" ]]; then
		end="s1.spi"
	elif [[ ${LAYER} = "2" ]]; then
		end="s2.spi"
	elif [[ ${LAYER} = "3" ]]; then
		end="s3.spi"
	elif [[ ${LAYER} = "4" ]]; then
		end="s4.spi"
	fi
	file=${TIME}$end
    echo FILE = $file

fi


cd $path/files
rm $file
cd $path/backupfiles
cp $file $path/files


cd $path/files

grep -v ^# $file > temp 
cat temp > $file #removed header
sed 's/0x3230//' $file > temp #removes hex characters
cat temp > $file
sed 's/0xF800//' $file > temp #also removes hex characters
cat temp > $file
sed 's/^[ \t]*//' $file > temp #removes initial spaces
cat temp  > $file
sed 's/        /,/' $file > temp #removes spaces after 0 masked channel
cat temp > $file
sed 's/ //' $file > temp 
cat temp > $file

for i in {1..8}
do
	sed 's/   /,/' $file > temp
	cat temp > $file
done



sed 's/  /,/' $file > temp
cat temp > $file
sed 's/ //' $file > temp
cat temp > $file



file="temp"
rm $file
echo fin

