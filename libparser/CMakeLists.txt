PROJECT(IMPROGRAMS)

SET(IMPROGRAMS_SRCS
	test.cpp
	manager.cpp
	parserkopete.cpp
	parserpidgin.cpp
	parserpsi.cpp
	parseramsn.cpp
	parsermercury.cpp
)

SET(IMPROGRAMS_HDRS
        manager.h
        improgram.h
        parserkopete.h
	parserpidgin.h
      	parserpsi.h
      	parseramsn.h
      	parsermercury.h
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

