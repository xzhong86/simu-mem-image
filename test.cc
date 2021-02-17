
#include "MemImage.h"

#include <iostream>

using namespace NS_MemImage;

int main()
{
    MemImage *mim = createMemImage("simple");

    unsigned long addr = 0x80000000;

    long value = 12345;
    mim->writeMem(addr+16, &value, sizeof(value));

    value = 54321;
    mim->writeMem(addr/2+16, &value, sizeof(value));

    long result;
    mim->readMem(addr+16, &result, sizeof(result));
    std::cout << "result: " << result << std::endl;

    mim->readMem(addr/2+16, &result, sizeof(result));
    std::cout << "result: " << result << std::endl;
}

// g++ -std=c++11 *.cc && ./a.out

