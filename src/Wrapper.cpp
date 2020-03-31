#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
namespace py = pybind11;

#include <qpOASES.hpp>
using namespace qpOASES;

using NumpyArray = py::array_t<double, py::array::c_style | py::array::forcecast>;

PYBIND11_MODULE(_qpoases, m)
{
    m.doc() = "qpOASES";

    py::class_<Options>(m, "Options")
        .def(py::init<>());

    py::class_<QProblemB>(m, "QProblemB")
        .def(py::init<>())
        .def(py::init<int>())
        .def(py::init<const QProblemB&>())
        .def("reset", &QProblemB::reset)
        .def("init",
             [](QProblemB &problem, NumpyArray H, NumpyArray g, NumpyArray lb, NumpyArray ub, int nWSR) {
                 auto H_ptr = static_cast<double *>(H.request().ptr);
                 auto g_ptr = static_cast<double *>(g.request().ptr);
                 auto lb_ptr = static_cast<double *>(lb.request().ptr);
                 auto ub_ptr = static_cast<double *>(ub.request().ptr);
                 return problem.init(H_ptr, g_ptr, lb_ptr, ub_ptr, nWSR);
             })
        .def("hotstart",
             [](QProblemB &problem, NumpyArray g_new, NumpyArray lb_new, NumpyArray ub_new, int nWSR) {
                 auto g_new_ptr = static_cast<double *>(g_new.request().ptr);
                 auto lb_new_ptr = static_cast<double *>(lb_new.request().ptr);
                 auto ub_new_ptr = static_cast<double *>(ub_new.request().ptr);
                 return problem.hotstart(g_new_ptr, lb_new_ptr, ub_new_ptr, nWSR);
             })
        .def("getWorkingSet",
             [](QProblemB &problem) {
                 auto result = py::array_t<double>(problem.getNV());
                 auto result_ptr = static_cast<double *>(result.request().ptr);
                 problem.getWorkingSet(result_ptr);
                 return result;
             })
        .def("getObjVal",
             [](QProblemB &problem) {
                 return problem.getObjVal();
             })
        .def("getPrimalSolution",
             [](QProblemB &problem) {
                 auto result = py::array_t<double>(problem.getNV());
                 auto result_ptr = static_cast<double *>(result.request().ptr);
                 problem.getPrimalSolution(result_ptr);
                 return result;
             })
        .def("getDualSolution",
             [](QProblemB &problem) {
                 auto result = py::array_t<double>(problem.getNV());
                 auto result_ptr = static_cast<double *>(result.request().ptr);
                 problem.getDualSolution(result_ptr);
                 return result;
             });

    py::class_<QProblem, QProblemB>(m, "QProblem")
        .def(py::init<int, int>())
        .def("init",
             [](QProblem &problem, NumpyArray H, NumpyArray g, NumpyArray A, NumpyArray lb, NumpyArray ub, NumpyArray lbA, NumpyArray ubA, int nWSR) {
                 auto H_ptr = static_cast<double *>(H.request().ptr);
                 auto g_ptr = static_cast<double *>(g.request().ptr);
                 auto A_ptr = static_cast<double *>(A.request().ptr);
                 auto lb_ptr = static_cast<double *>(lb.request().ptr);
                 auto ub_ptr = static_cast<double *>(ub.request().ptr);
                 auto lbA_ptr = static_cast<double *>(lbA.request().ptr);
                 auto ubA_ptr = static_cast<double *>(ubA.request().ptr);

                 return problem.init(H_ptr, g_ptr, A_ptr, lb_ptr, ub_ptr, lbA_ptr, ubA_ptr, nWSR);
             })
        .def("hotstart",
             [](QProblem &problem, NumpyArray g, NumpyArray lb, NumpyArray ub, NumpyArray lbA, NumpyArray ubA, int nWSR) {
                 auto g_ptr = static_cast<double *>(g.request().ptr);
                 auto lb_ptr = static_cast<double *>(lb.request().ptr);
                 auto ub_ptr = static_cast<double *>(ub.request().ptr);
                 auto lbA_ptr = static_cast<double *>(lbA.request().ptr);
                 auto ubA_ptr = static_cast<double *>(ubA.request().ptr);
                 return problem.hotstart(g_ptr, lb_ptr, ub_ptr, lbA_ptr, ubA_ptr, nWSR);
             });

    py::class_<SQProblem, QProblem>(m, "SQProblem")
        .def(py::init<>())
        .def(py::init<int, int>())
        .def("init",
             [](SQProblem &problem, NumpyArray H, NumpyArray g, NumpyArray A, NumpyArray lb, NumpyArray ub, NumpyArray lbA, NumpyArray ubA, int nWSR) {
                 auto H_ptr = static_cast<double *>(H.request().ptr);
                 auto g_ptr = static_cast<double *>(g.request().ptr);
                 auto A_ptr = static_cast<double *>(A.request().ptr);
                 auto lb_ptr = static_cast<double *>(lb.request().ptr);
                 auto ub_ptr = static_cast<double *>(ub.request().ptr);
                 auto lbA_ptr = static_cast<double *>(lbA.request().ptr);
                 auto ubA_ptr = static_cast<double *>(ubA.request().ptr);

                 return problem.init(H_ptr, g_ptr, A_ptr, lb_ptr, ub_ptr, lbA_ptr, ubA_ptr, nWSR);
             })
        .def("hotstart",
             [](SQProblem &problem, NumpyArray H, NumpyArray g, NumpyArray A, NumpyArray lb, NumpyArray ub, NumpyArray lbA, NumpyArray ubA, int nWSR) {
                 auto H_ptr = static_cast<double *>(H.request().ptr);
                 auto g_ptr = static_cast<double *>(g.request().ptr);
                 auto A_ptr = static_cast<double *>(A.request().ptr);
                 auto lb_ptr = static_cast<double *>(lb.request().ptr);
                 auto ub_ptr = static_cast<double *>(ub.request().ptr);
                 auto lbA_ptr = static_cast<double *>(lbA.request().ptr);
                 auto ubA_ptr = static_cast<double *>(ubA.request().ptr);
                 return problem.hotstart(H_ptr, g_ptr, A_ptr, lb_ptr, ub_ptr, lbA_ptr, ubA_ptr, nWSR);
             });

    py::enum_<returnValue>(m, "ReturnValue")
        .value("SUCCESSFUL_RETURN", returnValue::SUCCESSFUL_RETURN)
        .export_values();
}