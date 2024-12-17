from pathlib import Path
from subprocess import Popen
from random import randint
import filecmp

file_path=Path(__file__).parent.resolve()
data_path=file_path/"data.in"
std_path=file_path/"bf.exe"
ans_path=file_path/"data.ans"
test_path=file_path/"pokerface2.exe"
out_path=file_path/"data.out"

nl=[5,10,100,10000]
ml=[5,10,100,10000]
ll=[10,100,1000,2**31-1]
repeatl=[1,1,1,100]

for n,m,l,r in zip(nl,ml,ll,repeatl):
    for _ in range(r):
        with data_path.open("w") as data:
            data.write(f"{n} {m}\n")
            for i in range(n):
                left=randint(1,l)
                right=randint(1,l)
                if right<left:
                    left,right=right,left
                data.write(f"{left} {right}\n")
            for i in range(m):
                data.write(f"{randint(1,l)}\n")
        Popen(std_path,stdin=data_path.open("r"),stdout=ans_path.open("w")).wait()
        Popen(test_path,stdin=data_path.open("r"),stdout=out_path.open("w")).wait()
        if filecmp.cmp(out_path,ans_path,shallow=False):
            print("WA detacted:",ans_path.read_bytes(),out_path.read_bytes(),sep="\n")
            quit()
        else:
            print("AC")
