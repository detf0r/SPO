#!/bin/bash
awk '{
	line++
	count = 0
	for ( i = 1; i <= NF; i++ )
	{
		if ( $i ~ /^[A-Z]/)
			count ++
		
		
	}
	sum = sum + count
	average = sum / NF
	print "Строка " line ": " count 
}
END{ print "Среднее: " sum/line }
' $1
