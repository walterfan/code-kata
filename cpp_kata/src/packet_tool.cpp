#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <iostream>
#include <string>
#include <fstream>
#include "packet_tool.h"

extern "C"
{
    #include "lua.h"
    #include "lauxlib.h"
    #include "lualib.h"
    #include "util.h"
}


#define MTU 1400

static char databuf[MTU]; 
static char packet_string[MTU];
static bool g_debug_flag = false;
static const char* SEC_CONF= "SecurityConfiguration";

static ToolConfig s_tool_config;


using namespace std;
using namespace wfan;


enum ENCODING_TYPE {
    ENCODE_BASE64,
    ENCODE_AES,
    ENCODE_SRTP
};

static const char* USAGE = "-c|--config <config_file> (default is ./etc/packet_tool.lua) \n"
    "\t-i|--input <input file or string> \n"
    "\t[-o|--output <output file>] \n"
    "\t[-d|--decode or -e|--encode] (default is encode) \n"
    "\t[--fmt <aes|srtp>]\n"
    "\t[--verbose --brief --test --help] \n";



void set_debug_flag(bool flag) {
    g_debug_flag = flag;
}

void ERROR(const char *fmt, ...) {
    LOG(stderr, "ERROR: %s", log_msg);
}

void INFO(const char *fmt, ...) {
    LOG(stdout, "ERROR: %s", log_msg);
}

void LOG(FILE* fp, const char *fmt, ...) {
    if(!g_debug_flag) {
        return;
    }
    char log_msg[MTU] = {'\0'};
    int n = 0;
    va_list ap;
    va_start(ap, fmt);
    va_end(ap);
    n = vsnprintf(log_msg, MTU, fmt, ap);
    fprintf(fp, "DEBUG: %s", log_msg);

}


srtp_err_status_t dump_srtp_packet(srtp_hdr_t* hdr, uint32_t pkt_octet_len, std::string& json)
{
      int octets_in_rtp_header = 12;
      uint8_t *data = ((uint8_t *)hdr)+octets_in_rtp_header;
      int hex_len = pkt_octet_len-octets_in_rtp_header;

      /* sanity checking */
      if ((hdr == NULL) || (pkt_octet_len > MTU))
        return err_status_bad_param;


      /* write packet into string */
      sprintf(packet_string,
          "  { \"packet_type\":\"rtp\",\n"
          "  \"packet_len\" : %d, \n"
          "  \"packet_content\" : {\n"
          "  \"version\" : %d,\n"
          "  \"p\" : %d,\n"
          "  \"x\" : %d,\n"
          "  \"cc\" : %d,\n"
          "  \"m\" : %d,\n"
          "  \"pt\" : 0x%x,\n"
          "  \"seq\" : 0x%x(%d)\n"
          "  \"ts\" : 0x%x,\n"
          "  \"ssrc\" : 0x%x,\n"
          "  \"data\" : %s\n"
          "  } }",
          pkt_octet_len,
          hdr->version,
          hdr->p,
          hdr->x,
          hdr->cc,
          hdr->m,
          hdr->pt,
          ntohs(hdr->seq),ntohs(hdr->seq),
          ntohl(hdr->ts),
          ntohl(hdr->ssrc),
            octet_string_hex_string(data, hex_len));
      json = packet_string;
    return err_status_ok;
}



srtp_policy_t* create_srtp_policy(uint8_t* srtp_key_salt, srtp_ssrc_t ssrc_type, compute_method_t compute_m)
{
    srtp_policy_t* srtp_policy_ptr 
        = (srtp_policy_t*)crypto_alloc(sizeof(srtp_policy_t));
    
    crypto_policy_set_rtp_default(&srtp_policy_ptr->rtp);
    crypto_policy_set_rtcp_default(&srtp_policy_ptr->rtcp);

    srtp_policy_ptr->rtp.compute_type = compute_m;//compute_standard;
    srtp_policy_ptr->rtp.auth_tag_type = auth_tag_packet;
        
    srtp_policy_ptr->ssrc = ssrc_type;
    srtp_policy_ptr->key = srtp_key_salt;
    srtp_policy_ptr->ekt = NULL;
    srtp_policy_ptr->switchable = 0;
    srtp_policy_ptr->replayable = 0;
    srtp_policy_ptr->next = NULL;

    return srtp_policy_ptr;
}

