#=============================================================================#
# Author: Gregory DEPUILLE                                                    #
# Date:   18/02/2017                                                          #
# Description: Build du bridge par socket pour le RPLidar                     #
#=============================================================================#
# Contrôle de la version minimale de CMake
cmake_minimum_required(VERSION 3.2.2)

# Variable pour les dépendances externes
set(DOWNLOAD_DIR "download")

set(RPLIDAR_LIB_NAME   "rplidar_sdk")
set(RPLIDAR_SDK_DIR    "${DOWNLOAD_DIR}/rplidar_sdk/sdk/sdk")

set(JSON_LIB_DIR       "${DOWNLOAD_DIR}/json")

# Variable de build
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Détection des fichiers par OS
if (UNIX AND NOT APPLE)
    set(RPLIDAR_ARCH_DIR "linux")
elseif (UNIX AND APPLE)
    set(RPLIDAR_ARCH_DIR "macOS")
    add_definitions(-D_MACOS)
else()
    message(FATAL_ERROR "Arch OS is not valid")
endif()

#====================================================================#
#  Setup Project                                                     #
#====================================================================#
project(rplidar_bridge)

# Récupération des dépendances par le script de download
execute_process(
        COMMAND sh download.sh
        WORKING_DIRECTORY ..
)

# Compilation de la lib externe RPLidar #
#########################################
file(GLOB RPLIDAR_SRCS
        "${RPLIDAR_SDK_DIR}/src/rplidar_driver.cpp"
        "${RPLIDAR_SDK_DIR}/src/hal/thread.cpp"
        "${RPLIDAR_SDK_DIR}/src/arch/${RPLIDAR_ARCH_DIR}/net_serial.cpp"
        "${RPLIDAR_SDK_DIR}/src/arch/${RPLIDAR_ARCH_DIR}/timer.cpp"
        )

include_directories(
        "${RPLIDAR_SDK_DIR}/include/"
        "${RPLIDAR_SDK_DIR}/src/"
)
add_library(${RPLIDAR_LIB_NAME} STATIC ${RPLIDAR_SRCS})

# Récupération de la définition pour JSON #
###########################################
include_directories(${JSON_LIB_DIR})

# Récupération des sources du projet principale #
#################################################
file(GLOB SRCS "src/*.cpp")
file(GLOB HEADERS "src/*.h")

add_executable(${PROJECT_NAME} ${SRCS} ${HEADERS})
target_link_libraries(${PROJECT_NAME} ${RPLIDAR_LIB_NAME} pthread)
