#!/usr/bin/env bash

ROOT_DIR=$(pwd)
DOWNLOAD_DIR=$ROOT_DIR/download

RPLIDAR_SDK_VERSION=1.5.7
RPLIDAR_SDK_DIR=rplidar_sdk
RPLIDAR_FILENAME=${RPLIDAR_SDK_DIR}_v${RPLIDAR_SDK_VERSION}
RPLIDAR_DOWNLOAD_URL=http://www.slamtec.com/download/lidar/sdk/${RPLIDAR_FILENAME}.zip

JSON_VERSION=v2.1.1
JSON_DIR=json-$JSON_VERSION
JSON_FILENAME=json.hpp
JSON_DOWNLOAD_URL=https://github.com/nlohmann/json/releases/download/${JSON_VERSION}/${JSON_FILENAME}

echo "Download des dépendances externe"
if [ ! -d "$DOWNLOAD_DIR" ] ; then
    echo "-- Création du répertoire de download $DOWNLOAD_DIR"
    mkdir -p $DOWNLOAD_DIR
fi

cd $DOWNLOAD_DIR
if [ ! -f "$RPLIDAR_FILENAME.zip" ] ; then
  echo "---- Download RPLidar SDK $RPLIDAR_SDK_VERSION ..."
  curl -L $RPLIDAR_DOWNLOAD_URL -o $RPLIDAR_FILENAME.zip
  unzip $RPLIDAR_FILENAME.zip -d $RPLIDAR_SDK_DIR
fi

cd $DOWNLOAD_DIR
if [ ! -f "$JSON_DIR/$JSON_FILENAME" ] ; then
    echo "---- Download JSON $JSON_VERSION ..."
    mkdir -p $JSON_DIR json
    curl -L $JSON_DOWNLOAD_URL -o $JSON_DIR/$JSON_FILENAME
    cd json
    ln -sf ../$JSON_DIR/$JSON_FILENAME
fi
