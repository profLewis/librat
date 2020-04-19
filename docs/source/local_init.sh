#!/bin/bash
export HERE=/Users/plewis/librat/docs/source
export BPMS=${BPMS-${HERE%librat*}librat}
export BPMSROOT=$BPMS/test/test_examples
mkdir -p $BPMSROOT
export INIT=$BPMSROOT/examples_init.sh
