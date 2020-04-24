import sys,os

class BPMSENV():

  def __init__(self,BPMS=''):
    '''
     get BPMS root (librat) from
     this directory name
    '''
    if BPMS is '':
      if 'BPMS' not in os.environ:
        BPMS='librat'.join(os.getcwd().split('librat')[:-1])+'librat'
      else:
        BPMS=os.environ['BPMS']

    os.environ['BPMS'] = BPMS

    #  relative to BPMS
    SRC=os.sep.join([BPMS,'src'])
    BIN=os.sep.join([BPMS,'bin'])
    LIB=os.sep.join([BPMS,'src'])
    OBJ=os.sep.join([BPMS,'obj'])
    LIB_PYTHON=os.sep.join([BPMS,'lib','python'])
    # paths
    BINS = [SRC,BIN,LIB_PYTHON]
    for bin in BINS:
      sys.path.insert(0,bin)
      os.environ['PATH'] = ':'.join([bin,os.environ['PATH']])

    # for some reason, we cant set LD_LIBRARY_PATH
    # and DYLD_LIBRARY_PATH - maybe it would mess other
    # things up
    # _LD_LIBRARY_PATH
    if 'LD_LIBRARY_PATH' in os.environ:
        os.environ['_LD_LIBRARY_PATH'] = ':'.join([LIB,os.getenv("LD_LIBRARY_PATH")])
    else:
        os.environ['_LD_LIBRARY_PATH'] = LIB

    # _DYLD_LIBRARY_PATH
    if 'DYLD_LIBRARY_PATH' in os.environ:
        os.environ['_DYLD_LIBRARY_PATH'] = ':'.join([LIB,os.getenv("DYLD_LIBRARY_PATH")])
    else:
        os.environ['_DYLD_LIBRARY_PATH'] = LIB

    # set env variables
    os.environ['MATLIB'] = OBJ
    os.environ['RSRLIB'] = OBJ
    os.environ['ARARAT_OBJECT'] = OBJ
    os.environ['BPMS_FILES'] = OBJ
    os.environ['DIRECT_ILLUMINATION'] = OBJ
    os.environ['SKY_ILLUMINATION'] = OBJ

BPMSENV();
