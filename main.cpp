#include <iostream>

#include "controller.hpp"

int main(int argc, char* argv[])
{
    using namespace AttenderTest;

    Controller controller("tests", {"10101", 60, "D:/Development/attender/attender/bin/Debug/attender.exe"});
    for (auto& test : controller)
    {
        std::cout << test.getName() << "...\n";
        auto result = test.runTest();
        printResult(result) << "\n";
    }

    return 0;
}
