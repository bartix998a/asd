#for i in {1..2} do
	xd=$(ls -p sorIns/s5 | grep -v / | wc -l)
	for j in $(seq 1 $xd)
	do
		cat sorIns/s5/t$j | eval ./sorSolve.o > sorOuts/s5/r$j
	done
#done