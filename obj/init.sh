#!/bin/bash
#
# preamble 
#
export BPMS=/Users/plewis/librat

# set shell variables lib, bin, verbose
# with defaults in case not set 
lib=${lib-"$BPMS/src"}
bin=${bin-"$BPMS/src"}
VERBOSE=${VERBOSE-1}

# set up required environment variables for bash
export LD_LIBRARY_PATH="${lib}:${LD_LIBRARY_PATH}"
export DYLD_LIBRARY_PATH="${lib}:${DYLD_LIBRARY_PATH}"
export PATH="${bin}:${PATH}"

# set up required environment variables for librat
export BPMSROOT=${BPMSROOT-"$BPMS/test/test_examples"}
export MATLIB=$BPMSROOT
export RSRLIB=$BPMSROOT
export ARARAT_OBJECT=$BPMSROOT
export DIRECT_ILLUMINATION=$BPMSROOT
export BPMS_FILES=$BPMSROOT
export SKY_ILLUMINATION==$BPMSROOT
echo /Users/plewis/librat
if [ "$(which RATstart)" == "${bin}/RATstart" ]
then
  if [ "$VERBOSE" == 1 ]; then
      echo "RATstart found ok"
  fi
else
  # we should create them
  make clean all 
fi
