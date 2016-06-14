#include "xml_bindings.hpp"

#include "SimpleXML/xmlify/xmlify.hpp"
#include "SimpleXML/xml_parse/xml_parse.hpp"
#include "SimpleXML/xml_parse/sxml_generic_parser.hpp"

namespace AttenderTest
{
//#####################################################################################################################
    TestFile::Test loadTestFromStream(std::istream& stream)
    {
        TestFile::Test test;
        auto tree = SXML::parse_xml(stream);
        SXML::xml_parse(test, "test", tree);
        return test;
    }
//#####################################################################################################################
}
