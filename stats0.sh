#!/usr/bin/env bash

if [ ! $1 ]; then
  echo "Usage: ./stats.sh [size] [numberOfFiles] (format: outputX, place: /tests)"
  exit 1
fi

for (( i = 0; i < $2; i+=4 ))
do
  echo "Starting FIPS test$i"
  ./FIPS tests/output$i $1 > tests/stats_FIPS_output$i
done
wait

for (( i = 0; i < $2; i+=4 ))
do
  echo "Starting rabbit test$i"
  ./rabbit tests/output$i $1 > tests/stats_rabbit_output$i
done
wait

for (( i = 0; i < $2; i+=4 ))
do
  echo "Starting dieharder test$i"
  dieharder -a -f tests/output$i > tests/stats_dieharder_output$i
done
wait
