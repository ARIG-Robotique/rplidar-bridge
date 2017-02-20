#!/usr/bin/env bash
# Author : Gregory DEPUILLE
# Description : Wrapper pour la configuration et le build du projet

ROOT_DIR=$(pwd)
BUILD_DIR=$ROOT_DIR/build
DOWNLOAD_DIR=$ROOT_DIR/download

RPLIDAR_SDK_VERSION=1.5.7
RPLIDAR_SDK_DIR=rplidar_sdk
RPLIDAR_FILENAME=${RPLIDAR_SDK_DIR}_v${RPLIDAR_SDK_VERSION}

echo "Configuration du projet ARIG C++ RPLidar Bridge"

if [ ! -d "$BUILD_DIR" ] ; then
    echo "-- Création du répertoire de build $BUILD_DIR"
    mkdir -p $BUILD_DIR
fi
if [ ! -d "$DOWNLOAD_DIR" ] ; then
    echo "-- Création du répertoire de download $DOWNLOAD_DIR"
    mkdir -p $DOWNLOAD_DIR
fi

echo "-- Download des dépendances externe"
cd $DOWNLOAD_DIR
if [ ! -f "$RPLIDAR_FILENAME.zip" ] ; then
  echo "---- Download RPLidar SDK $RPLIDAR_SDK_VERSION ..."
  curl -L http://www.slamtec.com/download/lidar/sdk/$RPLIDAR_FILENAME.zip -o $RPLIDAR_FILENAME.zip
  unzip $RPLIDAR_FILENAME.zip -d $RPLIDAR_SDK_DIR
fi

echo "-- Build des dépendances externe"
if [ -d "$RPLIDAR_SDK_DIR" ] ; then
  echo "---- Build RPLidar SDK $RPLIDAR_SDK_VERSION ..."
  cd $DOWNLOAD_DIR/$RPLIDAR_SDK_DIR/sdk/sdk/
  make
else
  echo "-- ERROR : RPLidar SDK $RPLIDAR_SDK_VERSION manquant !!"
  exit 1
fi

cd $ROOT_DIR

echo "-- Configuration du build du projet"
cd $BUILD_DIR
cmake .. -Wno-dev -G "Unix Makefiles"

cd $ROOT_DIR
echo "Configuration terminé"

echo "Build du projet ARIG C++ RPLidar Bridge"
cd $BUILD_DIR
make

echo "Build terminé"
