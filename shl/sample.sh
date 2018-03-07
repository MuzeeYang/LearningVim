#!/bin/bash

#set -e
#set -u

#exec 2>errlog

#shell script:cut string
#${str:n:m}		print m chars starting with the (n+1)th char,(0-n) means (n)th char from the end 

#shell script:conditions of COMPARE
#operator				description
#=======================================
#about files:
#-e filename				if filename exist, then true
#-d filename				if filename is directory, then true
#-f filename				if filename is file,then true
#-L filename				if filename is symbol link, then true
#-r filename				if filename is readable, then true
#-w filename				if filename is writable, then ture
#-x filename				if filename is executable, then true
#filename1 -nt filename2	if filename1 is newer than filename2, then true
#filename1 -ot filename2	if filename1 is older than filename2, then true
#---------------------------------------
#about string:(using [""] to avoid space around itself)
#-z string				if string's length is 0, then true
#-n string				if string's length is not 0, then true
#string1 = string2		if string1 equals string2, then true
#string1 != string2		if string1 dosn't equals string2, then true
#---------------------------------------
#about number:
#num1 -eq num2			if num1 equals num2, then true
#num1 -ne num2			if num1 dosn't equal num2, then true
#num1 -lt num2			if num1 is lesser than num2, then true
#num1 -le num2			if num1 is not greater than num2, then true
#num1 -gt num2			if num1 is greater than num2, then true
#num1 -ge num2			if num1 is not lesser than num2, then true

echo "This is a sample,start."
echo "This is a sample,end."

