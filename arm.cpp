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

extern constexpr int sleep_usec = 300000;

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
    servo(finger_pin_, finger_close_bound);
    //for (; finger_value_ <= finger_close_bound; finger_value_ += 0.5) {
    //    printf("finger_value_ = %f\n", finger_value_);
    //    servo(finger_pin_, finger_value_);
    //    usleep(sleep_usec);
    //}
}

void Arm::openFinger() {
    servo(finger_pin_, finger_open_bound);
    //for (; finger_value_ >= finger_open_bound; finger_value_ -= 0.5) {
    //    printf("finger_value_ = %f\n", finger_value_);
    //    servo(finger_pin_, finger_value_);
    //    usleep(sleep_usec);
    //}
}

void Arm::twistWrist() {
    servo(wrist_pin_, wrist_back_bound);
    //for ( ; wrist_value_ <= wrist_back_bound; wrist_value_ += 1.0) {
    //    printf("wrist_value_ = %f\n", wrist_value_);
    //    servo(wrist_pin_, wrist_value_);
    //    usleep(sleep_usec);
    //}
}

void Arm::returnWrist() {
    servo(wrist_pin_, wrist_front_bound);
    //for ( ; wrist_value_ >= wrist_front_bound; wrist_value_ -= 1.0) {
    //    printf("wrist_value_ = %f\n", wrist_value_);
    //    servo(wrist_pin_, wrist_value_);
    //    usleep(sleep_usec);
    //}
}

void Arm::bendElbow() {
    servo(elbow_pin_, elbow_bend_bound);
    //for ( ; elbow_value_ <= elbow_bend_bound; elbow_value_ += 0.5) {
    //    printf("elbow_value_ = %f\n", elbow_value_);
    //    servo(elbow_pin_, elbow_value_);
    //    usleep(sleep_usec);
    //}
}

void Arm::straightenElbow() {
    servo(elbow_pin_, elbow_straight_bound);
    //for ( ; elbow_value_ >= elbow_straight_bound; elbow_value_ -= 0.5) {
    //    printf("elbow_value_ = %f\n", elbow_value_);
    //    servo(elbow_pin_, elbow_value_);
    //    usleep(sleep_usec);
    //}
}

void Arm::downShoulder() {
    for (; shoulder_value_ < shoulder_down_bound; shoulder_value_ += 0.2) {
        printf("shoudler_value_ = %f\n", shoulder_value_);
        servo(shoulder_pin_, shoulder_value_);
        usleep(sleep_usec);
    }
    //for (shoulder_value_ = shoulder_down_bound - 1; shoulder_value_ < shoulder_down_bound; shoulder_value_ += 0.1) {
    //    printf("shoudler_value_ = %f\n", shoulder_value_);
    //    servo(shoulder_pin_, shoulder_value_);
    //    usleep(sleep_usec);
    //}
}

void Arm::upShoulder() {
    //servo(shoulder_pin_, shoulder_up_bound);
    for (; shoulder_value_ > shoulder_up_bound; shoulder_value_ -= 0.2) {
        printf("shoudler_value_ = %f\n", shoulder_value_);
        servo(shoulder_pin_, shoulder_value_);
        usleep(sleep_usec);
    }
}
