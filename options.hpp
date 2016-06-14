#pragma once

#include <string>

namespace AttenderTest
{
    struct TestOptions
    {
        std::string port;
        unsigned int timeoutSeconds;
        std::string serverExecutable;
    };
}
