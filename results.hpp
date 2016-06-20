#pragma once

#include <string>

namespace AttenderTest
{
    struct TestResult
    {
        bool success;
        bool responseReceived;
        bool killed;
        std::string message;
        std::string fullResponse;
    };

    std::ostream& printResult(TestResult const& result);
}
