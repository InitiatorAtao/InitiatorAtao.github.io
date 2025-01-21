import os
from pathlib import Path
from subprocess import Popen
import random
import filecmp

file_path=Path(__file__).parent.resolve()

datain_path=file_path/"data.in"
teacher_path=file_path/"stock.exe"
test_path=file_path/"to_test.exe"
dataout_path=file_path/"data.out"
dataans_path=file_path/"data.ans"

n=10000

random.seed(os.urandom(128))

def datagen():
    with open(datain_path,"w") as datain_file:
        datain_file.write(str(n)+"\n")
        cnt=0
        s=0
        for _ in range(2*n):
            datain_file.write(str(random.randint(1,n)))
            if s>=n or (random.randint(0,1)==0 and cnt>0):
                datain_file.write("\n")
                cnt-=1
            else:
                datain_file.write(" "+str(random.randint(1,n))+"\n")
                cnt+=1
                s+=1

while True:
    datagen()
    Popen(teacher_path,stdin=datain_path.open("r"),stdout=dataans_path.open("w")).wait()
    Popen(test_path,stdin=datain_path.open("r"),stdout=dataout_path.open("w")).wait()
    if not filecmp.cmp(dataout_path,dataans_path):
        quit()
    else:
        print(dataans_path.read_text())
