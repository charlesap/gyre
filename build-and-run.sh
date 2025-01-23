#!/usr/bin/env bash

./gyre.py gyr/creature.gyr gyr/motor.gyr gyr/sensory.gyr gyr/brain.gyr
pushd .; cd c; gcc -shared -o cortical.so cortical.c; gcc -shared -o interface.so interface.c; popd
mpicc -o gyre c/gyre.c c/cortical.c c/interface.c
mpirun -np 8 ./gyre -d ./creature.def -m 1024

