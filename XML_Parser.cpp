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
const string WHITE_SPACE = " \t\r\n";

class XmlParser
{
public:
    XmlParser();
    ~XmlParser();

    string readXmlFileToString(string filePath);
    void ParserXml(string Text);

private:
    string getTagName(string xml);
    string getTagProperty(string xml);

    int hight_ = 0;
    vector<string> tagProperty_ {"id", "data"};
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

void XmlParser::ParserXml(string Text)
{
    for (int i = 0; i < Text.size(); i++)
    {
        if (Text[i] == LEFT_ANGLE_BRACKET)
        {
            for (int t = 1; t < Text.size()-1; t++)
            {
                if (Text[t] == RIGHT_ANGLE_BRACKET) {
                    string endstr;
                    string tagContent = Text.substr(i + 1, t - i - 1);
                    endstr.push_back(LEFT_ANGLE_BRACKET);
                    endstr.push_back(FORWARD_SLASH);
                    endstr += tagContent;
                    endstr.push_back(RIGHT_ANGLE_BRACKET);
                    string externinfo = getTagProperty(tagContent);//额外信息
                    if (externinfo != "")
                    {
                        tagContent = getTagName(tagContent);
                    }
                    if (Text[t - 1] == FORWARD_SLASH)//单标签
                    {
                        string d;
                        for (int n = 0; n < hight_; n++)
                        {
                            d += "\t";
                        }
                        cout << d << tagContent + externinfo<< "\n";
                        Text = Text.substr(t + 1, Text.size() - 1);
                        ParserXml(Text);
                        break;
                    }
                    int c = Text.find(endstr, 0) == -1 ? 0: Text.find(endstr, 0);
                    if (tagContent[0] == FORWARD_SLASH)
                    {
                        Text = Text.substr(t+1,Text.size()-1);
                        ParserXml(Text); break;
                    }
                    if (Text[t + 1] == LEFT_ANGLE_BRACKET)//无内容
                    {
                        if (Text.find(endstr, 0) != t + 1)
                        {
                            //cout << "发现树点：" << Text << "\t\t\t高度：" << hight_ << "\n";
                            string d;
                            for (int n = 0; n < hight_; n++)
                            {
                                d += "\t";
                            }
                            cout << d << tagContent+externinfo <<"：" << "\n";
                            Text = Text.substr(t + 1, Text.size() - 1);
                            hight_ += 1;
                            //cout << "裁剪树点：" << Text << "\t\t\t高度：" << hight_ << "\n";
                            ParserXml(Text);
                        }
                        break;
                    }
                    else//有内容
                    {
                        if (Text.find(endstr, 0) != t + 1)
                        {
                            string b = Text.substr(t + 1, Text.find(endstr, 0) - t - 1);
                            //cout << "找到" << endstr << "的值为：" << b << "\t\t\t高度：" << hight_ << "\n";
                            string d;
                            for (int n = 0; n < hight_; n++)
                            {
                                d += "\t";
                            }
                            cout <<d<<tagContent+externinfo << "：" << b << "\n";
                            if (Text[Text.find(endstr, 0) + endstr.size() + 1] == FORWARD_SLASH) { hight_ -= 1; }//Text = Text.substr(c, Text.size() - 1);
                            Text = Text.substr(c+endstr.size(), Text.size() - 1);
                            ParserXml(Text);
                        }
                        break;
                    }
                }
            }
            break;
        }
    }
}

string XmlParser::getTagName(string xml)
{
    int ss = 0;
    int ee = 0;
    for (int i = 0; i < xml.size(); i++)
    {
        if (xml[i] != LEFT_ANGLE_BRACKET && xml[i] != SPACE)
        {
            ss = i;
            break;
        }
    }
    for (int i = ss; i < xml.size(); i++)
    {
        if (xml[i] == SPACE)
        {
            ee = i;
        }
    }
    return xml.substr(ss, ee - ss);
}

string XmlParser::getTagProperty(string text)
{
    vector<string> propertys(tagProperty_.size());
    for (int p = 0; p < tagProperty_.size(); p++) {
        string keyStr = tagProperty_[p] + EQUAL_SIGN + QUOTATION_SIGN;
        if (text.find(keyStr, 0) != -1) {
            for (int i = text.find(keyStr, 0) + keyStr.size(); i < text.size(); i++) {
                if (text[i] == QUOTATION_SIGN) break;
                else propertys[p].push_back(text[i]);
            }
        }
    }
    string result;
    for(string property : propertys) {
        result += property;
    }
    return result;
}

int main()
{
    XmlParser xml;
    string Text = xml.readXmlFileToString("test/test0.xml");
    xml.ParserXml(Text);
}