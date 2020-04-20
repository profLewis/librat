#!/bin/bash
#
# defaults
#
export BPMS=${BPMS-/Users/plewis/librat}
export BPMSROOT=${BPMSROOT-/Users/plewis/librat/test/test_examples}
lib=${lib-"$BPMS/src"}
bin=${bin-"$BPMS/src"}
VERBOSE=${VERBOSE-0}
export TEMP=${TEMP-/tmp}

# set up required environment variables for bash
export LD_LIBRARY_PATH="${lib}:${LD_LIBRARY_PATH}"
export DYLD_LIBRARY_PATH="${lib}:${DYLD_LIBRARY_PATH}"
export PATH="${bin}:${PATH}"

export MATLIB=$BPMSROOT
export RSRLIB=$BPMSROOT
export ARARAT_OBJECT=$BPMSROOT
export DIRECT_ILLUMINATION=$BPMSROOT
export BPMS_FILES=$BPMSROOT
if [ "$(which RATstart)" == "${bin}/RATstart" ]
then
  if [ "$VERBOSE" == 1 ]; then
      echo "RATstart found ok"
  fi
else
  # we should create them
  make clean all 
fi
