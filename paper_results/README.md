# DDRoute: a Novel Depth-Driven Approach to the Qubit Routing Problem

These folders contain the scripts and results of the paper  **DDRoute: a Novel Depth-Driven Approach to the Qubit Routing Problem**, presented at DAC 2025.

## Results

This folder contains all the results, in the following format:

```
ALGORITHM, benchmark, circuit file, topology file, logical qubits, initial depth, initial gate count, initial cx count, final depth, final gate count, final cx count, final swap count, seed, routing time (s), memory footprint (kB)
```

## Scripts

This folder contains the python scripts used to generate the results in the *results* folder, and the circuits based on Qiskit's circuit library used for the scalability test.

## Topologies

This folder contains the coupling maps of the quantum processors employed.

# Implementation Notes

## Circuits Preprocessing and Generation

To ensure consistency across different benchmarks, all the circuits have been transpiled with this Qiskit command before testing the routing algorithms:
```python
from qiskit.compiler import transpile

# ...
gate_set = ['cx','rz','sx','x']
transpiled_circuit = transpile(circuit, basis_gates=gate_set, optimization_level=1)
# ...
```
Operations such as *reset* and *measure* have been removed.

The circuits generated using Qiskit's circuit library have been built with the *generate_lib_dataset.py* script.

## Bash Command

The result files have been generated with the following command:
```
timeout 180m time -f ",%M" python3 ALGORITHM.py circuit.qasm topology.txt seed >> ALGORITHM.csv 2>&1
```
Note that the time shown in the csv files corresponds to the *routing time*, and not to the overall execution time of the script.

## Routing Algorithms

The algorithms we used for comparison are SABRESwap and StochasticSwap with Qiskit v1.2.0, t|ket⟩ v1.31.0, Google Cirq v1.4.1, and MQT QMAP v2.6.0. The latter has an hard-coded limit of 128 logical and physical qubits: to test the algorithm on larger quantum circuits, it is sufficient to modify such limit and re-build the library before running the tests.

## References

[MQT Benchmark] [https://www.cda.cit.tum.de/mqtbench](https://www.cda.cit.tum.de/mqtbench/) \
[QASMBench] [https://github.com/pnnl/QASMBench](https://github.com/pnnl/QASMBench) \
[VeriQBench] [https://github.com/Veri-Q/Benchmark](https://github.com/Veri-Q/Benchmark)

[SABRESwap] [https://docs.quantum.ibm.com/api/qiskit/qiskit.transpiler.passes.SabreSwap](https://docs.quantum.ibm.com/api/qiskit/qiskit.transpiler.passes.SabreSwap) \
[StochasticSwap] [https://docs.quantum.ibm.com/api/qiskit/qiskit.transpiler.passes.StochasticSwap](https://docs.quantum.ibm.com/api/qiskit/qiskit.transpiler.passes.StochasticSwap) \
[t|ket⟩] [https://docs.quantinuum.com/tket/api-docs](https://docs.quantinuum.com/tket/api-docs/) \
[Google Cirq] [https://quantumai.google/cirq](https://quantumai.google/cirq) \
[MQT QMAP] [https://mqt.readthedocs.io/projects/qmap/en/latest](https://mqt.readthedocs.io/projects/qmap/en/latest/index.html)