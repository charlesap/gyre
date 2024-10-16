# gyre
brain compiler

Build the compiler:
```
cc scanner.h scanner.c generator.h generator.c parser.c
```

Compile the 'creature' to MPI c code:
```
./a.out creature.gyr motor.gyr sensory.gyr brain.gyr
```

Compile the MPI binary:
```
mpicc -o creature creature.c
```

Run the binary on 8 cores:
```
mpirun -np 8 ./creature
```