void destroy_srtp_policy(srtp_policy_t* srtp_policy_ptr)
{
    crypto_free(srtp_policy_ptr->ekt);
    crypto_free(srtp_policy_ptr);
}

static int data2file(uint8_t* pkt_ptr, uint32_t pkt_len, const char* filename)
{
  FILE * fp = fopen (filename, "wb");
  fwrite (pkt_ptr , sizeof(uint8_t), pkt_len, fp);
  fclose (fp);
  return 0;
}

srtp_err_status_t write_packet(uint8_t* pkt_ptr, 
    uint32_t pkt_octet_len,  const char* filename, FILE* fp)
{
    size_t cnt = 0;
    if(filename)
        cnt = data2file(pkt_ptr, pkt_octet_len, filename);
    else if(fp)
        cnt = fwrite(pkt_ptr , sizeof(uint8_t), pkt_octet_len, fp);
    else
        return err_status_bad_param;
    
    if(cnt == pkt_octet_len)
        return err_status_ok;
    else
        return err_status_fail;
}



namespace wfan {


    int read_config(const char* szSecConfigFile, SecurityConfiguration* pSecConfig, SecurityPolicy* pSecPolicy)
    {
        lua_State* pL = luaL_newstate();
        luaL_openlibs(pL);
        int nRet = luaL_dofile(pL, szSecConfigFile);
        if(nRet != 0)
        {
            ERROR("open %s, return %d because of error %s\n", 
                szSecConfigFile, nRet, lua_tostring(pL, -1));
        }
        lua_getglobal(pL, SEC_CONF);
        if(!lua_istable(pL, -1)) {
            ERROR("%s is not a table.\n", SEC_CONF); 
            return -1;
        }
        
        lua_getfield(pL,1,"cryptoSuiteType");   
        pSecConfig->cryptoSuiteType = (CryptoSuiteType)lua_tonumber(pL, -1); 
        DEBUG("cryptoSuiteType=%d\n", pSecConfig->cryptoSuiteType);
        
        lua_getfield(pL,1,"masterKeySaltLength");
        pSecConfig->masterKeySaltLength = lua_tonumber(pL, -1);
        DEBUG("masterKeySaltLength=%d\n", pSecConfig->masterKeySaltLength);

        pSecConfig->masterKeySalt = (uint8_t*)malloc(pSecConfig->masterKeySaltLength);
        lua_getfield(pL,1,"masterKeySalt"); 
        const char* base64MasterKeySalt = lua_tostring(pL, -1);
        string strMasterKeySalt;
        base64_decode(base64MasterKeySalt,strMasterKeySalt);    
        memmove(pSecConfig->masterKeySalt, strMasterKeySalt.c_str(), pSecConfig->masterKeySaltLength);
        DEBUG("masterKeySalt=%s\n", octet_string_hex_string(pSecConfig->masterKeySalt, strMasterKeySalt.length()));

        lua_getfield(pL,1,"rtpSecurityService");
        pSecConfig->rtpSecurityService = (SecurityService) lua_tonumber(pL, -1);
        DEBUG("rtpSecurityService=%d\n", pSecConfig->rtpSecurityService);

        lua_getfield(pL,1,"rtcpSecurityService");
        pSecConfig->rtcpSecurityService = (SecurityService)lua_tonumber(pL, -1); 
        DEBUG("rtcpSecurityService=%d\n", pSecConfig->rtcpSecurityService);
        
        lua_getfield(pL,1,"fecOder");    
        pSecConfig->fecOrder = (SRTPFecOrder)lua_tonumber(pL, -1);
        DEBUG("fecOder=%d\n", pSecConfig->fecOrder);

        lua_getfield(pL,1,"switchingRTPPacket");    
        pSecPolicy->switchingRTPPacket = lua_tonumber(pL, -1)==0?false:true;
        DEBUG("switchingRTPPacket=%d\n",pSecPolicy->switchingRTPPacket);

        lua_getfield(pL,1,"stripSRTPTagsAfterUnprotect");    
        pSecPolicy->stripSRTPTagsAfterUnprotect = lua_tonumber(pL, -1)==0?false:true;
        DEBUG("stripSRTPTagsAfterUnprotect=%d\n", pSecPolicy->stripSRTPTagsAfterUnprotect);

        lua_getfield(pL,1,"allowReplayedPacket");    
        pSecPolicy->allowReplayedPacket = lua_tonumber(pL, -1)==0?false:true;
        DEBUG("allowReplayedPacket=%d\n", pSecPolicy->allowReplayedPacket);

        //lua_getfield(pL,1,"streamDirection");    
        //g_stream_direction = (StreamDirection)lua_tonumber(pL, -1) ;
        //printf("streamDirection=%d\n", g_stream_direction);


        lua_getfield(pL,1,"ekt");
        //dump_lua_state(pL);
        if(lua_istable (pL, -1)) {
            pSecConfig->ekt = (EKTConfiguration*)malloc(sizeof(EKTConfiguration));
            lua_getfield(pL,-1,"cipherType");
            pSecConfig->ekt->cipherType =(EKTCipherType)lua_tonumber(pL, -1);
            DEBUG("ektCipherType=%d\n", pSecConfig->ekt->cipherType);

            lua_getfield(pL,-2,"keyLength");
            pSecConfig->ekt->keyLength=(uint32_t)lua_tonumber(pL, -1);
            DEBUG("keyLength=%d\n", pSecConfig->ekt->keyLength);
        
            pSecConfig->ekt->key = (uint8_t*)malloc(pSecConfig->ekt->keyLength);
            lua_getfield(pL,-3,"key");
            const char* base64EktKey = lua_tostring(pL, -1);
            string strEktKey;
            base64_decode(base64EktKey,strEktKey);    
        
            memmove(pSecConfig->ekt->key, strEktKey.c_str(), pSecConfig->ekt->keyLength);
            DEBUG("EktKey=%s\n",    octet_string_hex_string(pSecConfig->ekt->key, pSecConfig->ekt->keyLength));

            lua_getfield(pL,-4,"spi");
            pSecConfig->ekt->SPI=(uint16_t)lua_tonumber(pL, -1);
            DEBUG("spi=%d\n", pSecConfig->ekt->SPI);

        }
        
        int top = lua_gettop(pL);
        DEBUG("-- read policy finish, there are %d element in stack\n", top);
        lua_pop(pL, -1);
        lua_close(pL);
        return 0; 
    }

