from enum import Enum


class BooleanType(Enum):
    FALSE = 0
    TRUE = 1


class HessianType(Enum):
    ZERO = 0
    IDENTITY = 1
    POSDEF = 2
    POSDEF_NULLSPACE = 3
    SEMIDEF = 4
    INDEF = 5
    UNKNOWN = 6
