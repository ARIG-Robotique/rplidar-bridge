#!/usr/bin/env bash
# Author : Gregory DEPUILLE
# Description : Wrapper pour la configuration et le build du projet
PROJECT="ARIG C++ RPLidar Bridge (cross compile Raspberry PI x64) "

ROOT_DIR=$(pwd)
BUILD_DIR=$ROOT_DIR/build

echo "Build du projet $PROJECT"
if [ -d "$BUILD_DIR" ] ; then
    echo "-- Nettoyage du répertoire de build $BUILD_DIR"
    rm -Rf $BUILD_DIR
fi

echo "-- Création du répertoire de build $BUILD_DIR"
mkdir -p $BUILD_DIR

echo "-- Build du projet $PROJECT"
cd $BUILD_DIR
export PI_TOOLS_HOME=~/raspberry-tools
cmake -DCMAKE_TOOLCHAIN_FILE=raspberry.cmake ..
cmake --build .
echo "Build terminé"
