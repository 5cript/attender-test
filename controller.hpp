#pragma once

#include "options.hpp"
#include "test.hpp"
#include "xml_bindings.hpp"

#include <vector>
#include <string>

namespace AttenderTest
{
    class Controller
    {
    public:
        using iterator_type = std::vector <RestTester>::iterator;

    public:
        Controller(std::string const& testDirectory, TestOptions const& options);

        iterator_type begin();
        iterator_type end();

    private:
        std::vector <RestTester> tests;
    };

}
