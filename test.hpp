#pragma once

#include "xml_bindings.hpp"
#include "results.hpp"
#include "options.hpp"

#include <boost/process.hpp>

#include <vector>

namespace AttenderTest
{
    class RestTester
    {
    public:
        RestTester(std::string name, TestFile::Test test, TestOptions options);

        TestResult runTest() const;

        std::string getName() const;

        void cleanup() const;

    private:
        std::string compileRequest() const;
        TestResult checkResponse(int statusCode,
                                 std::string const& protocolVersion,
                                 std::string const& statusMessage,
                                 std::vector <std::string> const& header,
                                 std::string const& body) const;

        std::string runServer() const;

    private:
        std::string name_;
        TestFile::Test test_;
        TestOptions options_;
        boost::process::child server_;
    };
}
