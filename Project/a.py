#python3
#读取文件中 "yyyy-MM-dd HH:mm:ss" 并转换为当前时间
import time
import re
import sys

print(sys.argv)
print(sys.argv[1])
strnow=time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())
print (strnow)
f = open(sys.argv[1], "r")
fwrite=open(sys.argv[1]+'1', "w")
strformat='yyyy-MM-dd HH:mm:ss'
str = f.read()
f.seek(0,0)
#print(str)
print(str.find(strformat))
strnew=str.replace(strformat,strnow)
print(strnew)

f.close()



fwrite.write(strnew);
fwrite.close()
