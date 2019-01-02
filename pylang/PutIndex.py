#!/usr/bin/env python
#coding=utf-8

import sys
import re
import os

def getSN(tLevel, leng = 10, maxNo = 1000):
	sep = ''
	def level1(sep):
		for itemNo in range(1, 1+maxNo):
			yield str(itemNo) + sep
	def level2(sep): #maxNo no use, maxNo is 26*26
		for itemNoAlph in range(ord('a'), ord('z')+1):
			yield chr(itemNoAlph) + sep
		for itemNoAlph1 in range(ord('a'), ord('z')+1):
			for itemNoAlph2 in range(ord('a'), ord('z')+1):
				yield chr(itemNoAlph1) + chr(itemNoAlph2) + sep
		while(True):
			yield 'zz' + sep
	def level5(sep):
		sn = list(r'!@#$%^&*()')
		for itemNo in range(maxNo):
			yield sn[itemNo%len(sn)] + sep
	def level0():
		while(True):
			yield ''
	
	genList = (level0(), level1('.'), level2('.'), level1(')'), level2(')'), level5(' ')) + (level0(),)*(leng-6)
	return genList[tLevel:]

def getTitleLevel(line):
	matchObj = re.match(r'(\t*).+$', line)
	if(matchObj):
		return len(matchObj.groups(1)[0])
	else:
		return 0

def startAddSN(filename):
	with open(filename, mode="r") as f:
		context = f.readlines()

	with open(filename, mode="w") as f:
		try:
			snGen = list(getSN(0))
			curTl = 0
			for tl, line in ((getTitleLevel(l), l) for l in context):
				if(not line.strip()):
					f.write(os.linesep)
					continue
				if(tl < curTl):
					snGen[tl+1:] = list(getSN(tl+1))
				curTl = tl
				line = re.sub(r'^([0-9a-z]{1,3}[\.\)]|[!@#\$%\^&\*\(\)] )', '', line.strip(), 1)
				#f.write('\t'*tl + snGen[tl].next() + line + os.linesep)
				f.write('\t'*tl + next(snGen[tl]) + line + os.linesep)
		except Exception:
			f.truncate(0)
			f.seek(0)
			for line in context:
				f.write(line)
			raise

	
def getAvailableFiles(filename):
	if(os.path.isfile(filename)):
		return filename,
	elif(os.path.isdir(filename)):
		return tuple(os.listdir(filename))
	else:
		return ()

def main():
	if(len(sys.argv) < 2):
		print('Usage: Please input files.')
		return

	for fl in filter(lambda f: re.search(r'\.txt$', f), getAvailableFiles(sys.argv[1])):
		startAddSN(fl)
		#os.remove(fl+'.bk')
		print(fl + ' has been done.')


if(__name__ == '__main__'): main()

