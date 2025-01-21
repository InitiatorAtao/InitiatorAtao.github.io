import sys
from pathlib import Path
import subprocess
import filecmp

file_path=Path(__file__).parent.resolve()
code_path=file_path/"codes"
datagen_path=file_path/"data.exe"
datain_path=file_path/"data.in"
dataout_path=file_path/"data.out"
dataans_path=file_path/"data.ans"

while(True):
    subprocess.Popen(datagen_path).wait()
    for code in code_path.iterdir():
        codein_path=code_path/(code.stem+".in")
        if code.suffix==".exe" and not codein_path.exists() and code.stem=="07":
            with datain_path.open("r") as datain,dataout_path.open("w") as dataout:
                subprocess.Popen(code,stdin=datain,stdout=dataout).wait()
            if not filecmp.cmp(dataout_path,dataans_path) and dataout_path.stat().st_size!=0:
                print("code",code.stem,"output the wrong answer",dataout_path.read_text(),"with std",dataans_path.read_text())
                codein_path.write_bytes(datain_path.read_bytes())
                (code_path/(code.stem+".out")).write_bytes(dataout_path.read_bytes())
                (code_path/(code.stem+".ans")).write_bytes(dataans_path.read_bytes())
            elif filecmp.cmp(dataout_path,dataans_path):
                print("code",code.stem,"accepted")
            else:
                print("code",code.stem,"quited")
