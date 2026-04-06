#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "jsoncpp/json.h"


#include <iostream>
#include <string>
using namespace std;

#define COUT cout<<__FILE__<<", "<<__LINE__<<": "

int main(int c, char **v)
{
    COUT<<"--- jsoncpp test  ---"<<endl;

    string json_example = "{\"array\": \
                            [\"item1\", \
                            \"item2\"], \
                            \"not an array\": \
                            \"asdf\" \
                         }";

     Json::Value root;
     Json::Reader reader;
     bool parsedSuccess = reader.parse(json_example, 
                                   root, 
                                   false);
  
    return 0;
}
