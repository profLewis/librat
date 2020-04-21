import sys,os
BPMS='librat'.join(os.getcwd().split('librat')[:-1])+'librat'
NOTEBOOK=BPMS+os.sep+'notebooks'
OBJ=BPMS+os.sep+'obj'
LIB=BPMS+os.sep+'src'
BIN=BPMS+os.sep+'src'
sys.path.insert(0,os.sep.join([BPMS,'lib','python']))
sys.path.insert(0,NOTEBOOK)
sys.path.insert(0,BIN)
PATH_=os.getenv('PATH')
LD_LIBRARY_PATH_=os.getenv("LD_LIBRARY_PATH")
DYLD_LIBRARY_PATH_=os.getenv("DYLD_LIBRARY_PATH")
