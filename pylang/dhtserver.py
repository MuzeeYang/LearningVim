#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import socketserver as ss
import threading

DHT_IP = '0.0.0.0'
DHT_PORT = 9500
BOOT_SAMP = [('router.utorrent.com',6881),
			('dht.transmissionbt.com',6881),
			('router.bittorrent.com',6881)]
class DHTHandler(ss.BaseRequestHandler):
	def handle(self):
		pass

class DHTServer(ss.ThreadingMixIn, ss.UDPServer):
	def __init__(self, server_address=(DHT_IP, DHT_PORT), requsetHandlerClass=DHTHandler):
		ss.UDPServer.__init__(self, server_address, requsetHandlerClass)
		self.nodesdict = {}
		self.lock = threading.Lock()

	def bootstrap(self):
		pass

if(__name__ == '__main__'):
	print('Hello, server.')
