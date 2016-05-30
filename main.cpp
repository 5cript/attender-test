#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <fstream>
#include <sstream>
#include <boost/asio/ip/tcp.hpp>

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
    try
    {
        boost::asio::ip::tcp::iostream s;

        std::ifstream reader("./http_request.txt", std::ios_base::binary);
        if (!reader.good())
        {
            std::cout << "could not find file\n";
            return 0;
        }

        std::stringstream sstr;
        sstr << reader.rdbuf();
        std::cout << sstr.str() << "\n";

        // The entire sequence of I/O operations must complete within 60 seconds.
        // If an expiry occurs, the socket is automatically closed and the stream
        // becomes bad.
        s.expires_from_now(boost::posix_time::seconds(240));

        // Establish a connection to the server.
        s.connect("localhost", "10101");
        if (!s)
        {
            std::cout << "Unable to connect: " << s.error().message() << "\n";
            return 1;
        }

        s << sstr.str();

        // By default, the stream is tied with itself. This means that the stream
        // automatically flush the buffered output before attempting a read. It is
        // not necessary not explicitly flush the stream at this point.

        // Check that response is OK.
        std::string http_version;
        s >> http_version;
        unsigned int status_code;
        s >> status_code;
        std::string status_message;
        std::getline(s, status_message);
        if (!s || http_version.substr(0, 5) != "HTTP/")
        {
            std::cout << "Invalid response\n";
            return 1;
        }

        std::cout << "Response returned with status code " << status_code << "\n";

        // Process the response headers, which are terminated by a blank line.
        std::string header;
        while (std::getline(s, header) && header != "\r")
            std::cout << header << "\n";
        std::cout << "\n";

        // Write the remaining data to output.
        std::cout << s.rdbuf();
    }
    catch (std::exception& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
    }

    return 0;
}
