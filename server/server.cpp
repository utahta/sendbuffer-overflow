/**
 * server.cpp
 *
 */
#include "server.h"
#include "client.h"
#include <stdio.h>
#include <string.h>

CServer::CServer()
{
}

CServer::~CServer()
{
}

bool CServer::init()
{
	if( !m_listener.open() ){
		printf( "initialize failed." );
		return false;
	}
	event_set( &m_ev, m_listener.getSd(), EV_READ | EV_PERSIST, CServer::accept, this );
	event_add( &m_ev, NULL );
	return true;
}

void CServer::term()
{
	event_del( &m_ev );
	memset( &m_ev, 0, sizeof( m_ev ) );
	m_listener.close();
}

void CServer::accept( int fd, short flag, void *arg )
{
	CListener &listener = static_cast<CServer *>(arg)->m_listener;
	int sd = listener.accept();
	if( sd < 0 ){
		perror( "accept()" );
		return;
	}

	CClient *p = new CClient( sd );
	p->addReadEvent();

	printf( "connected. sd:%d\n", sd );
}

