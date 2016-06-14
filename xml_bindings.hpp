#pragma once

#include "SimpleXML/xmlify/sxml_fusion.hpp"
#include "SimpleXML/xml_parse/sxml_parse_fusion.hpp"

#include <boost/optional.hpp>
#include <string>
#include <vector>
#include <iosfwd>

namespace AttenderTest
{
    namespace TestFile
    {
        struct HeaderEntry : public SXML::Parsable <HeaderEntry>
                           , public SXML::Xmlifiable <HeaderEntry>
        {
            SXML::Attribute <std::string> key;
            SXML::Content <std::string> value;
        };

        struct Header : public SXML::Parsable <Header>
                      , public SXML::Xmlifiable <Header>
        {
            std::vector <HeaderEntry> entry; // wanna put into plural, but would be weird in XML.
        };

        struct Request : public SXML::Parsable <Request>
                       , public SXML::Xmlifiable <Request>
        {
            boost::optional <std::string> method; // defaults to GET
            boost::optional <std::string> url; // defaults to /
            std::string protocol;
            std::string version;
            boost::optional <Header> header;
            boost::optional <std::string> body;
        };

        struct Response : public SXML::Parsable <Response>
                        , public SXML::Xmlifiable <Response>
        {
            boost::optional <std::string> body; // ignored if not set
            boost::optional <int> code; // ignored if not set
            boost::optional <Header> header; // ignored if not set
        };

        struct Test : public SXML::Parsable <Test>
                    , public SXML::Xmlifiable <Test>
        {
            Request request;
            Response response;
        };
    }

    TestFile::Test loadTestFromStream(std::istream& stream);
}

BOOST_FUSION_ADAPT_STRUCT
(
    AttenderTest::TestFile::HeaderEntry,
    key, value
)

BOOST_FUSION_ADAPT_STRUCT
(
    AttenderTest::TestFile::Header,
    entry
)

BOOST_FUSION_ADAPT_STRUCT
(
    AttenderTest::TestFile::Request,
    method, url, protocol, version, header, body
)

BOOST_FUSION_ADAPT_STRUCT
(
    AttenderTest::TestFile::Response,
    body, code, header
)

BOOST_FUSION_ADAPT_STRUCT
(
    AttenderTest::TestFile::Test,
    request, response
)
