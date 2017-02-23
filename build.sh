#!/usr/bin/env bash
# Author : Gregory DEPUILLE
# Description : Wrapper pour la configuration et le build du projet
PROJECT="ARIG C++ RPLidar Bridge"

ROOT_DIR=$(pwd)
BUILD_DIR=$ROOT_DIR/build
DOWNLOAD_DIR=$ROOT_DIR/download

RPLIDAR_SDK_VERSION=1.5.7
RPLIDAR_SDK_DIR=rplidar_sdk
RPLIDAR_FILENAME=${RPLIDAR_SDK_DIR}_v${RPLIDAR_SDK_VERSION}

echo "Configuration du projet $PROJECT"
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
  make || exit 1
else
  echo "-- ERROR : RPLidar SDK $RPLIDAR_SDK_VERSION manquant !!"
  exit 1
fi

echo "-- Configuration du build du projet $PROJECT"


echo "---- Création du répertoire de build $BUILD_DIR"
rm -Rf $BUILD_DIR
mkdir -p $BUILD_DIR

cd $BUILD_DIR
echo "---- Download des dépendances Conan dependency manager"
conan install ../ || exit 1

echo "Configuration terminé"

echo "Build du projet $PROJECT"
cd $BUILD_DIR
conan build .. || exit 1
echo "Build terminé"
