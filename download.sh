#!/usr/bin/env bash

ROOT_DIR=$(pwd)
DOWNLOAD_DIR=${ROOT_DIR}/download

RPLIDAR_SDK_VERSION=v1.10.0
RPLIDAR_SDK_DIR=rplidar_sdk
RPLIDAR_FILENAME=${RPLIDAR_SDK_DIR}_${RPLIDAR_SDK_VERSION}
RPLIDAR_DOWNLOAD_URL=https://github.com/Slamtec/rplidar_sdk/archive/release/${RPLIDAR_SDK_VERSION}.zip

JSON_VERSION=v2.1.1
JSON_DIR=json-${JSON_VERSION}
JSON_FILENAME=json.hpp
JSON_DOWNLOAD_URL=https://github.com/nlohmann/json/releases/download/${JSON_VERSION}/${JSON_FILENAME}

SPDLOG_VERSION=1.3.1
SPDLOG_FILENAME=spdlog-${SPDLOG_VERSION}
SPDLOG_DOWNLOAD_URL=https://github.com/gabime/spdlog/archive/v${SPDLOG_VERSION}.zip

RASPBERRY_TOOLS=https://github.com/raspberrypi/tools.git

NEWER_RPI_TOOLS_VERSION=35ea4148c163f020b70b770a8b56517693682b1b
NEWER_RPI_TOOLS=https://github.com/rvagg/rpi-newer-crosstools.git

echo "-- Download external dependencies"
if [ ! -d "${DOWNLOAD_DIR}" ] ; then
    echo "---- Make download directory : $DOWNLOAD_DIR"
    mkdir -p ${DOWNLOAD_DIR}
fi

cd ${DOWNLOAD_DIR}
if [ ! -d "tools" ] ; then
    echo "---- Clone raspberry-tools ..."
    git clone --depth=1 ${RASPBERRY_TOOLS}
fi

cd ${DOWNLOAD_DIR}
if [ ! -d "newer-tools" ] ; then
    echo "---- Clone newer raspberry-tools ..."
    git clone ${NEWER_RPI_TOOLS} newer-tools
    cd newer-tools
    git checkout ${NEWER_RPI_TOOLS_VERSION}
fi

cd ${DOWNLOAD_DIR}
if [ ! -f "${RPLIDAR_FILENAME}.zip" ] ; then
  echo "---- Download RPLidar SDK ${RPLIDAR_SDK_VERSION} ..."
  curl -L ${RPLIDAR_DOWNLOAD_URL} -o ${RPLIDAR_FILENAME}.zip
  unzip ${RPLIDAR_FILENAME}.zip
  ln -s rplidar_sdk-release-${RPLIDAR_SDK_VERSION} ${RPLIDAR_SDK_DIR}
fi

cd ${DOWNLOAD_DIR}
if [ ! -f "${JSON_DIR}/${JSON_FILENAME}" ] ; then
    echo "---- Download JSON ${JSON_VERSION} ..."
    mkdir -p ${JSON_DIR} json
    curl -L ${JSON_DOWNLOAD_URL} -o ${JSON_DIR}/${JSON_FILENAME}
    cd json
    ln -sf ../${JSON_DIR}/${JSON_FILENAME}
fi

cd ${DOWNLOAD_DIR}
if [ ! -f "${SPDLOG_FILENAME}.zip" ] ; then
  echo "---- Download SPDLOG ${SPDLOG_VERSION} ..."
  curl -L ${SPDLOG_DOWNLOAD_URL} -o ${SPDLOG_FILENAME}.zip
  unzip ${SPDLOG_FILENAME}.zip

  ln -s ${SPDLOG_FILENAME}/include spdlog
fi