#include "zhml_parser.h"

int main(int argc, char* argv[])
{
    string filePath = "test/test.zhml";
    if (argc >= 2) {filePath = argv[1];}
    else {cerr << "Usage: zhml <file_path>" << endl;return 0;}
    int pos = filePath.find_last_of('.');
    string fileFormat = filePath.substr(pos + 1, filePath.size() - 1 - pos);
    if (fileFormat != "zhml") {
        cerr << "ZhmlParser can only parse .zhml file !" << endl;
        return 0;
    }

    ZhmlParser zhml(filePath);
    zhml.printTreeStruct();
    vector<Tag> tags = zhml.getZhmlTags();
}