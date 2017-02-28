/* RPLIDAR Bridge Application */
#include "SocketHelper.h"
#include "RPLidarHelper.h"

int main(int argc, const char *argv[]) {
    SocketHelper socket;
    RPLidarHelper lidar;

    // Initialisation RPLidar
    lidar.init();

    // Initialisation de la socket et attente de connexion.
    socket.init();
    socket.waitConnection();

    bool stop = false;
    while (!stop) {
        JsonQuery query = socket.getQuery();
        JsonResult result;
        if (query.action == DEVICE_INFO) {
            result = lidar.getDeviceInfo();

        } else if (query.action == HEALTH_INFO) {
            result = lidar.getHealth();

        } else if (query.action == START_SCAN) {
            result = lidar.startScan(query);

        } else if (query.action == SET_SPEED) {
            result = lidar.setMotorSpeed(query);

        } else if (query.action == GRAB_DATA) {
            result = lidar.grabScanData();

        } else if (query.action == EXIT) {
            cout << "Demande d'arret du programe" << endl;
            result.status = RESPONSE_OK;
            stop = true;

        } else {
            result.status = RESPONSE_ERROR;
            result.errorMessage = "Action non supporté";
        }

        result.action = query.action;
        socket.sendResponse(result);
    }

    // Fermeture de la socket
    socket.end();

    // Arret du lidar
    lidar.end();

    return 0;
}
