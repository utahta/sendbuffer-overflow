/**
 * connection.cpp
 *
 */
#include "connection.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

CConnection::CConnection( int sd ) : m_sd( sd )
{
}

CConnection::~CConnection()
{
}

int CConnection::open( const char* addr, short port )
{
    int ret = 0, sock_len = 0;
    struct sockaddr_in sock_in;

    if( ( m_sd = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 ){
        perror( "socket()" );
        return -1;
    }

    memset( &sock_in, 0, sizeof( sock_in ) );
    sock_in.sin_addr.s_addr = inet_addr( addr );
    sock_in.sin_family = AF_INET;
    sock_in.sin_port = htons( port );

    sock_len = sizeof( sock_in );
    if( ( ret = connect( m_sd, (struct sockaddr *)&sock_in, sock_len ) ) < 0 ){
        perror( "connect()" );
        return -1;
    }
    int flags = fcntl( m_sd, F_GETFL );
    if( flags >= 0 ){
    	fcntl( m_sd, F_SETFL, flags |= O_NONBLOCK );
    }
	return 0;
}

void CConnection::close()
{
	::close( m_sd );
	m_sd = -1;
}

int CConnection::send( CMessagePack &msg )
{
	msg.serialize( m_wbuff );
	return m_wbuff.write( m_sd, MSG_DONTWAIT );
}

int CConnection::resend()
{
	return m_wbuff.write( m_sd, MSG_DONTWAIT );
}

int CConnection::recv( MessagePackArray &msg_array )
{
	int res = m_rbuff.read( m_sd, MSG_DONTWAIT );
	if( res < 0 ){
		return res;
	}
	while( true ){
		res = m_msg.deserialize( m_rbuff );
		if( res < 0 ){
			return res;
		}

		if( m_msg.completed() ){
			msg_array.push_back( m_msg );
		}
		else{
			break;
		}
	}
	return 0;
}
