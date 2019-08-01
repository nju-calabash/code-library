#!/usr/bin/env python3
import sys
if len(sys.argv) > 2 : sys.stdin = open(sys.argv[2], 'r')
text = sys.stdin.readlines()
ls = []
for i, s in enumerate(text) :
    if sys.argv[1] in s : ls.append(i)
if len(ls) != 2 :
    sys.stderr.write('Illegal number of indicators ({0})!\n'.format(len(ls)))
    exit(1)
ls[0] += 1
while ls[0] < ls[1] and text[ls[0]].strip() == '\n' : ls[0] += 1
while ls[0] < ls[1] and text[ls[1]-1].strip() == '\n' : ls[1] -= 1
sys.stdout.write(''.join(text[ls[0] : ls[1]]))
