#!/bin/sh

for i in `seq -w 0 99`
do
    echo http://pf.is.s.u-tokyo.ac.jp/jp/class/syspro/kadai1/1.pdf.$i >> links.txt
done

xargs -P 5 -n 1 wget -nv <links.txt

cat 1.pdf.* > 1.pdf
