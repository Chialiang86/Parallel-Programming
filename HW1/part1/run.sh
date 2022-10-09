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
