#!/usr/bin/env python

import subprocess
import os


blockFile = open('block.txt', 'w')    # file with block multiplicated results
transpFile = open('transp.txt', 'w')  # file with transp multicplicated results
simpleFile = open('simple.txt', 'w')  # file with simple multicplicated results

#max_num = 4096
#max_num = 2048
max_num = 16384

subprocess.call(['make'])

n = 128

while (n < max_num):
	size = str(n)
	params = ['./test', size, size, size]
	result = subprocess.Popen(params, stdout = subprocess.PIPE)
	j = 0

	for line in iter(result.stdout.readline,''):
		if (j == 0):
			blockFile.write(str(n) + " " + line + "\n")
			j = 1
		elif (j == 1):
			transpFile.write(str(n) + " " + line + "\n")
			j = 2
		elif (j == 2):
			simpleFile.write(str(n) + " " + line + "\n")

	n = n * 2


blockFile.close()
transpFile.close()
simpleFile.close()
