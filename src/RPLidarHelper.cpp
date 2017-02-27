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