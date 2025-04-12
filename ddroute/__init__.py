"""DDRoute pass for Qiskit"""

import ddroute_cpp
from qiskit.circuit import QuantumRegister
from qiskit.circuit.library import SwapGate
from qiskit.dagcircuit import DAGCircuit
from qiskit.transpiler import CouplingMap, Layout
from qiskit.transpiler.passes import ApplyLayout, FullAncillaAllocation, EnlargeWithAncilla
from qiskit.transpiler.basepasses import TransformationPass
from qiskit.transpiler.passmanager import PassManager
from qiskit.transpiler.preset_passmanagers.plugin import PassManagerStagePlugin
import random

class DDPlace(TransformationPass):
    """
    A layout pass for quantum circuits using DDPlace.

    Args:
        coupling_map (qiskit.transpiler.CouplingMap): Target device connectivity map
        gate_lengths (dict[str, int], optional): Dictionary of gate lengths
        apply_layout (bool, default=True): apply layout to DAG
        seed (int, optional): Seed for underlying randomization
    """

    def __init__(self, coupling_map: CouplingMap, gate_durations: dict[str,int]=None, apply_layout: bool=True, seed: int=None):
        super().__init__()
        self.coupling_map = coupling_map
        if gate_durations is not None:
            self.__setattr__('gate_durations',gate_durations)
        else:
            self.__setattr__('gate_durations',{})
        if seed is not None:
            self.__setattr__('router_seed',seed%32000)
        else:
            self.__setattr__('router_seed',None)

        self.__setattr__('apply_layout',apply_layout)

        if not coupling_map.is_symmetric:
            raise ValueError('Asymmetric coupling maps are not supported')
        if not coupling_map.is_connected():
            raise ValueError('Disconnected coupling maps are not supported')


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
        qargs_inverse_map = {}

        topological_nodes = list(dag.topological_op_nodes())

        for id,node in enumerate(topological_nodes):
            if node.op.num_qubits == 1:
                if node.qargs[0] not in qargs_map:
                    qargs_map[node.qargs[0]] = len(qargs_map)
                    qargs_inverse_map[qargs_map[node.qargs[0]]] = node.qargs[0]

                gate_l = 1
                if node.name in self.gate_durations:
                    gate_l = self.gate_durations[node.name]

                cargs_list = []
                for ca in node.cargs:
                    if ca not in cargs_map:
                        cargs_map[ca] = len(cargs_map)
                    cargs_list += [cargs_map[ca]]
                
                s.add_operation_1(node.name,qargs_map[node.qargs[0]],node.params,cargs_list,gate_l,id)

            elif node.op.num_qubits == 2:
                if node.qargs[0] not in qargs_map:
                    qargs_map[node.qargs[0]] = len(qargs_map)
                    qargs_inverse_map[qargs_map[node.qargs[0]]] = node.qargs[0]
                if node.qargs[1] not in qargs_map:
                    qargs_map[node.qargs[1]] = len(qargs_map)
                    qargs_inverse_map[qargs_map[node.qargs[1]]] = node.qargs[1]

                gate_l = 1
                if node.name in self.gate_durations:
                    gate_l = self.gate_durations[node.name]

                cargs_list = []
                for ca in node.cargs:
                    if ca not in cargs_map:
                        cargs_map[ca] = len(cargs_map)
                    cargs_list += [cargs_map[ca]]

                s.add_operation_2(node.name,qargs_map[node.qargs[0]],qargs_map[node.qargs[1]],node.params,cargs_list,gate_l,id)

        r = ddroute_cpp.DDRouter(s,t)
       
        r.depth_driven_place()

        layout = Layout()
        for q in range(dag.num_qubits()):
            layout.add(qargs_inverse_map[q],r.ltp_map[q])

        self.property_set['layout'] = layout

        if self.apply_layout:
            ancilla_pass = FullAncillaAllocation(self.coupling_map)
            ancilla_pass.property_set = self.property_set
            dag = ancilla_pass.run(dag)
            enlarge_pass = EnlargeWithAncilla()
            enlarge_pass.property_set = ancilla_pass.property_set
            dag = enlarge_pass.run(dag)
            apply_pass = ApplyLayout()
            apply_pass.property_set = enlarge_pass.property_set
            dag = apply_pass.run(dag)
            self.property_set = apply_pass.property_set
            
        return dag


