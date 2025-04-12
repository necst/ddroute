# DDRoute

**DDRoute** is a placing and routing algorithm for quantum circuits, targeting the reduction of depth and execution time for hardware-compliant circuits.
This implementation provides a transpilation pass for Qiskit projects.

## Installation

You can install **DDRoute** by cloning this repository, and running the following commands:

```sh
pip install --upgrade pip setuptools pybind11 wheel
pip install .
```

# Usage

The following examples illustrates the usage of DDRoute.

## DDPlace and DDRoute Passes

DDPlace and DDRoute are available as layout and routing passes. These two procedures are independent, so any layout or routing pass can be employed along with each of the two passes.

**DDPlace layout pass:**
```python
DDPlace(coupling_map: CouplingMap, gate_durations: dict[str, int] = None, apply_layout: bool = True, seed: int = None)
```
**Arguments:**
 - coupling_map (*qiskit.transpiler.CouplingMap*): the coupling map of the targeted device.
 - gate_durations (*dict[str, int]*, optional): dictionary with quantum gates durations. Default: 1 for standard gates, 3 for SWAP gates.
 - apply_layout (*bool*, optional): if True, apply the resulting layout to the DAGCircuit. If False, only assign property_set['layout'] and leave the DAGCircuit instance unmodified. Default: True
 - seed (*int*, optional): seed for underlying randomization

\
**DDRoute routing pass:**
```python
DDRoute(coupling_map: CouplingMap, gate_durations: dict[str, int] = None, seed: int = None)
```
**Arguments:**
 - coupling_map (*qiskit.transpiler.CouplingMap*): the coupling map of the targeted device
 - gate_durations (*dict[str, int]*, optional): dictionary with quantum gates durations. Default: 1 for standard gates, 3 for SWAP gates
 - seed (*int*, optional): seed for underlying randomization

\
**Usage:**

```python
from ddroute import DDRoute, DDPlace
from qiskit import QuantumCircuit
from qiskit.transpiler import CouplingMap, PassManager
from qiskit.transpiler.passes import FullAncillaAllocation, EnlargeWithAncilla, ApplyLayout

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

# Define custom durations for (some) specific gates
# Default: 1 for standard gates, 3 for swap gates
gate_durations = {'cx': 2, 'swap': 6}

# Create a pass manager
pass_manager = PassManager()

# Create DDPlace layout pass
layout_pass = DDPlace(coupling_map,gate_durations=gate_durations,seed=42)

# Alternatively, apply the layout manually
layout_pass = [
    DDPlace(coupling_map,gate_durations=gate_durations,apply_layout=False,seed=42),
    FullAncillaAllocation(coupling_map),
    EnlargeWithAncilla(),
    ApplyLayout()
]

# Add layout pass to the pass manager
pass_manager.append(layout_pass)

# Add DDRoute to the pass manager
routing_pass = DDRoute(coupling_map,gate_durations=gate_durations,seed=42)
pass_manager.append(routing_pass)

# Run the pass manager
routed_circuit = pass_manager.run(circuit)

print("Routed Circuit:")
print(routed_circuit.draw())
```

## DDPlaceAndRoute Pass

DDPlaceAndRoute is a transformation pass useful when applying DDPlace and DDRoute in cascade, in order to avoid the repeated overhead due to the traversal of the DAGCircuit.

**DDPlaceAndRoute layout and routing pass:**
```python
DDPlaceAndRoute(coupling_map: CouplingMap, gate_durations: dict[str, int] = None, seed: int = None)
```
**Arguments:**
 - coupling_map (*qiskit.transpiler.CouplingMap*): the coupling map of the targeted device
 - gate_durations (*dict[str, int]*, optional): dictionary with quantum gates durations. Default: 1 for standard gates, 3 for SWAP gates
 - seed (*int*, optional): seed for underlying randomization

**Usage:**

```python
from ddroute import DDPlaceAndRoute
from qiskit import QuantumCircuit
from qiskit.transpiler import CouplingMap, PassManager

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

# Add DDPlaceAndRoute to the pass manager
# Using default gate durations
layout_routing_pass = DDPlaceAndRoute(coupling_map,seed=42)
pass_manager.append(layout_routing_pass)

# Run the pass manager
routed_circuit = pass_manager.run(circuit)

print("Routed Circuit:")
print(routed_circuit.draw())
```

## Qiskit *transpile* method

DDPlace and DDRoute are available as named options for *layout_method* and *routing_method* in Qiskit's *transpile* procedure.

**Usage:**

```python
from qiskit import QuantumCircuit, transpile
from qiskit.transpiler import CouplingMap

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

# Transpile the logical circuit
routed_circuit = transpile(
    circuit,
    # ...
    coupling_map=coupling_map,
    layout_method='ddplace',
    routing_method='ddroute',
    seed_transpiler=42
)

print("Routed Circuit:")
print(routed_circuit.draw())
```

## Contributors

Annechini, Alessandro and Venere, Marco and Sciuto, Donatella and Santambrogio, Marco D.

This work will be presented and 62nd Design Automation Conference 2025 (DAC 2025), in San Francisco (USA), June 22-25 2025.
