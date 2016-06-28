#include "program_options.hpp"
#include "controller.hpp"

#include <boost/filesystem.hpp>

#include <iostream>
#include <map>

template <typename T>
AttenderTest::TestResult runTest(T& test);

int main(int argc, char* argv[])
{
    using namespace AttenderTest;

    auto maybeOptions = loadProgramOptions(argc, argv);
    if (!maybeOptions)
        return 0;

    auto options = maybeOptions.get();

    boost::filesystem::create_directory("outputs");

    auto path = boost::filesystem::system_complete(argv[0]).remove_filename();
    path = boost::filesystem::canonical(path / ".." / ".." / ".." / "attender" / "bin" / "Debug" / "attender.exe").make_preferred();

    std::cout << "exepath: " << path.string() << "\n";

    Controller controller(options.testsFolder, {"10101", 3, path.string()});
    if (options.mode == TestMode::All)
    {
        std::map <std::string, TestResult> testResults;
        for (auto& test : controller)
        {
            testResults[test.getName()] = runTest(test);
        }
    }
    else if (options.mode == TestMode::Single)
    {
        for (auto& test : controller)
        {
            if (test.getName() == options.testFile)
            {
                runTest(test);
                break;
            }
        }
    }

    return 0;
}

template <typename T>
AttenderTest::TestResult runTest(T& test)
{
    std::cout << test.getName() << "...\n";
    auto result = test.runTest();
    result.killed = test.cleanup();
    printResult(result) << "\n";
    return result;
}
