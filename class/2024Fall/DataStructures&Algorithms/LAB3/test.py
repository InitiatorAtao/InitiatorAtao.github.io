from pathlib import Path
from subprocess import Popen,TimeoutExpired
from time import perf_counter_ns

file_path=Path(__file__).parent.resolve()
main_path=file_path/"main.exe"
data_format="data{id}.in"
ans_format="data{id}.ans"
out_format="data{id}{suffix}.out"

main_args1=["g","b"]
main_args2=["l","c","t"]

wait_seconds=60

for id in range(1,4):
    data_path=Path(data_format.format(id=id))
    for arg1 in main_args1:
        for arg2 in main_args2:
            out_path=Path(out_format.format(id=id,suffix=arg1+arg2))
            try:
                start_time=perf_counter_ns()
                Popen([main_path,arg1,arg2],stdin=data_path.open("r"),stdout=out_path.open("w")).wait(wait_seconds)
                end_time=perf_counter_ns()
            except TimeoutExpired as e:
                print(f"data{id} with args {arg1} {arg2} time out for {wait_seconds} seconds")
                continue
            print(f"data{id} with args {arg1} {arg2} run for {end_time-start_time} ns")