class DDRoute(TransformationPass):
    """
    A routing pass for quantum circuits using DDRoute.

    Args:
        coupling_map (qiskit.transpiler.CouplingMap): Target device connectivity map
        gate_lengths (dict[str, int], optional): Dictionary of gate lengths
        seed (int, optional): Seed for underlying randomization
    """

    def __init__(self, coupling_map: CouplingMap, gate_durations: dict[str,int]=None, seed: int=None):
        super().__init__()
        self.coupling_map = coupling_map
        if gate_durations is not None:
            self.__setattr__('gate_durations',gate_durations)
        else:
            self.__setattr__('gate_durations',{})
        if seed is not None:
            self.__setattr__('router_seed',seed%32000)
        else:
            self.__setattr__('router_seed',None)

        if not coupling_map.is_symmetric:
            raise ValueError('Asymmetric coupling maps are not supported')
        if not coupling_map.is_connected():
            raise ValueError('Disconnected coupling maps are not supported')

    def run(self, dag: DAGCircuit):

        if self.router_seed:
            ddroute_cpp.set_seed(self.router_seed)
        else:
            ddroute_cpp.set_seed(random.randint(0,32000))

        t = ddroute_cpp.DDRTopology(self.coupling_map.size())
        for e in self.coupling_map:
            t.add_edge(e[0],e[1],True)

        s = ddroute_cpp.DDRSchedule(dag.num_qubits())

        self.gate_durations['measure'] = 0.0

        qargs_map = {}
        cargs_map = {}
        qargs_inverse_map = {}
        cargs_inverse_map = {}

        topological_nodes = list(dag.topological_op_nodes())

        for id,node in enumerate(topological_nodes):

            if len(node.cargs)>0 and node.name != 'measure':
                raise ValueError('Unsupported operation: ' + node.name)

            if node.op.num_qubits == 1:
                if node.qargs[0] not in qargs_map:
                    qargs_map[node.qargs[0]] = node.qargs[0]._index
                    qargs_inverse_map[qargs_map[node.qargs[0]]] = node.qargs[0]

                gate_l = 1
                if node.name in self.gate_durations:
                    gate_l = self.gate_durations[node.name]

                cargs_list = []
                for ca in node.cargs:
                    if ca not in cargs_map:
                        cargs_map[ca] = len(cargs_map)
                        cargs_inverse_map[cargs_map[ca]] = ca
                    cargs_list += [cargs_map[ca]]
                
                s.add_operation_1(node.name,qargs_map[node.qargs[0]],node.params,cargs_list,gate_l,id)

            elif node.op.num_qubits == 2:
                if node.qargs[0] not in qargs_map:
                    qargs_map[node.qargs[0]] = node.qargs[0]._index
                    qargs_inverse_map[qargs_map[node.qargs[0]]] = node.qargs[0]
                if node.qargs[1] not in qargs_map:
                    qargs_map[node.qargs[1]] = node.qargs[1]._index
                    qargs_inverse_map[qargs_map[node.qargs[1]]] = node.qargs[1]

                gate_l = 1
                if node.name in self.gate_durations:
                    gate_l = self.gate_durations[node.name]

                cargs_list = []
                for ca in node.cargs:
                    if ca not in cargs_map:
                        cargs_map[ca] = len(cargs_map)
                        cargs_inverse_map[cargs_map[ca]] = ca
                    cargs_list += [cargs_map[ca]]

                s.add_operation_2(node.name,qargs_map[node.qargs[0]],qargs_map[node.qargs[1]],node.params,cargs_list,gate_l,id)

        r = ddroute_cpp.DDRouter(s,t)

        ltp_map = range(dag.num_qubits())

        r.set_initial_layout(ltp_map)

        if 'swap' in self.gate_durations:
            r.set_swap_depth(self.gate_durations['swap'])
        else:
            r.set_swap_depth(3)

        pc = r.depth_driven_routing()

        result = DAGCircuit()
        qreg = QuantumRegister(self.coupling_map.size(),'q')

        result.add_qreg(qreg)
        result.add_clbits(dag.clbits)
        for creg in dag.cregs.values():
            result.add_creg(creg)
        for var in dag.iter_input_vars():
            result.add_input_var(var)
        for var in dag.iter_captured_vars():
            result.add_captured_var(var)
        for var in dag.iter_declared_vars():
            result.add_declared_var(var)
        for stretch in dag.iter_captured_stretches():
            result.add_captured_stretch(stretch)
        for stretch in dag.iter_declared_stretches():
            result.add_declared_stretch(stretch)
        result.global_phase = dag.global_phase

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

        final_layout = Layout()
        for q in range(dag.num_qubits()):
            final_layout.add(dag.qubits[q],r.ltp_map[q])

        self.property_set['final_layout'] = final_layout

        return result

