#!/bin/sh

for i in `seq -w 0 99`
do
    wget http://pf.is.s.u-tokyo.ac.jp/jp/class/syspro/kadai1/1.pdf.$i
done

cat 1.pdf.* > 1.pdf
