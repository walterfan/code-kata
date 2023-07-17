/* 
 * File:   admin.h
 * Author: walter
 *
 * Created on March 18, 2015, 8:44 PM
 */

#ifndef __WF_ADMIN_H__
#define	__WF_ADMIN_H__

#include <string>
#include <stdint.h>

namespace wfan {
    
typedef struct stConfigure {
    bool debug_flag;
    uint8_t log_level;
    uint16_t signal_port;   // for signal and sharing
    uint16_t audio_port; // for audio
    uint16_t video_port; // for video
} CONFIG_ITEMS;

enum SERVICE_TYPE {
    CHAT  = 0x1,
    AUDIO = 0x2,
    VIDEO = 0x4,
    SHARING = 0x10
};

enum ENCODING_TYPE {
    ENCODE_HEX,
    ENCODE_BASE64,
    ENCODE_AES,
    ENCODE_SRTP
};

#ifdef __cplusplus
extern "C" {
#endif

// all of your legacy C code here

    int init(int argc, char* argv[]);
    int clean();
    
    int handle_args(int argc, char* argv[]);
    int handle_menu();
    
    int read_config(const char* szConfigFile, stConfigure* pConfig);

#ifdef __cplusplus
}
#endif    
}//end of namespace

#endif	/* __WF_ADMIN_H__ */

