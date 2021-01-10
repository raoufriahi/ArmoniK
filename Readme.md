# Heimdall include hServer and hClient #

# Build #
### Linux ###
	ubuntu: sudo apt-get install  "^libxcb.*" build-essential shtool libtool automake autoconf libbsd-dev libx11-xcb-dev libglu1-mesa-dev libxrender-dev libxi-dev cmake libx11-dev yasm libxext-dev libgl1-mesa-dev  zlib1g-dev freeglut3-dev uuid-dev
	centos 7: sudo yum install nasm xorg-x11-server-devel zlib-devel gcc gcc-c++ perl-version libxcb libxcb-devel xcb-util xcb-util-devel xcb-util-*-devel libX11-devel libXrender-devel libXi-devel redhat-lsb-core libxslt-devel cmake libuuid-devel
	
	Change VE_PATH in the rules.mk
	$source ./rules.mk
	$make
