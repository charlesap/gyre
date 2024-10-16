#!/usr/bin/env bash

cc -o gyre scanner.c generator.c parser.c
./gyre creature.gyr motor.gyr sensory.gyr brain.gyr
mpicc -o creature creature.c
mpirun -np 8 ./creature

