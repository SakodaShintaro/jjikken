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
    void closeFinger();
    void openFinger();
    void twistWrist();
    void downElbow();
    void upElbow();
    void downShoulder();
    void upShoulder();
private:
    int finger_pin_ = 35, wrist_pin_ = 33, elbow_pin_ = 32, shoulder_pin_ = 12;
    PyObject* func_;
};

#endif
