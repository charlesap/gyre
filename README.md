# gyre
A language for describing neural architecture and a compiler translating that language into MPI c code for execution on clusters of computers.

Requirements: python and an MPI c compiler.

For python:
```
python3 -m venv .venv
source .venv/bin/activate
export GYRDIR="."
pip install -r requirements-dev.txt
```

Translate the 'creature' to MPI c code:
```
./gyre.py gyr/*.gyr
```

Compile the MPI binary:
```
pushd .; cd c; gcc -shared -o cortical.so cortical.c; popd
mpicc -o creature creature.c c/cortical.c c/interface.c
```

Run the binary on 8 cores:
```
mpirun -np 8 ./creature
```

When developing:
```
ruff check .
ruff check . --fix
pytest
coverage report
```

