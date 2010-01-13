/**
 * listener.cpp
 *
 */
#include "listener.h"
#include <string.h>

CListener::CListener()
{
}

CListener::~CListener()
{
}

bool CListener::open()
{
    m_sd = socket( PF_INET, SOCK_STREAM, 0 );
    if( m_sd < 0 ) {
    	return false;
    }

    memset( &m_sock_in, 0, sizeof( m_sock_in ) );
    m_sock_in.sin_family = AF_INET;
    m_sock_in.sin_port = htons( LISTEN_PORT );
    m_sock_in.sin_addr.s_addr = INADDR_ANY;

    int so_opt = 1;
    setsockopt( m_sd, SOL_SOCKET, SO_REUSEADDR, (const char *)&so_opt, sizeof( so_opt ) );

    if( bind( m_sd, (struct sockaddr *) &m_sock_in, sizeof( m_sock_in ) ) < 0 ){
    	return false;
    }

    if( listen( m_sd, 128 ) < 0 ){
    	return false;
    }
	return true;
}

void CListener::close()
{
	::close( m_sd );
}

int CListener::accept()
{
    struct sockaddr_in cs_in;
    socklen_t len = sizeof( cs_in );
    return ::accept( m_sd, (struct sockaddr *) &cs_in, &len );
}

