#=============================================================================#
# Author: Gregory DEPUILLE                                                    #
# Date:   18/02/2017                                                          #
# Description: Build du bridge par socket pour le RPLidar                     #
#=============================================================================#
# Contrôle de la version minimale de CMake
cmake_minimum_required(VERSION 3.9.2)

# Check program requis
function (checkProgram name)
    message("-- Detecting ${name} program")
    find_program(PROGRAM_PRESENT ${name})
    if (NOT PROGRAM_PRESENT)
        message(FATAL_ERROR "${name} must be installed")
    else()
        message("-- Detecting ${name} program - done")
    endif()
endfunction(checkProgram)

# Variable pour les dépendances externes
set(DOWNLOAD_DIR "download")

# Pour faire de la cross compilation
if (DEFINED BUILD_PI)
    set(PI_TOOLS_HOME ${CMAKE_CURRENT_SOURCE_DIR}/${DOWNLOAD_DIR}/tools)
    set(CMAKE_TOOLCHAIN_FILE ./raspberry.cmake)
endif()
if (DEFINED BUILD_PI_NEW)
    set(PI_TOOLS_HOME ${CMAKE_CURRENT_SOURCE_DIR}/${DOWNLOAD_DIR}/newer-tools)
    set(CMAKE_TOOLCHAIN_FILE ./raspberry-new.cmake)
endif()
if (DEFINED BUILD_PI_64)
    set(PI_TOOLS_HOME ${CMAKE_CURRENT_SOURCE_DIR}/${DOWNLOAD_DIR}/gcc-linaro-aarch64-linux-gnu)
    set(CMAKE_TOOLCHAIN_FILE ./raspberry-64.cmake)
endif()

message(STATUS "Build PI     : ${BUILD_PI}")
message(STATUS "Build PI New : ${BUILD_PI_NEW}")
message(STATUS "Build PI 64  : ${BUILD_PI_64}")

# Variable de build
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if (NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    message(STATUS "Debug mode ON")
    add_definitions(-DDEBUG_MODE)
endif()

# Détection des fichiers par OS
if (UNIX AND NOT APPLE)
    set(RPLIDAR_ARCH_DIR "linux")
elseif (UNIX AND APPLE)
    set(RPLIDAR_ARCH_DIR "macOS")
    add_definitions(-D_MACOS)
else()
    message(FATAL_ERROR "Arch OS is not valid")
endif()

# Check program
checkProgram(git)
checkProgram(curl)
checkProgram(unzip)

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
set(RPLIDAR_LIB_NAME   "rplidar_sdk")
set(RPLIDAR_SDK_DIR    "${DOWNLOAD_DIR}/rplidar_sdk/sdk/sdk")

file(GLOB RPLIDAR_SRCS
        "${RPLIDAR_SDK_DIR}/src/rplidar_driver.cpp"
        "${RPLIDAR_SDK_DIR}/src/hal/thread.cpp"
        "${RPLIDAR_SDK_DIR}/src/arch/${RPLIDAR_ARCH_DIR}/net_serial.cpp"
        "${RPLIDAR_SDK_DIR}/src/arch/${RPLIDAR_ARCH_DIR}/net_socket.cpp"
        "${RPLIDAR_SDK_DIR}/src/arch/${RPLIDAR_ARCH_DIR}/timer.cpp"
        )

include_directories(
        "${RPLIDAR_SDK_DIR}/include/"
        "${RPLIDAR_SDK_DIR}/src/"
)
add_library(${RPLIDAR_LIB_NAME} STATIC ${RPLIDAR_SRCS})

# Récupération de la définition pour JSON #
###########################################
set(JSON_LIB_DIR "${DOWNLOAD_DIR}/json")
include_directories(${JSON_LIB_DIR})

# Récupération de la définition pour SPDLOG #
#############################################
set(SPDLOG_LIB_DIR "${DOWNLOAD_DIR}/spdlog")
include_directories(${SPDLOG_LIB_DIR})

# Récupération des sources du projet principale #
#################################################
file(GLOB SRCS "src/*.cpp")
file(GLOB HEADERS "src/*.h")

add_executable(${PROJECT_NAME} ${SRCS} ${HEADERS})
target_link_libraries(${PROJECT_NAME} ${RPLIDAR_LIB_NAME} pthread)
