import numpy as np
import pyzbar
import qrcode
import math
import sys

def toBinary(a):
  l,m=[],[]
  for i in a:
    l.append(ord(i))
  for i in l:
    m.append(int(bin(i)[2:]))
  return m

def toString(a):
  l=[]
  m=""
  for i in a:
    b=0
    c=0
    k=int(math.log10(i))+1
    for j in range(k):
      b=((i%10)*(2**j))   
      i=i//10
      c=c+b
    l.append(c)
  for x in l:
    m=m+chr(x)
  return m


def binaryToDecimal(binary):
     
    binary1 = binary
    decimal, i, n = 0, 0, 0
    while(binary != 0):
        dec = binary % 10
        decimal = decimal + dec * pow(2, i)
        binary = binary//10
        i += 1
    return (decimal)   
   


#########################


input_string = sys.argv[1]
div_num = sys.argv[2]
key= sys.argv[3]
print("input string = " + input_string)
print("div num = " + div_num)
div = (int(len(input_string))//int(div_num))
print("div = %d",  div)



mod = (int(len(input_string))%int(div_num))
print("mod = %d",  mod)
print("key = %d", key)

data = toBinary(input_string)


crc1=0;
for i in range(0,int(len(input_string))):
    a=data[i]+crc1
    
    b=binaryToDecimal(a)
    
    crc1=b%int(key)
    if (i == int(len(input_string)-1)):
            data[i]=data[i]+crc1

###########CRC


for i in range(0,div):
    datan = input_string[i*div:(i+1)*div]
    img = qrcode.make(datan)
    img.save("qr" + str(i)+".png")



datan=input_string[i*div:len(input_string)]
img = qrcode.make(datan)
img.save('qr'+str(i+1)+'.png')







