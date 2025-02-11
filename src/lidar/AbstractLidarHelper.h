//
// Created by Gregory DEPUILLE on 10/02/2025.
//


#ifndef ABSTRACTLIDARHELPER_H
#define ABSTRACTLIDARHELPER_H

#include "../Constantes.h"

using namespace std;

class AbstractLidarHelper {

public:

  AbstractLidarHelper(string comFile, unsigned int baudrate);

  virtual void init() = 0;
  virtual void end() = 0;

  virtual JsonResult getDeviceInfo();
  virtual JsonResult getHealth();

  virtual JsonResult startScan(JsonQuery q);
  virtual JsonResult stopScan();
  virtual JsonResult setMotorSpeed(JsonQuery q);

  virtual JsonResult grabScanData();

protected:
  string comFile;
  unsigned int baudrate;

  JsonResult notImplemented(string action);
};

#endif //ABSTRACTLIDARHELPER_H
