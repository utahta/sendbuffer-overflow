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
	struct event m_ev;

public:
	CClient( int sd=-1 );
	~CClient();

	int getSd(){
		return m_con.getSd();
	}

	void term();

	int addEvent();
	int updateEvent( int flags );
	void removeEvent();

	int open( const char *addr, short port );
	void close();

	int send( CMessagePack &msg );
	int resend();
	int recv();

	static void callback( int fd, short flag, void *arg );

};

#endif /* CLIENT_H_ */
