#!/bin/sh -e
./source.sh

if [ $# -ne 1 ]; then
    echo "Usage: ./run_me.sh <sw_emu/hw_emu/hw>"
    exit 1
fi

cd scripts
    chmod +x *.sh
    ./exe.sh

    ./build.sh $1
    ./run.sh $1 ../data/DB.csv ../data/R_peak.csv
cd ..
