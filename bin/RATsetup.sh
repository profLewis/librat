#!/bin/bash
#
# defaults
#
# if BPMS set

if [ -z "$BPMS" ]
then
  export HERE=$(dirname $0)
else
  export HERE=$BPMS/bin
fi

export PYTHONPATH="$HERE:$PYTHONPATH"
_BPMS=`python -c "from RATsetup import *;print(os.environ['BPMS'])"`
export BPMS=${BPMS-$_BPMS}

_MATLIB=`python -c 'from RATsetup import *;print(os.environ["MATLIB"])'`
_RSRLIB=`python -c "from RATsetup import *;print(os.environ['RSRLIB'])"`
_ARARAT_OBJECT=`python -c "from RATsetup import *;print(os.environ['ARARAT_OBJECT'])"`
_BPMS_FILES=`python -c "from RATsetup import *;print(os.environ['BPMS_FILES'])"`
_DIRECT_ILLUMINATION=`python -c "from RATsetup import *;print(os.environ['DIRECT_ILLUMINATION'])"`
_SKY_ILLUMINATION=`python -c "from RATsetup import *;print(os.environ['SKY_ILLUMINATION'])"`

export MATLIB=${MATLIB-$_MATLIB}
export RSRLIB=${RSRLIB-$_RSRLIB}
export ARARAT_OBJECT=${ARARAT_OBJECT-$_ARARAT_OBJECT}
export BPMS_FILES=${BPMS_FILES-$_BPMS_FILES}
export DIRECT_ILLUMINATION=${DIRECT_ILLUMINATION-$_DIRECT_ILLUMINATION}
export SKY_ILLUMINATION=${SKY_ILLUMINATION-$_SKY_ILLUMINATION}


export TEST=${TEST-0}
export TEMP=${TEMP-/tmp}

export PATH=`python -c "from RATsetup import *;print(os.environ['PATH'])"`
export LD_LIBRARY_PATH=`python -c "from RATsetup import *;print(os.environ['_LD_LIBRARY_PATH'])"`
export DYLD_LIBRARY_PATH=`python -c "from RATsetup import *;print(os.environ['_DYLD_LIBRARY_PATH'])"`

if [ "$TEST" == 1 ]; then
  echo 0 | RATstart test.obj
fi
