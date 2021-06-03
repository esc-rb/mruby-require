#!/usr/bin/env bash

set -eu

echo
echo "Compiling"
echo "= = ="

defaultClean="on"
echo "CLEAN: ${CLEAN:-(not set, default is "on")}"
clean=${CLEAN:-$defaultClean}

if [ $clean = "on" ]; then
  rake deep_clean
fi

rake --multitask
