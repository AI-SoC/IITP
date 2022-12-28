#!/bin/sh -e

cd ../run
make build TARGET=$1
cd ..
