#include"robot.hpp"

void Robot::run(Direction direction, int speed) {
    switch (direction) {
    case FORWARD:
        std::cout << "前進" << std::endl;
        right_wheel_.run(speed);
        left_wheel_.run(speed);
        break;
    case BACK:
        std::cout << "後進" << std::endl;
        right_wheel_.run(-speed);
        left_wheel_.run(-speed);
        break;
    case RIGHT:
        //右を向いてから前進
        std::cout << "右を向いてから前進" << std::endl;
        stopAndTurn(RIGHT);
        run(FORWARD, speed);
        break;
    case LEFT:
        //左を向いてから前進
        std::cout << "左を向いてから前進" << std::endl;
        stopAndTurn(LEFT);
        run(FORWARD, speed);
        break;
    default:
        //ここには来ないはず
        assert(false);
    }
}

void Robot::stop() {
    std::cout << "停止" << std::endl;
    right_wheel_.emergencyStop();
    left_wheel_.emergencyStop();
}

void Robot::stopAndTurn(Direction direction) {
    switch (direction) {
    case RIGHT:
        std::cout << "右を向く" << std::endl;
        right_wheel_.run(-default_speed / 2);
        left_wheel_.run(+default_speed / 2);
        //どれくらい回ったら止まるかも判断したけど……
        break;
    case LEFT:
        std::cout << "左を向く" << std::endl;
        right_wheel_.run(+default_speed / 2);
        left_wheel_.run(-default_speed / 2);
        //どれくらい回ったら止まるかも判断したけど……
        break;
    default:
        //ここには来ないはず
        assert(false);
    }
}

void Robot::turn(int speed) {
    if (speed >= 0) {
        //speedが正なら右を向く
        std::cout << "右を向く" << std::endl;
        right_wheel_.run(speed);
        left_wheel_.run(speed);
    } else {
        //speedが負なら左を向く
        std::cout << "左を向く" << std::endl;
        right_wheel_.run(-speed);
        left_wheel_.run(-speed);
    }
}

void Robot::loop() {
    std::cout << "loop開始" << std::endl;
    std::cout << "コマンドを入力してください" << std::endl;
    auto printHelp = [&](){
        printf("・help\n");
        printf("・speedUp\n");
        printf("・speedDown\n");
        printf("・goForward\n");
        printf("・goBack\n");
        printf("・curveRight\n");
        printf("・curveLeft\n");
        printf("・turnRight\n");
        printf("・turnLeft\n");
        printf("・stop\n");
        printf("・stopAndTurnRight\n");
        printf("・stopAndTurnLeft\n");
        printf("・traceHumanFace\n");
        printf("・approachObject\n");
        printf("・showNowView\n");
        printf("・openFinger\n");
        printf("・closeFinger\n");
        printf("・twistWrist\n");
        printf("・returnWrist\n");
        printf("・bendElbow\n");
        printf("・straightenElbow\n");
        printf("・downnShoulder\n");
        printf("・upShoulder\n");
        printf("・catch\n");
        printf("・open\n");
        printf("・twistAndOpen\n");
        printf("・playShogi\n");
        printf("・printGrad\n");
        printf("・goAlongLine\n");
    };
    printHelp();
    std::string s;
    std::unique_ptr<std::thread> thread_ptr;
    while (std::cin >> s) {
        stop_signal_ = false;
        if (s == "speedUp") {
            speedUp();
        } else if (s == "speedDown") {
            speedDown();
        } else if (s == "goForward") {
            run(FORWARD, default_speed);
        } else if (s == "goBack") {
            run(BACK, default_speed);
        } else if (s == "curveRight") {
            curve(RIGHT);
        } else if (s == "curveLeft") {
            curve(LEFT);
        } else if (s == "stopAndTurnRight") {
            stopAndTurn(RIGHT);
        } else if (s == "stopAndTurnLeft") {
            stopAndTurn(LEFT);
        } else if (s == "stop") {
            stop();
        } else if (s == "traceHumanFace") {
            traceHumanFace();
        } else if (s == "approachObject") {
            std::string input;
            std::thread t(&Robot::approachObject, this);
            while (std::cin >> input) {
                if (input == "stop") {
                    stop_signal_ = true;
                    stop();
                    break;
                }
            }
        } else if (s == "printHelp") {
            printHelp();
        } else if (s == "showNowView") {
            thread_ptr.reset(new std::thread(&Robot::showNowView, this));
        } else if (s == "openFinger") {
            arm_.openFinger();
        } else if (s == "closeFinger") {
            arm_.closeFinger();
        } else if (s == "twistWrist") {
            arm_.twistWrist();
        } else if (s == "returnWrist") {
            arm_.returnWrist();
        } else if (s == "bendElbow") {
            arm_.bendElbow();
        } else if (s == "straightenElbow") {
            arm_.straightenElbow();
        } else if (s == "downShoulder") {
            arm_.downShoulder();
        } else if (s == "upShoulder") {
            arm_.upShoulder();
        } else if (s == "catch") {
            arm_.bendElbow();
            arm_.downShoulder();
            arm_.closeFinger();
            arm_.upShoulder();
            arm_.straightenElbow();
        } else if (s == "open") {
            arm_.bendElbow();
            arm_.downShoulder();
            arm_.openFinger();
            arm_.upShoulder();
            arm_.straightenElbow();
        } else if (s == "twistAndOpen") {
            arm_.twistWrist();
            arm_.bendElbow();
            arm_.downShoulder();
            arm_.openFinger();
            arm_.upShoulder();
            arm_.straightenElbow();
            arm_.returnWrist();
        } else if (s == "playShogi") {
            playShogi();
        } else if (s == "printGrad") {
            std::string input;
            std::thread t(&Robot::printGrad, this);
            while (std::cin >> input) {
                if (input == "stop") {
                    stop_signal_ = true;
                    stop();
                    break;
                }
            }
        } else if (s == "goAlongLine") {
            std::string input;
            std::thread t(&Robot::goAlongLine, this);
            while (std::cin >> input) {
                if (input == "stop") {
                    stop_signal_ = true;
                    stop();
                    break;
                }
            }
        } else if (s == "help") {
            printHelp();
        } else if (s == "quit" || s == "exit") {
            if (thread_ptr->joinable()) {
                thread_ptr->join();
            }
            return;
        } else {
            std::cerr << "不正な入力" << std::endl;
        }
    }
}

