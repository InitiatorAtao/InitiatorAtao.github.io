from pathlib import Path
from random import randint
from subprocess import Popen

file_path=Path(__file__).parent.resolve()
std_path=file_path/"build"/"bf.exe"
test_path=file_path/"build"/"main.exe"
data_path=file_path/"data.in"
out_path=file_path/"data.out"
ans_path=file_path/"data.ans"

d_list=[2,3,4,5,5]
n_list=[5,10,100,1000,100000]
q_list=[5,100,100,100,200000]
range_list=[5,100,10000,10000000,10000000]
repeat_list=[100,20,20,20,20]

for d,n,q,r,repeat in zip(d_list,n_list,q_list,range_list,repeat_list):
    for i in range(repeat):
        with data_path.open("w") as file:
            file.write(f"{d} {n}\n")
            for j in range(n):
                for k in range(d):
                    file.write(f"{randint(-r,r)} ")
                file.write("\n")
            file.write(f"{q}\n")
            for j in range(q):
                for k in range(d):
                    file.write(f"{randint(-r,r)} ")
                file.write("\n")
        Popen(std_path,stdin=data_path.open("r"),stdout=ans_path.open("w")).wait(1500)
        if n<=1000:
            Popen(test_path,stdin=data_path.open("r"),stdout=out_path.open("w")).wait()
        if n<=1000 and out_path.read_text()!=ans_path.read_text():
            print(f"WA with output and ans: \n{out_path.read_text()}\n{ans_path.read_text()}")
            quit()
        else:
            print(f"AC in repeat {i}")
