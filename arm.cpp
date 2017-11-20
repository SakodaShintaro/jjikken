#include"arm.hpp"

Arm::Arm() {
    Py_Initialize();

    PyObject* pModule = PyImport_ImportModule("py_motor");
    if (pModule == NULL) {
        std::cout << "Moduleの読み込みに失敗" << std::endl;
        exit(1);
    }

    func_ = PyObject_GetAttrString(pModule, "move2");
    if (func_ == NULL) {
        std::cout << "関数の読み込みに失敗" << std::endl;
        exit(1);
    }

    if (!PyCallable_Check(func_)) {
        std::cout << "関数が実行できない" << std::endl;
        exit(1);
    }
}

Arm::~Arm() {
    Py_Finalize();
}

void Arm::closeFinger() {
    if (PyCallable_Check(func_)) {
        PyObject* pyArgs = PyTuple_New(2);
        if (pyArgs == NULL) {
            std::cerr << "pyArgsにおいてエラー" << std::endl;
        }

        PyObject* pyArg1 = PyLong_FromLong(finger_pin_);
        if (pyArgs == NULL) {
            std::cerr << "pyArg1においてエラー" << std::endl;
        }
        PyTuple_SetItem(pyArgs, 0, pyArg1);

        PyObject* pyArg2 = PyFloat_FromDouble(8.0);
        if (pyArgs == NULL) {
            std::cerr << "pyArg2においてエラー" << std::endl;
        }
        PyTuple_SetItem(pyArgs, 1, pyArg2);

        PyObject_CallObject(func_, pyArgs);
    } else {
        std::cerr << "実行不可能" << std::endl;
    }
}

void Arm::openFinger() {
    func_ = PyObject_GetAttrString(PyImport_ImportModule("py_motor"), "move2");
    if (PyCallable_Check(func_)) {
        Py_Initialize();

        PyObject* pyArgs = PyTuple_New(2);
        if (pyArgs == NULL) {
            std::cerr << "pyArgsにおいてエラー" << std::endl;
        }

        PyObject* pyArg1 = PyLong_FromLong(finger_pin_);
        if (pyArgs == NULL) {
            std::cerr << "pyArg1においてエラー" << std::endl;
        }
        PyTuple_SetItem(pyArgs, 0, pyArg1);

        PyObject* pyArg2 = PyFloat_FromDouble(5.0);
        if (pyArgs == NULL) {
            std::cerr << "pyArg2においてエラー" << std::endl;
        }
        PyTuple_SetItem(pyArgs, 1, pyArg2);

        PyObject_CallObject(func_, pyArgs);
    } else {
        std::cerr << "実行不可能" << std::endl;
    }
}
