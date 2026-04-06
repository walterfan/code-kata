/*
 * stream_monitor.h
 *
 *  Created on: Jan 29, 2024
 *      Author: Walter Fan
 */

#ifndef ECHO_STREAM_MONITOR_H_
#define ECHO_STREAM_MONITOR_H_

#include <string>
#include <ctime>

namespace wfan {

struct StreamEvent {
    
    std::time_t time;
    std::string name;
    int32_t result;
    uint32_t duration;
    std::string desc;

    std::string stream_id;

    std::time_t get_begin_time() { return time;}
    std::time_t get_end_time() { return time + duration;}

    void set_begin_time(std::time_t begin_time) {
        time = begin_time;
    }

    void set_end_time(std::time_t end_time) {
        duration = end_time - time;
    }
};

/**
 * @brief Stream Monitor
 * 
 * per every stream, record event/step time, name, result, duration 
 * and other properties
 * 
 */
class StreamMonitor {
public:
    virtual ~StreamMonitor() = default;

    virtual void record_event(StreamEvent event) = 0;

    virtual void summarize(const std::string& stream_id) = 0;
};

class StreamMonitorImpl: public StreamMonitor {
public:
    StreamMonitorImpl();
    virtual ~StreamMonitorImpl();

    virtual void record_event(StreamEvent event);

    virtual void summarize(const std::string& stream_id)     ;
};


} /* namespace wfan */

#endif /* ECHO_STREAM_MONITOR_H_ */
