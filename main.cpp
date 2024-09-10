


#include <iostream>
#include <cstddef>
#include "ObjectMemoryPool.h"




struct Example1 {
    unsigned char a;   // 1 byte
    unsigned int b;    // 4 bytes
    double c; // 8 bytes
};

int main()
{

    ObjectPoolNode<Example1> pList;

    printf("%d", sizeof(pList));

    


	return 0;
}


