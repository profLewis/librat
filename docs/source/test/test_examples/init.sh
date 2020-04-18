#!/bin/bash
#
# preamble 
#
BPMS=${BPMS-$(pwd)}
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
export TEST=${BPMS}/test/test_example
export MATLIB=$TEST
export RSRLIB=$TEST
export ARARAT_OBJECT=$TEST
export DIRECT_ILLUMINATION=$TEST
export BPMS_FILES=$TEST

if [ "$(which start)" == "${bin}/start" ]
then
  if [ "$VERBOSE" ]; then
      echo "start found ok"
  fi
else
  # we should create them
  make clean all 
fi
