/**
 * message_pack.h
 *
 */

#ifndef MESSAGE_PACK_H_
#define MESSAGE_PACK_H_

#include "stream_buffer.h"
#include <vector>

class CMessagePack
{
public:
	static const int MAX_BUFFER_LEN = 1024;
	static const int MAX_HEADER_LEN = 18; // body_size(2) + hash(16)
	static const int MAX_BODY_LEN = (MAX_BUFFER_LEN - MAX_HEADER_LEN);

private:
	enum{
		eSTATUS_INITIALIZE,
		eSTATUS_HEADER_READ,
		eSTATUS_BODY_READ,
		eSTATUS_COMPLETE,
	};

	int m_status;
	char m_head[MAX_HEADER_LEN+1];
	int m_hpos;
	char m_body[MAX_BODY_LEN+1];
	int m_bpos;
	int m_blen;

public:
	CMessagePack();
	~CMessagePack();

	bool completed(){
		return m_status == eSTATUS_COMPLETE;
	}

	void init();
	int packString( char *str, int size );
	int unpackString( char *str, int size );

	int serialize( CStreamBuffer &s );
	int deserialize( CStreamBuffer &s );

};
typedef std::vector<CMessagePack> MessagePackArray;

#endif /* MESSAGE_PACK_H_ */
