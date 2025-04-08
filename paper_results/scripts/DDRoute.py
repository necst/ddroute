import ddroute_cpp
import time
import sys
import os
from qiskit import QuantumCircuit
from qiskit.transpiler import CouplingMap


def parse_topology_list(filename):
    c_map = []
    with open(filename, "r") as t_file:
        next(t_file)
        for line in t_file:
            c_map += [[int(line.split()[0]),int(line.split()[1])]]
    res = CouplingMap(c_map)
    res.make_symmetric()
    return (c_map, res.size())


circuit_file = sys.argv[1]
circ = QuantumCircuit.from_qasm_file(circuit_file)

topology_file = sys.argv[2]
top, npq = parse_topology_list(topology_file)

ddroute_cpp.set_seed(int(sys.argv[3]))

#PREPARE TOPOLOGY
t = ddroute_cpp.DDRTopology(npq)
for e in top:
    t.add_edge(e[0],e[1],True)

s = ddroute_cpp.DDRSchedule(circ.num_qubits)

for node in circ.data:
    if node.operation.num_qubits == 1:
        s.add_operation_1(node.operation.name,node.qubits[0]._index,node.operation.params)
    elif node.operation.num_qubits == 2:
        s.add_operation_2(node.operation.name,node.qubits[0]._index,node.qubits[1]._index,node.operation.params)

lc = ddroute_cpp.DDRScheduleReader(s)
r = ddroute_cpp.DDRouter(s,t)

#ROUTE CIRCUIT
ts = time.time()
r.depth_driven_initial_mapping()
pc = r.depth_driven_routing()
te = time.time()

gate_set = ['cx','rz','sx','x']

#Collect metrics
route_time = te-ts

init_depth = lc.depth()
final_depth = pc.depth()
num_qubits = circ.num_qubits

initial_cx = lc.gate_count("cx")
final_cx = pc.gate_count("cx") + pc.gate_count("swap")*3
swap_count = pc.gate_count("swap")

initial_operations = 0
final_operations = 0
for g in gate_set:
    initial_operations = initial_operations + lc.gate_count(g)
    final_operations = final_operations + pc.gate_count(g)
final_operations = final_operations + pc.gate_count("swap")*3

print(f'DDROUTE,{os.path.basename(circuit_file)},{os.path.basename(topology_file)},{num_qubits},{init_depth},{initial_operations},{initial_cx},{final_depth},{final_operations},{final_cx},{swap_count},{int(sys.argv[3])},{route_time}',end="")
