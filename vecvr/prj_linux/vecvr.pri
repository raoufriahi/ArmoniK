# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

# This is a reminder that you are using a generated .pro file.
# Remove it when you are finished editing this file.
message("You are running qmake on a generated .pro file. This may not work!")


HEADERS += ../main/link/linkstream.hpp \
    ../prj_win32/resource.h
SOURCES += ../main/main.cpp \
    ../main/webserver.cpp \
	../main/mediaserver.cpp \
    ../../3rdparty/civetweb/src/CivetServer.cpp \
    ../../3rdparty/civetweb/src/civetweb.c \
    ../main/link/linkhandler.cpp \
    ../main/link/linkstream.cpp
TRANSLATIONS += ../main/translations/opencvr_zh.ts
RESOURCES += ../main/vscmainwindows.qrc