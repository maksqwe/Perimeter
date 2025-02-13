'''
>>> from bienstman3_ext import *

>>> try:
...    V()
... except RuntimeError, x:
...    print x
... else:
...    print 'expected an exception'
...
This class cannot be instantiated from Python

'''
def run(args = None):
    import sys
    import doctest

    if args is not None:
        sys.argv = args
    return doctest.testmod(sys.modules.get(__name__))
    
if __name__ == '__main__':
    print "running..."
    import sys
    status = run()[0]
    if (status == 0): print "Done."
    sys.exit(status)
