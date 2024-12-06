#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct Tag
{
    string tagName;
    vector<string> tagProperty;
    string content;
    int height;
};

class XmlParser
{
public:
    XmlParser(string filePath);
    ~XmlParser();

    vector<Tag> getXmlTags();
    void printTreeStruct();

private:
    void parseXml(string text, int tagHeight = 0);
    string readXmlFileToString(string filePath);
    string getTagName(string text);
    vector<string> getTagProperty(string text);
    string getTagContent(string text);

    string filePath_;
    vector<string> tagProperty_ {"id", "data"};
    vector<Tag> tags_;
};