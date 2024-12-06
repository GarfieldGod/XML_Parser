#include "zhml_parser.h"

int main()
{
    ZhmlParser zhml("test/test.zhml");
    zhml.printTreeStruct();
    vector<Tag> tags = zhml.getZhmlTags();
}