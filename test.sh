#!/usr/bin/env bash

if [ ! $1 ] || [ ! $2 ]; then
  echo "Usage: ./test.sh [numberOfTests] [numberOfBytesPerOutput]"
  exit 1
fi

START=1
while [[ $i -le $1 ]]
do
  ./backdoor ./tests/key$i ./tests/output$i $2
  ((i = i + 1))
done

python3 averageTime.py
