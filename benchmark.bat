echo "Secuencial .5 MB"
FOR /L %%A IN (1,1,5) DO (      
    main.exe test\test_5.txt secuencial
)

echo "Secuencial 1 MB"
FOR /L %%A IN (1,1,5) DO (      
    main.exe test\test_1.txt secuencial
)

echo "Secuencial 2 MB"
FOR /L %%A IN (1,1,5) DO (      
    main.exe test\test_2.txt secuencial
)

echo "Secuencial 4 MB"
FOR /L %%A IN (1,1,5) DO (      
    main.exe test\test_4.txt secuencial
)

echo "Secuencial 8 MB"
FOR /L %%A IN (1,1,5) DO (      
    main.exe test\test_8.txt secuencial
)

echo "Secuencial 16 MB"
FOR /L %%A IN (1,1,5) DO (      
    main.exe test\test_16.txt secuencial
)

echo "Secuencial 32 MB"
FOR /L %%A IN (1,1,5) DO (      
    main.exe test\test_32.txt secuencial
)


pause