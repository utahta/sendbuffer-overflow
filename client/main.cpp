/**
 * send buffer test client.
 */
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h> 
#include <event.h>
#include "message_pack.h"
#include "client.h"

#define CONNECT_ADDR "127.0.0.1"
#define CONNECT_PORT (12345)

void writeev_callback( int fd, short flag, void *arg )
{
	printf( "Write callback!\n" );
	CClient *client = static_cast<CClient *>(arg);

	if( flag & EV_WRITE ){
		char str[128];
    	CMessagePack msg;
		msg.init();
		msg.packString( str, sizeof( str ) );
		client->send( msg );
	}
}

void oneshot_send( int fd, short flag, void *arg )
{
	CClient *client = static_cast<CClient *>(arg);

	int so_opt = 0;
    socklen_t so_len = sizeof( so_opt );
	if( getsockopt( client->getSd(), SOL_SOCKET, SO_SNDBUF, &so_opt, &so_len ) != 0 ){
		printf( "Failed to get SO_SNDBUF option to the socket. %s. %d bytes", strerror(errno), so_opt );
    }
    printf( "send buffer size. :%d\n", so_opt );

    char str[128];
//    printf( "press enter:" );
//    fgets( str, sizeof(str), stdin );

    // create data.
    memset( str, 0, sizeof( str ) );
    for( int i = 0; i < sizeof( str )-1; ++i ){
    	str[i] = 'a'+ (i % 26);
    }

//    for( int i = 0; i < (128*1024); ++i ){
    for( int i = 0; i < 500; ++i ){
    	CMessagePack msg;
		msg.init();
		msg.packString( str, sizeof( str ) );
		client->send( msg );
    }
    client->addReadEvent();
}

int main(void)
{
	event_init();

	CClient *client = new CClient;
	if( client->open( CONNECT_ADDR, CONNECT_PORT ) < 0 ){
		return 0;
	}

    struct timeval t;
    timerclear( &t );
    struct event ev;
    evtimer_set( &ev, oneshot_send, client );
    evtimer_add( &ev, &t );

    event_dispatch();
    return 0;
}

