# gyre
A language for describing neural architecture and a compiler translating that language into MPI c code for execution on clusters of computers.

Requirements: python and an MPI c compiler.


Translate the 'creature' to MPI c code:
```
./gyre.py *.gyr
```

Compile the MPI binary:
```
mpicc -o creature creature.c
```

Run the binary on 8 cores:
```
mpirun -np 8 ./creature
```
