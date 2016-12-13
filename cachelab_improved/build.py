#!/usr/bin/env python

import subprocess
import os 

iter_num = 50
range_num = 32


outFile = open('output.txt', 'w')
valueList = [0 for i in range(range_num)]

subprocess.call(['gcc', 'cache.c', '-o', 'cache'])

for j in range(iter_num):
	result = subprocess.Popen(['sudo', './cache'], stdout = subprocess.PIPE)

	i = 0

	for line in iter(result.stdout.readline,''):
		valueList[i] += float(((line.rstrip()).split())[1]) 
		i += 1

for i in range(range_num):
	print valueList[i]/iter_num

for i in range(range_num):
	outFile.write(str(i + 1) + " " + str(valueList[i]/iter_num) + "\n")

outFile.close()
