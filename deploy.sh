#!/bin/sh

ROBOT_NAME=nerell
INSTALL_DIR=/opt

echo "Compilation ..."
./build-pi.sh

echo "Déploiement Applicatif ..."
scp ./build-pi/bin/rplidar_bridge $ROBOT_NAME:$INSTALL_DIR/
