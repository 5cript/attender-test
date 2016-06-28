#pragma once

#include <boost/optional.hpp>

#include <string>

namespace AttenderTest
{
    enum class TestMode
    {
        None = 0,
        All = 1,
        Single = 2
    };

    struct ProgramOptions
    {
        std::string testsFolder = "tests";
        TestMode mode = TestMode::All;
        std::string testFile = "";
    };

    boost::optional <ProgramOptions> loadProgramOptions(int argc, char* argv[]);
}
