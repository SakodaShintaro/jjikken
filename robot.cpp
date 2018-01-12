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

void Robot::stopAndTurn(Direction direction, double p) {
    switch (direction) {
    case RIGHT:
        std::cout << "右を向く" << std::endl;
        right_wheel_.run(-default_speed / 2 * p);
        left_wheel_.run(+default_speed / 2 * p);
        //どれくらい回ったら止まるかも判断したけど……
        break;
    case LEFT:
        std::cout << "左を向く" << std::endl;
        right_wheel_.run(+default_speed / 2 * p);
        left_wheel_.run(-default_speed / 2 * p);
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
        printf("・goForward\n");
        printf("・goBack\n");
        printf("・turnRight\n");
        printf("・turnLeft\n");
        printf("・stop\n");
        printf("・traceHumanFace\n");
        printf("・approachObject\n");
        printf("・showNowView\n");
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
        printf("・turnRight90\n");
        printf("・turnLeft90\n");
        printf("・goSquare(n)\n");
        printf("・sequence\n");
        printf("・prepareForCatch\n");
        printf("・catchAndUp\n");
        printf("・capture\n");
        printf("・detectPiece\n");
        printf("・detectAndCatch\n");
    };
    printHelp();
    std::string s;
    std::unique_ptr<std::thread> thread_ptr;
    while (std::cin >> s) {
        stop_signal_ = false;
        if (s == "goForward") {
            run(FORWARD, default_speed);
        } else if (s == "goBack") {
            run(BACK, default_speed);
        } else if (s == "stop") {
            stop();
        } else if (s == "traceHumanFace") {
            traceHumanFace();
        } else if (s == "detectPiece") {
            cv::Point p;
            while (true) {
                camera_.detectPiece(p);
                printf("(%d, %d)\n", p.x, p.y);
            }
        } else if (s == "detectAndCatch") {
            goGoodPosition();
            prepareForCatch();
            catchAndUp();
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
            catchObject();
        } else if (s == "open") {
            releaseObject(false);
        } else if (s == "twistAndOpen") {
            releaseObject(true);
        } else if (s == "playShogi") {
            playShogi();
        } else if (s == "turnRight90") {
            turn90(RIGHT);
        } else if (s == "turnLeft90") {
            turn90(LEFT);
        } else if (s == "goSquare") {
            int n;
            std::cin >> n;
            goSquare(n);
            backSquare(n);
        } else if (s == "sequence") {
            turn90(LEFT);
            goSquare(3);
            turn90(RIGHT);
            goSquare(2);
            catchObject();
            goSquare(1);
            releaseObject(false);
            backSquare(3);
            turn90(RIGHT);
            goSquare(3);
            turn90(LEFT);
        } else if (s == "prepareForCatch") {
            prepareForCatch();
        } else if (s == "catchAndUp") {
            catchAndUp();
        } else if (s == "capture") {
            run(FORWARD);
            while (true) {
                camera_.capture();
                sleep(1);
            }
        } else if (s == "help") {
            printHelp();
        } else if (s == "quit" || s == "exit") {
            if (thread_ptr->joinable()) {
                thread_ptr->join();
            }
            return;
        } else if (s == "detectSomething") {
            bool t = camera_.detectSomething();
            std::cout << std::boolalpha << t << std::endl;
            if (!t) {
                run(FORWARD);
            } else {
                stop();
            }
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
            run(FORWARD);
        } else {
            while (ds_.measureDistance() < threshold) {
                stopAndTurn(LEFT);
            }
            stop();
        }
    }
}

