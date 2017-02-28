//
// Created by gregorydepuille@sglk.local on 27/02/17.
//

#include "RPLidarHelper.h"

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
        fail.action = DEVICE_INFO;
        fail.errorMessage = "Impossible de récupérer les infos";
        return fail;
    }

    // Récupération du numéro de série
    ostringstream serialStream;
    for (int i = 0 ; i < 16 ; i ++) {
        _u8 v = deviceInfo.serialnum[i];
        serialStream << setw(2) << setfill('0') << hex << v;
    }

    // Récupération du firmware
    ostringstream firmStream;
    firmStream << (deviceInfo.firmware_version >> 8) << "." << (deviceInfo.firmware_version & 0xFF);
    if ((deviceInfo.firmware_version & 0xFF) <= 10) {
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
    rplidar_response_device_health_t healthinfo;
    if (IS_FAIL(this->driver->getHealth(healthinfo))) {
        JsonResult fail;
        fail.status = RESPONSE_ERROR;
        fail.action = HEALTH_INFO;
        fail.errorMessage = "Impossible de récupérer les infos de santé";
        return fail;
    }

    string libelle;
    if (healthinfo.status == RPLIDAR_STATUS_ERROR) {
        libelle = "ERROR";
    } else if (healthinfo.status == RPLIDAR_STATUS_WARNING) {
        libelle = "WARNING";
    } else {
        libelle = "OK";
    }

    json datas;
    datas["value"] = healthinfo.status;
    datas["libelle"] = libelle;
    datas["errorCode"] = healthinfo.error_code;

    JsonResult r;
    r.status = RESPONSE_OK;
    r.action = DEVICE_INFO;
    r.datas = datas;
    return r;
}

JsonResult RPLidarHelper::startScan(JsonQuery q) {
    this->driver->startMotor();
    this->driver->startScan();
    return this->setMotorSpeed(q);
}

JsonResult RPLidarHelper::setMotorSpeed(JsonQuery q) {
    JsonResult r;
    r.action = q.action;
    r.datas = q.datas;
    if (!q.datas["speed"].is_null()) {
        this->setMotorSpeed((_u16) q.datas["speed"]);
        r.status = RESPONSE_OK;
    } else {
        r.status = RESPONSE_ERROR;
    }
    return r;
}

void RPLidarHelper::setMotorSpeed(_u16 speed) {
    if (speed > MAX_MOTOR_PWM) {
        speed = MAX_MOTOR_PWM;
    } else if (speed < 0) {
        speed = 0;
    }
    this->driver->setMotorPWM(speed);
}

JsonResult RPLidarHelper::grabScanData() {
    JsonResult r;
    r.action = GRAB_DATA;

    rplidar_response_measurement_node_t nodes[360*2];
    size_t   count = _countof(nodes);

    if (IS_OK(this->driver->grabScanData(nodes, count))) {
        this->driver->ascendScanData(nodes, count);

        r.status = RESPONSE_OK;

        int ignored = 0;

        json scanDatas = json::array();
        for (int pos = 0; pos < (int) count ; ++pos) {
            if (nodes[pos].distance_q2 == 0) {
                ignored++;
                continue;
            }

            float angleDeg = (nodes[pos].angle_q6_checkbit >> RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT)/64.0f;
            float distanceMm = nodes[pos].distance_q2 / 4.0f;
            bool syncBit = nodes[pos].sync_quality & RPLIDAR_RESP_MEASUREMENT_SYNCBIT == 1;
            int quality = nodes[pos].sync_quality >> RPLIDAR_RESP_MEASUREMENT_QUALITY_SHIFT;

            json v;
            v["angleDeg"] = angleDeg;
            v["distanceMm"] = distanceMm;
            v["syncBit"] = syncBit;
            v["quality"] = quality;
            scanDatas.push_back(v);
        }

        r.datas["ignored"] = ignored;
        r.datas["scan"] = scanDatas;
    } else {
        r.status = RESPONSE_ERROR;
        r.errorMessage = "Erreur lors de la récupération des données du SCAN";
    }

    return r;
}

void RPLidarHelper::end() {
    this->driver->stop();
    this->driver->stopMotor();
    this->driver->disconnect();

    RPlidarDriver::DisposeDriver(this->driver);
}