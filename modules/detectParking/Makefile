CPROG=detectParking
-include ../../common.mk

AS              = $(PRJ_CROSS)as
LD              = $(PRJ_CROSS)ld
CC              = $(PRJ_CROSS)gcc
CPP             = $(PRJ_CROSS)g++
AR              = $(PRJ_CROSS)ar
NM              = $(PRJ_CROSS)nm
STRIP           = $(PRJ_CROSS)strip
OBJCOPY         = $(PRJ_CROSS)objcopy
OBJDUMP         = $(PRJ_CROSS)objdump
RANLIB          = $(PRJ_CROSS)ranlib

ifeq ($(CPROG_OS),)
  ifeq ($(OS),Windows_NT)
    CPROG_OS = WIN32
  else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        CPROG_OS = LINUX
    else
        ifeq ($(UNAME_S),Darwin)
            CPROG_OS = OSX
        else
            CPROG_OS = BSD
        endif
    endif
  endif
endif

OUTPUT     =
#RAPID_EPOLL
CPPFLAGS	= -std=c++11 -DLINUX -DLINUX_OS -D__STDC_CONSTANT_MACROS 
CFLAGS	= -DLINUX -DLINUX_OS -D__STDC_CONSTANT_MACROS 
ifneq ($(strip $(VE_OS)), macos)
CFLAGS += -DRAPID_EPOLL
endif
ifneq ($(strip $(PRJ_CROSS)), )
CFLAGS += -DARM_LINUX
endif

CFLAGS += -fpic
CPPFLAGS += -fpic
PERM = -m 0755


MACHINE := $(shell uname -m)

DEBUG_BUILD	= -g -Wall

IFLAGS 	=	-I./include -I../../3rdparty/opencv/include\
			-I../../3rdparty/opencv/cmake/build\
			-I../../3rdparty/opencv/modules/core/include\
			-I../../3rdparty/opencv/modules/calib3d/include\
			-I../../3rdparty/opencv/modules/features2d/include\
			-I../../3rdparty/opencv/modules/flann/include\
			-I../../3rdparty/opencv/modules/highgui/include\
			-I../../3rdparty/opencv/modules/imgcodecs/include\
			-I../../3rdparty/opencv/modules/videoio/include\
			-I../../3rdparty/opencv/modules/imgproc/include\
			-I../../3rdparty/opencv/modules/ml/include\
			-I../../3rdparty/opencv/modules/objdetect/include\
			-I../../3rdparty/opencv/modules/photo/include\
			-I../../3rdparty/opencv/modules/shape/include\
			-I../../3rdparty/opencv/modules/stitching/include\
			-I../../3rdparty/opencv/modules/superres/include\
			-I../../3rdparty/opencv/modules/video/include\
			-I../../3rdparty/opencv/modules/videostab/include
			

LIB_SOURCES = ./src/main.cpp ./src/ConfigLoad.cpp ./src/Parking.cpp  ./src/utils.cpp

LDFLAGS += -L$(VE_PATH)/output/$(VE_INSTALL_DIR)/lib\
		-lopencv_calib3d -lopencv_core  -lopencv_dnn -lopencv_features2d\
		-lopencv_flann -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_ml -lopencv_objdetect\
		-lopencv_photo -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_videoio -lopencv_video -lopencv_videostab 
LDFLAGS += -fpic -lpthread -ldl -lstdc++ -lm

all:$(CPROG)
$(CPROG):
	$(CPP) $(LIB_SOURCES) -o $(CPROG) $(LDFLAGS) $(CPPFLAGS) $(IFLAGS)
	cp $(CPROG) ../../output/$(VE_INSTALL_DIR)/

clean:distclean 
	rm -f $(CPROG)  
distclean: 
