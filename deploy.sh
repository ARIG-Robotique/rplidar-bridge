#!/bin/sh

ROBOT_NAME=${1}
INSTALL_DIR=/home/pi

EXEC=build-pi
if [ "${ROBOT_NAME}" == "odin" ] ; then
  EXEC=build-pi-new
fi

echo "Compilation ..."
./${EXEC}.sh

echo "Déploiement Applicatif ..."
scp ./${EXEC}/bin/rplidar_bridge $ROBOT_NAME:$INSTALL_DIR/
