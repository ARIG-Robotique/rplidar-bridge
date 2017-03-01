#=============================================================================#
# Author: Gregory DEPUILLE                                                    #
# Date:   18/02/2017                                                          #
# Description: Build du bridge par socket pour le RPLidar                     #
#=============================================================================#
# Contrôle de la version minimale de CMake
cmake_minimum_required(VERSION 3.2.2)

# Variable de build
set(CMAKE_CXX_STANDARD 11)
set(RPLIDAR_SDK_DIR    "download/rplidar_sdk/sdk")
set(JSON_LIB_DIR       "download/json")

#====================================================================#
#  Setup Project                                                     #
#====================================================================#
project(rplidar_bridge)

# Récupération de la définition de la lib externe RPLidar
include_directories(${RPLIDAR_SDK_DIR}/sdk/include/)
link_directories(${RPLIDAR_SDK_DIR}/output/Linux/Release)

# Récupération de la définition pour JSON
include_directories(${JSON_LIB_DIR}/src/)

# Récupération des sources du projets
file(GLOB SRCS "src/*.cpp")
file(GLOB HEADERS "src/*.h")

add_executable(${PROJECT_NAME} ${SRCS} ${HEADERS})
target_link_libraries(${PROJECT_NAME} rplidar_sdk pthread ${CONAN_LIBS})
