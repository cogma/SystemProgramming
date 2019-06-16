#!/bin/sh

wc -l `find ~/Desktop/coreutils-8.9 -name "*.c"`|sort  > result.txt
