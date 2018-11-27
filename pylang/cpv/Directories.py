#!/usr/bin/env python3
import os

def DetectDir(path):
	for rootPath, dirs, files in os.walk(path, followlinks=True):
		for f in files:
			yield ('f', ExtractDir(os.path.join(rootPath, f), path))
		for d in dirs:
			yield ('d', ExtractDir(os.path.join(rootPath, d), path))
	return

def CopyFile(dFile, sFile, fSize):
	fCache = 1024
	try:
		sf = open(sFile, 'rb')
		df = open(dFile, 'wb')
		while(fSize > 0):
			df.write(sf.read(fCache))
			fSize -= fCache
		return 0
	except:
		return -1

def GetFileInfo(f):
	try:
		os_res = os.stat(f)
		return (int(os_res.st_mtime), os_res.st_size)
	except:
		return ()

def ExtractDir(sPath, xPath):
	#exPathLst = list(filter(lambda x: x, exPath.split(sep=os.sep)[1:]))
	xPathLst = [e for e in xPath.split(sep=os.sep)[1:] if e]
	sPathLst = [e for e in sPath.split(sep=os.sep)[1:] if e]
	return os.sep.join(sPathLst[len(xPathLst):])

