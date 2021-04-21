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
    this->driver = RPlidarDriver::CreateDriver(DRIVER_TYPE_SERIALPORT);
    if (!this->driver) {
        cerr << "Insufficent memory, exit" << endl;
        exit(2);
    }

    // try to connect
    if (IS_FAIL(this->driver->connect(this->comFile.c_str(), this->baudrate))) {
        cerr << "Error, cannot bind to the specified serial port " << this->comFile << endl;
        exit(3);
    }

    // get scan modes
    if (IS_FAIL(this->driver->getAllSupportedScanModes(this->scanModes))) {
        cerr << "Error, cannot get all supported scan modes";
        exit(4);
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

    json data;
    data["serialNumber"] = serialStream.str();
    data["hardwareVersion"] = deviceInfo.hardware_version;
    data["firmwareVersion"] = firmStream.str();
    //data["scanModes"] = this->scanModes;

    JsonResult r;
    r.status = RESPONSE_OK;
    r.action = DEVICE_INFO;
    r.data = data;
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

    string state;
    if (healthinfo.status == RPLIDAR_STATUS_ERROR) {
        state = "ERROR";
    } else if (healthinfo.status == RPLIDAR_STATUS_WARNING) {
        state = "WARNING";
    } else {
        state = "OK";
    }

    json data;
    data["value"] = healthinfo.status;
    data["state"] = state;
    data["errorCode"] = (unsigned int) healthinfo.error_code;

    JsonResult r;
    r.status = RESPONSE_OK;
    r.action = DEVICE_INFO;
    r.data = data;
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
    if (IS_FAIL(this->driver->startScan(false, true))) {
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
    r.data = q.data;
    u_result result = RESULT_OK;
    if (!q.data["speed"].is_null()) {
        result = this->setMotorSpeed((_u16) q.data["speed"]);
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

    rplidar_response_measurement_node_hq_t nodes[8192];
    size_t nodeCount = sizeof(nodes)/sizeof(rplidar_response_measurement_node_hq_t);

    u_result res = this->driver->grabScanDataHq(nodes, nodeCount);
    if (IS_OK(res)) {
        r.status = RESPONSE_OK;

        int ignored = 0;
        json scanData = json::array();
        for (int pos = 0; pos < (int) nodeCount ; ++pos) {
            float distanceMm = nodes[pos].dist_mm_q2 / 4.0f;
            if ((distanceMm < 150) || (distanceMm > 3600)) {
                ignored++;
                continue;
            }

            float angleDeg = (nodes[pos].angle_z_q14 * 90.0f) / 16384.0f;
            bool syncBit = nodes[pos].flag;
            int quality = nodes[pos].quality;

            // Transposition des angles dans le repère robot
            if (angleDeg < 180) {
                angleDeg *= -1;
            } else {
                angleDeg = 360 - angleDeg;
            }

            json v;
            v["angleDeg"] = angleDeg;
            v["distanceMm"] = distanceMm;
            v["syncBit"] = syncBit;
            v["quality"] = quality;
            scanData.push_back(v);
        }

        r.data["ignored"] = ignored;
        r.data["scan"] = scanData;
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