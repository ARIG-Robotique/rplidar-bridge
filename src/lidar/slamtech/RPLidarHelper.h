//
// Created by gregorydepuille@sglk.local on 27/02/17.
//

#ifndef RPLIDAR_BRIDGE_RPLIDARHELPER_H
#define RPLIDAR_BRIDGE_RPLIDARHELPER_H

#include <rplidar.h>
#include "../AbstractLidarHelper.h"

using namespace rp::standalone::rplidar;

class RPLidarHelper final : public AbstractLidarHelper {

public:
    RPLidarHelper(string comFile = "/dev/ttyUSB0", unsigned int baudrate = 115200) : AbstractLidarHelper(comFile, baudrate) {};

    void init() override;
    void end() override;

    JsonResult getDeviceInfo() override;
    JsonResult getHealth() override;

    JsonResult startScan(JsonQuery q) override;
    JsonResult stopScan() override;
    JsonResult setMotorSpeed(JsonQuery q) override;

    JsonResult grabScanData() override;

private:
    const uint16_t MAX_MOTOR_PWM = 1023;

    RPlidarDriver * driver;
    vector<RplidarScanMode> scanModes;
    u_result setMotorSpeed(_u16 speed);
};

#endif //RPLIDAR_BRIDGE_RPLIDARHELPER_H
