#!/bin/bash
export HERE=/Users/plewis/librat/docs/source
export BPMS=${BPMS-${HERE%librat*}librat}
export BPMSROOT=$BPMS/obj
mkdir -p $BPMSROOT
export INIT=$BPMS/bin/examples_init.sh
