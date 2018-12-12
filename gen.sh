#!/usr/bin/env bash

if [ ! $1 ] || [ ! $2 ]; then
  echo "Usage: ./gen.sh [numberOfTests] [numberOfBytesPerOutput]"
  exit 1
fi

for (( i = 0; i < $1; i++ ))
do
  echo "Generating output$i..."
  ./backdoor -c ./tests/key$i ./tests/output$i $2
  echo "output$i generated"
  echo ""
done

python3 averageTime.py
