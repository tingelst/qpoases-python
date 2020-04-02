#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
namespace py = pybind11;

#include <qpOASES.hpp>
using namespace qpOASES;

using NumpyArray = py::array_t<double, py::array::c_style | py::array::forcecast>;

bool tobool(BooleanType boolean)
{
    switch (boolean)
    {
    case BT_FALSE:
        return false;
    case BT_TRUE:
        return true;
    }
}

PYBIND11_MODULE(_qpoases, m)
{
    m.doc() = "qpOASES";

    py::class_<Options>(m, "Options")
        .def(py::init<>());

    py::enum_<HessianType>(m, "HessianType")
        .value("ZERO", HST_ZERO)
        .value("IDENTITY", HST_IDENTITY)
        .value("POSDEF", HST_POSDEF)
        .value("POSDEF_NULLSPACE", HST_POSDEF_NULLSPACE)
        .value("SEMIDEF", HST_SEMIDEF)
        .value("INDEF", HST_INDEF)
        .value("UNKNOWN", HST_UNKNOWN)
        .export_values();

    py::enum_<BooleanType>(m, "BooleanType")
        .value("TRUE", BT_TRUE)
        .value("FALSE", BT_FALSE)
        .export_values();

    py::enum_<PrintLevel>(m, "PrintLevel")
        .value("DEBUG_ITER", PL_DEBUG_ITER)
        .value("TABULAR", PL_TABULAR)
        .value("NONE", PL_NONE)
        .value("LOW", PL_LOW)
        .value("MEDIUM", PL_MEDIUM)
        .value("HIGH", PL_HIGH)
        .export_values();

    py::enum_<QProblemStatus>(m, "QProblemStatus")
        .value("NOT_INITIALISED", QPS_NOTINITIALISED)
        .value("PREPARING_AUXILIARY_QP", QPS_PREPARINGAUXILIARYQP)
        .value("AUXILIARY_QP_SOLVED", QPS_AUXILIARYQPSOLVED)
        .value("PERFORMING_HOMOTOPY", QPS_PERFORMINGHOMOTOPY)
        .value("HOMOTOPY_QP_SOLVED", QPS_HOMOTOPYQPSOLVED)
        .value("SOLVED", QPS_SOLVED)
        .export_values();

    py::class_<QProblemB>(m, "QProblemB")
        .def(py::init<>())
        .def(py::init<int, HessianType, BooleanType>(), py::arg("nV"), py::arg("hessian_type") = HST_UNKNOWN, py::arg("allocate_dense_matrices") = BT_TRUE)
        .def(py::init<const QProblemB &>())
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
        .def("getWorkingSetBounds",
             [](QProblemB &problem) {
                 auto result = py::array_t<double>(problem.getNV());
                 auto result_ptr = static_cast<double *>(result.request().ptr);
                 problem.getWorkingSetBounds(result_ptr);
                 return result;
             })
        // .def("getWorkingSetConstraints",
        //      [](QProblemB &problem) {
        //          auto result = py::array_t<double>(problem.getNV());
        //          auto result_ptr = static_cast<double *>(result.request().ptr);
        //          problem.getWorkingSetConstraints(result_ptr);
        //          return result;
        //      })
        .def("getNZ", &QProblemB::getNZ)

        .def("getObjVal",
             [](QProblemB &problem) {
                 return problem.getObjVal();
             })
        .def("getObjVal",
             [](QProblemB &problem, NumpyArray x) {
                 auto x_ptr = static_cast<double *>(x.request().ptr);
                 return problem.getObjVal(x_ptr);
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
             })
        .def("getStatus", &QProblemB::getStatus)
        .def("isInitialized",
             [](QProblemB &problem) {
                 return tobool(problem.isInitialised());
             })
        .def("isSolved",
             [](QProblemB &problem) {
                 return tobool(problem.isSolved());
             })
        .def("isInfeasible",
             [](QProblemB &problem) {
                 return tobool(problem.isInfeasible());
             })
        .def("isUnbounded",
             [](QProblemB &problem) {
                 return tobool(problem.isUnbounded());
             })
        .def("getHessianType", &QProblemB::getHessianType)
        .def("setHessianType", &QProblemB::setHessianType)
        .def("usingRegularisation",
             [](QProblemB &problem) {
                 return tobool(problem.usingRegularisation());
             })
        .def("getOptions", &QProblemB::getOptions)
        .def("setOptions", &QProblemB::setOptions)
        .def("getPrintLevel", &QProblemB::getPrintLevel)
        .def("setPrintLevel", &QProblemB::setPrintLevel)
        .def("getCount", &QProblemB::getCount)
        .def("resetCounter", &QProblemB::resetCounter)
        .def("printProperties", &QProblemB::printProperties)
        .def("printOptions", &QProblemB::printOptions);

    py::class_<QProblem, QProblemB>(m, "QProblem")
        .def(py::init<int, int, HessianType, BooleanType>(), py::arg("nV"), py::arg("nC"), py::arg("hessian_type") = HST_UNKNOWN, py::arg("alloc_dense_mats") = BT_TRUE)
        .def(
            "init",
            [](QProblem &problem, NumpyArray H, NumpyArray g, NumpyArray A, NumpyArray lb,
               NumpyArray ub, NumpyArray lbA, NumpyArray ubA, int nWSR, std::optional<double *const> cputime,
               std::optional<NumpyArray> xopt, std::optional<NumpyArray> yopt) {
                auto H_ptr = static_cast<double *>(H.request().ptr);
                auto g_ptr = static_cast<double *>(g.request().ptr);
                auto A_ptr = static_cast<double *>(A.request().ptr);
                auto lb_ptr = static_cast<double *>(lb.request().ptr);
                auto ub_ptr = static_cast<double *>(ub.request().ptr);
                auto lbA_ptr = static_cast<double *>(lbA.request().ptr);
                auto ubA_ptr = static_cast<double *>(ubA.request().ptr);

                double *xopt_ptr = nullptr;
                if (xopt)
                {
                    xopt_ptr = static_cast<double *>(xopt.value().request().ptr);
                }
                double *yopt_ptr = nullptr;
                if (yopt)
                {
                    yopt_ptr = static_cast<double *>(yopt.value().request().ptr);
                }

                return problem.init(H_ptr, g_ptr, A_ptr, lb_ptr, ub_ptr, lbA_ptr, ubA_ptr, nWSR, cputime.value_or(nullptr), xopt_ptr, yopt_ptr);
            },
            py::arg("H"), py::arg("g"), py::arg("A"), py::arg("lb"), py::arg("ub"), py::arg("lbA"), py::arg("ubA"), py::arg("nWSR"),
            py::arg("cputime"), py::arg("xopt"), py::arg("yopt"))
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
        .def(py::init<int, int, HessianType, BooleanType>(), py::arg("nV"), py::arg("nC"), py::arg("hessian_type") = HST_UNKNOWN, py::arg("alloc_dense_mats") = BT_TRUE)
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