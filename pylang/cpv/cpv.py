#!/usr/bin/env python3
import Directories as ds
import time

def CopyFileWrapper(dFile, sFile, fSize, fType, newFile):
	print('[{time}]{file}: '.format(time=time.strftime('%H:%M:%S'), file=newFile), end='')
	if(fType == 'f' and ds.CopyFile(dFile, sFile, fSize) == 0):
		print('OK')
	elif(fType == 'l' and ds.CopySLink(dFile, sFile) == 0):
		print('OK')
	else:
		print('Failed')
	

def main():
	if(len(ds.argv) < 3):
		print('usage wrong!')
		return
	elif(not ds.GetFileType(ds.argv[1]) == 'd'):
		print('%s: not exists!' % ds.argv[1])
		return
	elif(not ds.GetFileType(ds.argv[2]) == 'd'):
		print('%s: not exists!' % ds.argv[2])
		return

	print('====' + time.strftime('%Y/%m/%d') + '====')
	for tp, ffile in ds.DetectDir(ds.argv[1]):
		orgPath = ds.JoinDir(ds.argv[1], ffile)
		modPath = ds.JoinDir(ds.argv[2], ffile)
		if(tp == 'd'):
			ds.MakeDir(modPath)
		else:
			modType = ds.GetFileType(modPath)
			orgFileCtime, orgFileSize = ds.GetFileInfo(orgPath)
			if(modType == 'x'):
				CopyFileWrapper(modPath, orgPath, orgFileSize, tp, ffile)
			elif(modType == 'd'):
				ds.RemoveDir(modPath)
				CopyFileWrapper(modPath, orgPath, orgFileSize, tp, ffile)
			else:
				modFileCtime, modFileSize = ds.GetFileInfo(modPath)
				if(orgFileCtime > modFileCtime):
					CopyFileWrapper(modPath, orgPath, orgFileSize, tp, ffile)
	

if(__name__ == '__main__'):
	main()
