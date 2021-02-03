# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

TEMPLATE = lib
TARGET = veuilib
CONFIG += plugin c++11
QMAKE_CXXFLAGS += -std=c++11 -O2 -Wall -Wextra -pedantic -g -include utility/type.hpp 
DESTDIR = ./
QT += core sql network xml  widgets gui printsupport concurrent
CONFIG += debug
DEFINES += USE_WEBSOCKET PROTOBUF_USE_DLLS LINUX_OS QT_DLL QT_NETWORK_LIB QT_HELP_LIB QT_OPENGL_LIB QT_SCRIPT_LIB QT_WIDGETS_LIB QT_XML_LIB QT_XMLPATTERNS_LIB VEUILIB_LIB WITH_OPENSSL WITH_DOM WITH_PURE_VIRTUAL VE_EXPORTS
#linux-cross-g++ {
# DEFINES +=ARM_LINUX
#}
INCLUDEPATH += ./GeneratedFiles \
    . \
    ./GeneratedFiles/Debug \
    ./../onvifcpplib/include \
    ./../../velib/include/utility \
    ./../../velib/include/vdb \
    ./../../velib/include/vplay \
    ./../../velib/include \
    ./../onvifcpplib/onvifgen \
    ./../onvifcpplib/gsoap/gsoap/runtime \
    ./../../3rdparty/openssl/include \
    ./../../include \
    ./../../3rdparty/leveldb-posix/include \
    ./../../3rdparty/poco/Data/include \
    ./../../3rdparty/poco/Data/SQLite/include \
    ./../../3rdparty/poco/Foundation/include \
    ./../../3rdparty/poco/Net/include \
    ./../../3rdparty/poco/Util/include \
    ./../../3rdparty/onvifc/include \
    ./../../veuilib/rtspserver/include \
    ./../../3rdparty/civetweb/include \
    ./../include \
    ./../src \
    ./../src/vvidwidget \
    ./../src/cmnui \
    ./../src/vvidpb \
    ./../src/emap \
    ./../../xcmnlib/include/utility \
    ./../../xcmnlib/include \
    ./../../oapi \
    ./../../3rdparty/autojsoncxx/include \
    ./../../3rdparty/autojsoncxx/rapidjson/include \
    ./../../3rdparty/ffmpeg \
    ./../../include \
    ./../../3rdparty/protobuf/src \
    ./../../3rdparty/spdlog/include\
    ../../output/$$(VE_INSTALL_DIR)/include/QtCore\
    ../../output/$$(VE_INSTALL_DIR)/include/QtGui\
    ../../output/$$(VE_INSTALL_DIR)/include/QtWidgets\
    ../../output/$$(VE_INSTALL_DIR)/include/QtXml\
    ../../output/$$(VE_INSTALL_DIR)/include/QtNetwork\
    ../../output/$$(VE_INSTALL_DIR)/include/QtXmlPatterns

LIBS += \
    -L"./../../3rdparty/leveldb/" \
    -L"./Debug" \
    -L"./../../3rdparty/openssl-linux/" \
    -L"./../../3rdparty/onvifc/prj_linux/lib" \
    -L"./../../3rdparty/leveldb/" \
    -L"./../../veuilib/rtspserver/lib/linux" \
    -L"./../onvifcpplib/lib/linux/" \
    -L../../output/$$(VE_INSTALL_DIR)/lib \
    -L../../output/$$(VE_INSTALL_DIR)/lib/x86_64-linux-gnu \
    -lcmnlib\
    -lavcodec \
    -lavformat \
    -lavutil \
    -lswscale \
    -lleveldb \
    -lvelib \
    -lonvifcpplib \
    -lPocoFoundation\
    -lPocoDataSQLite\
    -lPocoData\
    -lssl \
    -lprotobuf \
    -lcrypto\
    -ldl

ARM_LINUX {
DEFINES +=ARM_LINUX
}else
{
    unix:!macx {
        LIBS += \

    }
}


DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(../prj_linux/veuilib.pri)
SOURCES += ../../include/config/linksystem.pb.cc
SOURCES += ../../include/config/linkproto.pb.cc
SOURCES += ../../include/config/vidconf.pb.cc
SOURCES += ../../include/config/restsystem.pb.cc
