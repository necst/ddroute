# DDRoute: a Novel Depth-Driven Approach to the Qubit Routing Problem

These folders contain the scripts and results of the paper  **DDRoute: a Novel Depth-Driven Approach to the Qubit Routing Problem**.

## Circuits

This folder contains the quantum circuits generated with Qiskit to test each routing algorithm on the IBM Condor topology.

## Results

This folder contains all the results, in the following format:

```
ALGORITHM, circuit file, topology file, logical qubits, initial depth, initial gate count, initial cx count, final depth, final gate count, final cx count, final swap count, seed, routing time (s), memory footprint (kB)
```

## Scripts

This folder contains the python scripts used to generate the results in the *results* folder, and the circuits in the *circuits* folder.

## Topologies

This folder contains the coupling maps of the quantum processors employed for numerical validation.