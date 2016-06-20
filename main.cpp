#include <iostream>
#include <map>

#include "controller.hpp"

int main(int argc, char* argv[])
{
    using namespace AttenderTest;

    std::map <std::string, TestResult> testResults;

    Controller controller("tests", {"10101", 5, "D:/Development/attender/attender/bin/Debug/attender.exe"});
    for (auto& test : controller)
    {
        std::cout << test.getName() << "...\n";
        auto result = test.runTest();
        result.killed = test.cleanup();
        printResult(result) << "\n";

        testResults[test.getName()] = result;
    }

    return 0;
}
