/*
 * Event.h
 *
 *  Created on: 2014-5-28
 *      Author: walter fan learn POSA2
 */

#ifndef __EVENT_H__
#define __EVENT_H__

typedef unsigned long HANDLE;

typedef unsigned int Event_Type;

enum {
	READ_EVENT = 01,
	ACCEPT_EVENT = 01,
	WRITE_EVENT = 02,
	TIMEOUT_EVENT = 04,
	SIGNAL_EVENT = 010,
	CLOSE_EVENT = 020

};

class Time_Value
{
};

class Event_Handler {
public:
	virtual void handle_input(HANDLE handle) = 0;

    virtual void handle_output(HANDLE handle) = 0;

    virtual void handle_timeout(const Time_Value&) = 0;

    virtual void handle_close(HANDLE handle, Event_Type et) = 0;

	virtual HANDLE get_handle() const = 0;

	virtual ~Event_Handler() {};
};

class Reactor_Implementation;

class Reactor
{
public:
    virtual void register_handler(Event_Handler *eh, Event_Type et) = 0;

    virtual void register_handler(HANDLE handle, Event_Handler *eh, Event_Type et) = 0;

    virtual void remove_handler(Event_Handler *eh, Event_Type et) = 0;

    virtual void remove_handler(HANDLE handle, Event_Type et) = 0;

    void handle_events(Time_Value *timeout=0);

    static Reactor* instance();
private:
    Reactor_Implementation * reactor_imple_;

};
#endif /* __EVENT_H__ */
