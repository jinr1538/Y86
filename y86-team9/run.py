#!/usr/bin/python3

import os
import sys

files = ['error1','error2','error3',
		 'error4','error5','error6','error7','error8','error9',
		 'error10','error11','error12','lab6a','lab6b','lab6c',
		 'lab7a','lab7b','lab7c','lab7d','lab7e','lab7f',
		 'lab8a','lab8b','lab8c','lab8d','lab8e','lab8f',
 		 'lab8g','lab8h','lab8i','lab8j','lab8k','lab8l',
 		 'lab8m','lab8n','lab8o','lab8p','lab8q','lab8r',
		 'lab9a','lab9b','lab9c','lab9d','lab9e','lab9f', 'lab9g',
		 'lab10a','lab10b','lab10c','lab10d','lab10e','lab10f',
 		 'lab10g','lab10h','lab10i','lab10j','lab10k','lab10l',
 		 'lab10n','lab10o',
		 'lab11a','lab11b','lab11c','lab11d','lab11e','lab11f',
		 'lab11g','lab11h','lab11i','lab11j',
 		 'lab12a','lab12b','lab12c','lab12d','lab12e','lab12f',
		 'lab12g','lab12h'		 
		 ]

testdir = '/u/css/classes/3481/211/lab12/Test/'
outputdir = 'Test/'
#---------------------------------------------------------------------
# The following was used to generate master .dump files

# efb = '/u/css/classes/3481/201/lab12/efbyess'
# os.system('rm -f '+testdir+'*.dump')
# for fname in files:
	# input = testdir+fname+".yo"
	# os.system(efb + ' -fdemw ' + input + ' > ' +testdir+fname+'.dump')
# sys.exit(0)
#---------------------------------------------------------------------
if not (os.path.isdir(testdir)):
	print("Need to create /u/css/.../Test directory first\n")
	quit()


if not (os.path.isdir(outputdir)):
	print("Need to create Test directory first\n")
	quit()

if not (os.path.exists('yess')):
	print("No executable 'yess' found")
	quit()

passing = 0

for fname in files:
	input = testdir+fname+".yo"
	efboutput = testdir+fname+".dump"
	studentoutput = outputdir+fname+'.dump'
	studentproblems = outputdir+fname+'.problems'
	
	#make sure input file exists
	if not os.path.exists(input):
		print('missing input file: '+input)
		break
	
	print('Testing '+fname+'...',end="")
	
	#remove old dump and problem files
	os.system('rm -f '+studentoutput+' '+studentproblems)
	
	#run student yess on input file - trace all stages: -fdemw
	os.system('ulimit -f 2000;./yess -fdemw ' + input + ' > ' + studentoutput)
	
	#compare student output with efb output
	os.system('diff '+ studentoutput + ' ' + efboutput 
	          + ' > ' + studentproblems)
	
	#if there are differences, keep problem and dump files
	if not os.system('test -s '+studentproblems):
		print('Failed.')
	else:
		print('Passed.')
		os.system('rm -f '+studentoutput+' '+studentproblems)
		passing += 1

total = len(files)
print(str(passing) +' out of '+str(total)+' passed.\n')
if passing != total:
	print('See Test directory for failed tests\n')
	
		

 


