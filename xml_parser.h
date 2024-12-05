#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct Tag;

class XmlParser
{
public:
    XmlParser();
    ~XmlParser();

    string readXmlFileToString(string filePath);
    void ParserXml(string text, int tagHeight = 0);
    void printTreeStruct();

private:
    string getTagName(string text);
    vector<string> getTagProperty(string text);
    string getTagContent(string text);

    vector<string> tagProperty_ {"id", "data"};
    vector<Tag> tags_;
};