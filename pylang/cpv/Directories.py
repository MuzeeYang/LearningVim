#!/usr/bin/env python3
import os

argv = os.sys.argv

def DetectDir(path):
	for rootPath, dirs, files in os.walk(path, followlinks=False):
		for f in files:
			fullPath = os.path.join(rootPath, f)
			yield (GetFileType(fullPath), ExtractDir(fullPath, path))
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

def CopySLink(dFile, sFile):
	try:
		os.symlink(os.readlink(sFile), dFile)
		return 0
	except:
		return -1

def GetFileInfo(f):
	try:
		os_res = os.lstat(f)
		return (int(os_res.st_mtime), os_res.st_size)
	except:
		return ()

def ExtractDir(sPath, xPath):
	#exPathLst = list(filter(lambda x: x, exPath.split(sep=os.sep)[1:]))
	xPathLst = [e for e in xPath.split(sep=os.sep)[1:] if e]
	sPathLst = [e for e in sPath.split(sep=os.sep)[1:] if e]
	return os.sep.join(sPathLst[len(xPathLst):])

def JoinDir(sPath, xPath):
	return os.path.join(sPath, xPath)

def GetFileType(f):
	if(os.path.isdir(f)):
		return 'd'
	elif(os.path.islink(f)):
		return 'l'
	elif(os.path.isfile(f)):
		return 'f'
	else:
		return 'x'

def RemoveDir(path):
	for rootPath, dirs, files in os.walk(path, followlinks=False):
		for f in files:
			os.remove(f)
	os.removedirs(path)

def RemoveAll(path):
	fType = GetFileType(path)
	if(fType == 'x'):
		pass
	elif(fType == 'd'):
		RemoveDir(path)
	else:
		os.remove(path)

def MakeDir(path):
	fType = GetFileType(path)
	if(fType == 'd'):
		return
	elif(fType == 'x'):
		pass
	else:
		os.remove(path)
	os.mkdir(path)
