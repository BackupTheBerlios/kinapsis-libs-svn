PROJECT(IMPROGRAMS)

SET(IMPROGRAMS_SRCS
	test.cpp
	manager.cpp
	#improgram.cpp
	kopete.cpp
	#	pidgin.cpp
	#	psi.cpp
	#	sim.cpp
	#	amsn.cpp
	#	mercury.cpp
)

SET(IMPROGRAMS_HDRS
        manager.h
        improgram.h
        kopete.h
	#  pidgin.h
      	#  amsn.h
      	#  psi.h
      	#  sim.h
      	#  mercury.h
)

FIND_PACKAGE(Qt4 REQUIRED)

INCLUDE( ${QT_USE_FILE} )

INCLUDE_DIRECTORIES(
        ${CMAKE_CURRENT_BINARY_DIR}
	${QT_QTXML_INCLUDE_DIR}
)

ADD_EXECUTABLE(test ${IMPROGRAMS_SRCS} ${IMPROGRAMS_HDRS})

TARGET_LINK_LIBRARIES(test  
	${QT_QTXML_LIBRARY}
)