void Robot::speedUp() {
    right_wheel_.speedUp();
    left_wheel_.speedUp();
}

void Robot::speedDown() {
    right_wheel_.speedDown();
    left_wheel_.speedDown();
}

void Robot::curve(Direction direction) {
    switch (direction) {
    case RIGHT:
        std::cout << "右に曲がる" << std::endl;
        //left_wheel_.speedUp();
        right_wheel_.speedDown();
        break;
    case LEFT:
        std::cout << "左に曲がる" << std::endl;
        //right_wheel_.speedUp();
        left_wheel_.speedDown();
        break;
    default:
        //ここには来ないはず
        assert(false);
    }
}

void Robot::traceHumanFace() {
    static const int A = 1;
    while (true) {
        int distance_from_center;
        if (camera_.detectHumanFace(distance_from_center)) {
            while (camera_.detectHumanFace(distance_from_center) && distance_from_center >= 10) {
                std::cout << "distance_from_center = " << distance_from_center << std::endl;
                int rotate_speed = distance_from_center * A;
                std::cout << "rotate_speed = " << rotate_speed << std::endl;
                std::cin >> rotate_speed;
                turn(rotate_speed);
            }
        } else {
            stop();
        }
    }
}

void Robot::approachObject() {
    static const int threshold = 50;
    while (true) {
        if (stop_signal_) {
            break;
        }
        auto distance = ds_.measureDistance();
        if (distance >= threshold && distance <= 1000) {
            //run(FORWARD);
        } else {
            while (ds_.measureDistance() < threshold) {
                //stopAndTurn(LEFT);
            }
            //stop();
        }
    }
}

void Robot::printGrad() {
    while (true) {
        auto gradients = camera_.getGradient();
        printf("表示\n");
        for (auto grad : gradients) {
            printf("grad = %f, atan = %f\n", grad, atan(grad) * 180.0 / (3.1415926535 / 2));
        }
        sleep(1);
    }
}

void Robot::goAlongLine() {
    static double PI = 3.1415926535;
    long long i = 0;
    while (true) {
        i++;
        //sleep(1);
        auto gradients = camera_.getGradient();
        printf("%lld回目\n", i);
        for (auto grad : gradients) {
            printf("grad = %f, atan = %f\n", grad, atan(grad) * 180.0 / (PI / 2));
        }

        if (gradients.size() == 0) {
            printf("空\n");
            run(FORWARD, default_speed);
            continue;
        }

        double ave = 0;
        int num = 0;
        for (auto grad : gradients) {
            double angle = atan(grad) * 180.0 / (PI / 2);
            if (abs(angle) <= 45) {
                //横の線
                ave += angle;
            } else {
                //縦の線
                ave += (angle >= 0 ? -(180.0 - angle) : 180.0 + angle);
            }
            num++;
        }
        ave /= num;
        //目標値との差
        printf("ave = %f\n", ave);
        if (abs(ave) >= 45) {
            std::cerr << "さすがに45度以上はおかしい" << std::endl;
            stop();
            sleep(5);
            continue;
        }
        if (ave > 1.0) {
            curve(RIGHT);
            if (usleep(ave * 10000) < 0) {
                std::cerr << "usleep失敗" << std::endl;
            }
            run(FORWARD, default_speed);
            usleep(100000);
        } else if (ave < -1.0) {
            curve(LEFT);
            if (usleep(-ave * 10000) < 0) {
                std::cerr << "usleep失敗" << std::endl;
            }
            run(FORWARD, default_speed);
            usleep(100000);
        } else {
            run(FORWARD, default_speed);
            sleep(1);
        }
    }
}
