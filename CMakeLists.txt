cmake_minimum_required(VERSION 3.15)
project(UbCore)
set(CMAKE_CXX_STANDARD 11)
execute_process(COMMAND lsb_release -si OUTPUT_VARIABLE CA_OS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND uname -m COMMAND sed "s/x86_//;s/i[3-6]86/32/" OUTPUT_VARIABLE CA_ARCH OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND lsb_release -sr OUTPUT_VARIABLE CA_VER OUTPUT_STRIP_TRAILING_WHITESPACE)
set(CA_INSTALL_DIR "${CA_OS}-${CA_VER}-${CA_ARCH}bit")
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/output/${CA_INSTALL_DIR})
add_definitions( -fPIC                 -DUSE_WEBSOCKET 
                -DLINUX_OS             -DQT_DLL 
                -DQT_MULTIMEDIA_LIB    -DQT_MULTIMEDIAWIDGETS_LIB 
                -DQT_NETWORK_LIB       -DQT_QML_LIB 
                -DQT_QUICK_LIB         -DQT_SQL_LIB 
                -DQT_CONCURRENT_LIB    -DQT_HELP_LIB 
                -DQT_PRINTSUPPORT_LIB  -DQT_SCRIPT_LIB 
                -DQT_SVG_LIB           -DQT_WEBKITWIDGETS_LIB 
                -DQT_WIDGETS_LIB       -DQT_XML_LIB 
                -DQT_XMLPATTERNS_LIB   -DQT_NO_BLUEZ 
                -DQT_NO_UDEV           -DWITH_OPENSSL 
                -DWITH_DOM             -DWITH_PURE_VIRTUAL
                )
#add_definitions(-DENABLE_RTMP2RTSP -DENABLE_RTSP2RTMP -DENABLE_HLS)
add_subdirectory(xcmnlib)
add_subdirectory(vecvr)
add_subdirectory(onvif)
