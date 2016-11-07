#!/usr/bin/env python

import subprocess
import os 

iter_num = 50

outFile = open('output.txt', 'w')
valueList = [0 for i in range(32)]

subprocess.call(['gcc', 'cache.c', '-o', 'cache'])

for j in range(iter_num):
	result = subprocess.Popen(['sudo', './cache'], stdout = subprocess.PIPE)

	i = 0

	for line in iter(result.stdout.readline,''):
		valueList[i] += int(((line.rstrip()).split())[1]) 
		i += 1

for i in range(32):
	print valueList[i]/iter_num

for i in range(32):
	outFile.write(str(i + 1) + " " + str(valueList[i]/iter_num) + "\n")

outFile.close()
