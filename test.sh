#!/usr/bin/env bash

if [ ! $1 ] || [ ! $2 ]; then
  echo "Usage: ./test.sh [numberOfTests] [numberOfBytesPerOutput]"
  exit 1
fi

for (( i = 0; i < $1; i++ ))
do
  echo "Generating output$i..."
  ./backdoor ./tests/key$i ./tests/output$i $2
  echo "output$i generated, begining statistical tests n°$i..."
  dieharder -a -f ./tests/output$i > ./tests/stats_output$i
  echo "Statistical tests n°$i done!"
  echo ""
done

python3 averageTime.py
