#!/usr/bin/env bash

if [ ! $1 ] || [ ! $2 ]; then
  echo "Usage: ./test.sh [numberOfTests] [numberOfBytesPerOutput]"
  exit 1
fi

for (( i = 0; i < $1; i++ ))
do
  ./backdoor ./tests/key$i ./tests/output$i $2
  dieharder -a -f ./tests/output$i > ./tests/stats_output$i
done

python3 averageTime.py
