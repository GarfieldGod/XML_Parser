#include "xml_parser.h"

int main()
{
    XmlParser xml("test/test0.xml");
    xml.printTreeStruct();
    vector<Tag> tags = xml.getXmlTags();
}