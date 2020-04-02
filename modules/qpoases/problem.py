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

    def init(self, H, g, A, lb, ub, lbA, ubA, nwsr, cputime=None, xopt=None, yopt=None, guesses_bounds=None, guessed_constraints=None, R=None):
        '''Initialises a QP problem with given QP data and tries to solve it using at most nWSR iterations.'''
        return ReturnValue(self._impl.init(H, g, A, lb, ub, lbA, ubA, nwsr, cputime, xopt, yopt))

    def hotstart(self, g, lb, ub, lbA, ubA, nWSR):
        return self._impl.hotstart(g, lb, ub, lbA, ubA, nWSR)


class SQProblem(QProblem):
    def __init__(self, nv, nc, hessian_type=HessianType.UNKNOWN, allocate_dense_matrices=True):
        self._impl = _qpoases.SQProblem(
            nv, nc, _qpoases.HessianType(hessian_type.value), _qpoases.BooleanType(allocate_dense_matrices))

    def init(self, H, g, A, lb, ub, lbA, ubA, nWSR):
        return self._impl.init(H, g, A, lb, ub, lbA, ubA, nWSR)

    def hotstart(self, H, g, A, lb, ub, lbA, ubA, nWSR):
        return self._impl.hotstart(H, g, A, lb, ub, lbA, ubA, nWSR)
