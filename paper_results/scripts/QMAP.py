import time
import sys
import os
from qiskit import QuantumCircuit, transpile
from qiskit.transpiler import CouplingMap
from qiskit.compiler import transpile
from mqt.qmap import Architecture, compile

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
arch = Architecture(coupling_map.size(),set(coupling_map.get_edges()))

#Standard gate set
gate_set = ['cx','rz','sx','x']

transpiled_qc = circ

#Route the circuit
ts = time.time()
qmap_qc = compile(circ , arch , method='heuristic',add_measurements_to_mapped_circuit=False,pre_mapping_optimizations=False,post_mapping_optimizations=False)[0]
te = time.time()
routed_qc = transpile(qmap_qc, basis_gates=gate_set, optimization_level = 0)

#Collect metrics
route_time = te-ts
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

print(f'QMAP,{os.path.basename(sys.argv[1])},{os.path.basename(sys.argv[2])},{num_qubits},{init_depth},{initial_operations},{initial_cx},{final_depth},{final_operations},{final_cx},{swap_count},{sys.argv[3]},{route_time}',end="")