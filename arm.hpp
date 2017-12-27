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
    void servo(int pin, double value);
    void servoG(int pin, double target_value, double cur_value);
    void closeFinger();
    void openFinger();
    void twistWrist();
    void returnWrist();
    void bendElbow();
    void straightenElbow();
    void downShoulder();
    void upShoulder();
private:
    static double angle2servo(double angle);
    int finger_pin_ = 35, wrist_pin_ = 33, elbow_pin_ = 32, shoulder_pin_ = 12;
    double finger_value_, wrist_value_, elbow_value_, shoulder_value_;
    PyObject *func_, *func2_;

    static constexpr double finger_close_bound = 6.5;
    static constexpr double finger_open_bound = 3.0;
    static constexpr double wrist_front_bound = 4.0;
    static constexpr double wrist_back_bound = 13.0;
    static constexpr double elbow_straight_bound = 5.0;
    //static constexpr double elbow_bend_bound = 6.5;
    static double elbow_bend_bound;
    static constexpr double shoulder_up_bound = 6.0;
    //static constexpr double shoulder_down_bound = 9.25;
    static double shoulder_down_bound;
};

#endif
