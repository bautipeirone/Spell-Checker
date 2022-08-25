#!/bin/bash

DIR="testdata"
EXT="_out.txt"

make

for file in $DIR/* ; do
	output=$(basename "$file" .txt)$EXT
  echo time ./main "$file" "$output"
done

make clean
