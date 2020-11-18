#!/bin/bash

echo "Concurrente 0.5 MB"
for i in {1..5}
do
    ./a.out test/test_5.txt
done

echo "Secuencial 0.5 MB"
for i in {1..5}
do    
    ./a.out test/test_5.txt secuencial
done

echo "Concurrente 1 MB"
for i in {1..5}
do
    ./a.out test/test_1.txt
done

echo "Secuencial 1 MB"
for i in {1..5}
do    
    ./a.out test/test_1.txt secuencial
done

echo "Concurrente 2 MB"
for i in {1..5}
do
    ./a.out test/test_2.txt
done

echo "Secuencial 2 MB"
for i in {1..5}
do    
    ./a.out test/test_2.txt secuencial
done

echo "Concurrente 4 MB"
for i in {1..5}
do
    ./a.out test/test_4.txt
done

echo "Secuencial 4 MB"
for i in {1..5}
do    
    ./a.out test/test_4.txt secuencial
done

echo "Concurrente 8 MB"
for i in {1..5}
do
    ./a.out test/test_8.txt
done

echo "Secuencial 8 MB"
for i in {1..5}
do    
    ./a.out test/test_8.txt secuencial
done

echo "Concurrente 16 MB"
for i in {1..5}
do
    ./a.out test/test_16.txt
done

echo "Secuencial 16 MB"
for i in {1..5}
do    
    ./a.out test/test_16.txt secuencial
done

echo "Concurrente 32 MB"
for i in {1..5}
do
    ./a.out test/test_32.txt
done

echo "Secuencial 32 MB"
for i in {1..5}
do    
    ./a.out test/test_32.txt secuencial
done


