import numpy as np

# from _qpoases import SQProblem
from qpoases import SQProblem

H = np.array([1.0, 0.0, 0.0, 0.5]).reshape((2, 2))
A = np.array([1.0, 1.0]).reshape((2, 1))
g = np.array([1.5, 1.0])
lb = np.array([0.5, -2.0])
ub = np.array([5.0, 2.0])
lbA = np.array([-1.0])
ubA = np.array([2.0])

example = SQProblem(2, 1)
ret = example.init(H, g, A, lb, ub, lbA, ubA, 10)

print(example.get_primal_solution())

g_new   = np.array([1.0, 1.5])
lb_new  = np.array([0.0, -1.0])
ub_new  = np.array([5.0, -0.5])
lbA_new = np.array([-2.0])
ubA_new = np.array([1.0])

ret = example.hotstart(H, g_new, A, lb_new, ub_new, lbA_new, ubA_new, 10)
print(example.get_primal_solution())




