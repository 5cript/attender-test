#include "controller.hpp"

#include <boost/filesystem.hpp>

namespace AttenderTest
{
//#####################################################################################################################
    Controller::Controller(std::string const& testDirectory, TestOptions const& options)
        : tests{}
    {
        namespace fs = boost::filesystem;

        fs::recursive_directory_iterator i {testDirectory}, end;
        for (; i != end; ++i)
        {
            auto p = i->path();
            // loadTestFromStream
            if (fs::is_regular_file(p))
            {
                std::ifstream reader(p.string(), std::ios_base::binary);
                tests.emplace_back(p.filename().string(), loadTestFromStream(reader), options);
            }
        }
    }
//---------------------------------------------------------------------------------------------------------------------
    Controller::iterator_type Controller::begin()
    {
        return std::begin(tests);
    }
//---------------------------------------------------------------------------------------------------------------------
    Controller::iterator_type Controller::end()
    {
        return std::end(tests);
    }
//---------------------------------------------------------------------------------------------------------------------
//#####################################################################################################################
}
