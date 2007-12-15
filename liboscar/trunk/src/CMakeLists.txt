# the next line sets up include and link directories and defines some variables that we will use.
# you can modify the behavior by setting some variables, e.g.
#   set(QT_USE_OPENGL TRUE)
# -> this will cause cmake to include and link against the OpenGL module
INCLUDE(${QT_USE_FILE})

# the variable "LibImMsnP_SRCS" contains all .cpp files of this project
SET(liboscar_SRCS
	buffer.cpp
	capabilities.cpp
	capabilitiestlv.cpp
	client.cpp
	connection.cpp
	connectionlistener.cpp
	connectionresult.cpp
	contact.cpp
	directconnectiontlv.cpp
	errortlv.cpp
	families.cpp
	flap.cpp
	ftdata.cpp
	istypinglistener.cpp
	message.cpp
	messagelistener.cpp
	#	oftparser.cpp
	parserbase.cpp
	parser.cpp
	passwordtlv.cpp
	presencelistener.cpp
	roster.cpp
	rosterlistener.cpp
	sblitem.cpp
	servertlv.cpp
	snac_bos.cpp
	snac_contact.cpp
	snac.cpp
	snac_icbm.cpp
	snac_icq.cpp
	snac_interval.cpp
	snac_location.cpp
	snac_newuser.cpp
	snac_roster.cpp
	snac_service.cpp
	statustlv.cpp
	tlvchain.cpp
	tlv.cpp
	uin.cpp
	uinregistrationlistener.cpp
	userinfo.cpp
)

SET(liboscar_HDRS
	buffer.h 
	capabilities.h 
	client.h 
	connection.h 
	connectionresult.h 
	contact.h 
	connectionlistener.h 
	directconnectiontlv.h 
	families.h 
	ftdata.h
	istypinglistener.h 
	liboscar.h 
	message.h 
	messagelistener.h 
	roster.h
	rosterlistener.h 
	parserbase.h 
	parser.h 
	presencelistener.h 
	sblitem.h 
	tlv.h 
	uin.h
	uinregistrationlistener.h 
	userinfo.h
)

FIND_PACKAGE(Qt4 REQUIRED)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_BINARY_DIR}
	${QT_INCLUDE_DIR}
	${QT_QTNETWORK_INCLUDE_DIR}
)

# tell cmake to create .moc files for all files in the variable LibImMsnP_SRCS that require such a file.
# note: this assumes that you use #include "header.moc" in your files
QT4_AUTOMOC(${liboscar_SRCS})

# Enable the instalation of the library
ADD_LIBRARY(oscar SHARED ${liboscar_SRCS})
INSTALL(TARGETS oscar DESTINATION ${CMAKE_INSTALL_PREFIX}/lib )
INSTALL(FILES ${liboscar_HDRS} DESTINATION
	${CMAKE_INSTALL_PREFIX}/include/liboscar )
