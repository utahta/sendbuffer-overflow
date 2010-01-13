AM_CPPFLAGS = -I$(top_srcdir)/lib \
               @SBT_LIBEVENT_INCLUDE@
LIBS = @SBT_LIBEVENT_LIBS@

bin_PROGRAMS = sbt_client
sbt_client_SOURCES = main.cpp

sbt_client_LDADD = $(top_srcdir)/lib/libcore.la