#pragma once

#include <string>

namespace AttenderTest
{
    struct TestResult
    {
        bool success;
        bool responseReceived;
        std::string message;
        std::string fullResponse;
    };

    std::ostream& printResult(TestResult const& result);
}
