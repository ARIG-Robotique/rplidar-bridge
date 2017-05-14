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
        char buff[3];
        snprintf(buff, sizeof(buff), "%02X", v);
        string tmp = buff;
        serialStream << tmp;
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
    datas["errorCode"] = (unsigned int) healthinfo.error_code;

    JsonResult r;
    r.status = RESPONSE_OK;
    r.action = DEVICE_INFO;
    r.datas = datas;
    return r;
}

JsonResult RPLidarHelper::startScan(JsonQuery q) {
    if (IS_FAIL(this->driver->startMotor())) {
        JsonResult r;
        r.action = q.action;
        r.status = RESPONSE_ERROR;
        r.errorMessage = "Impossible de démarrer le moteur";
        return r;
    };
    if (IS_FAIL(this->driver->startScan())) {
        JsonResult r;
        r.action = q.action;
        r.status = RESPONSE_ERROR;
        r.errorMessage = "Impossible de démarrer le scan";
        return r;
    }
    return this->setMotorSpeed(q);
}

JsonResult RPLidarHelper::stopScan() {
    JsonResult r;
    r.action = STOP_SCAN;
    r.status = RESPONSE_OK;

    if (IS_FAIL(this->driver->stop())) {
        r.status = RESPONSE_ERROR;
        r.errorMessage = "Impossible d'arreter le scan";
    }
    if (IS_FAIL(this->driver->stopMotor())) {
        r.status = RESPONSE_ERROR;
        r.errorMessage = "Impossible d'arreter le moteur";
    }

    return r;
}

JsonResult RPLidarHelper::setMotorSpeed(JsonQuery q) {
    JsonResult r;
    r.action = q.action;
    r.datas = q.datas;
    u_result result = RESULT_OK;
    if (!q.datas["speed"].is_null()) {
        result = this->setMotorSpeed((_u16) q.datas["speed"]);
    }
    if (IS_FAIL(result)) {
        r.status = RESPONSE_ERROR;
    } else {
        r.status = RESPONSE_OK;
    }
    return r;
}

u_result RPLidarHelper::setMotorSpeed(_u16 speed) {
    if (speed > MAX_MOTOR_PWM) {
        speed = MAX_MOTOR_PWM;
    } else if (speed < 0) {
        speed = 0;
    }
    return this->driver->setMotorPWM(speed);
}

JsonResult RPLidarHelper::grabScanData() {
    JsonResult r;
    r.action = GRAB_DATA;

    rplidar_response_measurement_node_t nodes[360*2];
    size_t   count = _countof(nodes);

    u_result res = this->driver->grabScanData(nodes, count);
    if (IS_OK(res)) {
        this->driver->ascendScanData(nodes, count);

        r.status = RESPONSE_OK;

        int ignored = 0;

        json scanDatas = json::array();
        for (int pos = 0; pos < (int) count ; ++pos) {
            float distanceMm = nodes[pos].distance_q2 / 4.0f;
            if ((distanceMm < 150) || (distanceMm > 3600)) {
                ignored++;
                continue;
            }

            float angleDeg = (nodes[pos].angle_q6_checkbit >> RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT)/64.0f;
            bool syncBit = nodes[pos].sync_quality & RPLIDAR_RESP_MEASUREMENT_SYNCBIT == 1;
            int quality = nodes[pos].sync_quality >> RPLIDAR_RESP_MEASUREMENT_QUALITY_SHIFT;

            // Transposition des angles dans le repère robot
            if (angleDeg > 0 && angleDeg < 180) {
                angleDeg *= -1;
            } else {
                angleDeg -= 360;
            }

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
    this->stopScan();
    this->driver->disconnect();

    RPlidarDriver::DisposeDriver(this->driver);
}