#!/bin/bash

cat $1 |while read LINE
do
	echo "$LINE:"
	grep $LINE $2
done
