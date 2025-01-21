from pathlib import Path
from subprocess import Popen
from random import randint,choice
from sortedcontainers import SortedSet
from hashlib import md5
from time import time

file_path=Path(__file__).parent.resolve()
test_path=file_path/"BBST.exe"
data_path=file_path/"data.in"
out_path=file_path/"data.out"
ans_path=file_path/"data.ans"
csv_path=file_path/"data.csv"

def gen_random(tree,i,n):
    X_MIN,X_MAX=0,8500000
    op=randint(1,3)
    if op==1:
        k=randint(X_MIN,X_MAX)
        while k in tree:
            k=randint(X_MIN,X_MAX)
        tree.add(k)
        return f"A {k}\n",""
    elif op==2 and tree:
        k=choice(tree)
        tree.remove(k)
        return f"B {k}\n",""
    else:
        k=randint(X_MIN,X_MAX)
        index=tree.bisect(k)
        if index:
            return f"C {k}\n",f"{tree[index-1]}\n"
        else:
            return f"C {k}\n","-1\n"

def gen_one_query_after_all_inserted(tree,i,n):
    X_MIN,X_MAX=0,8500000
    if i+1<n:
        k=randint(X_MIN,X_MAX)
        while k in tree:
            k=randint(X_MIN,X_MAX)
        tree.add(k)
        return f"A {k}\n",""
    else:
        k=randint(X_MIN,X_MAX)
        index=tree.bisect(k)
        if index:
            return f"C {k}\n",f"{tree[index-1]}\n"
        else:
            return f"C {k}\n","-1\n"

def file_md5(file_path):
    hash_md5 = md5()
    with file_path.open("rb") as f:
        for chunk in iter(lambda: f.read(4096), b""):
            hash_md5.update(chunk)
    return hash_md5.hexdigest()

ns=[100,100,
    1000,1000,
    10000,10000,
    100000,100000,
    1000000,1000000]
repeats=[10,10,
         10,10,
         10,10,
         10,10,
         10,10]
gens=[gen_random,gen_one_query_after_all_inserted,
      gen_random,gen_one_query_after_all_inserted,
      gen_random,gen_one_query_after_all_inserted,
      gen_random,gen_one_query_after_all_inserted,
      gen_random,gen_one_query_after_all_inserted]

with csv_path.open("w",encoding="utf-8") as csv_file:
    for n,repeat,gen in zip(ns,repeats,gens):
        avl_run_time_sum=0.0
        splay_run_time_sum=0.0
        for i in range(repeat):
            with data_path.open("w",encoding="utf-8") as data_file,ans_path.open("w",encoding="utf-8") as ans_file:
                tree=SortedSet()
                data_file.write(f"{n}\n")
                for j in range(n):
                    data,ans=gen(tree,j,n)
                    data_file.write(data)
                    ans_file.write(ans)
            avl_start_time=time()
            Popen([test_path,"avl"],
                  stdin=data_path.open("r",encoding="utf-8"),
                  stdout=out_path.open("w",encoding="utf-8")).wait()
            avl_end_time=time()
            avl_run_time_sum+=avl_end_time-avl_start_time
            if file_md5(out_path)!=file_md5(ans_path):
                print(f"avl WA on {n} {gen} {i}")
                quit()
            else:
                print(f"avl AC on {n} {gen} {i}")
            splay_start_time=time()
            Popen([test_path,"splay"],
                  stdin=data_path.open("r",encoding="utf-8"),
                  stdout=out_path.open("w",encoding="utf-8")).wait()
            splay_end_time=time()
            splay_run_time_sum+=splay_end_time-splay_start_time
            if file_md5(out_path)!=file_md5(ans_path):
                print(f"splay WA on {n} {gen} {i}")
                quit()
            else:
                print(f"splay AC on {n} {gen} {i}")
        csv_file.write(f"{n},{gen.__name__},{avl_run_time_sum/repeat},{splay_run_time_sum/repeat}\n")
