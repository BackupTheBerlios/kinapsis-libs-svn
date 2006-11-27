AM_CPPFLAGS = $(all_includes)

lib_LTLIBRARIES = libimmsnp.la 

libimmsnp_la_LIBADD = -lqt -lcurl -lpthread
#$(LIB_KPARTS)
libimmsnp_la_LDFLAGS = $(all_libraries) -version-info 0:1:0 -no-undefined
libimmsnp_la_SOURCES = client.cpp msocket.cpp md5.cpp connectionlistener.cpp buffer.cpp parsernotificationserver.cpp connection.cpp login.cpp


libimmsnpincludedir = $(includedir)/libimmsnp
libimmsnpinclude_HEADERS = msocket.h

METASOURCES = AUTO
