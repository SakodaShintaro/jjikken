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
private:
    int pin_for_finger_;
    char *python_script_;
    PyObject* main_;
    PyObject* func_;
};

#endif
