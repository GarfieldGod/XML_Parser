#include "zhml_parser.h"

const char LEFT_ANGLE_BRACKET = '<';
const char RIGHT_ANGLE_BRACKET = '>';
const char FORWARD_SLASH = '/';
const char SPACE = ' ';
const char EQUAL_SIGN = '=';
const char QUOTATION_SIGN = '\"';
const char UNDERLINE = '_';
const char COLON = ':';
const string WHITE_SPACE = " \t\r\n";

ZhmlParser::ZhmlParser(string filePath) {
    filePath_ = filePath;
    string text = readZhmlFileToString(filePath_);
    parseZhml(text);
}

ZhmlParser::~ZhmlParser() {}

vector<Tag> ZhmlParser::getZhmlTags() 
{
    return tags_;
}

void ZhmlParser::parseZhml(string text, int tagHeight)
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
        //Single Tag
        if (text[tagHeadEnd - 1] == FORWARD_SLASH)
        {
            text = text.substr(tagHeadEnd + 1, text.size() - 1);
            parseZhml(text, tagHeight);
        //Double Tag
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
                if (forwardText != "") parseZhml(forwardText, tagHeight + 1);
                if (backText != "") parseZhml(backText, tagHeight);
            }
        }
    }
}

void ZhmlParser::printTreeStruct()
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
  
        if(tag.content != "") cout << offset + tag.tagName + properties + COLON << SPACE + tag.content << endl;
        else cout << offset + tag.tagName + properties << endl;
    }
}

string ZhmlParser::readZhmlFileToString(string filePath)
{
    ifstream file;
    file.open(filePath);

    string result;
    string readLine;
    while (getline(file, readLine))
    {
        int start = readLine.find_first_not_of(WHITE_SPACE);
        int end = readLine.find_last_not_of(WHITE_SPACE);
        string invalidLine = readLine[start] != '#' ? readLine.substr(start, end - start + 1) : "";
        result += invalidLine;
    }
    file.close();

    return result;
}

string ZhmlParser::getTagName(string text)
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

vector<string> ZhmlParser::getTagProperty(string text)
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

string ZhmlParser::getTagContent(string text)
{
    int tagHeadEnd = text.find(RIGHT_ANGLE_BRACKET, 0);
    int nextHeadPos = text.find(LEFT_ANGLE_BRACKET, tagHeadEnd);
    string frontContent = text.substr(tagHeadEnd, nextHeadPos - tagHeadEnd);
    return text.substr(tagHeadEnd + 1, nextHeadPos - tagHeadEnd - 1);
}