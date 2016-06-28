#include "program_options.hpp"

#include <boost/program_options.hpp>

#include <iostream>

namespace AttenderTest
{
//#####################################################################################################################
    boost::optional <ProgramOptions> loadProgramOptions(int argc, char* argv[])
    {
        namespace po = boost::program_options;

        ProgramOptions options;

        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("testsFolder", po::value <std::string>(), "set the folder where all tests are located")
            ("mode", po::value <int>(), "set the test mode")
            ("testFile", po::value <std::string>(), "set the specific test file. Only has effect if mode is set to 2 (single)")
        ;

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help"))
        {
            std::cout << desc << "\n";
            return boost::none;
        }
        if (vm.count("testsFolder"))
            options.testsFolder = vm["testsFolder"].as <std::string>();

        if (vm.count("mode"))
            options.mode = static_cast <TestMode> (vm["mode"].as <int>());

        if (vm.count("testFile"))
            options.testFile = vm["testFile"].as <std::string>();

        return options;
    }
//---------------------------------------------------------------------------------------------------------------------
//#####################################################################################################################
}
