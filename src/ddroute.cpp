#include "utils.hpp"
#include "schedule.hpp"
#include "topology.hpp"
#include "router.cpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

void set_seed(int seed){
    std::srand(seed);
}

PYBIND11_MODULE(ddroute_cpp, m){
    m.doc() = "DDRoute: C++ implementation";

    m.def("set_seed",&set_seed);
    
    py::class_<DDRSchedule>(m, "DDRSchedule")
        .def(py::init<int>(), 
        R"pbdoc(
            DDRSchedule constructor

            @param num_qubit: number of logical qubits
        )pbdoc")
        .def("add_operation_1", &DDRSchedule::add_operation_1, py::arg("name"), py::arg("qubit"), py::arg("gargs"), py::arg("cargs"), py::arg("gate_depth")=1, py::arg("gate_id")=-1, 
        R"pbdoc(
            Insert a single-qubit quantum gate in the circuit

            @param name: gate name
            @param qubit: quantum gate operand
            @param gargs: gate additional arguments
            @param cargs: classical arguments
            @param gate_depth: gate duration (default: 1)
            @param gate_id: gate numerical id (optional)
        )pbdoc")
        .def("add_operation_2", &DDRSchedule::add_operation_2, py::arg("name"), py::arg("qubit1"), py::arg("qubit2"), py::arg("gargs"), py::arg("cargs"), py::arg("gate_depth")=1, py::arg("gate_id")=-1, 
        R"pbdoc(
            Insert a two-qubit quantum gate in the circuit

            @param name: gate name
            @param qubit1: quantum gate first operand
            @param qubit2: quantum gate second operand
            @param gargs: gate additional arguments
            @param cargs: classical arguments
            @param gate_depth: gate duration (default: 1)
            @param gate_id: gate numerical id (optional)
        )pbdoc")
        .def("depth", &DDRSchedule::depth, 
        R"pbdoc(
            Quantum circuit depth
        )pbdoc")
        .def("depth_2g", &DDRSchedule::depth_2g, 
        R"pbdoc(
            Quantum circuit two-qubit-gates depth
        )pbdoc");

    py::class_<DDRTopology>(m, "DDRTopology")
        .def(py::init<int>(), 
        R"pbdoc(
            DDRTopology constructor

            @param num_nodes: number of physical qubits
        )pbdoc")
        .def("add_edge", &DDRTopology::add_edge, 
        R"pbdoc(
            Insert an edge in the coupling map

            @param a: first physical qubit
            @param b: second physical qubit
            @param symmetric: insert also link from b to a
        )pbdoc")
        .def("get_distance", &DDRTopology::get_distance, 
        R"pbdoc(
            Compute the SWAP distance between two physical qubits in the coupling map

            @param a: first physical qubit
            @param b: second physical qubit
            @return SWAP (i.e. non weighted) distance between a and b
        )pbdoc")
        .def("get_predecessor", &DDRTopology::get_predecessor, 
        R"pbdoc(
            Compute the predecessor of 'b' along the optimal path from 'a' to 'b' in the coupling map

            @param a: first physical qubit
            @param b: second physical qubit
            @return The predecessor of 'b' along the optimal path from 'a' to 'b'
        )pbdoc")
        .def("is_connected", &DDRTopology::is_connected, 
        R"pbdoc(
            Check if two qubits are connected in the coupling map
            
            @param a: first physical qubit
            @param b: second physical qubit
            @return True if a and b are connected, False otherwise
        )pbdoc");

    py::class_<DDRScheduleReader>(m, "DDRScheduleReader")
        .def(py::init<DDRSchedule&>(), 
        R"pbdoc(
            DDRScheduleReader constructor

            @param s: DDRSchedule to read
        )pbdoc")
        .def("next", &DDRScheduleReader::next, 
        R"pbdoc(
            Iterate over the next gate in the quantum circuit
            
            @return True if the current gate is valid, False if the end of the circuit has been reached
        )pbdoc")
        .def("depth", &DDRScheduleReader::depth, 
        R"pbdoc(
            Quantum circuit depth
        )pbdoc")
        .def("qubit_depth", &DDRScheduleReader::qubit_depth, 
        R"pbdoc(
            Qubit depth

            @param q: qubit
        )pbdoc")
        .def("gate_count", &DDRScheduleReader::gate_count, 
        R"pbdoc(
            Quantum circuit gate count

            @param name: name of the quantum gate to count
        )pbdoc")
        .def("name", &DDRScheduleReader::name, py::return_value_policy::copy, 
        R"pbdoc(
            Current gate name
        )pbdoc")
        .def("operands", &DDRScheduleReader::operands, py::return_value_policy::copy, 
        R"pbdoc(
            Current gate quantum operands
        )pbdoc")
        .def("gate_args", &DDRScheduleReader::gate_args, py::return_value_policy::copy, 
        R"pbdoc(
            Current gate additional arguments
        )pbdoc")
        .def("cargs", &DDRScheduleReader::cargs, py::return_value_policy::copy, 
        R"pbdoc(
            Current gate classical arguments
        )pbdoc")
        .def("gate_id", &DDRScheduleReader::gate_id, py::return_value_policy::copy, 
        R"pbdoc(
            Current gate id
        )pbdoc");

    py::class_<DDRouter>(m, "DDRouter")
        .def(py::init<DDRSchedule&,DDRTopology&>(), 
        R"pbdoc(
            DDRouter constructor

            @param c: DDRSchedule of the logical circuit
            @param t: DDRTopology of the targeted quantum processor
        )pbdoc")
        .def("set_swap_depth", &DDRouter::set_swap_depth, 
        R"pbdoc(
            Set SWAP gate duration

            @param value: SWAP duration
        )pbdoc")
        .def("set_initial_layout", &DDRouter::set_initial_layout, 
        R"pbdoc(
            Set initial layout

            @param ltp: logical to physical map (i.e. ltp[0] is the physical qubit where the logical qubit 0 is mapped)
        )pbdoc")
        .def("depth_driven_place", &DDRouter::depth_driven_place, 
        R"pbdoc(
            Set initial layout using DDPlace
        )pbdoc")
        .def("depth_driven_routing", &DDRouter::depth_driven_routing, 
        R"pbdoc(
            Run DDRoute on the logical quantum circuit

            @return A DDRScheduleReader instance of the routed quantum circuit
        )pbdoc")
        .def_readonly("ltp_map", &DDRouter::ltp_map, 
        R"pbdoc(
            Logical-to-physical map
        )pbdoc");
}