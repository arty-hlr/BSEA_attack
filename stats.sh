#!/usr/bin/env bash

if [ ! $1 ]; then
  echo "Usage: ./stats.sh [numberOfFiles] (format: outputX, place: /tests)"
  exit 1
fi

for (( i = 0; i < $1; i++ ))
do
  dieharder -a -f ./tests/output$i > stats_output$i
done
