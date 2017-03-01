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

JSON_GIT_REPO=https://github.com/nlohmann/json.git
JSON_GIT_VERSION=v2.1.1

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

cd $DOWNLOAD_DIR
if [ ! -d "json" ] ; then
    echo "---- Clone du repo pour la dépendance JSON $JSON_GIT_REPO"
    git clone $JSON_GIT_REPO
fi
cd json
echo "---- Utilisation de la version $JSON_GIT_VERSION pour le JSON"
git checkout $JSON_GIT_VERSION

cd $DOWNLOAD_DIR
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

echo "Configuration terminé"

echo "Build du projet $PROJECT"
cd $BUILD_DIR
cmake ..
cmake --build .
echo "Build terminé"
