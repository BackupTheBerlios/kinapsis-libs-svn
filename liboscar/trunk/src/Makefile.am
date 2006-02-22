AM_CPPFLAGS = $(all_includes)

SUBDIRS = .

lib_LTLIBRARIES = liboscar.la

liboscar_la_LIBADD = -lqt
#$(LIB_KPARTS)
liboscar_la_LDFLAGS = $(all_libraries) -version-info 0:1:0 -no-undefined
liboscar_la_SOURCES = buffer.cpp capabilities.cpp capabilitiestlv.cpp \
		      client.cpp connection.cpp connectionlistener.cpp connectionresult.cpp \
		      contact.cpp directconnectiontlv.cpp errortlv.cpp families.cpp \
		      flap.cpp istypinglistener.cpp message.cpp messagelistener.cpp \
		      parser.cpp roster.cpp rosterlistener.cpp servertlv.cpp snac.cpp \
		      snac_bos.cpp snac_contact.cpp snac_icbm.cpp snac_icq.cpp \
		      snac_interval.cpp snac_newuser.cpp snac_location.cpp snac_roster.cpp \
		      snac_service.cpp statustlv.cpp passwordtlv.cpp presencelistener.cpp \
		      tlv.cpp tlvchain.cpp sblitem.cpp uin.cpp uinregistrationlistener.cpp \
		      userinfo.cpp

liboscarincludedir = $(includedir)/liboscar
liboscarinclude_HEADERS = buffer.h capabilities.h client.h connection.h connectionresult.h contact.h \
			  connectionlistener.h directconnectiontlv.h families.h \
			  istypinglistener.h liboscar.h message.h messagelistener.h roster.h \
			  rosterlistener.h parser.h presencelistener.h sblitem.h tlv.h uin.h \
			  uinregistrationlistener.h userinfo.h

METASOURCES = AUTO
