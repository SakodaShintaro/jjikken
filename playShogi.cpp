#include"robot.hpp"
#include"position.hpp"

void Robot::playShogi() {
    Position position;
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
                moves.push_back(bestmove);
                position.doMove(stringToMove(bestmove));
                doMove(position.lastMove());
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

void Robot::doMove(const Move& move) {
    //初期位置からfromへ
    int from_rank = SquareToRank[move.from()];
    printf("from_rank = %d\n", from_rank);
    int from_file = SquareToFile[move.from()];
    printf("from_file = %d\n", from_file);
    ///筋を合わせる
    int file_diff = 5 - from_file;
    if (file_diff < 0) {
        turn90(LEFT);
        goSquare(-file_diff);
        //goSquareAlongLine(-file_diff);
        turn90(RIGHT);
    } else if (file_diff > 0) {
        turn90(RIGHT);
        goSquare(file_diff);
        //goSquareAlongLine(file_diff);
        turn90(LEFT);
    }
    ///段を合わせる
    goSquare(9 - from_rank);
    //goSquareAlongLine(9 - from_rank);

    //駒を掴む
    //catchObject();
    run(BACK);
    usleep(1000000);
    stop();
    //ここで駒がつかめる位置に調整
    goGoodPosition();
    catchObject();
    usleep(100000);

    //fromからtoへ
    int to_rank = SquareToRank[move.to()];
    int to_file = SquareToFile[move.to()];
    ///筋を合わせる
    file_diff = from_file - to_file;
    if (file_diff < 0) {
        turn90(LEFT);
        goSquare(-file_diff);
        //goSquareAlongLine(-file_diff);
        turn90(RIGHT);
    } else if (file_diff > 0) {
        turn90(RIGHT);
        goSquare(file_diff);
        //goSquareAlongLine(file_diff);
        turn90(LEFT);
    }
    ///段を合わせる
    int rank_diff = from_rank - to_rank;
    if (rank_diff < 0) {
        backSquare(-rank_diff);
    } else {
        goSquare(rank_diff);
        //goSquareAlongLine(rank_diff);
    }

    //駒を離す
    run(BACK);
    usleep(1000000);
    stop();
    run(FORWARD);
    usleep(100000);
    stop();
    usleep(100000);
    releaseObject(move.isPromote());

    //初期位置に戻る
    ///段を戻す
    backSquare(9 - to_rank);

    file_diff = to_file - 5;
    if (file_diff < 0) {
        turn90(LEFT);
        goSquare(-file_diff);
        //goSquareAlongLine(-file_diff);
        turn90(RIGHT);
    } else if (file_diff > 0) {
        turn90(RIGHT);
        goSquare(file_diff);
        //goSquareAlongLine(file_diff);
        turn90(LEFT);
    }
}
