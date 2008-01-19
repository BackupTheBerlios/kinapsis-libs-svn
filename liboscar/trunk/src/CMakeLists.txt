# the next line sets up include and link directories and defines some variables that we will use.
# you can modify the behavior by setting some variables, e.g.
#   set(QT_USE_OPENGL TRUE)
# -> this will cause cmake to include and link against the OpenGL module
INCLUDE(${QT_USE_FILE})

# the variable "LibImMsnP_SRCS" contains all .cpp files of this project
SET(CMAKE_BUILD_TYPE Debug)

SET(liboscar_SRCS
	avatarlistener.cpp
	avatarprocess.cpp
	buffer.cpp
	capabilities.cpp
	capabilitiestlv.cpp
	client.cpp
	connection.cpp
	connectionlistener.cpp
	connectionresult.cpp
	contact.cpp
	contactavatartlv.cpp
	directconnectiontlv.cpp
	errortlv.cpp
	families.cpp
	flap.cpp
	ftcaptlv.cpp
	ftdata.cpp
	ftstatus.cpp
	filetransferlistener.cpp
	filetransferprocess.cpp
	filereceiveservice.cpp
	filesendservice.cpp
	istypinglistener.cpp
	loginservice.cpp
	loginst1process.cpp
	loginst2process.cpp
	message.cpp
	messagelistener.cpp
	oftheader.cpp
	oftparser.cpp
	oftproxycommand.cpp
	oftproxyparser.cpp
	offlinemessagesprocess.cpp
	oscarconnectionresult.cpp
	oscarparser.cpp
	parser.cpp
	passwordtlv.cpp
	presencelistener.cpp
	presenceprocess.cpp
	#	process.cpp
	roster.cpp
	rosterlistener.cpp
	rosterprocess.cpp
	sblitem.cpp
	servertlv.cpp
	service.cpp
	servicesetupprocess.cpp
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
	snac_ssbi.cpp
	statustlv.cpp
	tlvchain.cpp
	tlv.cpp
	uin.cpp
	uinregistrationlistener.cpp
	uinregistrationservice.cpp
	uinregistrationprocess.cpp
	userinfo.cpp
)

SET(liboscar_HDRS
	avatarlistener.h
	avatarprocess.h
	buffer.h 
	capabilities.h 
	client.h 
	connection.h 
	connectionresult.h 
	contact.h 
	contactavatartlv.h
	connectionlistener.h 
	directconnectiontlv.h 
	families.h 
	filetransferlistener.h
	filetransferprocess.h
	ftdata.h
	ftstatus.h
	istypinglistener.h 
	liboscar.h 
	loginservice.h
	loginst1process.h
	loginst2process.h
	message.h 
	messagelistener.h 
	roster.h
	rosterlistener.h 
	rosterprocess.h 
	service.h
	servicesetupprocess.h
	offlinemessagesprocess.h
	oscarconnectionresult.h
	oscarparser.h 
	parser.h 
	presencelistener.h 
	presenceprocess.h 
	sblitem.h 
	servertlv.h
	snac_bos.h
	snac_contact.h
	snac.h
	snac_icbm.h
	snac_icq.h
	snac_interval.h
	snac_location.h
	snac_newuser.h
	snac_roster.h
	snac_service.h
	tlv.h 
	tlvchain.h 
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
