/**
 * client.h
 *
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#include "connection.h"
#include <event.h>

const int TEST_MSGLEN = 256;

class CClient
{
private:
	CConnection m_con;
	struct event m_rev;
	struct event m_wev;

public:
	CClient( int sd=-1 );
	~CClient();

	int getSd(){
		return m_con.getSd();
	}

	void term();

	int addReadEvent();
	int addWriteEvent();
	void removeEvent();
	void removeReadEvent();
	void removeWriteEvent();

	int open( const char *addr, short port );
	void close();

	int send( CMessagePack &msg );
	int resend();
	int recv();

	static void callback( int fd, short flag, void *arg );

};

#endif /* CLIENT_H_ */
