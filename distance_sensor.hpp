#ifndef DISTANCE_SENSOR_HPP
#define DISTANCE_SENSOR_HPP

#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <time.h>
#include "/usr/include/linux/i2c-dev.h"

class DistanceSensor {
public:
    DistanceSensor();
    int measureDistance();
private:
    int fd_;
};

#endif
