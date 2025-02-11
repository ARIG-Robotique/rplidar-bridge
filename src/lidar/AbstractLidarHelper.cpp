//
// Created by Gregory DEPUILLE on 10/02/2025.
//

#include "AbstractLidarHelper.h"
#include <string>

AbstractLidarHelper::AbstractLidarHelper(string comFile, unsigned int baudrate) {
  this->comFile = std::move(comFile);
  this->baudrate = baudrate;
}

JsonResult AbstractLidarHelper::getDeviceInfo() {
  return notImplemented(DEVICE_INFO);
}
JsonResult AbstractLidarHelper::getHealth() {
  return notImplemented(HEALTH_INFO);
}

JsonResult AbstractLidarHelper::startScan(JsonQuery q) {
  return notImplemented(q.action);
}
JsonResult AbstractLidarHelper::stopScan(){
  return notImplemented(STOP_SCAN);
}
JsonResult AbstractLidarHelper::setMotorSpeed(JsonQuery q){
  return notImplemented(q.action);
}

JsonResult AbstractLidarHelper::grabScanData(){
  return notImplemented(GRAB_DATA);
}

JsonResult AbstractLidarHelper::notImplemented(string action){
	JsonResult notImplemented;
    notImplemented.status = RESPONSE_NOT_IMPLEMENTED;
    notImplemented.action = action;
    notImplemented.errorMessage = "Not yet implemented";
    return notImplemented;
}