    srtp_policy_t* create_policy_as_config() {
        if(s_tool_config.srtp_policy) {
            return s_tool_config.srtp_policy;
        }
        srtp_ekt_policy_t*  ekt_policy_ptr = create_ekt_policy(
            s_tool_config.security_config.ekt->key, 3, 0., 
            s_tool_config.security_config.ekt->SPI);
        
        srtp_ssrc_t ssrc_type;
        
        if(s_tool_config.stream_direction == STREAM_RECVING) {
            ssrc_type.type = ssrc_any_outbound;
        } else {
            ssrc_type.type = ssrc_any_inbound;
        }
        
        ssrc_type.value = 0;
        srtp_policy_t* srtp_policy_ptr = create_srtp_policy(
            s_tool_config.security_config.masterKeySalt,ssrc_type, compute_standard);
        srtp_policy_ptr->ekt = ekt_policy_ptr;
        s_tool_config.srtp_policy = srtp_policy_ptr;

        return srtp_policy_ptr;
    }

	int handle_menu() {
		return 0;
	}

	int handle_args(int argc, char* argv[]) {
        if(argc > 1) {
            read_raw(argv[1]);
        }
		return 0;
	}

	int init(int argc, char* argv[]) {
		return 0;
	}

	int clean() {
		return 0;
	}

    int read_raw(const char* szFile) {
        streampos size;
        char * memblock;

        ifstream file (szFile, ios::in|ios::binary|ios::ate);
        if (file.is_open())
        {
            size = file.tellg();
            memblock = new char [size];
            file.seekg (0, ios::beg);
            file.read (memblock, size);
            file.close();

            delete[] memblock;
        }
        return (int)size;
    }

}//end of wfan namespace

int main(int argc, char* argv[])
{
    int ret = init(argc, argv);
    if(ret) {
        fprintf(stderr, "init error: %d\n", ret);
        return ret;
    }
    if(argc == 1) {
        fprintf(stdout, "Usage: %s %s",argv[0], USAGE);
        ret = handle_menu();
    }
    else {
        ret = handle_args(argc, argv);
    }
    clean();
    return ret;
}
