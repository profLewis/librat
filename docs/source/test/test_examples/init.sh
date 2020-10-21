#!/bin/bash
#
# preamble 
#
ARCH=$(uname -m)
BPMS=${BPMS-$(pwd)}
# set shell variables lib, bin, verbose
# with defaults in case not set 
lib=${lib-"$BPMS/lib/$ARCH"}
bin=${bin-"$BPMS/bin/$ARCH"}
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


var=$(diff "$(which RATstart)" "${bin}/RATstart")
#store diff exit code
exit_code=$?
# remember that this is not the same as count of differences
lines_output_by_diff=$(wc -l <<< "")

echo "$var"

if (($exit_code == 0)); then
  if [ "$VERBOSE" == "1" ] ; then 
    echo "RATstart found ok"
  fi
else
  cd /Users/plewis/Documents/GitHub/librat; make clean all test install 
fi
