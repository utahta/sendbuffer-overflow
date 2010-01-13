/**
 * message_pack.cpp
 *
 */
#include "message_pack.h"
#include "wrap_syscall.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <errno.h>

CMessagePack::CMessagePack() : m_status( eSTATUS_INITIALIZE ), m_hpos(0), m_bpos(0), m_blen(0)
{
	memset( m_head, 0, sizeof( m_head ) );
	memset( m_body, 0, sizeof( m_body ) );
}

CMessagePack::~CMessagePack()
{
}

void CMessagePack::init()
{
	m_status = eSTATUS_INITIALIZE;
	memset( m_head, 0, sizeof( m_head ) );
	memset( m_body, 0, sizeof( m_body ) );
	m_hpos = 0;
	m_bpos = 0;
	m_blen = 0;
}

int CMessagePack::packString( char *str, int size )
{
	memset( m_body, 0, sizeof( m_body ) );

	if( size > MAX_BODY_LEN ){
		printf( "In setString(). too large size. size:%d max_len:%d\n", size, MAX_BODY_LEN );
		return -1;
	}
	memcpy( m_body, str, size );
	m_blen = size;
	return 0;
}

int CMessagePack::unpackString( char *str, int size )
{
	if( size > m_blen ){
		printf( "In getString(). too large size. size:%d max_len:%d\n", size, m_blen );
		return -1;
	}
	memcpy( str, m_body, size );
	return 0;
}

int CMessagePack::serialize( CStreamBuffer &s )
{
	char buff[MAX_BUFFER_LEN];
	memset( buff, 0, sizeof( buff ) );
	int buff_size = MAX_HEADER_LEN + m_blen;

	memset( &m_head, 0, sizeof( m_head ) );
	m_head[0] = ((m_blen >> 8) & 0xff);
	m_head[1] = (m_blen & 0x00ff);
	m_head[2] = 't';
	m_head[3] = 'e';
	m_head[4] = 's';
	m_head[5] = 't';

	memcpy( buff, m_head, MAX_HEADER_LEN );
	memcpy( buff+MAX_HEADER_LEN, m_body, m_blen );

	s.append( buff, MAX_HEADER_LEN+m_blen );
	return 0;
}

int CMessagePack::deserialize( CStreamBuffer &s )
{
	if( m_status != eSTATUS_HEADER_READ &&
		m_status != eSTATUS_BODY_READ ){
		init();
		m_status = eSTATUS_HEADER_READ;
	}

	int res = 0;
	if( m_status == eSTATUS_HEADER_READ ){
		if( s.used() < MAX_HEADER_LEN ){
			return 0;
		}
		s.get( m_head, MAX_HEADER_LEN );
		s.compact( MAX_HEADER_LEN );

		m_blen = static_cast<short>(((m_head[0] & 0xff) << 8) | (m_head[1] & 0xff));
		char tmp[5];
		tmp[0] = m_head[2];
		tmp[1] = m_head[3];
		tmp[2] = m_head[4];
		tmp[3] = m_head[5];
		tmp[4] = '\0';
		if( strcmp( "test", tmp ) != 0 ){
			printf( "invalid hash. tmp:%s\n", tmp );
			return -1;
		}

		if( m_blen > MAX_BODY_LEN ){
			printf( "body is too large. blen:%d\n", m_blen );
			return -1;
		}
		m_status = eSTATUS_BODY_READ;
	}

	if( m_status == eSTATUS_BODY_READ ){
		if( s.used() < m_blen ){
			return 0;
		}
		s.get( m_body, m_blen );
		s.compact( m_blen );

		m_status = eSTATUS_COMPLETE;
	}
	return 0;
}

