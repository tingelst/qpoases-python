import _qpoases

from qpoases.types import HessianType


class SQProblem:
    def __init__(self, nv, nc, hessian_type=HessianType.UNKNOWN, allocate_dense_matrices=True):
        self.__impl = _qpoases.SQProblem(
            nv, nc, _qpoases.HessianType(hessian_type.value), _qpoases.BooleanType(allocate_dense_matrices))

    def init(self, H, g, A, lb, ub, lbA, ubA, nWSR):
        return self.__impl.init(H, g, A, lb, ub, lbA, ubA, nWSR)

    def hotstart(self, H, g, A, lb, ub, lbA, ubA, nWSR):
        return self.__impl.hotstart(H, g, A, lb, ub, lbA, ubA, nWSR)
