#!/bin/sh

ROBOT_NAME=${1}
INSTALL_DIR=/home/pi

EXEC=build-pi
if [ "${ROBOT_NAME}" == "nerell" ] ; then
  EXEC=build-pi-new
elif [ "${ROBOT_NAME}" == "odin" ] ; then
  EXEC=build-pi-64
else
  echo "[ ERROR ] Robot ${ROBOT_NAME} inconnu."
  exit 1
fi

echo "Compilation ..."
./${EXEC}.sh

echo "Déploiement Applicatif ..."
scp ./${EXEC}/bin/rplidar_bridge $ROBOT_NAME:$INSTALL_DIR/
