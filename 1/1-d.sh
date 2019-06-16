#!/bin/sh

for j in `find kadai-d -name "*.cpp"`
do
mv $j ${j%.cpp}.cc
done

for i in `find kadai-d -name "*.cc"`
do  
    sed -i -e "s/NEET the 3rd/Taku/g" $i
    sed -i -e "s/neet3@example.com/taku@example.com/g" $i
    sed -i -e 's/ *$//' $i
done
