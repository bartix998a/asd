m=200
for n in $(seq 500 1000)
do
	((number= 1 + $n - 500))
	echo $n > malIns/s1/t${number}
	echo $m >> malIns/s1/t${number}
	for i in $(seq 1 $m)
	do
		((a=1 + $RANDOM % ($n - 1)))
		((temp = $n - $a))
		((b=$a + $RANDOM % $temp))
		color="B"
		if (( $RANDOM % 2 == 1 ))
		then
			color="C"
		fi
		echo "${a} ${b} ${color}" >> malIns/s1/t${number}
	done
	cat malIns/s1/t${number} | eval ./simpleMal.o > malOuts/s1/r${number}
done
((m = 1000000))
((n=1000000))
((number=502))
echo $n > malIns/s1/t${number}
	echo $m >> malIns/s1/t${number}
	for i in $(seq 1 $m)
	do
		((a=1 + (10*$RANDOM) % ($n - 1)))
		((temp = $n - $a))
		((b=$a + (10*$RANDOM) % $temp))
		color="B"
		if (( $RANDOM % 2 == 1 ))
		then
			color="C"
		fi
		echo "${a} ${b} ${color}" >> malIns/s1/t${number}
	done
	cat malIns/s1/t${number} | eval ./simpleMal.o > malOuts/s1/r${number}