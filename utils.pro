TEMPLATE = lib

QT -= gui

CONFIG += staticlib
CONFIG += c++14

INCLUDEPATH += "$$_PRO_FILE_PWD_/include"
win32 {
INCLUDEPATH += $(BOOST_ROOT)
}

TARGET = "$$LIBUTILS"

HEADERS +=	\
		include/Singleton.inl \
                include/CPConverter.h \
		include/CPEnum.h \
		include/logger.hpp \
		include/FormatHelper.h \
		include/Formatter.h \
		include/enumdefinition.inl \
		include/crc.hpp \
		include/typedefines.hpp \
		include/support.hpp \
		include/debug.hpp \
		include/threading.hpp

SOURCES += \
		sources/CPConverter.cpp \
		sources/logger.cpp \
                sources/formathelper.cpp \
		sources/Formatter.cpp \
		sources/crc.cpp \
		sources/support.cpp \
		sources/debug.cpp \
		sources/threading.cpp

win32 {
    INCLUDEPATH += c:/local/boost_1_61_0
    INCLUDEPATH += $(BOOST_ROOT)
}

CONFIG(debug, debug|release) {
    CONFIGURATION=debug
    DEFINES += DEBUG_ON
} else {
    CONFIGURATION=release
}
DESTDIR = "$$ROOT_DIR/$$BUILD_MV3000_DIR/$$CONFIGURATION/$$TARGET"

OBJECTS_DIR = \""$$DESTDIR/.obj\""

LIBS += -lboost_date_time
LIBS += -lboost_thread
LIBS += -lboost_regex

# source code encoding
QMAKE_CXXFLAGS += -finput-charset="UTF-8"
unix {
QMAKE_CXXFLAGS += -Werror

QMAKE_CXXFLAGS += -Wall -fno-strict-aliasing
}
