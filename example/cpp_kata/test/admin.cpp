#include "TinyUtil.h"
#include "admin.h"
#include <memory>

using namespace std;

namespace wfan {
    int init(int argc, char* argv[]){
        //if the args specify the configure file -f
        return 0;
    }
    
    int clean() {
        return 0;
    }
    
    int handle_args(int argc, char* argv[]) {
        return 0;
    }
    
    int handle_menu() {
        return 0;
    }
    
    int read_config(const char* szConfigFile, stConfigure* pConfig) {
        return 0;
    }
}

using namespace wfan;
int main(int argc, char *argv[])
{
    int ret = init(argc, argv);
    if(ret) {
        fprintf(stderr, "initiate error: %d\n", ret);
        return ret;
    }
    
    if(argc == 1) {
        ret = handle_menu();
    } else {
        ret = handle_args(argc, argv);
    }
    
    clean();
    return ret;
}
