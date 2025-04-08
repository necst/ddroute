import time
import sys
import os
from qiskit import QuantumCircuit, transpile, qasm3
from qiskit.transpiler import CouplingMap
from qiskit.compiler import transpile

def parse_topology(filename):
    c_map = []
    with open(filename, "r") as t_file:
        next(t_file)
        for line in t_file:
            c_map += [[int(line.split()[0]),int(line.split()[1])]]
    res = CouplingMap(c_map)
    res.make_symmetric()
    return res

#Read circuit from QASM file
circ = QuantumCircuit.from_qasm_file(sys.argv[1])

#Read topology from txt
coupling_map = parse_topology(sys.argv[2])

#Seed
arg_seed = int(sys.argv[3])

#Standard gate set
gate_set = ['cx','rz','sx','x']

transpiled_qc = circ

#Route the circuit
pts = time.process_time()
ts = time.time()
routed_qc = transpile(transpiled_qc, layout_method='sabre', routing_method='sabre', coupling_map=coupling_map, basis_gates=gate_set, optimization_level = 0, seed_transpiler=arg_seed)
pte = time.process_time()
te = time.time()

#Collect metrics
route_time = min(te-ts,pte-pts)
init_depth = transpiled_qc.depth()
final_depth = routed_qc.depth()
num_qubits = transpiled_qc.num_qubits

initial_cx = transpiled_qc.count_ops()['cx']
final_cx = routed_qc.count_ops()['cx']
swap_count = (final_cx-initial_cx)//3

initial_operations = 0
final_operations = 0
for g in gate_set:
    if g in transpiled_qc.count_ops().keys():
        initial_operations = initial_operations + transpiled_qc.count_ops()[g]
    if g in routed_qc.count_ops().keys():
        final_operations = final_operations + routed_qc.count_ops()[g]

print(f'SABRE,{os.path.basename(sys.argv[1])},{os.path.basename(sys.argv[2])},{num_qubits},{init_depth},{initial_operations},{initial_cx},{final_depth},{final_operations},{final_cx},{swap_count},{sys.argv[3]},{route_time}',end="")