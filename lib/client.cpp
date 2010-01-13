/**
 * client.cpp
 *
 */
#include <stdio.h>
#include <string.h>
#include <event.h>
#include <linux/sockios.h>
#include <sys/ioctl.h>
#include "client.h"

CClient::CClient( int sd ) : m_con( sd )
{
}

CClient::~CClient()
{
}

void CClient::term()
{
	close();
	removeEvent();
}

int CClient::addReadEvent()
{
	event_set( &m_ev, m_con.getSd(), EV_READ | EV_PERSIST, CClient::callback, this );
	return event_add( &m_ev, NULL );
}

int CClient::addWriteEvent()
{
	// When the send-buffer overflows, this function is called.
	event_set( &m_wev, m_con.getSd(), EV_WRITE, CClient::callback, this );
	return event_add( &m_wev, NULL );
}

void CClient::removeEvent()
{
	if( event_initialized( &m_ev ) ){
		event_del( &m_ev );
	}
	memset( &m_ev, 0, sizeof( m_ev ) );
}

int CClient::open( const char *addr, short port )
{
	return m_con.open( addr, port );
}

void CClient::close()
{
	m_con.close();
}

int CClient::send( CMessagePack &msg )
{
	int res = m_con.send( msg );

	int value;
	ioctl( m_con.getSd(), SIOCOUTQ, &value );
	printf( "send buffer size:%d res:%d sd:%d\n", value, res, m_con.getSd() );

	if( res == ERR_SEND_FAILED ){
		addWriteEvent();
	}
	return res;
}

int CClient::resend()
{
	int res = m_con.resend();

	int value;
	ioctl( m_con.getSd(), SIOCOUTQ, &value );
	printf( "resend buffer size:%d res:%d sd:%d\n", value, res, m_con.getSd() );

	if( res == ERR_SEND_FAILED ){
		addWriteEvent();
	}
	return res;
}

int CClient::recv()
{
#if 1
	// debug code.
	int value = 0;
	ioctl( m_con.getSd(), SIOCINQ, &value );
	printf( "recv buffer size:%d sd:%d\n", value, m_con.getSd() );
	sleep(3);
#endif

	MessagePackArray msg_array;
	int res = m_con.recv( msg_array );
	if( res < 0 ){
		term();
		return res;
	}

	static int count = 0;
	for( MessagePackArray::iterator it = msg_array.begin(); it != msg_array.end(); ++it ){
		CMessagePack &msg = (*it);
		++count;
		char buff[128];
		msg.unpackString( buff, sizeof( buff ) );
		printf( "message received. sd:%d count:%d str:%s\n", m_con.getSd(), count, buff );
	}
	return 0;
}

void CClient::callback( int fd, short flag, void *arg )
{
	CClient *client = static_cast<CClient *>(arg);
	if( client == NULL ){
		printf( "client is null.\n" );
		::close( fd );
		return;
	}

	if( flag & EV_READ ){
		client->recv();
	}
	else if( flag & EV_WRITE ){
		client->resend(); // writing is possible.
	}
}
