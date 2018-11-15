#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from hashlib import md5
from time import time

def randomStr(strLen):
	return md5(str(time())).digest()[:strLen]

def getKeyFromStr(s_id):
	r = 0
	for b in [ord(i) for i in s_id]:
		for i in reversed(range(8)):
			r += b >> i
			r <<= 1
	return r >> 1

	

