/* 
 * File:   admin.h
 * Author: walter
 *
 * Created on March 18, 2015, 8:44 PM
 */

#ifndef __WF_PACKET_H__
#define	__WF_PACKET_H__

#include <string>
#include <stdint.h>
#include "srtp2/srtp.h"


namespace wfan {

	int read_raw(const char* szFile);

	int handle_menu();

	int handle_args(int argc, char* argv[]);

	int init(int argc, char* argv[]);

	int clean();
}

#endif