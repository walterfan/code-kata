/*
 * TinyLog.h
 *
 *  Created on: Nov 11, 2014
 *      Author: walter
 */

#ifndef TINYLOG_H_
#define TINYLOG_H_

namespace wfan {


typedef enum {
	LOG_FATAL,
	LOG_ERROR,
	LOG_WARNING,
	LOG_INFO,
	LOG_DEBUG
} log_level_t;

class LogMessage {

};

class TinyLog {
public:
	TinyLog();
	virtual ~TinyLog();
};

} /* namespace wfan */

#endif /* TINYLOG_H_ */
