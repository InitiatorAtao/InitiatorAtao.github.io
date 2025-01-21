import tkinter as tk
from PIL import Image,ImageDraw,ImageTk
from pathlib import Path
from ctypes import windll
from pix2tex.cli import LatexOCR
import serial,json

width,height=255,320
last=[-1,-1]

image=Image.new("RGBA",(width,height),"white")
draw=ImageDraw.Draw(image)
ser=serial.Serial("COM9",9600,timeout=None)

root=tk.Tk()
root.title("Arduino handwriting")

canvas=tk.Canvas(root,width=width, height=height)
canvas.pack()
photo=ImageTk.PhotoImage(image)
canvas.create_image(0,0,image=photo,anchor=tk.NW)

file_path=Path(__file__).parent.resolve()
temp_path=Path(r"D:/temp/temp.png")

show_count=100
count=0

def show_image():
    global width,height,last,image,draw,ser,canvas,photo,temp_path,count,show_count
    if ser.in_waiting:
        line=ser.readline()
        try:
            pair=json.loads(line.decode("utf-8"))
            if pair[0]==-1:
                image=Image.new("RGB",(width,height),"white")
                draw=ImageDraw.Draw(image)
            elif pair[0]<width and pair[1]<height:
                if last[0]>0 and last[1]>0:
                    if abs(last[0]-pair[0])<10 and abs(last[1]-pair[1])<10:
                        draw.line([(last[0],last[1]),(pair[0],pair[1])],fill="black",width=2)
                        image.save(temp_path, 'BMP')
                last=pair
            photo=ImageTk.PhotoImage(image)
            canvas.create_image(0,0,image=photo,anchor=tk.NW)
            canvas.image=photo
        except Exception as e:
            print(e)
            pass
    root.after(10,show_image)

root.after(10,show_image)

root.mainloop()

ser.close()
