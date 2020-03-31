try:
    from _qpoases._qpoases import *
except ImportError as ex:
    print(ex)
    error_msg = 'Failed to import the qpOASES C-module'
    raise ImportError(error_msg) from ex