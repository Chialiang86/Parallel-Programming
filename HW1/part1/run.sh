# bin/sh

VECTOR_WIDTH=(2 4 8 16)

for width in "${VECTOR_WIDTH[@]}"
do
    echo "========================================================"
    echo "// Define vector unit width here
    #define VECTOR_WIDTH $width
    #define EXP_MAX 10" > def.h 
    make
    ./myexp -s 10000
    make clean
done

for (( i=0; i < 10; i++ ))
do
    echo $i
    ./test_auto_vectorize -t 1
done

8.33462
8.35775
8.38798
8.37651
8.35002
8.39909
8.3614
8.37502
8.33932
8.31084

0
Running test1()...
Elapsed execution time of the loop in test1():
2.63355sec (N: 1024, I: 20000000)
1
Running test1()...
Elapsed execution time of the loop in test1():
2.63899sec (N: 1024, I: 20000000)
2
Running test1()...
Elapsed execution time of the loop in test1():
2.67191sec (N: 1024, I: 20000000)
3
Running test1()...
Elapsed execution time of the loop in test1():
2.66897sec (N: 1024, I: 20000000)
4
Running test1()...
Elapsed execution time of the loop in test1():
2.64094sec (N: 1024, I: 20000000)
5
Running test1()...
Elapsed execution time of the loop in test1():
2.6921sec (N: 1024, I: 20000000)
6
Running test1()...
Elapsed execution time of the loop in test1():
2.65665sec (N: 1024, I: 20000000)
7
Running test1()...
Elapsed execution time of the loop in test1():
2.65208sec (N: 1024, I: 20000000)
8
Running test1()...
Elapsed execution time of the loop in test1():
2.65227sec (N: 1024, I: 20000000)
9
Running test1()...
Elapsed execution time of the loop in test1():
2.6419sec (N: 1024, I: 20000000)
