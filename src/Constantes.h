//
// Created by gregorydepuille@sglk.local on 28/02/17.
//

#ifndef RPLIDAR_BRIDGE_CONSTANTES_H
#define RPLIDAR_BRIDGE_CONSTANTES_H

#include <json.hpp>

using json = nlohmann::json;

#define DEVICE_INFO "DEVICE_INFO"
#define HEALTH_INFO "HEALTH_INFO"
#define START_MOTOR "START_MOTOR"
#define STOP_MOTOR  "STOP_MOTOR"
#define SET_SPEED   "SET_SPEED"
#define START_SCAN  "START_SCAN"
#define STOP_SCAN   "STOP_SCAN"

#define EXIT        "EXIT"

#define RESPONSE_OK     "OK"
#define RESPONSE_ERROR  "ERROR"

struct JsonResult {
    std::string status;
    std::string action;
    std::string errorMessage;
    json datas;
};

struct JsonQuery {
    std::string action;
    json datas;
};

#endif //RPLIDAR_BRIDGE_CONSTANTES_H
