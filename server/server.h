/**
 * server.h
 *
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <event.h>
#include "listener.h"

class CServer
{
private:
	struct event m_ev;
	CListener m_listener;

public:
	CServer();
	~CServer();

	bool init();
	void term();

	static void accept( int fd, short flag, void *arg );

};

#endif /* SERVER_H_ */
