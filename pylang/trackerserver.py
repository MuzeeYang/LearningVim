#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import socketserver as ss
import threading
from dhtserver import DHTServer

class TrackerHandler(ss.BaseRequestHandler):
	def handle(self):
		query = urlparse(self.path)[4]
		print(query)

class TrackerServer(ss.ThreadingMixIn, ss.TCPServer):
	def __init__(self, server_address, requsetHandlerClass=TrackerHandler):
		ss.TCPServer.__init__(self, server_address, requsetHandlerClass)
		self.peersdict = {}

		self.dhtserver = DHTServer()
		self.dhtserver.pd = self.peersdict
		#self.dhtserver.deamon = True
		self.dhtthread = threading.Thread(target=self.dhtserver.server_forever)
		self.dhtthread.setDeamon(True)
		self.dhtthread.start()

if(__name__ == '__main__'):
	print('Hello, server.')
