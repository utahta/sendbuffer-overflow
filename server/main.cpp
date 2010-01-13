/**
 * send buffer test server.
 */
#include <stdio.h>
#include <stdlib.h>
#include <event.h>
#include "server.h"

int main()
{
	event_init();

	CServer svr;
	if( !svr.init() ){
		exit(1);
	}

	event_dispatch();

	svr.term();
	return 0;
}

