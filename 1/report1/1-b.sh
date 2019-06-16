#!/bin/sh

wc -l `find ~/Desktop/coreutils-8.9 -name "*.c"`|sort|awk '{ print $2 }'|grep  -v "合計"> result.txt
