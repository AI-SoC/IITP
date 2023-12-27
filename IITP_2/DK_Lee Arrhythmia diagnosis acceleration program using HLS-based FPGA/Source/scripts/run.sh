#!/bin/sh -e

cd ../run
make run TARGET=$1
cd ..
