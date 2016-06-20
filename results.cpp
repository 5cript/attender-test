#include "results.hpp"

#include <iostream>

namespace AttenderTest
{
//#####################################################################################################################
    std::ostream& printResult(TestResult const& result)
    {
        if (result.success)
            std::cout << "Succeeded\n";
        else
            std::cout << "Failed\n";
        std::cout << result.message << "\n";

        if (result.responseReceived)
            std::cout << "Response was:\n";
        std::cout << result.fullResponse << "\n";

        if (result.killed)
            std::cout << "Process had to be killed\n";

        return std::cout;
    }
//---------------------------------------------------------------------------------------------------------------------
//#####################################################################################################################
}
