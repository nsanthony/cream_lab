#! /bin/bash

# 2564 channels

cd ~/cream/cal/files
file="20160929-172216c.spi"
file="20160929-172216c.spi"
rm $file
cd ~/cream/cal/backupfiles
cp $file ~/cream/cal/files

cd ~/cream/cal/files
grep -v ^# $file > 20160929-172216c.temp
cat temp > $file
rm temp

