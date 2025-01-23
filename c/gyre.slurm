#!/bin/bash

#SBATCH -e %J.err
#SBATCH -o %J.out
#SBATCH -J gyre

#SBATCH --partition=default
#SBATCH --nodes=5
#SBATCH --tasks-per-node=16
#SBATCH --time=0:10:00

set -eo pipefail -o nounset


mpirun -np 80 ./gyre -d ../creature.gyr -m 1024
