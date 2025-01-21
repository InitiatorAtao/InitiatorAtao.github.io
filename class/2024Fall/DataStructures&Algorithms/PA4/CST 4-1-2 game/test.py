from pathlib import Path
from random import randint
from subprocess import Popen

file_path=Path(__file__).parent.resolve()
data_path=file_path/"data.in"
std_path=file_path/"bf.exe"
ans_path=file_path/"data.ans"
test_path=file_path/"game.exe"
out_path=file_path/"data.out"

n_list=[5,10,100,1000]
m_list=[7,20,1000,100000]
repeat_list=[10,20,20,20]

for n,m,r in zip(n_list,m_list,repeat_list):
    for _ in range(r):
        with data_path.open("w") as file:
            file.write(f"{n} {m}\n")
            for i in range(m):
                file.write(f"{randint(1,n)} {randint(1,n)}\n")
        Popen(std_path,stdin=data_path.open("r"),stdout=ans_path.open("w")).wait()
        Popen(test_path,stdin=data_path.open("r"),stdout=out_path.open("w")).wait()
        if ans_path.read_text()!=out_path.read_text():
            print(f"WA with ans:\n{ans_path.read_text()}\nout:\n{out_path.read_text()}")
            quit()
        else:
            print(f"AC in {n} {m} {_}")
