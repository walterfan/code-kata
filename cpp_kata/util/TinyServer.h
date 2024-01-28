/*
 * TinyServer.h
 *
 *  Created on: 2014-6-14
 *      Author: walter
 */

#ifndef TINYSERVER_H_
#define TINYSERVER_H_

#include <stdint.h>

typedef enum {
	TRANSPORT_TCP = 1,
	TRANSPORT_UDP = 2,
	TRANSPORT_TLS = 3,
	TRANSPORT_DTLS = 6
} TRANSPORT_PROTOCOL;

typedef enum {
	SVC_ECHO,
	SVC_TIME,
	SVC_HTTP,
	SVC_SIP,
	SVC_XMPP,
	SVC_WEB_SOCKET,
	SVC_RTP,
	SVC_FTP,
	SVC_TELNET
} SVC_PROTOCOL;


typedef enum {
	OPT_HOST,
	OPT_PORT
} SVC_OPTION;


class ITinyServer {
public:
	virtual ~ITinyServer() {};

	virtual void SetOption(int optionKey, void* optionValue) = 0;
	virtual void* GetOption(int optionKey) = 0;

	virtual int Init(TRANSPORT_PROTOCOL trptType=TRANSPORT_TCP, int nPort=2018) = 0;
	virtual int Start() = 0;
	virtual int Run() = 0;
	virtual int Stop() = 0;
	virtual int Clean() = 0;
};


class CTinyEchoServer: public ITinyServer
{
public:
	CTinyEchoServer();
	virtual ~CTinyEchoServer();

	virtual void SetOption(int optionKey, void* optionValue) ;
	virtual void* GetOption(int optionKey) ;

	virtual int Init(TRANSPORT_PROTOCOL trptType=TRANSPORT_TCP, int nPort=2018);
	virtual int Start();
	virtual int Run();
	virtual int Stop();
	virtual int Clean();
private:
	TRANSPORT_PROTOCOL m_trptType;
	SVC_PROTOCOL m_svcType;
	int m_nPort;
};

class CTinyServerFactory
{
public:
	static ITinyServer* CreateServer(SVC_PROTOCOL svcType);
};

#endif /* TINYSERVER_H_ */
