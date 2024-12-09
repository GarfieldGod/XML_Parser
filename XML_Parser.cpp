﻿#include <iostream>
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

void XmlParser::ParserXml(string text, int tagHeight)
{
    int tagHeadStart = text.find(LEFT_ANGLE_BRACKET, 0);
    int tagHeadEnd = text.find(RIGHT_ANGLE_BRACKET, 0);
    if (tagHeadStart != -1 && tagHeadEnd != -1) {
        string tagHead = text.substr(tagHeadStart, tagHeadEnd - tagHeadStart + 1);

        string tagName = getTagName(text);
        vector<string> tagProperty = getTagProperty(tagHead);
        string content = getTagContent(text);
        Tag tag = {tagName, tagProperty, content, tagHeight};
        tags_.push_back(tag);
        //单标签
        if (text[tagHeadEnd - 1] == FORWARD_SLASH)
        {
            text = text.substr(tagHeadEnd + 1, text.size() - 1);
            ParserXml(text, tagHeight);
        //双标签
        } else {
            string tagEnd;
            tagEnd.push_back(LEFT_ANGLE_BRACKET);
            tagEnd.push_back(FORWARD_SLASH);
            tagEnd += tagName;
            tagEnd.push_back(RIGHT_ANGLE_BRACKET);

            int tagEndPos = text.find(tagEnd, 0);
            if (tagEndPos != -1) {
                int forwardTextStart = text.find(LEFT_ANGLE_BRACKET, tagHeadEnd);
                string forwardText = text.substr(forwardTextStart, tagEndPos - forwardTextStart);
                string backText = text.substr(tagEndPos + tagEnd.size(), text.size() - 1);
                if (forwardText != "") ParserXml(forwardText, tagHeight + 1);
                if (backText != "") ParserXml(backText, tagHeight);
            }
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
    for (int tagHeadStart = 0; tagHeadStart < text.size(); tagHeadStart++) {
        if (text[tagHeadStart] != LEFT_ANGLE_BRACKET && text[tagHeadStart] != SPACE) {
            startPos = tagHeadStart;
            break;
        }
    }
    for (int tagHeadStart = startPos + 1; tagHeadStart < text.size(); tagHeadStart++) {
        if (text[tagHeadStart] == RIGHT_ANGLE_BRACKET || text[tagHeadStart] == SPACE) {
            endPos = tagHeadStart;
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
            for (int tagHeadStart = text.find(keyStr, 0) + keyStr.size(); tagHeadStart < text.size(); tagHeadStart++) {
                if (text[tagHeadStart] == QUOTATION_SIGN) break;
                else properties[p].push_back(text[tagHeadStart]);
            }
        }
    }
    return properties;
}

string XmlParser::getTagContent(string text)
{
    int tagHeadEnd = text.find(RIGHT_ANGLE_BRACKET, 0);
    int nextHeadPos = text.find(LEFT_ANGLE_BRACKET, tagHeadEnd);
    string frontContent = text.substr(tagHeadEnd, nextHeadPos - tagHeadEnd);
    return text.substr(tagHeadEnd + 1, nextHeadPos - tagHeadEnd - 1);
}

int main()
{
    XmlParser xml;
    string text = xml.readXmlFileToString("test/test0.xml");
    xml.ParserXml(text);
    xml.printTreeStruct();
}