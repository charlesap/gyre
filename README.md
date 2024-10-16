# gyre
A language for describing neural architecture and a compiler translating that language into MPI c code for execution on clusters of computers.

Requirements: a serial c compiler and an MPI c compiler.

Build the gyre translator:
```
cc -o gyre scanner.c generator.c parser.c
```

Translate the 'creature' to MPI c code:
```
./gyre creature.gyr motor.gyr sensory.gyr brain.gyr
```

Compile the MPI binary:
```
mpicc -o creature creature.c
```

Run the binary on 8 cores:
```
mpirun -np 8 ./creature
```
