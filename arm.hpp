#ifndef ARM_HPP
#define ARM_HPP

#include<Python.h>
#include<iostream>
#include<fstream>
#include<string>
#include<cstring>

class Arm{
public:
    Arm();
    ~Arm();
    void servo(int pin, double angle);
    void closeFinger();
    void openFinger();
    void twistWrist();
    void returnWrist();
    void downElbow();
    void upElbow();
    void downShoulder();
    void upShoulder();
private:
    static double angle2servo(double angle);
    int finger_pin_ = 35, wrist_pin_ = 33, elbow_pin_ = 32, shoulder_pin_ = 12;
    double finger_value_, wrist_value_, elbow_value_, shoulder_value_;
    PyObject* func_;
};

#endif
