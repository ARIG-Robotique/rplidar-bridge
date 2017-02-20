#=============================================================================#
# Author: Gregory DEPUILLE                                                    #
# Date:   18/02/2017                                                          #
# Description: Build du bridge par socket pour le RPLidar                     #
#=============================================================================#
# Contrôle de la version minimale de CMake
cmake_minimum_required(VERSION 3.4)

# Variable de build
set(RPLIDAR_SDK_DIR    "download/rplidar_sdk/sdk")

#====================================================================#
#  Setup Project                                                     #
#====================================================================#
project(RPLidarBridge)

# Récupération de la définition de la lib externe RPLidar
include_directories(${RPLIDAR_SDK_DIR}/sdk/include/)
link_directories(${RPLIDAR_SDK_DIR}/output/Linux/Release)

# Récupération des sources du projets
file(GLOB SRCS "src/*.cpp")
file(GLOB HEADERS "include/*.h")

add_executable(rplidar-bridge ${SRCS} ${HEADERS})
target_link_libraries(rplidar-bridge rplidar_sdk pthread)
