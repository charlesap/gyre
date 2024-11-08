#!/usr/bin/env bash

./gyre.py gyr/creature.gyr gyr/motor.gyr gyr/sensory.gyr gyr/brain.gyr
pushd .; cd c; gcc -shared -o cortical.so cortical.c; popd
mpicc -o creature creature.c
mpirun -np 8 ./creature

