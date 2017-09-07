TEMPLATE = lib

QT -= core gui

CONFIG += staticlib
CONFIG += c++14

INCLUDEPATH += "$$_PRO_FILE_PWD_/include"

TARGET = "$$LIBUTILS"

HEADERS +=	include/logger.hpp \
		include/formathelper.hpp \
		include/enumdefinition.inl \
		include/crc.hpp \
		include/typedefines.hpp \
		include/support.hpp \
		include/debug.hpp \
		include/threading.hpp

SOURCES +=	sources/logger.cpp \
		sources/formathelper.cpp \
		sources/crc.cpp \
		sources/support.cpp \
		sources/debug.cpp \
		sources/threading.cpp

win32 {
    INCLUDEPATH += c:/local/boost_1_61_0
}

CONFIG(debug, debug|release) {
    CONFIGURATION=debug
    DEFINES += DEBUG_ON
} else {
    CONFIGURATION=release
}
DESTDIR = "$$ROOT_DIR/build/$$CONFIGURATION/$$TARGET"

OBJECTS_DIR = \""$$DESTDIR/.obj\""

LIBS += -lboost_date_time
LIBS += -lboost_thread
LIBS += -lboost_regex

# source code encoding
QMAKE_CXXFLAGS += -finput-charset="UTF-8"
QMAKE_CXXFLAGS += -Werror
QMAKE_CXXFLAGS += -Wall
