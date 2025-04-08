import random
from qiskit import QuantumCircuit, transpile, qasm3, qasm2
from qiskit.compiler import transpile
from qiskit.circuit.library import QFT, TwoLocal, CDKMRippleCarryAdder

gate_set = ['cx','rz','sx','x']

for n_qubit in range(10,501,10):
    circ = QuantumCircuit(n_qubit)
    circ = circ.compose(CDKMRippleCarryAdder(int((n_qubit-2)/2)))
    circ = circ.compose(TwoLocal(n_qubit, 'rz', 'cx', entanglement='full', parameter_prefix='f'))
    circ = circ.compose(QFT(num_qubits=n_qubit, do_swaps=False, inverse=False, insert_barriers=False))
    circ = circ.compose(CDKMRippleCarryAdder(int((n_qubit-2)/2)))
    circ = circ.compose(TwoLocal(n_qubit, 'rz', 'cx', entanglement='full', parameter_prefix='k'))
    circ = circ.compose(QFT(num_qubits=n_qubit, do_swaps=False, inverse=False, insert_barriers=False))
    circ = circ.assign_parameters([random.uniform(0.1,0.9) for j in range(n_qubit*8)])

    transpiled_qc = transpile(circ, basis_gates=gate_set, optimization_level = 1)

    file_name = "../circuits/qiskit_lib/qiskit_lib_" + str(n_qubit) + ".qasm"

    with open(file_name, "w") as file:
        file.write(qasm3.dumps(transpiled_qc, disable_constants=True , experimental=qasm3.ExperimentalFeatures.SWITCH_CASE_V1))
        file.flush()

    file_name = "../circuits/qiskit_lib/qasm2/qiskit_lib_" + str(n_qubit) + ".qasm"

    with open(file_name, "w") as file:
        file.write(qasm2.dumps(transpiled_qc))
        file.flush()