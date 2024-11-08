# production 
from lark import Lark, Transformer, Tree
import os
import math
import psutil

from ctypes import CDLL

#libfact = CDLL("./c/cortical.so")
#
#assert libfact.fact(6) == 720
#assert libfact.fact(0) == 1
#assert libfact.fact(-42) == 1


def produce(current,thecst):
    ok = True
    if "GYREDIR" in os.environ.keys():
        gyredir = os.environ["GYREDIR"]
        print(gyredir)
    else:
        gyredir = "."
   
    pfile = open(gyredir+"/c/prologue.c")
    prologue = pfile.read()
    pfile.close()
    efile = open(gyredir+"/c/epilogue.c")
    epilogue = efile.read()
    efile.close()

    print("generating",current+".c")
    cfile = open(current+".c", "w")
    cfile.write(prologue)
    cfile.write(epilogue)

    for name,thing in thecst.items():
#        print(name,thing)
        pass

    cfile.close()
    ncores=os.cpu_count()
    ram=psutil.virtual_memory().total 
    print("suggested compile: mpicc -o "+current+" "+current+".c")
    print("suggested run: mpirun -np "+str(ncores)+" ./"+current+" -m "+str(math.floor(ram/1024/1024/ncores)))

    return ok
