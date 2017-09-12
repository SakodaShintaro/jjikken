#include <cstdio>
#include <fcntl.h>
#include "unistd.h"
#include <cstdlib>
#include "/usr/include/linux/i2c-dev.h"

class DistanceSensor {
public:
    DistanceSensor();
    int measureDistance();
private:
    int fd_;
};

DistanceSensor::DistanceSensor() {
    char filename[20];

    sprintf(filename, "/dev/i2c-1");
    fd_ = open(filename, O_RDWR);
    if (fd_ < 0) {
        printf("Error on open\n");
        exit(1);
    }

    if (ioctl(fd_ , I2C_SLAVE, (0xE0 >> 1)) < 0) {
        printf("Error on slave address 0xE0\n");
        exit(1);
    }
    
}

int DistanceSensor::measureDistance() {
    int range = 0;
    char buf[10];

    buf[0] = 0x00;
    buf[1] = 0x51;
    if ((write(fd_, buf, 2)) != 2) {
        printf("0xE0 Error send the read command\n");
        exit(1);
    }
    usleep(66000);

    buf[0] = 0x02;
    if ((write(fd_, buf, 1)) != 1) {
        printf("0xE0 Error on select the Range High Byte\n");
        exit(1);
    }

    if ((read(fd_, buf, 1)) != 1) {
        printf("0xE0 Error on read the Range High Byte\n");
        exit(1);
    }
    range = buf[0] << 8;

    buf[0] = 0x03;
    if ((write(fd_, buf, 1)) != 1) {
        printf( "0xE0 Error on select the Range Low Byte\n" );
        exit( 1 );
    }
 
    if ( ( read( fd_, buf, 1 ) ) != 1 ) {
        printf( "0xE0 Error on read the Range Low Byte\n" );
        exit( 1 );
    }

    range |= buf[0];

    printf("range = %d\n", range);
    return range;
}
