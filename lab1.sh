#!/bin/bash
if [ -z "$1" ]
then
	echo "PID not entered"
	exit 1
fi
parent=$(ps -o ppid= $1)
echo "Killing process $parent $(ps -p $parent -o comm=)..." 
kill $parent
