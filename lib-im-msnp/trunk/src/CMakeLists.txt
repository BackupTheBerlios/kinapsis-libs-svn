# the next line sets up include and link directories and defines some variables that we will use.
# you can modify the behavior by setting some variables, e.g.
#   set(QT_USE_OPENGL TRUE)
# -> this will cause cmake to include and link against the OpenGL module
INCLUDE(${QT_USE_FILE})

# the variable "LibImMsnP_SRCS" contains all .cpp files of this project
SET(LibImMsnP_SRCS
	client.cpp
	msocket.cpp
	buffer.cpp
	cmdlogin.cpp
	parsernotificationserver.cpp
	challenge.cpp
	soap.cpp
	cmdnotificationserver.cpp
	cmdswichboardserver.cpp
	parserswichboardserver.cpp
	contact.cpp
	roster.cpp
	msn.cpp
	connectionlistener.cpp
	presencelistener.cpp
	rosterlistener.cpp
	chatlistener.cpp
)

SET(LibImMsnP_HDRS
	libimmsnp.h
	client.h
	msocket.h
	buffer.h
	command.h
	cmdlogin.h
	parsernotificationserver.h
	challenge.h
	soap.h
	cmdnotificationserver.h
	cmdswichboardserver.h
	parserswichboardserver.h
	contact.h
	roster.h
	msn.h
	connectionlistener.h
	presencelistener.h
	rosterlistener.h
	chatlistener.h
)

FIND_PACKAGE(Qt4 REQUIRED)
FIND_PACKAGE(OpenSSL REQUIRED)
FIND_PACKAGE(CURL REQUIRED)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_BINARY_DIR}
	${QT_INCLUDE_DIR}
	${QT_QTNETWORK_INCLUDE_DIR}
	${OpenSSL_INCLUDE_DIR}
	${CURL_INCLUDE_DIR}
)

# tell cmake to create .moc files for all files in the variable LibImMsnP_SRCS that require such a file.
# note: this assumes that you use #include "header.moc" in your files
QT4_AUTOMOC(${LibImMsnP_SRCS})

# create an executable file named "LibImMsnP" from the source files in the variable "LibImMsnP_SRCS".
ADD_EXECUTABLE(msn ${LibImMsnP_SRCS} ${LibImMsnP_HDRS})

# link the "LibImMsnP" target against the Qt libraries. which libraries exactly, is defined by the "include(${QT_USE_FILE})" line above, which sets up this variable.
TARGET_LINK_LIBRARIES(msn ${QT_LIBRARIES} ${QT_QTNETWORK_LIBRARIES} ${OpenSSL_LIBRARY} ${CURL_LIBRARY})