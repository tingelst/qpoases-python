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

BooleanType toBooleanType(bool bool_)
{
  if (bool_)
  {
    return BT_TRUE;
  }
  else
  {
    BT_FALSE;
  }
}

PYBIND11_MODULE(_qpoases, m)
{
  m.doc() = "qpOASES";

  py::class_<Options>(m, "Options").def(py::init<>());

  py::class_<Bounds>(m, "Bounds").def(py::init<>());

  py::class_<Constraints>(m, "Constraints").def(py::init<>());
  // py::class_<ConstraintProduct>(m, "ConstraintProduct").def(py::init<>());

  py::enum_<HessianType>(m, "HessianType")
      .value("ZERO", HST_ZERO)
      .value("IDENTITY", HST_IDENTITY)
      .value("POSDEF", HST_POSDEF)
      .value("POSDEF_NULLSPACE", HST_POSDEF_NULLSPACE)
      .value("SEMIDEF", HST_SEMIDEF)
      .value("INDEF", HST_INDEF)
      .value("UNKNOWN", HST_UNKNOWN)
      .export_values();

  py::enum_<BooleanType>(m, "BooleanType").value("TRUE", BT_TRUE).value("FALSE", BT_FALSE).export_values();

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
      .def(py::init<int, HessianType, BooleanType>(), py::arg("nv"), py::arg("hessian_type") = HST_UNKNOWN,
           py::arg("allocate_dense_matrices") = BT_TRUE)
      .def(py::init<const QProblemB&>())
      .def("reset", &QProblemB::reset)
      .def(
          "init",
          [](QProblemB& problem, NumpyArray H, NumpyArray g, NumpyArray lb, NumpyArray ub, int nWSR,
             std::optional<double* const> cputime, std::optional<NumpyArray> xopt, std::optional<NumpyArray> yopt,
             std::optional<const Bounds* const> guessed_bounds, std::optional<NumpyArray> R) {
            auto H_ptr = static_cast<double*>(H.request().ptr);
            auto g_ptr = static_cast<double*>(g.request().ptr);
            auto lb_ptr = static_cast<double*>(lb.request().ptr);
            auto ub_ptr = static_cast<double*>(ub.request().ptr);
            double* xopt_ptr = nullptr;
            if (xopt)
            {
              xopt_ptr = static_cast<double*>(xopt.value().request().ptr);
            }
            double* yopt_ptr = nullptr;
            if (yopt)
            {
              yopt_ptr = static_cast<double*>(yopt.value().request().ptr);
            }
            double* R_ptr = nullptr;
            if (R)
            {
              R_ptr = static_cast<double*>(R.value().request().ptr);
            }
            return problem.init(H_ptr, g_ptr, lb_ptr, ub_ptr, nWSR, cputime.value_or(nullptr), xopt_ptr, yopt_ptr,
                                guessed_bounds.value_or(nullptr), R_ptr);
          },
          py::arg("H"), py::arg("g"), py::arg("lb"), py::arg("ub"), py::arg("nwsr"), py::arg("cputime"),
          py::arg("xopt"), py::arg("yopt"), py::arg("guessed_bounds"), py::arg("R"))
      .def(
          "hotstart",
          [](QProblemB& problem, NumpyArray g, NumpyArray lb, NumpyArray ub, int nWSR,
             std::optional<double* const> cputime, std::optional<const Bounds* const> guessed_bounds) {
            auto g_ptr = static_cast<double*>(g.request().ptr);
            auto lb_ptr = static_cast<double*>(lb.request().ptr);
            auto ub_ptr = static_cast<double*>(ub.request().ptr);
            return problem.hotstart(g_ptr, lb_ptr, ub_ptr, nWSR, cputime.value_or(nullptr),
                                    guessed_bounds.value_or(nullptr));
          },
          py::arg("g"), py::arg("lb"), py::arg("ub"), py::arg("nwsr"), py::arg("cputime"), py::arg("guessed_bounds"))
      .def("getWorkingSet",
           [](QProblemB& problem) {
             auto result = py::array_t<double>(problem.getNV());
             auto result_ptr = static_cast<double*>(result.request().ptr);
             problem.getWorkingSet(result_ptr);
             return result;
           })
      .def("getWorkingSetBounds",
           [](QProblemB& problem) {
             auto result = py::array_t<double>(problem.getNV());
             auto result_ptr = static_cast<double*>(result.request().ptr);
             problem.getWorkingSetBounds(result_ptr);
             return result;
           })
      .def("getWorkingSetConstraints",
           [](QProblemB& problem) {
             auto result = py::array_t<double>(problem.getNV());
             auto result_ptr = static_cast<double*>(result.request().ptr);
             problem.getWorkingSetConstraints(result_ptr);
             return result;
           })
      .def("getBounds",
           [](QProblemB& problem) {
             Bounds bounds;
             auto ret = problem.getBounds(bounds);
             return std::make_tuple(ret, bounds);
           })
      .def("getNV", &QProblemB::getNV)
      .def("getNFR", &QProblemB::getNFR)
      .def("getNFX", &QProblemB::getNFX)
      .def("getNFV", &QProblemB::getNFV)
      .def("getNZ", &QProblemB::getNZ)
      .def("getObjVal", [](QProblemB& problem) { return problem.getObjVal(); })
      .def("getObjVal",
           [](QProblemB& problem, NumpyArray x) {
             auto x_ptr = static_cast<double*>(x.request().ptr);
             return problem.getObjVal(x_ptr);
           })
      .def("getPrimalSolution",
           [](QProblemB& problem) {
             auto result = py::array_t<double>(problem.getNV());
             auto result_ptr = static_cast<double*>(result.request().ptr);
             problem.getPrimalSolution(result_ptr);
             return result;
           })
      .def("getDualSolution",
           [](QProblemB& problem) {
             auto result = py::array_t<double>(problem.getNV());
             auto result_ptr = static_cast<double*>(result.request().ptr);
             problem.getDualSolution(result_ptr);
             return result;
           })
      .def("getStatus", &QProblemB::getStatus)
      .def("isInitialized", [](QProblemB& problem) { return tobool(problem.isInitialised()); })
      .def("isSolved", [](QProblemB& problem) { return tobool(problem.isSolved()); })
      .def("isInfeasible", [](QProblemB& problem) { return tobool(problem.isInfeasible()); })
      .def("isUnbounded", [](QProblemB& problem) { return tobool(problem.isUnbounded()); })
      .def("getHessianType", &QProblemB::getHessianType)
      .def("setHessianType", &QProblemB::setHessianType)
      .def("usingRegularisation", [](QProblemB& problem) { return tobool(problem.usingRegularisation()); })
      .def("getOptions", &QProblemB::getOptions)
      .def("setOptions", &QProblemB::setOptions)
      .def("getPrintLevel", &QProblemB::getPrintLevel)
      .def("setPrintLevel", &QProblemB::setPrintLevel)
      .def("getCount", &QProblemB::getCount)
      .def("resetCounter", &QProblemB::resetCounter)
      .def("printProperties", &QProblemB::printProperties)
      .def("printOptions", &QProblemB::printOptions);

  py::class_<QProblem, QProblemB>(m, "QProblem")
      .def(py::init<int, int, HessianType, BooleanType>(), py::arg("nV"), py::arg("nC"),
           py::arg("hessian_type") = HST_UNKNOWN, py::arg("allocate_dense_matrices") = BT_TRUE)
      .def("reset", &QProblem::reset)
      .def(
          "init",
          [](QProblem& problem, NumpyArray H, NumpyArray g, NumpyArray A, NumpyArray lb, NumpyArray ub, NumpyArray lbA,
             NumpyArray ubA, int nWSR, std::optional<double* const> cputime, std::optional<NumpyArray> xopt,
             std::optional<NumpyArray> yopt, std::optional<const Bounds* const> guessed_bounds,
             std::optional<const Constraints* const> guessed_constraints, std::optional<NumpyArray> R) {
            auto H_ptr = static_cast<double*>(H.request().ptr);
            auto g_ptr = static_cast<double*>(g.request().ptr);
            auto A_ptr = static_cast<double*>(A.request().ptr);
            auto lb_ptr = static_cast<double*>(lb.request().ptr);
            auto ub_ptr = static_cast<double*>(ub.request().ptr);
            auto lbA_ptr = static_cast<double*>(lbA.request().ptr);
            auto ubA_ptr = static_cast<double*>(ubA.request().ptr);
            double* xopt_ptr = nullptr;
            if (xopt)
            {
              xopt_ptr = static_cast<double*>(xopt.value().request().ptr);
            }
            double* yopt_ptr = nullptr;
            if (yopt)
            {
              yopt_ptr = static_cast<double*>(yopt.value().request().ptr);
            }
            double* R_ptr = nullptr;
            if (R)
            {
              R_ptr = static_cast<double*>(R.value().request().ptr);
            }
            return problem.init(H_ptr, g_ptr, A_ptr, lb_ptr, ub_ptr, lbA_ptr, ubA_ptr, nWSR, cputime.value_or(nullptr),
                                xopt_ptr, yopt_ptr, guessed_bounds.value_or(nullptr),
                                guessed_constraints.value_or(nullptr), R_ptr);
          },
          py::arg("H"), py::arg("g"), py::arg("A"), py::arg("lb"), py::arg("ub"), py::arg("lbA"), py::arg("ubA"),
          py::arg("nWSR"), py::arg("cputime"), py::arg("xopt"), py::arg("yopt"), py::arg("guessed_bounds"),
          py::arg("guessed_constraints"), py::arg("R"))
      .def(
          "hotstart",
          [](QProblem& problem, NumpyArray g, NumpyArray lb, NumpyArray ub, NumpyArray lbA, NumpyArray ubA, int nWSR,
             std::optional<double* const> cputime, std::optional<const Bounds* const> guessed_bounds,
             std::optional<const Constraints* const> guessed_constraints) {
            auto g_ptr = static_cast<double*>(g.request().ptr);
            auto lb_ptr = static_cast<double*>(lb.request().ptr);
            auto ub_ptr = static_cast<double*>(ub.request().ptr);
            auto lbA_ptr = static_cast<double*>(lbA.request().ptr);
            auto ubA_ptr = static_cast<double*>(ubA.request().ptr);
            return problem.hotstart(g_ptr, lb_ptr, ub_ptr, lbA_ptr, ubA_ptr, nWSR, cputime.value_or(nullptr),
                                    guessed_bounds.value_or(nullptr), guessed_constraints.value_or(nullptr));
          },
          py::arg("g"), py::arg("lb"), py::arg("ub"), py::arg("lbA"), py::arg("ubA"), py::arg("nWSR"),
          py::arg("cputime"), py::arg("guessed_bounds"), py::arg("guessed_constraints"))
      .def(
          "solveCurrentEQP",
          [](QProblem& problem, int nrhs, NumpyArray g, NumpyArray lb, NumpyArray ub, NumpyArray lbA, NumpyArray ubA) {
            auto g_ptr = static_cast<double*>(g.request().ptr);
            auto lb_ptr = static_cast<double*>(lb.request().ptr);
            auto ub_ptr = static_cast<double*>(ub.request().ptr);
            auto lbA_ptr = static_cast<double*>(lbA.request().ptr);
            auto ubA_ptr = static_cast<double*>(ubA.request().ptr);

            auto x = py::array_t<double>(problem.getNV());
            auto x_ptr = static_cast<double*>(x.request().ptr);
            auto y = py::array_t<double>(problem.getNV());
            auto y_ptr = static_cast<double*>(y.request().ptr);

            auto res = problem.solveCurrentEQP(nrhs, g_ptr, lb_ptr, ub_ptr, lbA_ptr, ubA_ptr, x_ptr, y_ptr);
            return std::make_tuple(res, x, y);
          },
          py::arg("nrhs"), py::arg("g"), py::arg("lb"), py::arg("ub"), py::arg("lbA"), py::arg("ubA"))

      .def("getWorkingSet",
           [](QProblem& problem) {
             auto working_set = py::array_t<double>(problem.getNV());
             auto working_set_ptr = static_cast<double*>(working_set.request().ptr);
             auto res = problem.getWorkingSetBounds(working_set_ptr);
             return std::make_tuple(res, working_set);
           })

      .def("getWorkingSetBounds",
           [](QProblem& problem) {
             auto working_set_bounds = py::array_t<double>(problem.getNV());
             auto working_set_bounds_ptr = static_cast<double*>(working_set_bounds.request().ptr);
             auto res = problem.getWorkingSetBounds(working_set_bounds_ptr);
             return std::make_tuple(res, working_set_bounds);
           })
      .def("getWorkingSetConstraints",
           [](QProblem& problem) {
             auto working_set_constraints = py::array_t<double>(problem.getNC());
             auto working_set_constraints_ptr = static_cast<double*>(working_set_constraints.request().ptr);
             auto res = problem.getWorkingSetConstraints(working_set_constraints_ptr);
             return std::make_tuple(res, working_set_constraints);
           })
      .def("getConstraints", &QProblem::getConstraints)
      .def("getNC", &QProblem::getNC)
      .def("getNEC", &QProblem::getNEC)
      .def("getNAC", &QProblem::getNAC)
      .def("getNIAC", &QProblem::getNIAC)
      .def("getNZ", &QProblem::getNZ)
      .def("getDualSolution",
           [](QProblem& problem) {
             auto n = problem.getNV() + problem.getNC();
             auto dual_solution = py::array_t<double>(n);
             auto dual_solution_ptr = static_cast<double*>(dual_solution.request().ptr);
             auto res = problem.getDualSolution(dual_solution_ptr);
             return std::make_tuple(res, dual_solution);
           })
      .def("printProperties", &QProblem::printProperties)
      .def("getFreeVariablesFlag",
           [](QProblem& problem) {
             auto var_is_free = py::array_t<double>(problem.getNV());
             auto var_is_free_ptr = static_cast<double*>(var_is_free.request().ptr);
             auto res = problem.getWorkingSetConstraints(var_is_free_ptr);
             return std::make_tuple(res, var_is_free);
           })
      // .def("setConstraintProduct", &QProblem::setConstraintProduct)

      ;

  py::class_<SQProblem, QProblem>(m, "SQProblem")
      .def(py::init<>())
      .def(py::init<int, int, HessianType, BooleanType>(), py::arg("nv"), py::arg("nc"),
           py::arg("hessian_type") = HST_UNKNOWN, py::arg("allocate_dense_matrices") = BT_TRUE)
      .def("reset", &SQProblem::reset)
      .def(
          "hotstart",
          [](SQProblem& problem, NumpyArray H, NumpyArray g, NumpyArray A, NumpyArray lb, NumpyArray ub, NumpyArray lbA,
             NumpyArray ubA, int nWSR, std::optional<double* const> cputime,
             std::optional<const Bounds* const> guessed_bounds,
             std::optional<const Constraints* const> guessed_constraints) {
            auto H_ptr = static_cast<double*>(H.request().ptr);
            auto g_ptr = static_cast<double*>(g.request().ptr);
            auto A_ptr = static_cast<double*>(A.request().ptr);
            auto lb_ptr = static_cast<double*>(lb.request().ptr);
            auto ub_ptr = static_cast<double*>(ub.request().ptr);
            auto lbA_ptr = static_cast<double*>(lbA.request().ptr);
            auto ubA_ptr = static_cast<double*>(ubA.request().ptr);
            return problem.hotstart(H_ptr, g_ptr, A_ptr, lb_ptr, ub_ptr, lbA_ptr, ubA_ptr, nWSR,
                                    cputime.value_or(nullptr), guessed_bounds.value_or(nullptr),
                                    guessed_constraints.value_or(nullptr));
          },
          py::arg("H"), py::arg("g"), py::arg("A"), py::arg("lb"), py::arg("ub"), py::arg("lbA"), py::arg("ubA"),
          py::arg("nWSR"), py::arg("cputime"), py::arg("guessed_bounds"), py::arg("guessed_constraints"))

      ;

  py::class_<SQProblemSchur, SQProblem>(m, "SQProblemSchur")
      .def(py::init<>())
      .def(py::init<int, int, HessianType, int>(), py::arg("nv"), py::arg("nc"), py::arg("hessian_type") = HST_UNKNOWN,
           py::arg("max_schur_updates") = 75)
      .def("reset", &SQProblemSchur::reset)
      .def("getNumFactorizations", &SQProblemSchur::getNumFactorizations)
      .def(
          "resetSchurComplement",
          [](SQProblemSchur& problem, bool allow_inertia_correction) {
            problem.resetSchurComplement(toBooleanType(allow_inertia_correction));
          },
          py::arg("allow_inertia_correction"));

  py::enum_<returnValue>(m, "ReturnValue")
      .value("SUCCESSFUL_RETURN", returnValue::SUCCESSFUL_RETURN)

      .export_values();
}