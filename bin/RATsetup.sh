#!/bin/bash
#
# defaults
#
# if BPMS set

HERE=$(pwd)
ARCH=$(uname -m)

if [ -z "$BPMS" ]
then
  export THERE=$(dirname $0)
else
  export THERE=$BPMS/bin
fi
cd ${THERE}
THERE=$(pwd)
export PYTHONPATH="$THERE:$PYTHONPATH"
_BPMS=`python -c "from RATsetup import *;print(os.environ['BPMS'])"`
export BPMS=${BPMS-$_BPMS}

#echo "BPMS: $BPMS"

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

d=\$
cat << EOF > ~/.bpms_profile

export BPMS="$BPMS"
export PATH="${PATH}:${BPMS}/bin:${BPMS}/bin/${ARCH}:${d}PATH"
if [ -z "$LD_LIBRARY_PATH" ] ; then
  export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}"
else
  export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${d}LD_LIBRARY_PATH"
fi
if [ -z "DYLD_LIBRARY_PATH" ] ; then
  export DYLD_LIBRARY_PATH="${DYLD_LIBRARY_PATH}"
else
  export DYLD_LIBRARY_PATH="${DYLD_LIBRARY_PATH}:${d}DYLD_LIBRARY_PATH"
fi

export TEST="$TEST"
export TEMP="$TEMP"

export MATLIB="$MATLIB"
export RSRLIB="$RSRLIB"
export ARARAT_OBJECT="$ARARAT_OBJECT"
export BPMS_FILES="$BPMS_FILES"
export DIRECT_ILLUMINATION="$DIRECT_ILLUMINATION"
export SKY_ILLUMINATION="$SKY_ILLUMINATION"

if [ -z "PYTHONPATH" ] ; then
  export PYTHONPATH="${PYTHONPATH}"
else
  export PYTHONPATH="${PYTHONPATH}:${d}PYTHONPATH"
fi

EOF

if [ -z "$SHELL" ] ; then
  SHELL=/bin/bash
fi
profile=~/.$(echo $SHELL| awk -F/ '{print $NF}')rc
touch $profile
grep -v < $profile bpms_profile > $TEMP/$$
mv $TEMP/$$ $profile 
echo "source ~/.bpms_profile" >> $profile

# and bash as we will use that
profile=~/.bashrc
touch $profile
grep -v < $profile bpms_profile > $TEMP/$$
mv $TEMP/$$ $profile
echo "source ~/.bpms_profile" >> $profile


cd "${HERE}"
