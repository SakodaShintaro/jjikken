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
        printf("p : speedUp\n");
        printf("q : speedDown\n");
        printf("f : goForward\n");
        printf("b : goBack\n");
        printf("r : curveRight\n");
        printf("l : curveLeft\n");
        printf("R : turnRight\n");
        printf("L : turnLeft\n");
        printf("s : stop\n");
        printf("a : traceHumanFace\n");
        printf("d : approachObject\n");
        printf("h : help\n");
        printf("o : openFinger\n");
        printf("c : closeFinger\n");
        printf("w : show now view\n");
    };
    printHelp();
    char c;
    while (std::cin >> c) {
        switch (c) {
        case 'p':
            speedUp();
            break;
        case 'q':
            speedDown();
            break;
        case 'f':
            run(FORWARD, default_speed);
            break;
        case 'b':
            run(BACK, default_speed);
            break;
        case 'r':
            curve(RIGHT);
            break;
        case 'l':
            curve(LEFT);
            break;
        case 'R':
            stopAndTurn(RIGHT);
            break;
        case 'L':
            stopAndTurn(LEFT);
            break;
        case 's':
            stop();
            break;
        case 'a':
            traceHumanFace();
            break;
        case 'd':
            approachObject();
            break;
        case 'o':
            arm_.openFinger();
            break;
        case 'c':
            arm_.closeFinger();
            break;
        case 'h':
            printHelp();
            break;
        case 'w':
            std::cout << "start show" << std::endl;
            showNowView();
            std::cout << "end show" << std::endl;
            break;
        case 'x':
            return;
        default:
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
        left_wheel_.speedUp();
        break;
    case LEFT:
        std::cout << "左に曲がる" << std::endl;
        right_wheel_.speedUp();
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
