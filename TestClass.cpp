#include <iostream>
#include "TestClass.h"

TestClass::TestClass()
{
    std::cout << "test class" << std::endl;
}
TestClass::TestClass(const char* s)
{
    std::cout << "test class: " << s << std::endl;
}

TestClass::~TestClass()
{
    
}