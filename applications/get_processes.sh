#!/bin/bash

RAWIN=$(ps -o pid,user,%mem,command ax | grep -v PID | sort -bnr -k3 | awk '/[0-9]*/{print $1 ":" $2 ":" $4}')

for i in $RAWIN
do
	echo $i
done
