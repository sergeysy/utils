TEMPLATE =	lib

CONFIG += c++14
CONFIG += staticlib

INCLUDEPATH += "$$_PRO_FILE_PWD_/include"

TARGET = "$$LIBUTILS"

HEADERS +=	include/logger.hpp \
		include/formathelper.hpp \
		include/enumdefinition.inl \
		include/crc.hpp \
		include/typedefines.hpp \
		include/support.hpp

SOURCES +=	sources/logger.cpp \
		sources/formathelper.cpp \
		sources/crc.cpp \
		sources/support.cpp

CONFIG(debug, debug|release) {
    CONFIGURATION=debug
} else {
    CONFIGURATION=release
}
DESTDIR = "$$ROOT_DIR/build/$$CONFIGURATION/$$TARGET"

OBJECTS_DIR = \""$$DESTDIR/.obj\""

# source code encoding
QMAKE_CXXFLAGS += -finput-charset="UTF-8"
QMAKE_CXXFLAGS += -Werror
QMAKE_CXXFLAGS += -Wall
