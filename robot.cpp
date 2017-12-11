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
    };
    printHelp();
    std::string s;
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
            showNowView();
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
        } else if (s == "quit" || s == "exit") {
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

void Robot::playShogi() {
    //子プロセスとして思考エンジンを動かす
    //音声入力で人間の指し手を取得(Juliusを使う)
    //内部的に持っているのはSFEN文字列だけ(大丈夫か?)
    //まずはロボット先手Onlyで考えましょうか

    //準備1
    ///思考エンジンの準備
    enum {
        PARENT2CHILD, CHILD2PARENT
    };
    int pipefd[2][2];
    if (pipe(pipefd[PARENT2CHILD]) < 0) {
        perror("pipe");
    }
    if (pipe(pipefd[CHILD2PARENT]) < 0) {
        perror("pipe");
    }

    if (fork() == 0) {
        //子プロセス

        //パイプの準備
        dup2(pipefd[PARENT2CHILD][0], 0);
        close(pipefd[PARENT2CHILD][0]);
        close(pipefd[PARENT2CHILD][1]);
        dup2(pipefd[CHILD2PARENT][1], 1);
        close(pipefd[CHILD2PARENT][0]);
        close(pipefd[CHILD2PARENT][1]);

        //思考エンジンを起動
        char* argv[2];
        argv[0] = (char*)malloc(20);
        strcpy(argv[0], "./kaitei_gcc");
        argv[1] = NULL;
        execvp(argv[0], argv);
    }

    close(pipefd[PARENT2CHILD][0]);
    close(pipefd[CHILD2PARENT][1]);
    //指し手文字列のvectorを準備
    std::vector<std::string> moves;

    //usiを送る
    char buf[2048];
    if (write(pipefd[PARENT2CHILD][1], "usi\n", 5) < 0) {
        perror("write usi");
        exit(1);
    }

    if (read(pipefd[CHILD2PARENT][0], buf, 2048) < 0) {
        perror("read usi");
        exit(1);
    }
    printf("%s", buf);

    //setoptionを送る
    //if (write(pipefd[PARENT2CHILD][1], "setoption name USI_Hash value 256\n", 100) < 0) {
    //    perror("write setoption USI_Hash");
    //    exit(1);
    //}
    //if (write(pipefd[PARENT2CHILD][1], "setoption name byoyomi_margin value 0\n", 100) < 0) {
    //    perror("write setoption USI_Hash");
    //    exit(1);
    //}
    //if (write(pipefd[PARENT2CHILD][1], "setoption name random_turn value 0\n", 100) < 0) {
    //    perror("write setoption random_turn");
    //    exit(1);
    //}
    //if (write(pipefd[PARENT2CHILD][1], "setoption name fixed_depth value 0\n", 100) < 0) {
    //    perror("write setoption fixed_depth");
    //    exit(1);
    //}

    //isreadyを送る
    //printf("isreadyを送る前\n");
    //if (write(pipefd[PARENT2CHILD][1], "isready\n", 15) < 0) {
    //    perror("write isready");
    //    exit(1);
    //}
    //printf("isreadyを送った後\n");
    //if (read(pipefd[CHILD2PARENT][0], buf, 2047) < 0) {
    //    perror("read readyok");
    //    exit(1);
    //}
    //printf("readした後\n");
    //printf("%s", buf);

    //usinewgameを送る
    //if (write(pipefd[PARENT2CHILD][1], "usinewgame\n", 20) < 0) {
    //    perror("write isready");
    //    exit(1);
    //}
    //if (read(pipefd[CHILD2PARENT][0], buf, 2047) < 0) {
    //    perror("read usinewgame");
    //    exit(1);
    //}
    //printf("%s", buf);

    while (true) {
        //思考する
        char message_to_usi_engine[2048] = "position startpos moves";
        for (auto move : moves) {
            strcat(message_to_usi_engine, " ");
            strcat(message_to_usi_engine, move.c_str());
        }
        strcat(message_to_usi_engine, "\n");
        strcat(message_to_usi_engine, "go btime 0 wtime 0 byoyomi 2000\n");
        printf("%s\n", message_to_usi_engine);

        if (write(pipefd[PARENT2CHILD][1], message_to_usi_engine, strlen(message_to_usi_engine)) < 0) {
            perror("read message_to_usi_engine");
            exit(1);
        }

        sleep(2);

        char buf[2048];
        if (read(pipefd[CHILD2PARENT][0], buf, 2048) < 0) {
            perror("read usi_bestmove");
            exit(1);
        }

        char *ptr = strtok(buf, "\n");
        printf("%s\n", ptr);
        while (ptr != NULL) {
            ptr = strtok(NULL, "\n");
            printf("%s\n", ptr);
            if (strncmp(ptr, "bestmove", 8) == 0) {
                printf("今回のbestmove->%s---------------------------\n", ptr);
                std::string bestmove = ptr + 9;
                //ここで動く
                //pieceMove();
                moves.push_back(bestmove);
                //strncpy(bestmove, ptr + 9);
                break;
            }
        }

        //指し手をもらう
        std::string human_move;
        std::cin >> human_move;
        std::cout << "もらった指し手" << human_move << std::endl;
        moves.push_back(human_move);
    }
}
