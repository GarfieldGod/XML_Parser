#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

const char LEFT_ANGLE_BRACKET = '<';
const char RIGHT_ANGLE_BRACKET = '>';
const char FORWARD_SLASH = '/';
const char SPACE = ' ';
const char EQUAL_SIGN = '=';
const char QUOTATION_SIGN = '\"';
const char UNDERLINE = '_';
const char COLON = ':';
const string WHITE_SPACE = " \t\r\n";

enum TagType {
    TAG_DEFAULT,
    TAG_SINGLE,
    TAG_DOUBLE_STRUCT,
    TAG_DOUBLE_CONTENT
};

struct Tag
{
    string tagName;
    vector<string> tagProperty;
    TagType type;
    string content;
    int height;
};

class XmlParser
{
public:
    XmlParser();
    ~XmlParser();

    string readXmlFileToString(string filePath);
    void ParserXml(string text);
    void printTreeStruct();

private:
    string getTagName(string xml);
    vector<string> getTagProperty(string xml);

    int hight_ = 0;
    vector<string> tagProperty_ {"id", "data"};
    vector<Tag> tags_;
};

XmlParser::XmlParser() {}

XmlParser::~XmlParser() {}

string XmlParser::readXmlFileToString(string filePath)
{
    ifstream file;
    file.open(filePath);

    string result;
    string readLine;
    while (getline(file, readLine))
    {
        int start = readLine.find_first_not_of(WHITE_SPACE);
        int end = readLine.find_last_not_of(WHITE_SPACE);
        string invalidLine = readLine.substr(start, end - start + 1);
        result += invalidLine;
    }
    file.close();

    return result;
}

void XmlParser::ParserXml(string text)
{
    for (int i = 0; i < text.size(); i++)
    {
        if (text[i] == LEFT_ANGLE_BRACKET)
        {
            for (int t = 1; t < text.size()-1; t++)
            {
                if (text[t] == RIGHT_ANGLE_BRACKET) {
                    string tagHead = text.substr(i + 1, t - i - 1);
                    string tagName = getTagName(tagHead);
                    vector<string> tagProperty = getTagProperty(tagHead);
                    TagType tagType = TAG_DEFAULT;
                    string content = "";
                    //单标签
                    if (text[t - 1] == FORWARD_SLASH)
                    {
                        tagType = TAG_SINGLE;
                        Tag tag = {tagName, tagProperty, tagType, content, hight_};
                        tags_.push_back(tag);

                        text = text.substr(t + 1, text.size() - 1);
                        ParserXml(text);
                    //双标签
                    } else {
                        string tagEnd;
                        tagEnd.push_back(LEFT_ANGLE_BRACKET);
                        tagEnd.push_back(FORWARD_SLASH);
                        tagEnd += tagName;
                        tagEnd.push_back(RIGHT_ANGLE_BRACKET);

                        int tagEndPos = text.find(tagEnd, 0) == -1 ? 0: text.find(tagEnd, 0);
                        if (tagHead[0] == FORWARD_SLASH)
                        {
                            text = text.substr(t + 1,text.size() - 1);
                            ParserXml(text); break;
                        }
                        //无内容标签
                        if (text[t + 1] == LEFT_ANGLE_BRACKET) {
                            if (text.find(tagEnd, 0) != t + 1) {
                                tagType = TAG_DOUBLE_STRUCT;
                                Tag tag = {tagName, tagProperty, tagType, content, hight_};
                                tags_.push_back(tag);

                                text = text.substr(t + 1, text.size() - 1);
                                hight_++;
                            }
                        //有内容标签
                        } else {
                            if (text.find(tagEnd, 0) != t + 1) {
                                content = text.substr(t + 1, text.find(tagEnd, 0) - t - 1);
                                tagType = TAG_DOUBLE_CONTENT;
                                Tag tag = {tagName, tagProperty, tagType, content, hight_};
                                tags_.push_back(tag);

                                if (text[text.find(tagEnd, 0) + tagEnd.size() + 1] == FORWARD_SLASH) { hight_--; }
                                text = text.substr(tagEndPos + tagEnd.size(), text.size() - 1);
                            }
                        }
                        ParserXml(text);
                    }
                    break;
                }
            }
            break;
        }
    }
}

void XmlParser::printTreeStruct()
{
    for(Tag tag : tags_) {
        string offset;
        for (int n = 0; n < tag.height; n++) {offset += "\t";}
        string properties;
        for (string property : tag.tagProperty) {
            if (property != "") {
                properties += UNDERLINE + property;
            }
        }
        cout << offset << tag.tagName + properties + COLON << tag.content << "\n";
    }
}

string XmlParser::getTagName(string text)
{
    int startPos = 0, endPos = 0;
    for (int i = 0; i < text.size(); i++) {
        if (text[i] != LEFT_ANGLE_BRACKET && text[i] != SPACE) {
            startPos = i;
            break;
        }
    }
    for (int i = startPos + 1; i <= text.size(); i++) {
        if (i == text.size() || text[i] == SPACE) {
            endPos = i;
            break;
        }
    }
    return text.substr(startPos, endPos - startPos);
}

vector<string> XmlParser::getTagProperty(string text)
{
    vector<string> properties(tagProperty_.size());
    for (int p = 0; p < tagProperty_.size(); p++) {
        string keyStr = tagProperty_[p] + EQUAL_SIGN + QUOTATION_SIGN;
        if (text.find(keyStr, 0) != -1) {
            for (int i = text.find(keyStr, 0) + keyStr.size(); i < text.size(); i++) {
                if (text[i] == QUOTATION_SIGN) break;
                else properties[p].push_back(text[i]);
            }
        }
    }
    return properties;
}

int main()
{
    XmlParser xml;
    string text = xml.readXmlFileToString("test/test0.xml");
    xml.ParserXml(text);
    xml.printTreeStruct();
}