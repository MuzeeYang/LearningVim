#!/usr/bin/env python3
import Directories as ds
import os
import time

def CopyFileWrapper(dFile, sFile, fSize, newFile):
	print('[{time}]{file}: '.format(time=time.strftime('%H:%M:%S'), file=newFile), end='')
	if(ds.CopyFile(dFile, sFile, fSize) == 0):
		print('OK')
	else:
		print('Failed')
	

def main():
	if(len(os.sys.argv) < 3):
		print('usage wrong!')
		return
	elif(not os.path.isdir(os.sys.argv[1])):
		print('%s: not exists!' % os.sys.argv[1])
		return
	elif(not os.path.isdir(os.sys.argv[2])):
		print('%s: not exists!' % os.sys.argv[2])
		return

	print('====' + time.strftime('%Y/%m/%d') + '====')
	for tp, ffile in ds.DetectDir(os.sys.argv[1]):
		orcPath = os.path.join(os.sys.argv[1], ffile)
		modPath = os.path.join(os.sys.argv[2], ffile)
		if(tp == 'd'):
			if(os.path.isdir(modPath)):
				pass
			else:
				os.mkdir(modPath)
		else:
			orcFileCtime, orcFileSize = ds.GetFileInfo(orcPath)
			if(os.path.exists(modPath)):
				modFileCtime, modFileSize = ds.GetFileInfo(modPath)
				if(orcFileCtime > modFileCtime):
					CopyFileWrapper(modPath, orcPath, orcFileSize, ffile)
			else:
				CopyFileWrapper(modPath, orcPath, orcFileSize, ffile)
	

if(__name__ == '__main__'):
	main()
