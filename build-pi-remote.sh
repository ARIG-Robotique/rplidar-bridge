#!/usr/bin/env bash
# Author : Gregory DEPUILLE
# Description : Wrapper pour la configuration et le build du projet
PROJECT="ARIG C++ RPLidar Bridge (Raspberry PI) "

ROBOT=$1
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