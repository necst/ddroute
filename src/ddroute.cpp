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
        .def(py::init<int>())
        .def("add_operation_1", &DDRSchedule::add_operation_1, py::arg("name"), py::arg("qubit"), py::arg("gargs"), py::arg("cargs"), py::arg("gate_depth")=1, py::arg("gate_id")=-1)
        .def("add_operation_2", &DDRSchedule::add_operation_2, py::arg("name"), py::arg("qubit1"), py::arg("qubit2"), py::arg("gargs"), py::arg("cargs"), py::arg("gate_depth")=1, py::arg("gate_id")=-1)
        .def("depth", &DDRSchedule::depth)
        .def("depth_2g", &DDRSchedule::depth_2g);

    py::class_<DDRTopology>(m, "DDRTopology")
        .def(py::init<int>())
        .def("add_edge", &DDRTopology::add_edge)
        .def("get_distance", &DDRTopology::get_distance)
        .def("get_predecessor", &DDRTopology::get_predecessor)
        .def("is_connected", &DDRTopology::is_connected);

    py::class_<DDRScheduleReader>(m, "DDRScheduleReader")
        .def(py::init<DDRSchedule&>())
        .def("next", &DDRScheduleReader::next)
        .def("depth", &DDRScheduleReader::depth)
        .def("qubit_depth", &DDRScheduleReader::qubit_depth)
        .def("gate_count", &DDRScheduleReader::gate_count)
        .def("name", &DDRScheduleReader::name, py::return_value_policy::copy)
        .def("operands", &DDRScheduleReader::operands, py::return_value_policy::copy)
        .def("gate_args", &DDRScheduleReader::gate_args, py::return_value_policy::copy)
        .def("cargs", &DDRScheduleReader::cargs, py::return_value_policy::copy)
        .def("gate_id", &DDRScheduleReader::gate_id, py::return_value_policy::copy);

    py::class_<DDRouter>(m, "DDRouter")
        .def(py::init<DDRSchedule&,DDRTopology&>())
        .def("set_swap_depth", &DDRouter::set_swap_depth)
        .def("depth_driven_place", &DDRouter::depth_driven_place)
        .def("depth_driven_routing", &DDRouter::depth_driven_routing)
        .def_readonly("ltp_map", &DDRouter::ltp_map);
}