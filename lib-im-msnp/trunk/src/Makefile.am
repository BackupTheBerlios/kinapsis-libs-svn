AM_CPPFLAGS = $(all_includes)



lib_LTLIBRARIES = libimmsnp.la 

libimmsnp_la_LIBADD = -lqt -lcurl -lpthread
#$(LIB_KPARTS)
libimmsnp_la_LDFLAGS = $(all_libraries) -version-info 0:1:0 -no-undefined
libimmsnp_la_SOURCES = client.cpp msocket.cpp connection.cpp commands.cpp parser.cpp md5.cpp connectionlistener.cpp contact.cpp rosterlistener.cpp presencelistener.cpp chat.cpp chatlistener.cpp roster.cpp buffer.cpp parsernotificationserver.cpp


libimmsnpincludedir = $(includedir)/libimmsnp
libimmsnpinclude_HEADERS = msocket.h

METASOURCES = AUTO
