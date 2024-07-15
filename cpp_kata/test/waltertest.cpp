#include "TinyUtil.h"
#include "TinyServer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory>

class CTag {
public:
	CTag() {}
	~CTag() {}
};

static char the_day[10] = "";

#define EXECUTE_JOURNEY(date) do \
	{ \
		extern int journey##date(int argc, char *argv[]); \
        if(strlen(the_day) > 0) { \
            if(strncmp(#date, the_day, 8) ==0) { \
                printf("-----"#date"-----\n");\
                journey##date(argc, argv);  \
            } \
        }\
		else {\
            printf("-----"#date"-----\n");\
            journey##date(argc, argv) ; \
        } \
	} while(0)

int split(string strValue, string separator, vector<string>& strArr)
{
    if(strValue.empty())
        return 0;
    
	string::size_type pos0 = 0;
	string::size_type pos1 = strValue.find(separator, pos0);
	string::size_type seplen = separator.size();

	int cnt = 0;
	while(string::npos != pos1) {
        strArr.push_back(strValue.substr(pos0, pos1-pos0));
        cnt++;
		pos0 = pos1 + seplen;
		pos1 = strValue.find(separator, pos0);
	}
    
    if(pos1 > pos0) {
        strArr.push_back(strValue.substr(pos0, pos1 - pos0));
        cnt++;
    }

    return cnt;
}

struct FrameDataList {
    FrameDataList* next = NULL;
};

struct FrameData {
    FrameDataList* obj_meta_list = NULL;
};



int main(int argc, char *argv[])
{
    
    if(argc > 1 ) {
        if(strlen(argv[1]) == 8) {
            strncpy(the_day, argv[1], 8);
            the_day[8] = '\0';
        }
    } else {
        printf("--launch jouneries--\n");
	}
    auto frame_meta = new FrameData();

    for (auto l_obj = frame_meta->obj_meta_list; l_obj != NULL; l_obj = l_obj->next)
    {
        printf("obj_meta_list is null");
    }


    return 0;
}

