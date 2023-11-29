#!/bin/bash
rm OutLog/*
xd=$(ls -p ${1}Ins/ | grep -v / | wc -l)
PURPLE='\033[0;35m'
NC='\033[0m'
if [[ -f ${1}.c ]] then
	gcc -g ${1}.c -o ${1}.o
fi

if [[ -f ${1}.cpp ]] then 
	g++ -g ${1}.cpp -o ${1}.o
fi

if (( $# == 2 )) then 
	series=$2
	xd=$(ls -p ${1}Ins/${2}/ | grep -v / | wc -l)
else
	series=""
	xd=$(ls -p ${1}Ins/ | grep -v / | wc -l)
fi
for i in $(seq 1 $xd)
do
	cat ${1}Ins/${series}/t$i | eval ./$1.o > OutLog/o$i
	res="$(diff  OutLog/o$i ${1}Outs/${series}/r$i)"
	if [[ "$res" != "" ]]
	then
		echo -e "${PURPLE}W tescie${NC} $i"
		if [[ $series != "" ]] then
			echo -e "${PURPLE}z serii${NC} ${series}"
		fi
		echo "$res"
	fi
done
	
