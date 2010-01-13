AM_CPPFLAGS = -I$(top_srcdir)/lib \
               @SBT_LIBEVENT_INCLUDE@
LIBS = @SBT_LIBEVENT_LIBS@

bin_PROGRAMS = sbt_server
sbt_server_SOURCES = main.cpp \
                      listener.cpp listener.h \
                      server.cpp server.h
                      
sbt_server_LDADD = $(top_srcdir)/lib/libcore.la