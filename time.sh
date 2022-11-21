#!/bin/bash

DIR="testdata"
EXT="_out.txt"
EMPTY="empty.txt"

make

for file in $DIR/* ; do
	output=$(basename "$file" .txt)$EXT
  echo "" > "$EMPTY"
  echo "Ejecutando correcion sobre $file"
  time ./main "$file" "$output" "$EMPTY"
  echo "---------------------------------"
done

make clean
