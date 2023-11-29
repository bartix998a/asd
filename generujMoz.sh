#!/bin/bash
i=0

for n in $(seq 200 400) 
do
	for k in $(seq 60 70)
	do
		i=$((i+1))
		echo "${n} ${k}" > mozIns/s1/t${i}
		echo $(seq 1 $n) >> mozIns/s1/t${i}
		cat mozIns/s1/t${i} | eval ./moz.o > mozOuts/s1/r${i}
	done
done