void Robot::turn90(Direction direction) {
    assert(direction == RIGHT || direction == LEFT);

    auto gradients = camera_.getGradient();
    if (gradients.size() == 0) {
        printf("空\n");
    } else {
        for (auto grad : gradients) {
            printf("grad = %f, atan = %f\n", grad, atan(grad) * 180.0 / (CV_PI / 2));
        }
    }

    bool turn45 = false;
    double before = 0.0, bbefore = 0.0;

    stopAndTurn(direction);
    while (true) {
        auto gradients = camera_.getGradient();
        if (gradients.size() == 0) {
            printf("空\n");
        } else {
            double ave = 0;
            int num = 0;
            for (auto grad : gradients) {
                double angle = atan(grad) * 180.0 / (CV_PI / 2);
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
            //printf("ave = %f\n", ave);

            if (!turn45 && direction == LEFT && ave >= 40) {
                turn45 = true;
                printf("flag on --------------------------------\n");
            }
            if (!turn45 && direction == RIGHT && ave <= -40) {
                turn45 = true;
                printf("flag on --------------------------------\n");
            }

            double value = std::abs(ave) + std::abs(before) + std::abs(bbefore) / 3.0;

            if (turn45 && value <= 4.0) {
                stop();
                return;
            }
            printf("value = %f\n", value);

            if (turn45) {
                stopAndTurn(direction, std::min(value / 45.0 + 0.1, 1.0));
            }
            bbefore = before;
            before = ave;
        }
    }
}

void Robot::goSquare(int n) {
    printf("start goSquare(%d)\n", n);
    run(FORWARD, default_speed);
    double before = 0.0;
    bool over = false;
    int over_square = 0;
    for (int i = 0; i < 4000; i++) {
        auto ys = camera_.getHorizontalLineY();
        std::vector<double> y_ave;
        std::vector<int> num;
        //printf("now itr i = %d\n", i);
        for (auto y : ys) {
            if (y_ave.size() == 0) {
                y_ave.push_back(y);
                num.push_back(1);
            } else {
                bool flag = false;
                for (unsigned int i = 0; i < y_ave.size(); i++) {
                    if (y_ave[i] / num[i] - 5 <= y && y <= y_ave[i] / num[i] + 5) {
                        y_ave[i] += y;
                        num[i]++;
                        flag = true;
                        break;
                    }
                }
                if (!flag) {
                    y_ave.push_back(y);
                    num.push_back(1);
                }
            }
        }

        if (y_ave.size() == 0) {
            printf("空\n");
        } else if (y_ave.size() == 1) {
            before = y_ave[0] / num[0];
            printf("before = %15f\n", before);
        } else {
            for (unsigned int j = 0; j < y_ave.size(); j++) {
                if (y_ave[j] / num[j] <= before + 3) {
                    before = y_ave[j] / num[j];
                    break;
                }
            }
            printf("before = %15f\n", before);
        }
        if (!over && before >= 300) {
            printf("over_on ----------------------------\n");
            over = true;
        }
        static const int goal = 140;
        static const int margin = 20;
        if (over && goal - margin <= before && before < goal + margin) {
            over = false;
            printf("over_off----------------------------\n");
            over_square++;
            printf("over_square = %d\n", over_square);
            if (over_square == n) {
                stop();
                return;
            }
        }

        //ここから横の調整
        auto gradients = camera_.getGradient();
        double grad;
        if (gradients.size() == 0) {
            printf("空\n");
        } else {
            double ave = 0;
            int num = 0;
            for (auto grad : gradients) {
                double angle = atan(grad) * 180.0 / (CV_PI / 2);
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
            printf("grad = %f\n", ave);
            grad = ave;
        }

        static const double threshold_grad = 3.0;
        if (std::abs(grad) <= threshold_grad) {
            double x = camera_.getVerticalLineX();
            std::cout << "x = " << x << std::endl;
            if (x < 0) {
                run(FORWARD);
                continue;
            }
            static const int goal_x = 400, margin_x = 20;
            if (x < goal_x - margin_x) {
                curve(LEFT);
                usleep(6e4);
                run(FORWARD);
            } else if (x > goal_x + margin_x) {
                curve(RIGHT);
                usleep(6e4);
                run(FORWARD);
            }
        } else {
            if (grad > 0) {
                curve(RIGHT);
                usleep(6e4);
                run(FORWARD);
            } else {
                curve(LEFT);
                usleep(6e4);
                run(FORWARD);
            }
        }

    }
    stop();
}

void Robot::backSquare(int n) {
    run(BACK, default_speed);
    double before = 0.0;
    bool over = false;
    int over_square = 0;
    for (int i = 0; i < 40000; i++) {
        auto ys = camera_.getHorizontalLineY();
        std::vector<double> y_ave;
        std::vector<int> num;
        //printf("now itr i = %d\n", i);
        for (auto y : ys) {
            if (y_ave.size() == 0) {
                y_ave.push_back(y);
                num.push_back(1);
            } else {
                bool flag = false;
                for (unsigned int i = 0; i < y_ave.size(); i++) {
                    if (y_ave[i] / num[i] - 5 <= y && y <= y_ave[i] / num[i] + 5) {
                        y_ave[i] += y;
                        num[i]++;
                        flag = true;
                        break;
                    }
                }
                if (!flag) {
                    y_ave.push_back(y);
                    num.push_back(1);
                }
            }
        }

        if (y_ave.size() == 0) {
            printf("空\n");
        } else if (y_ave.size() == 1) {
            before = y_ave[0] / num[0];
            printf("%15f\n", before);
        } else {
            for (unsigned int j = 0; j < y_ave.size(); j++) {
                if (y_ave[j] / num[j] <= before + 3) {
                    before = y_ave[j] / num[j];
                    break;
                }
            }
            printf("%15f\n", before);
        }
        if (!over && before <= 40) {
            printf("over_on------------------------------\n");
            over = true;
        }
        static const int goal = 140;
        static const int margin = 20;
        if (over && goal - margin <= before && before < goal + margin) {
            over = false;
            printf("over_off-----------------------------\n");
            over_square++;
            printf("over_square = %d\n", over_square);
            if (over_square == n) {
                stop();
                return;
            }
        }

        //if (over && 30 <= before && before < 40) {
        //    over = false;
        //    over_square++;
        //    printf("over_square = %d\n", over_square);
        //    if (over_square == n) {
        //        stop();
        //        return;
        //    }
        //}
        //printf("\n");
    }
    stop();
}

void Robot::catchObject() {
    arm_.bendElbow();
    arm_.downShoulder();
    sleep(1);
    arm_.closeFinger();
    arm_.upShoulder();
    arm_.straightenElbow();
}

void Robot::prepareForCatch() {
    arm_.openFinger();
    arm_.bendElbow();
    arm_.downShoulder();
}

void Robot::catchAndUp() {
    arm_.closeFinger();
    arm_.upShoulder();
    arm_.straightenElbow();
}

void Robot::releaseObject(bool twist) {
    if (twist) {
        arm_.twistWrist();
    }
    arm_.bendElbow();
    arm_.downShoulder();
    arm_.openFinger();
    arm_.upShoulder();
    arm_.straightenElbow();
    if (twist) {
        arm_.returnWrist();
    }
}

void Robot::goGoodPosition() {
    static const int ok = 200;
    static const int margin = 1;
    cv::Point p, before(0, 0);
    while (true) {
        if (!camera_.detectPiece(p)) {
            printf("認識失敗\n");
            stop();
            continue;
        }

        printf("(%3d, %3d)\n", p.x, p.y);
        if (before.x == 0 && before.y == 0) {
            before = p;
        }

        if (ok - margin <= (p.y + before.y) / 2 && (p.y + before.y) / 2 <= ok + margin) {
            printf("ちょうど良い\n");
            break;
        } else if ((p.y + before.y) / 2 < ok - margin) {
            printf("yが小さい\n");
            run(FORWARD, default_speed / 8);
        } else {
            printf("yが大きい\n");
            run(BACK, default_speed / 8);
        }
        before = p;
    }
    stop();
}
