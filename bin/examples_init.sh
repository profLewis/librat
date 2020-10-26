#!/bin/bash
#
# defaults
#
export ARCH=$(uname -m)
export BPMS=${BPMS-/Users/plewis/Documents/GitHub/librat}
export BPMSROOT=${BPMSROOT-$BPMS/obj}
lib=${lib-$BPMS/lib/$ARCH}
bin=${bin-$BPMS/bin/$ARCH}
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
export SKY_ILLUMINATION==$BPMSROOT


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
