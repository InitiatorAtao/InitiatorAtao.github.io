from pathlib import Path
from random import shuffle
from subprocess import Popen,PIPE

file_path=Path(__file__).parent.resolve()
data_path=file_path/"data.in"
test_path=file_path/"sort.exe"
n_limit_list=[(10,1e3),
              (1e2,1e4),
              (1e3,3e5),
              (1e4,2e5),
              (1e4,1e5),
              (1e4,9e4),
              (1e4,8e4),
              (1e4,7.4e4),
              (1e4,7.2e4),
              (1e6,13e6),
              (1e5,35e5),
              (1e5,20e5),
              (1e5,10e5),
              (1e5,9.3e5),
              (1e5,9e5),
              (1e5,8.7e5),
              (1e6,11.7e6),
              (1e6,11e6),
              (1e4,7e4),
              (1e6,10.3e6)]

for n,limit in n_limit_list:
    a=[i for i in range(int(n))]
    shuffle(a)
    with data_path.open("w") as data_file:
        data_file.write(f"{int(n)} {int(limit)}\n")
        for i in a:
            data_file.write(f"{i} ")
    stdout,stderr=Popen(test_path,stdin=data_path.open("r"),stdout=PIPE,stderr=PIPE).communicate()
    print(stdout,stderr,sep="\n")
