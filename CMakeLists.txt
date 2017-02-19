#=============================================================================#
# Author: Gregory DEPUILLE                                                    #
# Date:   18/02/2017                                                          #
# Description: Build du bridge par socket pour le RPLidar                     #
#=============================================================================#
# Contrôle de la version minimale de CMake
cmake_minimum_required(VERSION 3.4)

# Compilateur flags
#set(ARDUINO_C_FLAGS      "-ffunction-sections -fdata-sections")
#set(ARDUINO_CXX_FLAGS    "${ARDUINO_C_FLAGS} -fno-exceptions")
#set(ARDUINO_LINKER_FLAGS "-Wl,--gc-sections")

#set(ARDUINO_CXX_FLAGS    "-std=c++11")

#====================================================================#
#  Setup Project                                                     #
#====================================================================#
project(RPLidarBridge)

# Récupération des sources
file(GLOB SRCS "src/*.cpp")
file(GLOB HEADERS "include/*.h")

add_executable(rplidar-bridge ${SRCS} ${HEADERS})