class DDPlaceAndRoute(TransformationPass):
    """
    A layour and routing pass for quantum circuits using DDPlace and DDRoute.

    Args:
        coupling_map (qiskit.transpiler.CouplingMap): Target device connectivity map
        gate_lengths (dict[str, int], optional): Dictionary of gate lengths
        seed (int, optional): Seed for underlying randomization
    """

    def __init__(self, coupling_map: CouplingMap, gate_durations: dict[str,int]=None, seed: int=None):
        super().__init__()
        self.coupling_map = coupling_map
        if gate_durations is not None:
            self.__setattr__('gate_durations',gate_durations)
        else:
            self.__setattr__('gate_durations',{})
        if seed is not None:
            self.__setattr__('router_seed',seed%32000)
        else:
            self.__setattr__('router_seed',None)

        if not coupling_map.is_symmetric:
            raise ValueError('Asymmetric coupling maps are not supported')
        if not coupling_map.is_connected():
            raise ValueError('Disconnected coupling maps are not supported')

    def run(self, dag: DAGCircuit):

        if self.router_seed:
            ddroute_cpp.set_seed(self.router_seed)
        else:
            ddroute_cpp.set_seed(random.randint(0,32000))

        t = ddroute_cpp.DDRTopology(self.coupling_map.size())
        for e in self.coupling_map:
            t.add_edge(e[0],e[1],True)

        s = ddroute_cpp.DDRSchedule(dag.num_qubits())

        self.gate_durations['measure'] = 0.0

        qargs_map = {}
        cargs_map = {}
        qargs_inverse_map = {}
        cargs_inverse_map = {}

        topological_nodes = list(dag.topological_op_nodes())

        for id,node in enumerate(topological_nodes):

            if len(node.cargs)>0 and node.name != 'measure':
                raise ValueError('Unsupported operation: ' + node.name)

            if node.op.num_qubits == 1:
                if node.qargs[0] not in qargs_map:
                    qargs_map[node.qargs[0]] = node.qargs[0]._index
                    qargs_inverse_map[qargs_map[node.qargs[0]]] = node.qargs[0]

                gate_l = 1
                if node.name in self.gate_durations:
                    gate_l = self.gate_durations[node.name]

                cargs_list = []
                for ca in node.cargs:
                    if ca not in cargs_map:
                        cargs_map[ca] = len(cargs_map)
                        cargs_inverse_map[cargs_map[ca]] = ca
                    cargs_list += [cargs_map[ca]]
                
                s.add_operation_1(node.name,qargs_map[node.qargs[0]],node.params,cargs_list,gate_l,id)

            elif node.op.num_qubits == 2:
                if node.qargs[0] not in qargs_map:
                    qargs_map[node.qargs[0]] = node.qargs[0]._index
                    qargs_inverse_map[qargs_map[node.qargs[0]]] = node.qargs[0]
                if node.qargs[1] not in qargs_map:
                    qargs_map[node.qargs[1]] = node.qargs[1]._index
                    qargs_inverse_map[qargs_map[node.qargs[1]]] = node.qargs[1]

                gate_l = 1
                if node.name in self.gate_durations:
                    gate_l = self.gate_durations[node.name]

                cargs_list = []
                for ca in node.cargs:
                    if ca not in cargs_map:
                        cargs_map[ca] = len(cargs_map)
                        cargs_inverse_map[cargs_map[ca]] = ca
                    cargs_list += [cargs_map[ca]]

                s.add_operation_2(node.name,qargs_map[node.qargs[0]],qargs_map[node.qargs[1]],node.params,cargs_list,gate_l,id)

        r = ddroute_cpp.DDRouter(s,t)
        if 'swap' in self.gate_durations:
            r.set_swap_depth(self.gate_durations['swap'])
        else:
            r.set_swap_depth(3)
       
        r.depth_driven_place()

        layout = Layout()
        for q in range(dag.num_qubits()):
            layout.add(qargs_inverse_map[q],r.ltp_map[q])

        self.property_set['layout'] = layout

        pc = r.depth_driven_routing()

        result = DAGCircuit()
        qreg = QuantumRegister(self.coupling_map.size(),'q')

        result.add_qreg(qreg)
        result.add_clbits(dag.clbits)
        for creg in dag.cregs.values():
            result.add_creg(creg)
        for var in dag.iter_input_vars():
            result.add_input_var(var)
        for var in dag.iter_captured_vars():
            result.add_captured_var(var)
        for var in dag.iter_declared_vars():
            result.add_declared_var(var)
        for stretch in dag.iter_captured_stretches():
            result.add_captured_stretch(stretch)
        for stretch in dag.iter_declared_stretches():
            result.add_declared_stretch(stretch)
        result.global_phase = dag.global_phase

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

        final_layout = Layout()
        for q in range(dag.num_qubits()):
            final_layout.add(dag.qubits[q],r.ltp_map[q])

        self.property_set['final_layout'] = final_layout

        return result

class DDPlacePassManager(PassManagerStagePlugin):
    def pass_manager(self, pass_manager_config, optimization_level=None) -> PassManager:
        pm = PassManager()
        pm.append(DDPlace(pass_manager_config.coupling_map,apply_layout=True,seed=pass_manager_config.seed_transpiler))
        return pm

class DDRoutePassManager(PassManagerStagePlugin):
    def pass_manager(self, pass_manager_config, optimization_level=None) -> PassManager:
        pm = PassManager()
        pm.append(DDRoute(pass_manager_config.coupling_map,seed=pass_manager_config.seed_transpiler))
        return pm
