# Heimdall include hServer and hClient #

# Build #
### Linux ###
	$sudo apt-get install  "^libxcb.*" build-essential shtool libtool automake autoconf libbsd-dev libx11-xcb-dev libglu1-mesa-dev libxrender-dev libxi-dev cmake libx11-dev yasm libxext-dev libgl1-mesa-dev  zlib1g-dev freeglut3-dev uuid-dev
	$source ./rules.mk
	$make
