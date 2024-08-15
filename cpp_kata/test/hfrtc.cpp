#include "TinyUtil.h"
#include "TinyServer.h"
#include <memory>

using namespace std;
using namespace wfan;

int main(int argc, char *argv[])
{
    printf("== WebRTC signal and media server start...===\n");
    
    unique_ptr<ITinyServer> pt(CTinyServerFactory::CreateServer(SVC_ECHO) );
    pt->Init();
    pt->Start();
    pt->Stop();
    pt->Clean();

    return 0;
}
