#!/usr/bin/env bash

./gyre.py gyr/creature.gyr gyr/motor.gyr gyr/sensory.gyr gyr/brain.gyr
pushd .; rm gyre; cd c; make clean; make shared; make; mv gyre ../; popd
mpirun -np 8 ./gyre -d ./creature.def -m 1024

