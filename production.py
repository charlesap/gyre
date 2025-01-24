# production 
from lark import Lark, Transformer, Tree
import os
import math
import psutil

from ctypes import CDLL

def produce(current,thecst):
    ok = True
    if "GYREDIR" in os.environ.keys():
        gyredir = os.environ["GYREDIR"]
        print(gyredir)
    else:
        gyredir = "."
   

    print("generating",current+".def")
    cfile = open(current+".def", "w")
    cfile.write("gyre")
#    cfile.write(epilogue)

    for name,thing in thecst.items():
#        print(name,thing)
        pass

    cfile.close()
    ncores=os.cpu_count()
    ram=psutil.virtual_memory().total 
    print("suggested compile: rm gyre; cd c; make clean; make shared; make; mv gyre ../; cd ..")
    print("suggested run: mpirun -np "+str(ncores)+" ./gyre -d ./"+current+".def -m "+str(math.floor(ram/1024/1024/ncores)))

    return ok
