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
    com_help();
    
    printf("ルームを作成しますか？(y/n): ");
    InputMode(&me);
    if (me.status == STATUS_REQ_ROOM_JOIN) {
        printf("ルーム番号を入力してください: ");
        InputRoomId(&me);
    }

    while (1) {

        // コマンドの入力
        if (me.status == STATUS_RES_GAME_UNDECIDED) {
            printf("入力してください(%d): ", me.cost);
            InputHand(&me);
        }

        /* Send the string to the server */
        if (send(sock, &me, sizeof(me), 0) != sizeof(me))
            DieWithError("send() sent a different number of bytes than expected");

        /* Receive the same string back from the server */
        if ((recv(sock, &me, sizeof(me), 0)) <= 0)
            DieWithError("recv() failed or connection closed prematurely");
        printf("Received: %d\n", me.status);      /* Print the echo buffer */

        // ゲームの終了判定
        if (me.status == STATUS_RES_GAME_LOSE || me.status == STATUS_RES_GAME_WIN) {
            break;
        }
    }

    close(sock);
    exit(0);
}
