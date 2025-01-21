from pathlib import Path
from subprocess import Popen
from random import randint

file_path=Path(__file__).parent.resolve()
data_path=file_path/"data.in"
out_path=file_path/"data.out"
ans_path=file_path/"data.ans"
std_path=file_path/"bf.exe"
test_path=file_path/"melody.exe"

n_list=[5,50]
m_list=[5,50]
append_list=[2,10]
check_list=[3,20]
a_list=[9,9]
repeat_list=[20,100]

for n,m,append,check,a,repeat in zip(n_list,m_list,append_list,check_list,a_list,repeat_list):
    for _ in range(repeat):
        now_n=n
        with data_path.open("w") as data_file:
            data_file.write(f"{n} {m}\n")
            for i in range(n):
                data_file.write(f"{randint(1,a)} ")
            data_file.write("\n")
            for i in range(m):
                if randint(0,1):
                    data_file.write(f"1 {append} ")
                    for j in range(append):
                        data_file.write(f"{randint(1,a)} ")
                    now_n=now_n+append
                else:
                    data_file.write(f"2 {check} ")
                    for j in range(check):
                        data_file.write(f"{randint(1,now_n)} ")
                data_file.write("\n")
        Popen(std_path,
              stdin=data_path.open("r"),
              stdout=ans_path.open("w")).wait()
        Popen(test_path,
              stdin=data_path.open("r"),
              stdout=out_path.open("w")).wait()
        if ans_path.read_text()!=out_path.read_text():
            print(f"WA on input:\n{data_path.read_text()}\nanswer:\n{ans_path.read_text()}\noutput:\n{out_path.read_text()}")
            quit()
        else:
            print(f"AC in {n} {m} repeat {repeat}")
