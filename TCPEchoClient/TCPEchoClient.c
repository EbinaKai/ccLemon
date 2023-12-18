#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <arpa/inet.h>  // for htonl()
#include "../common/ccLemon.h"

void DieWithError(char *errorMessage);  /* Error handling function */

int main(int argc, char *argv[])
{
    int sock;                        /* Socket descriptor */
    struct sockaddr_in echoServAddr; /* Echo server address */
    unsigned short echoServPort;     /* Echo server port */
    char *servIP;                    /* Server IP address (dotted quad) */
    int receivedInt, SendCommand;
    player me;

    if ((argc < 2) || (argc > 3))    /* Test for correct number of arguments */
    {
       fprintf(stderr, "Usage: %s <Server IP> [<Echo Port>]\n",
               argv[0]);
       exit(1);
    }

    servIP = argv[1];             /* First arg: server IP address (dotted quad) */

    if (argc == 3)
        echoServPort = atoi(argv[2]); /* Use given port, if any */
    else
        echoServPort = 7;  /* 7 is the well-known port for the echo service */

    /* Create a reliable, stream socket using TCP */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");

    /* Construct the server address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));     /* Zero out structure */
    echoServAddr.sin_family      = AF_INET;             /* Internet address family */
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);   /* Server IP address */
    echoServAddr.sin_port        = htons(echoServPort); /* Server port */

    /* Establish the connection to the echo server */
    if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError("connect() failed");

    // プレイヤーの初期化
    PlayerInit(&me);

    // コマンドヘルプの表示
    CommandHelp();

    while (1) {

        // ステータスによってコンソールに表示するメッセージを変更
        switch(me.status) {
            case STATUS_REQ_GAME_HELP:
                CommandHelp();
                me.status = STATUS_RES_GAME_UNDECIDED;
                break;
            case STATUS_RES_ROOM_NOT_FOUND:
                printf("ルームが見つかりませんでした。");
                break;
            case STATUS_RES_ROOM_IS_FULL:
                printf("ルームが満員です。");
                break;
            case STATUS_RES_ROOM_CREATED:
                printf("ルームを作成しました。ルーム番号は%dです。\n", me.roomID);
                break;
            case STATUS_RES_ROOM_JOINNED:
                printf("ルームに参加しました。\n");
                break;
            case STATUS_RES_GAME_TIMEOUT:
                printf("対戦相手がタイム・アウトしました。\n");
                cancelCommand(&me);
                break;
            case STATUS_RES_GAME_UNDECIDED: 
            case STATUS_RES_GAME_WIN: 
            case STATUS_RES_GAME_LOSE: 
                printf("YOU        ENEMY\n");
                printf("%-7s vs %-7s\n", getHandName(me.cmd), getHandName(me.enemyCmd));
                break;
            case STATUS_RES_ROOM_IS_NOT_FULL:
                printf("対戦相手の参加を待っています。\n");
                me.cost = 0;    // コストの初期化
                break;
            default:
                break;
        }

        // ステータスによって処理を分岐
        switch(me.status) {
            case STATUS_INIT:
            case STATUS_RES_ROOM_NOT_FOUND:
            case STATUS_RES_ROOM_IS_FULL:
                printf("ルームを作成しますか？(y/n/q): ");
                InputMode(&me);
                if (me.status != STATUS_REQ_ROOM_JOIN) 
                    break;
            
            case STATUS_REQ_ROOM_JOIN:
                printf("ルーム番号を入力してください: ");
                InputRoomId(&me);
                break;
            case STATUS_RES_ROOM_CREATED:
            case STATUS_RES_ROOM_JOINNED:
            case STATUS_RES_ROOM_IS_NOT_FULL:
            case STATUS_RES_GAME_TIMEOUT:
            case STATUS_RES_GAME_UNDECIDED:
                printf("コマンドを入力してください(%d): ", me.cost);
                InputHand(&me);
                if (me.status == STATUS_REQ_GAME_HELP ) {
                    printf("コマンドのヘルプを表示します\n");
                    continue;
                }
                break;
            default:
                break;
        }

        // ゲームの終了判定
        if (me.status == STATUS_RES_GAME_LOSE || me.status == STATUS_RES_GAME_WIN || me.status == STATUS_RES_GAME_QUIT) {
            break;
        }

        // 送信ステータスをデバッグ
        // printf("%s\n", getStatusName(me.status));

        /* Send the string to the server */
        if (send(sock, &me, sizeof(me), 0) != sizeof(me))
            DieWithError("send() sent a different number of bytes than expected");

        /* Receive the same string back from the server */
        if ((recv(sock, &me, sizeof(me), 0)) <= 0)
            DieWithError("recv() failed or connection closed prematurely");
    }

    // ソケットのクローズ
    close(sock);

    // 結果の表示
    if (me.status == STATUS_RES_GAME_WIN) {
        printf("あなたの勝ちです！\n");
    } else if (me.status == STATUS_RES_GAME_LOSE) {
        printf("あなたの負けです...\n");
    } else if (me.status == STATUS_RES_GAME_QUIT) {
        printf("ゲームを終了しました。\n");
    }

    // 終了
    exit(0);
}
