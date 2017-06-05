#! /bin/bash

if  [[ $1 = "cal" ]]; then
	file="050.cal_ch_mask.crm"
elif [[ $1 = "scd" ]]; then
 	file="052.scd_ch_mask.crm"
fi

path="/Users/nsanthony/cream/cal"
cd $path/files

rm masking_list.csv

cd $path/backupfiles
cp $file $path/files/
cd $path/files

mv $file masking_list.csv

file="masking_list.csv"

grep -v ^# $file > temp 
cat temp > $file #removed header

sed 's/SCI_STV CAL, //' $file > temp #removes unneeded info 
cat temp > $file

sed 's/SCI_STV bSCD, //' $file > temp #removes unneeded bit
cat temp > $file

sed 's/SCI_STV SCDL1, //' $file > temp #removes unneeded bit
cat temp > $file

sed 's/SCI_STV SCDL2, //' $file > temp #removes unneeded bit
cat temp > $file

sed '/ DISABLE/d' $file > temp #removes lines with disabled notification in it 
cat temp > $file

sed 's/STV =//' $file > temp #removes STV setting bit
cat temp > $file

sed 's/  //' $file > temp #removes extra spaces
cat temp > $file 

sed '/^$/d' $file > temp
cat temp > $file


rm temp
# echo fin
