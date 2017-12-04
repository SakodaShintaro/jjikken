#include"arm.hpp"

Arm::Arm() {
    Py_Initialize();

    PyObject* pModule = PyImport_ImportModule("py_motor");
    if (pModule == NULL) {
        std::cout << "Moduleの読み込みに失敗" << std::endl;
        exit(1);
    }

    func_ = PyObject_GetAttrString(pModule, "move");
    if (func_ == NULL) {
        std::cout << "関数の読み込みに失敗" << std::endl;
        exit(1);
    }

    if (!PyCallable_Check(func_)) {
        std::cout << "関数が実行できない" << std::endl;
        exit(1);
    }

    finger_value_ = 2.5;
    wrist_value_ = 4.0;
    elbow_value_ = 5.0;
    shoulder_value_ = 5;
}

Arm::~Arm() {
    Py_Finalize();
}

void Arm::servo(int pin, double angle) {
    func_ = PyObject_GetAttrString(PyImport_ImportModule("py_motor"), "move");
    if (PyCallable_Check(func_)) {
        PyObject* pyArgs = PyTuple_New(2);
        if (pyArgs == NULL) {
            std::cerr << "pyArgsにおいてエラー" << std::endl;
        }

        PyObject* pyArg1 = PyLong_FromLong(pin);
        if (pyArgs == NULL) {
            std::cerr << "pyArg1においてエラー" << std::endl;
        }
        PyTuple_SetItem(pyArgs, 0, pyArg1);

        PyObject* pyArg2 = PyFloat_FromDouble(angle);
        if (pyArgs == NULL) {
            std::cerr << "pyArg2においてエラー" << std::endl;
        }
        PyTuple_SetItem(pyArgs, 1, pyArg2);

        PyObject_CallObject(func_, pyArgs);
    } else {
        std::cerr << "実行不可能" << std::endl;
    }
}

void Arm::closeFinger() {
    printf("finger_value_ = %f\n", finger_value_);
    for (finger_value_ = 2.5; finger_value_ <= 12; finger_value_ += 0.5) {
        printf("finger_value_ = %f\n", finger_value_);
        servo(finger_pin_, finger_value_);
        sleep(1);
    }
}

void Arm::openFinger() {
    for (finger_value_ = 12; finger_value_ >= 2.5; finger_value_ -= 0.5) {
        printf("finger_value_ = %f\n", finger_value_);
        servo(finger_pin_, finger_value_);
        sleep(1);
    }
}

void Arm::twistWrist() {
    servo(wrist_pin_, 8.0);
}

void Arm::returnWrist() {
    servo(wrist_pin_, 5.0);
}

void Arm::downElbow() {
    servo(elbow_pin_, 8.0);
}

void Arm::upElbow() {
    servo(elbow_pin_, 5.5);
}

void Arm::downShoulder() {
    servo(shoulder_pin_, 8.0);
}

void Arm::upShoulder() {
    servo(shoulder_pin_, 5.5);
}
