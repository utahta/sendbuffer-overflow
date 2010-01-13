AM_CPPFLAGS = @SBT_LIBEVENT_INCLUDE@
LIBS = @SBT_LIBEVENT_LIBS@

lib_LTLIBRARIES = libcore.la
libcore_la_SOURCES = connection.cpp connection.h\
                     message_pack.cpp message_pack.h\
                     client.cpp client.h\
                     stream_buffer.cpp stream_buffer.h\
                     wrap_syscall.h
