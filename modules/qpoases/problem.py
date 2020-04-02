import _qpoases

from qpoases.types import HessianType, ReturnValue


class QProblemB:
    def __init__(self, nv):
        self._impl = _qpoases.QProblemB(nv)

    def reset(self):
        '''Clears all data structures except for QP data.'''
        return ReturnValue(self._impl.reset())

    # def init(self, H, g, lb, ub, nWSR)

    def get_primal_solution(self):
        return self._impl.getPrimalSolution()

    def get_dual_solution(self):
        return self._impl.getDualSolution()


class QProblem(QProblemB):
    def __init__(self, nv, nc, hessian_type=HessianType.UNKNOWN, allocate_dense_matrices=True):
        self._impl = _qpoases.QProblem(
            nv, nc, _qpoases.HessianType(hessian_type.value), _qpoases.BooleanType(allocate_dense_matrices))

    def init(self, H, g, A, lb, ub, lbA, ubA, nwsr, cputime=None, xopt=None, yopt=None, guessed_bounds=None, guessed_constraints=None, R=None):
        '''Initialises a QP problem with given QP data and tries to solve it using at most nWSR iterations.'''
        return ReturnValue(self._impl.init(H, g, A, lb, ub, lbA, ubA, nwsr, cputime, xopt, yopt, guessed_bounds, guessed_constraints, R))

    def hotstart(self, g, lb, ub, lbA, ubA, nWSR, cputime=None, guessed_bounds=None, guessed_constraints=None):
        return self._impl.hotstart(g, lb, ub, lbA, ubA, nWSR, cputime, guessed_bounds, guessed_constraints)


class SQProblem(QProblem):
    def __init__(self, nv, nc, hessian_type=HessianType.UNKNOWN, allocate_dense_matrices=True):
        self._impl = _qpoases.SQProblem(
            nv, nc, _qpoases.HessianType(hessian_type.value), _qpoases.BooleanType(allocate_dense_matrices))

    def hotstart(self, H, g, A, lb, ub, lbA, ubA, nWSR, cputime=None, guessed_bounds=None, guessed_constraints=None):
        return self._impl.hotstart(H, g, A, lb, ub, lbA, ubA, nWSR, cputime, guessed_bounds, guessed_constraints)
