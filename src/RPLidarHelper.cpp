//
// Created by gregorydepuille@sglk.local on 27/02/17.
//

#include "RPLidarHelper.h"

#include <iostream>

RPLidarHelper::RPLidarHelper(string comFile, unsigned int baudrate) {
    this->comFile = comFile;
    this->baudrate = baudrate;
}

void RPLidarHelper::init() {
    // create the driver instance
    this->driver = RPlidarDriver::CreateDriver(RPlidarDriver::DRIVER_TYPE_SERIALPORT);
    if (!this->driver) {
        cerr << "Insufficent memory, exit" << endl;
        exit(2);
    }

    // try to connect
    if (IS_FAIL(this->driver->connect(this->comFile.c_str(), this->baudrate))) {
        cerr << "Error, cannot bind to the specified serial port " << this->comFile << endl;
        exit(3);
    }
}

JsonResult RPLidarHelper::getDeviceInfo() {
    rplidar_response_device_info_t deviceInfo;
    if (IS_FAIL(this->driver->getDeviceInfo(deviceInfo))) {
        JsonResult fail;
        fail.status = RESPONSE_ERROR;
        fail.errorMessage = "Impossible de récupérer les infos";
        return fail;
    }

    // Récupération du numéro de série
    ostringstream serialStream;
    for (int i = 0 ; i < 16 ; i ++) {
        _u8 v = deviceInfo.serialnum[i];
        if (v < 10) {
            serialStream << "0";
        }
        serialStream << v;
    }

    // Récupération du firmware
    ostringstream firmStream;
    firmStream << (deviceInfo.firmware_version >> 8) <<  "." << (deviceInfo.firmware_version & 0xFF);
    if ((deviceInfo.firmware_version & 0xFF) < 11) {
        firmStream << "0";
    }

    json datas;
    datas["serialNumber"] = serialStream.str();
    datas["hardwareVersion"] = deviceInfo.hardware_version;
    datas["firmwareVersion"] = firmStream.str();

    JsonResult r;
    r.status = RESPONSE_OK;
    r.action = DEVICE_INFO;
    r.datas = datas;
    return r;
}

JsonResult RPLidarHelper::getHealth() {
    JsonResult r;
    return r;
}

void RPLidarHelper::startMotor(int speed) {

}

void RPLidarHelper::setMotorSpeed(int speed) {

}

JsonResult RPLidarHelper::grabScanData() {
    JsonResult r;
    return r;
}

void RPLidarHelper::end() {
    this->driver->stop();
    this->driver->stopMotor();
    this->driver->disconnect();

    RPlidarDriver::DisposeDriver(this->driver);
}
