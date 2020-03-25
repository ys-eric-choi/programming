# -*- coding: utf-8 -*-
# Python 2.x
import urllib2
import time
import sys

fWrite = open(sys.argv[1], 'w')

for i in range(300):
	try:
		response = urllib2.urlopen("http://127.0.0.1:7124/?reqmsg=Test", timeout=3)
		fWrite.write(str(i))
		fWrite.write("\n")
	except:
		fWrite.write('time-out!\n')
		fWrite.flush()
	time.sleep(0.5)

fWrite.close()

