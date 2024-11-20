# gyre
A higher level language for describing neural architecture and a compiler translating that language into MPI c code for execution on clusters of computers.

## Syntax Examples

modular source:
```
for creature.
``` 

comments:
```
-- general structure
```

decomposition of space:
```
creature is
  senses, effectors, brain.
```

parameterization of space:
```
l|r in h.          -- left/right half.
l|b|r in v.        -- left/binocular/right view.
n|t in s.          -- nasal/temporal side.
o|u in p.          -- dorsal/ventral position.
```

subscripts apply parameters to regions which subregions inherit:
```
brainₕ is          -- two hemispheres, l and r
  cortex, thalamus, bah, stem.
thalamus is        -- implicit ₕ
  AGN, LGN, MGN. 
bah is             -- implicit ₕ
  bganglia, amygdala, hcampus.
```

regions may feed-forward to multiple other regions preserving or permuting ordering:
```
LGN maps to V1.
MGN maps to A1.
V1 maps to V2 via AGN.
retinaₕₜ maps to LGNₕ•L1 , LGNₕ•L3 , LGNₕ•L5 .  -- binocular vision interleaved because
retinaₕₙ flips to LGNₕ̃•L2 , LGNₕ̃•L4 , LGNₕ̃•L6 . -- nasal half goes to other hemisphere
cochn spreads across MSO, LSO.    -- phase advances across the region and
cochnₕ spreads across MSOₕ̃, LSOₕ̃. -- ₕ̃ spreads phase backwards for coincidence detect
```

physical proximity allows lateral connections:
```
V1 borders V2, V3.
V2 borders V1, V4.
V3 borders V1, V5.
V4 borders V2.
V5 borders V3.
```


```
hypercolumn hc₀‥₂₅₅ is
  columnᵥ c is
    minicolumn mc₀‥₁₂₇ is
      L2, L3, L4, L5, L6.


LGN is
  L1, L2, L3, L4, L5, L6.

V1 is
```


##Requirements 

python and an MPI c compiler.

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
pushd .; cd c; gcc -shared -o cortical.so cortical.c; gcc -shared -o interface.so interface.c; popd
mpicc -o creature creature.c c/cortical.c c/interface.c
```

Run the binary on 8 cores:
```
mpirun -np 8 ./gyre -d ./creature.def -m 1024
```

When developing:
```
ruff check .
ruff check . --fix
pytest
coverage report
```

