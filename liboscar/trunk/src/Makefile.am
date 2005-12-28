AM_CPPFLAGS = $(all_includes)

SUBDIRS = . examples

lib_LTLIBRARIES = liboscar.la

liboscar_la_LIBADD = -lqt
#$(LIB_KPARTS)
liboscar_la_LDFLAGS = $(all_libraries) -version-info 0:1:0 -no-undefined
liboscar_la_SOURCES = buffer.cpp capabilities.cpp capabilitiestlv.cpp client.cpp connection.cpp \
		      connectionresult.cpp directconnectiontlv.cpp errortlv.cpp families.cpp \
		      flap.cpp parser.cpp servertlv.cpp snac.cpp snac_location.cpp snac_service.cpp \
		      statustlv.cpp passwordtlv.cpp tlv.cpp uin.cpp

liboscarincludedir = $(includedir)/liboscar
liboscarinclude_HEADERS = buffer.h client.h connection.h connectionresult.h \
			  flap.h parser.h snac.h tlv.h uin.h

METASOURCES = AUTO
