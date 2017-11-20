#include"arm.hpp"

Arm::Arm() {
    //pythonのモーター制御スクリプトを読み込む
    std::ifstream ifs("py_motor2.py");
    if (ifs.fail()) {
        std::cerr << "ファイルオープン失敗" << std::endl;
        return;
    }
    int begin = static_cast<int>(ifs.tellg());
    ifs.seekg(0, ifs.end);
    int end = static_cast<int>(ifs.tellg());
    int size = end - begin;
    ifs.clear();
    ifs.seekg(0, ifs.beg);
    python_script_ = new char[size + 1];
    python_script_[size] = '\0';
    ifs.read(python_script_, size);

    //ひどいやり方だなぁ
    Py_Initialize();
    PyRun_SimpleString(python_script_);

    main_ = PyModule_GetDict(PyImport_ImportModule("__main__"));
    func_ = PyDict_GetItemString(main_, "move2");
}

Arm::~Arm() {
    Py_Finalize();
    delete[] python_script_;
}

void Arm::closeFinger() {
    if (PyCallable_Check(func_)) {
        PyObject* pyArgs = PyTuple_New(2);
        if (pyArgs == NULL) {
            std::cerr << "pyArgsにおいてエラー" << std::endl;
        }

        PyObject* pyArg1 = PyLong_FromLong(33);
        if (pyArgs == NULL) {
            std::cerr << "pyArg1においてエラー" << std::endl;
        }
        PyTuple_SetItem(pyArgs, 0, pyArg1);

        PyObject* pyArg2 = PyLong_FromLong(8);
        if (pyArgs == NULL) {
            std::cerr << "pyArg2においてエラー" << std::endl;
        }
        PyTuple_SetItem(pyArgs, 1, pyArg2);

        PyObject_CallObject(func_, pyArgs);
    }
}

void Arm::openFinger() {
    if (PyCallable_Check(func_)) {
        PyObject* pyArgs = PyTuple_New(2);
        if (pyArgs == NULL) {
            std::cerr << "pyArgsにおいてエラー" << std::endl;
        }

        PyObject* pyArg1 = PyLong_FromLong(33);
        if (pyArgs == NULL) {
            std::cerr << "pyArg1においてエラー" << std::endl;
        }
        PyTuple_SetItem(pyArgs, 0, pyArg1);

        PyObject* pyArg2 = PyLong_FromLong(5);
        if (pyArgs == NULL) {
            std::cerr << "pyArg2においてエラー" << std::endl;
        }
        PyTuple_SetItem(pyArgs, 1, pyArg2);

        PyObject_CallObject(func_, pyArgs);
    }
}
