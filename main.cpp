#include "xml_parser.h"

int main()
{
    XmlParser xml;
    string text = xml.readXmlFileToString("test/test0.xml");
    xml.ParserXml(text);
    xml.printTreeStruct();
}