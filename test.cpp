#include "test.hpp"

#include <fstream>
#include <sstream>
#include <boost/asio/ip/tcp.hpp>
#include <boost/algorithm/string.hpp>

namespace AttenderTest
{
//#####################################################################################################################
    RestTester::RestTester(std::string name, TestFile::Test test, TestOptions options)
        : name_(std::move(name))
        , test_(std::move(test))
        , options_(std::move(options))
    {
    }
//---------------------------------------------------------------------------------------------------------------------
    std::string RestTester::getName() const
    {
        return name_;
    }
//---------------------------------------------------------------------------------------------------------------------
    std::string RestTester::compileRequest() const
    {
        std::stringstream stream;
        if (test_.request.method)
            stream << test_.request.method.get();
        else
            stream << "GET";
        stream << ' ';

        if (test_.request.url)
            stream << test_.request.url.get();
        else
            stream << "/";
        stream << ' ';

        stream << test_.request.protocol;
        stream << "/";
        stream << test_.request.version;

        stream << "\r\n";

        if (test_.request.header)
        {
            auto header = test_.request.header.get();

            for (auto const& i : header.entry)
            {
                stream << static_cast <std::string> (i.key) << ": " << static_cast <std::string> (i.value) << "\r\n";
            }
        }
        stream << "\r\n";

        if (test_.request.body)
            stream << test_.request.body.get();

        return stream.str();
    }
//---------------------------------------------------------------------------------------------------------------------
    std::string RestTester::runServer() const
    {
        using namespace boost::process;

        std::vector<std::string> args;
        args.push_back(options_.serverExecutable);
        args.push_back("--port=10101");

        server_ = execute(
            set_args(args),
            throw_on_error()
        );
    }
//---------------------------------------------------------------------------------------------------------------------
    TestResult RestTester::runTest() const
    {
        using namespace std::string_literals;

        #define FAIL(MESSAGE) \
            TestResult{false, false, MESSAGE, ""}

        try
        {
            runServer();

            using boost::asio::ip::tcp;
            boost::asio::ip::tcp::iostream stream;

            stream.expires_from_now(boost::posix_time::seconds(options_.timeoutSeconds));
            stream.connect("localhost", options_.port);
            if (!stream)
                return FAIL(stream.error().message());

            stream << compileRequest();

            std::string httpVersion;
            stream >> httpVersion;
            unsigned int statusCode;
            stream >> statusCode;
            std::string statusMessage;
            std::getline(stream, statusMessage);
            if (!stream)
            {
                return FAIL("stream read error: "s + stream.error().message());
            }
            if (httpVersion.length() < 5 || httpVersion.substr(0, 5) != test_.request.protocol + "/")
            {
                return FAIL("protocol mismatch: "s + httpVersion);
            }
            if (httpVersion.length() == 8)
            {
                auto version = httpVersion.substr(5, 3);
                if (version != "1.0" && version != "1.1" && version != "2.0")
                    return FAIL("unexpected version: "s + version);
            }
            else
            {
                return FAIL("http&version string is too long: "s + httpVersion);
            }

            std::string headerLine;
            std::vector <std::string> headerLines;
            while (std::getline(stream, headerLine) && headerLine != "\r")
                headerLines.push_back(headerLine);

            std::stringstream body;
            body << stream.rdbuf();

            return checkResponse(statusCode, httpVersion, statusMessage, headerLines, body.str());
        }
        catch(std::exception const& exc)
        {
            return FAIL(exc.what());
        }

        #undef FAIL
    }
//---------------------------------------------------------------------------------------------------------------------
    void cleanup() const
    {
        using namespace boost::process;

        auto pipe = create_pipe();

    }
//---------------------------------------------------------------------------------------------------------------------
    TestResult RestTester::checkResponse(int statusCode,
                                         std::string const& protocolVersion,
                                         std::string const& statusMessage,
                                         std::vector <std::string> const& header,
                                         std::string const& body) const
    {
        using namespace std::string_literals;
        auto compileResult = [&]()
        {
            std::stringstream sstr;
            sstr << protocolVersion << ' ' << statusCode << ' ' << statusMessage << "\r\n";
            for (auto const& i: header)
                sstr << i << "\r\n";
            sstr << "\r\n";
            sstr << body;
            return sstr.str();
        };

        #define FAIL(MESSAGE) \
            TestResult{false, true, MESSAGE, compileResult()}

        #define SUCCESS(MESSAGE) \
            TestResult{true, true, MESSAGE, compileResult()}

        if (test_.response.code)
            if (test_.response.code.get() != statusCode)
                return FAIL("status code mismatch: "s + std::to_string(test_.response.code.get()) + " expected, but got: " + std::to_string(statusCode));

        if (test_.response.body)
            if (test_.response.body.get() != body)
                return FAIL("body mismatch, got: "s + body);

        if (test_.response.header)
        {
            auto receivedHeader = TestFile::Header{};

            for (auto const& i : header)
            {
                auto colpos = i.find(':');
                if (colpos == std::string::npos)
                    return FAIL("header field does not contain colon");

                TestFile::HeaderEntry entry;
                entry.key = boost::algorithm::trim_right_copy(i.substr(0, colpos));
                entry.value = boost::algorithm::trim_left_copy(i.substr(colpos + 1, i.size() - colpos - 1));

                receivedHeader.entry.push_back(entry);
            }

            auto expectedHeader = test_.response.header.get();
            for (auto const& i : expectedHeader.entry)
            {
                auto iter = std::find_if(std::begin(receivedHeader.entry), std::end(receivedHeader.entry), [&i](auto const& entry) {
                    return (static_cast <std::string> (entry.key) == static_cast <std::string> (i.key))
                        && (static_cast <std::string> (entry.value) == static_cast <std::string> (i.value));
                });

                if (iter == std::end(receivedHeader.entry))
                    return FAIL("expected header field missing: "s + static_cast <std::string> (i.key));
            }

            if (expectedHeader.entry.size() != receivedHeader.entry.size())
            {
                return FAIL("header size mismatch");
            }
        }


        return SUCCESS("no differences");

        #undef FAIL
        #undef SUCCESS
    }
//#####################################################################################################################
}
