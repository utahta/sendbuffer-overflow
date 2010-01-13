/**
 * stream_buffer.cpp
 *
 */
#include "stream_buffer.h"
#include "wrap_syscall.h"
#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <iostream>

CStreamBuffer::CStreamBuffer( size_t initial_size ) : m_buffer(NULL), m_used(0), m_capacity(0), m_offset(0)
{
	m_buffer = static_cast<char *>(::malloc( initial_size ));
	if( m_buffer == NULL ){
		throw std::bad_alloc();
	}
	m_capacity = initial_size;
}

CStreamBuffer::~CStreamBuffer()
{
	free();
}

void CStreamBuffer::free()
{
	if( m_buffer != NULL ){
		::free( m_buffer );
		m_buffer = NULL;
	}
	m_capacity = 0;
	m_used = 0;
	m_offset = 0;
}

void CStreamBuffer::clear()
{
	memset( m_buffer, 0, m_capacity );
	m_used = 0;
	m_offset = 0;
}

int CStreamBuffer::append( char* data, size_t len )
{
	if( m_used + len > m_capacity ){
		//TODO expand
		printf( "need expand(). size:%d capacity:%d", (m_used+len), m_capacity );
	}
	else{
		memcpy( m_buffer+m_used, data, len );
		m_used += len;
	}
	return 0;
}

void CStreamBuffer::compact( size_t size )
{
	if( size >= m_capacity ){
		memset( m_buffer, 0, m_capacity );
		m_used = 0;
		m_offset = 0;
	}
	else{
		memmove( m_buffer, m_buffer+size, m_capacity-size );
		m_used -= (m_used > size) ? size : m_used;
		m_offset -= (m_offset > size) ? size : m_offset;
	}
}

int CStreamBuffer::get( char *buff, size_t size )
{
	if( size > m_used ){
		return -1;
	}
	memcpy( buff, m_buffer, size );
	return 0;
}


int CStreamBuffer::read( int sd, int flag )
{
	if( m_capacity == m_used ){
		//TODO expand
	}

	int res = 0;
	WRAP_SYSCALL( res, recv( sd, static_cast<void *>(m_buffer+m_used), m_capacity-m_used, flag ) );
	if( res == 0 ){
		printf( "EOF. sd:%d\n", sd );
		return -1;
	}

	if( res < 0 ){
		if( errno == EAGAIN ||
			errno == EWOULDBLOCK ){
			perror( "not ready yet" );
			return 0;
		}
		else{
			perror("recv()");
			return -1;
		}
	}
	m_used += res;
	return 0;
}

int CStreamBuffer::write( int sd, int flag )
{
	int res = 0;
	char* p = m_buffer + m_offset;
	size_t len = m_used - m_offset;
	while( len > 0 ){
		WRAP_SYSCALL( res, send( sd, p, len, flag ) );
		if( res < 0 ){
			if( errno == EAGAIN ||
				errno == EWOULDBLOCK ){
				perror( "not ready yet" );
				return ERR_SEND_FAILED;
			}
			else{
				perror( "couldn't send" );
				return -1;
			}
		}
		len -= res;
		p += res;
		m_offset += res;
	}
	compact( m_offset );
	return 0;
}
