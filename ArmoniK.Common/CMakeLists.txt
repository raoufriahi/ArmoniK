cmake_minimum_required(VERSION 3.15)
project(ArmoniK.Common)


file(GLOB HEADER  "header/cppkit/*.h" 
                   "header/AVKit/*.h"
                  "header/MediaParser/*.h"
                  "header/Webby/*.h"
                  "header/XSDK/*.h"
                  )
                  
file(GLOB SOURCES "src/AVKit/*.cpp"
                  "src/MediaParser/*.cpp"
                  "src/Webby/*.cpp"
                  "src/XSDK/*.cpp"
                  "src/cppkit/*.cpp"
                  )


include_directories(
    ./header
    ../3rdparty/ffmpeg/
    )
add_definitions(-DLINUX_OS=1)
ADD_LIBRARY( ${PROJECT_NAME} SHARED
              ${SOURCES} ${HEADER})


install(TARGETS ${PROJECT_NAME} 
            LIBRARY DESTINATION lib/
            ARCHIVE DESTINATION lib/)
