/**
 * stream_buffer.h
 *
 */

#ifndef STREAM_BUFFER_H_
#define STREAM_BUFFER_H_

#include <stdlib.h>

const int ERR_SEND_FAILED = -10;

class CStreamBuffer
{
private:
	char* m_buffer;
	size_t m_used;
	size_t m_capacity;
	size_t m_offset;

public:
	CStreamBuffer( size_t initial_size = 1024*1024 );
	~CStreamBuffer();

	size_t used(){ return m_used; }

	void free();
	void clear();
	int append( char* data, size_t len );
	void compact( size_t size );
	int get( char *buff, size_t size );

	int read( int sd, int flag );
	int write( int sd, int flag );

};

#endif /* STREAM_BUFFER_H_ */
