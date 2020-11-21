echo "debug -t .5 MB"
FOR /L %%A IN (1,1,5) DO (      
    main.exe test\test_5.txt debug -t
)

echo "debug -t 1 MB"
FOR /L %%A IN (1,1,5) DO (      
    main.exe test\test_1.txt debug -t
)

echo "debug -t 2 MB"
FOR /L %%A IN (1,1,5) DO (      
    main.exe test\test_2.txt debug -t
)

echo "debug -t 4 MB"
FOR /L %%A IN (1,1,5) DO (      
    main.exe test\test_4.txt debug -t
)

echo "debug -t 8 MB"
FOR /L %%A IN (1,1,5) DO (      
    main.exe test\test_8.txt debug -t
)

echo "debug -t 16 MB"
FOR /L %%A IN (1,1,5) DO (      
    main.exe test\test_16.txt debug -t
)

echo "debug -t 32 MB"
FOR /L %%A IN (1,1,5) DO (      
    main.exe test\test_32.txt debug -t
)


pause