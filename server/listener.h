/**
 * listener.h
 *
 */

#ifndef LISTENER_H_
#define LISTENER_H_

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <event.h>

class CListener
{
public:
	static const int LISTEN_PORT = 12345;

private:
	int m_sd;
	struct sockaddr_in m_sock_in;

public:
	CListener();
	~CListener();

	int getSd(){
		return m_sd;
	}

	bool open();
	void close();

	int accept();

};

#endif /* LISTENER_H_ */
