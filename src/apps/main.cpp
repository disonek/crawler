#include <iostream>
#include "Hello.hpp"

int main()
{
    intnamespace::Hello hello;
    std::cout << "Ala ma " << hello.getValue() << " kotow" << std::endl;
}