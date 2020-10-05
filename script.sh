#!/bin/bash

echo "Tamaño del archivo"
ls -lh test.txt

echo "Secuencial"
./a.out test.txt secuencial

echo "Concurrente"
./a.out test.txt