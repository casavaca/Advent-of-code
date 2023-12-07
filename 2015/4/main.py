#!/usr/bin/env python3
import hashlib

KEY="iwrupvqb"
i = 0

while True:
    ans = hashlib.md5((KEY + str(i)).encode('utf-8')).hexdigest()
    if ans[0:5] == "00000":
        print(i)
        break;
    i += 1

i = 0
while True:
    ans = hashlib.md5((KEY + str(i)).encode('utf-8')).hexdigest()
    if ans[0:6] == "000000":
        print(i)
        break;
    i += 1
