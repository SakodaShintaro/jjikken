#include"arm.hpp"

double Arm::shoulder_down_bound;
double Arm::elbow_bend_bound;

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

    std::cout << "elbow_bend_bound(7.4ぐらい) : ";
    std::cin >> elbow_bend_bound;

    std::cout << "shoulder_down_bound(9.0ぐらい) : ";
    std::cin >> shoulder_down_bound;

    shoulder_value_ = shoulder_up_bound + 0.1;
    upShoulder();
    elbow_value_ = elbow_straight_bound;
    straightenElbow();
    wrist_value_ = wrist_front_bound;
    returnWrist();
    finger_value_ = finger_open_bound;
    openFinger();
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

void Arm::servoG(int pin, double target_value, double cur_value) {
    func2_ = PyObject_GetAttrString(PyImport_ImportModule("py_motor"), "gradualMove");
    if (PyCallable_Check(func2_)) {
        PyObject* pyArgs = PyTuple_New(3);
        if (pyArgs == NULL) {
            std::cerr << "pyArgsにおいてエラー" << std::endl;
        }

        PyObject* pyArg1 = PyLong_FromLong(pin);
        if (pyArgs == NULL) {
            std::cerr << "pyArg1においてエラー" << std::endl;
        }
        PyTuple_SetItem(pyArgs, 0, pyArg1);

        PyObject* pyArg2 = PyFloat_FromDouble(target_value);
        if (pyArgs == NULL) {
            std::cerr << "pyArg2においてエラー" << std::endl;
        }
        PyTuple_SetItem(pyArgs, 1, pyArg2);

        PyObject* pyArg3 = PyFloat_FromDouble(cur_value);
        if (pyArgs == NULL) {
            std::cerr << "pyArg3においてエラー" << std::endl;
        }
        PyTuple_SetItem(pyArgs, 2, pyArg3);

        PyObject_CallObject(func2_, pyArgs);
    } else {
        std::cerr << "実行不可能" << std::endl;
    }
}

void Arm::closeFinger() {
    servo(finger_pin_, finger_close_bound);
}

void Arm::openFinger() {
    servo(finger_pin_, finger_open_bound);
}

void Arm::twistWrist() {
    servo(wrist_pin_, wrist_back_bound);
}

void Arm::returnWrist() {
    servo(wrist_pin_, wrist_front_bound);
}

void Arm::bendElbow() {
    servo(elbow_pin_, elbow_bend_bound);
}

void Arm::straightenElbow() {
    servo(elbow_pin_, elbow_straight_bound);
}

void Arm::downShoulder() {
    servoG(shoulder_pin_, shoulder_down_bound, shoulder_value_);
    shoulder_value_ = shoulder_down_bound;
}

void Arm::upShoulder() {
    servoG(shoulder_pin_, shoulder_up_bound, shoulder_value_);
    shoulder_value_ = shoulder_up_bound;
}
