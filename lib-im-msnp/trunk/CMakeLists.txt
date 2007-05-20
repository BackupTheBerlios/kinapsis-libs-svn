# The name of our project is "HELLO".  CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.
PROJECT(LibImMsnP)

CMAKE_MINIMUM_REQUIRED(VERSION 2.4.6) 
FIND_PACKAGE(Qt4 REQUIRED)
FIND_PACKAGE(OpenSSL REQUIRED)
#FIND_PACKAGE(Crypto REQUIRED)

#IF(NOT OpenSSL_FOUND)
## I'll want to do a version check at some point.
#  MESSAGE(FATAL_ERROR "OpenSSL is missing")
#ENDIF(NOT OpenSSL_FOUND)

# Recurse into the "Hello" and "Demo" subdirectories.  This does not actually
# cause another cmake executable to run.  The same process will walk through
# the project's entire directory structure.
SUBDIRS(src)
