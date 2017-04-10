#!/usr/bin/env bash
# Author : Gregory DEPUILLE
# Description : Wrapper pour la configuration et le build du projet
PROJECT="ARIG C++ RPLidar Bridge (cross compile Raspberry PI ou remote build)"

BUILD_MODE=${1:-"local"}
ROBOT=${2:-"nerell"}

echo "Build du projet $PROJECT"
echo " - Build local : ./build-pi.sh [local]"
echo " - Build remote : ./build-pi.sh remote [nerell]"
echo ""

if [ "$BUILD_MODE" == "local" ] ; then
    ROOT_DIR=$(pwd)
    BUILD_DIR=$ROOT_DIR/build-pi

    if [ -d "$BUILD_DIR" ] ; then
        echo "-- Nettoyage du répertoire de build $BUILD_DIR"
        rm -Rf $BUILD_DIR
    fi

    echo "-- Création du répertoire de build $BUILD_DIR"
    mkdir -p $BUILD_DIR

    echo "-- Build du projet $PROJECT"
    cd $BUILD_DIR
    export PI_TOOLS_HOME=/opt/raspberrypi-tools
    cmake -DCMAKE_TOOLCHAIN_FILE=raspberry.cmake ..
    cmake --build .
    echo "Build terminé"

elif [ "$BUILD_MODE" == "remote" ] ; then
    DEST_DIR=/home/pi/rplidarBridge

    echo "-- SSH : Création répertoire RPLidar Bridge"
    ssh $ROBOT mkdir -p $DEST_DIR

    echo "-- SSH : Nettoyage des sources"
    ssh $ROBOT rm -Rvf $DEST_DIR/src

    echo "-- SSH : Copie des sources sur le Raspberry $ROBOT";
    scp build.sh $ROBOT:$DEST_DIR/
    scp download.sh $ROBOT:$DEST_DIR/
    scp CMakeLists.txt $ROBOT:$DEST_DIR/
    scp -r src $ROBOT:$DEST_DIR/

    echo "-- SSH : Build sur le Raspberry $ROBOT"
    ssh $ROBOT "cd $DEST_DIR ; sh build.sh"

    echo "-- SSH : Build terminé"
else
    echo "Build mode [$BUILD_MODE] non supporté. Seulement local et remore."
fi