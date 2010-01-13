/**
 * connection.h
 *
 */

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <unistd.h>
#include "message_pack.h"
#include "stream_buffer.h"

class CConnection
{
private:
	int m_sd;
	CStreamBuffer m_wbuff; ///< write buffer
	CStreamBuffer m_rbuff; ///< read buffer
	CMessagePack m_msg; ///< temporary

public:
	CConnection( int sd=-1 );
	~CConnection();

	int getSd(){
		return m_sd;
	}

	int open( const char* addr, short port );
	void close();

	int send( CMessagePack &msg );
	int resend();
	int recv( MessagePackArray &msg_array );

};


#endif /* CONNECTION_H_ */
