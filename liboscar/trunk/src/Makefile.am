AM_CPPFLAGS = $(all_includes)

lib_LTLIBRARIES = liboscar.la

liboscar_la_LIBADD = -lqt
#$(LIB_KPARTS)
liboscar_la_LDFLAGS = $(all_libraries) -version-info 0:1:0 -no-undefined
liboscar_la_SOURCES = buffer.cpp client.cpp connection.cpp connectionresult.cpp \
		      flap.cpp parser.cpp snac.cpp tlv.cpp uin.cpp

liboscarincludedir = $(includedir)/liboscar
liboscarinclude_HEADERS = buffer.h client.h connection.h connectionresult.h \
			  flap.h parser.h snac.h tlv.h uin.h

METASOURCES = AUTO
