//
// Created by gregorydepuille@sglk.local on 27/02/17.
//

#ifndef RPLIDAR_BRIDGE_RPLIDARHELPER_H
#define RPLIDAR_BRIDGE_RPLIDARHELPER_H

#include <string>
#include <rplidar.h>
#include "Constantes.h"

using namespace std;
using namespace rp::standalone::rplidar;

class RPLidarHelper {

public:
    RPLidarHelper(string comFile = "/dev/ttyUSB0", unsigned int baudrate = 115200);

    void init();

    JsonResult getDeviceInfo();
    JsonResult getHealth();

    void startMotor(int speed = DEFAULT_MOTOR_PWM);
    void setMotorSpeed(int speed = DEFAULT_MOTOR_PWM);

    JsonResult grabScanData();

    void end();

private:
    string comFile;
    unsigned int baudrate;

    RPlidarDriver * driver;

};


#endif //RPLIDAR_BRIDGE_RPLIDARHELPER_H
