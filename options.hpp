#pragma once

#include <string>

namespace AttenderTest
{
    struct TestOptions
    {
        std::string port = "10101";
        unsigned int timeoutSeconds = 10;
        std::string serverExecutable = "";
    };
}
