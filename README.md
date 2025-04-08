# DDRoute

**DDRoute** is a placing and routing algorithm for quantum circuits, targeting the reduction of depth and execution time for hardware-compliant circuits.
This implementation provides a transpilation pass for Qiskit projects.

## Installation

You can install **DDRoute** by cloning this repository, and running the following commands:

```sh
pip install --upgrade pip setuptools pybind11 wheel
pip install .
```

## Usage

The following example illustrates the usage of DDRoute.

```python
from qiskit import QuantumCircuit
from qiskit.transpiler import CouplingMap, PassManager
from ddroute import DDRoute

num_qubits = 10
                
# Create or import a quantum circuit
circuit = QuantumCircuit(num_qubits)
circuit.h(0)
for q in range(1,num_qubits):
    circuit.cx(0, q)

print("Original Circuit:")
print(circuit.draw())

# Create or import a coupling map
# Example: linear topology
coupling_map = CouplingMap(
    [(i,i+1) for i in range(num_qubits-1)] +
    [(i+1,i) for i in range(num_qubits-1)])

# Create a pass manager
pass_manager = PassManager()

# Add DDRoute to the pass manager
# Define custom length for specific gates
# Default: 1 for standard gates, 3 for swap gates
ddroute_pass = DDRoute(coupling_map,{'cx': 2, 'swap': 6},seed=42)
pass_manager.append(ddroute_pass)

# Run the pass manager
routed_circuit = pass_manager.run(circuit)

print("Routed Circuit:")
print(routed_circuit.draw())
```

## Contributors

Annechini, Alessandro and Venere, Marco and Sciuto, Donatella and Santambrogio, Marco D.

This work will be presented and 62nd Design Automation Conference 2025 (DAC 2025), in San Francisco (USA), June 22-25 2025.
