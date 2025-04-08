"""DDRoute pass for Qiskit"""

import ddroute_cpp
import random
from qiskit.transpiler import CouplingMap
from qiskit.transpiler.basepasses import TransformationPass
from qiskit.dagcircuit import DAGCircuit
from qiskit.circuit import QuantumRegister
from qiskit.circuit.library import SwapGate

class DDRoute(TransformationPass):
    """
    A routing pass for quantum circuits using DDRoute.

    Args:
        coupling_map (qiskit.transpiler.CouplingMap): Target device connectivity map.
        gate_lengths (dict[str, int], optional): Dictionary of gate lengths.
        seed (int, optional): Seed for underlying routing randomization.
    """

    def __init__(self, coupling_map: CouplingMap, gate_depths: dict[str,int]=None, seed: int=None):
        super().__init__()
        self.coupling_map = coupling_map
        if gate_depths is not None:
            self.__setattr__('gate_depths',gate_depths)
        else:
            self.__setattr__('gate_depths',{})
        if seed is not None:
            self.__setattr__('router_seed',seed%32000)
        else:
            self.__setattr__('router_seed',None)

    def run(self, dag: DAGCircuit):

        if self.router_seed:
            ddroute_cpp.set_seed(self.router_seed)
        else:
            ddroute_cpp.set_seed(random.randint(0,32000))

        t = ddroute_cpp.DDRTopology(self.coupling_map.size())
        for e in self.coupling_map:
            t.add_edge(e[0],e[1],True)

        s = ddroute_cpp.DDRSchedule(dag.num_qubits())

        qargs_map = {}
        cargs_map = {}

        topological_nodes = list(dag.topological_op_nodes())

        for id,node in enumerate(topological_nodes):
            if node.op.num_qubits == 1:
                if node.qargs[0] not in qargs_map:
                    qargs_map[node.qargs[0]] = len(qargs_map)

                gate_l = 1
                if node.name in self.gate_depths:
                    gate_l = self.gate_depths[node.name]

                cargs_list = []
                for ca in node.cargs:
                    if ca not in cargs_map:
                        cargs_map[ca] = len(cargs_map)
                    cargs_list += [cargs_map[ca]]
                
                s.add_operation_1(node.name,qargs_map[node.qargs[0]],node.params,cargs_list,gate_l,id)

            elif node.op.num_qubits == 2:
                if node.qargs[0] not in qargs_map:
                    qargs_map[node.qargs[0]] = len(qargs_map)
                if node.qargs[1] not in qargs_map:
                    qargs_map[node.qargs[1]] = len(qargs_map)

                gate_l = 1
                if node.name in self.gate_depths:
                    gate_l = self.gate_depths[node.name]

                cargs_list = []
                for ca in node.cargs:
                    if ca not in cargs_map:
                        cargs_map[ca] = len(cargs_map)
                    cargs_list += [cargs_map[ca]]

                s.add_operation_2(node.name,qargs_map[node.qargs[0]],qargs_map[node.qargs[1]],node.params,cargs_list,gate_l,id)

        r = ddroute_cpp.DDRouter(s,t)
        if 'swap' in self.gate_depths:
            r.set_swap_depth(self.gate_depths['swap'])
        else:
            r.set_swap_depth(3)
       
        r.depth_driven_place()

        pc = r.depth_driven_routing()

        result = DAGCircuit()
        qreg = QuantumRegister(self.coupling_map.size(),'q')
        result.add_qreg(qreg)
        result.add_clbits(dag.clbits)
        for cr in dag.cregs:
            result.add_creg(dag.cregs[cr])

        while pc.next():
            if pc.gate_id() < 0:
                if pc.name() == 'swap':
                    result.apply_operation_back(SwapGate(),qargs=[qreg[pc.operands()[0]],qreg[pc.operands()[1]]],cargs=[],check=False)
            else:
                orig_node = topological_nodes[pc.gate_id()]
                if len(pc.operands()) == 1:
                    result.apply_operation_back(orig_node.op, qargs=[qreg[pc.operands()[0]]], cargs=orig_node.cargs, check=False)
                else:
                    result.apply_operation_back(orig_node.op, qargs=[qreg[pc.operands()[0]],qreg[pc.operands()[1]]], cargs=orig_node.cargs, check=False)
                start = pc.gate_id()+1
                while start < len(topological_nodes) and len(topological_nodes[pc.gate_id()].qargs) == 0:
                    orig_node = topological_nodes[start]
                    result.apply_operation_back(orig_node.op, qargs=[], cargs=orig_node.cargs, check=False)
                    start += 1

        return result
