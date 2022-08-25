#!/bin/bash

DIR="testdata"
EXT="_out.txt"

make

for file in $DIR/* ; do
	output=$(basename "$file" .txt)$EXT
  echo "Ejecutando correcion sobre $file"
  time ./main "$file" "$output"
  echo "---------------------------------"
done

make clean
