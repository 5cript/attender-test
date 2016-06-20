#pragma once

#include "xml_bindings.hpp"
#include "results.hpp"
#include "options.hpp"

#include <boost/asio/ip/tcp.hpp>
#include <boost/process.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>

#include <vector>
#include <memory>

namespace AttenderTest
{
    class RestTester
    {
    public:
        RestTester(std::string name, TestFile::Test test, TestOptions options);
        TestResult runTest();
        std::string getName() const;
        bool cleanup() const;

    private:
        std::string compileRequest() const;
        TestResult checkResponse(int statusCode,
                                 std::string const& protocolVersion,
                                 std::string const& statusMessage,
                                 std::vector <std::string> const& header,
                                 std::string const& body) const;

        std::string runServer();

    private:
        std::string name_;
        TestFile::Test test_;
        TestOptions options_;
        std::unique_ptr <boost::process::child> server_;
        boost::iostreams::file_descriptor_sink serverStdIn_;
    };
}
