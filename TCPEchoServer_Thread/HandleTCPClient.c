#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h>     /* for close() */
#include <netinet/in.h>  // for ntohl()
#include "../common/ccLemon.h"

#define RCVBUFSIZE 32   /* Size of receive buffer */

void DieWithError(char *errorMessage);  /* Error handling function */

void HandleTCPClient(int clntSocket)
{
    char echoBuffer[RCVBUFSIZE];        /* Buffer for echo string */
    ssize_t recvMsgSize;                    /* Size of received message */
    int receivedInt;
    player p;

    // CPUの作成 (CPUは常にチャージを選択する)
    player cpu;
    cpu.cost = 0;
    cpu.cmd = HAND_CHARGE;

    /* Receive message from client */
    if ((recvMsgSize = recv(clntSocket, &p, sizeof(player), 0)) < 0)
        DieWithError("recv() failed");

    /* Send received string and receive again until end of transmission */
    while (recvMsgSize > 0)      /* zero indicates end of transmission */
    {
        printf("Command: %d\n", p.cmd);
        if (p.status == STATUS_REQ_ROOM_CREATE) {
            // ルームの作成処理をここに追加
            printf("room created\n");
            p.status = STATUS_RES_GAME_UNDECIDED;
        } else if (p.status == STATUS_REQ_ROOM_JOIN) {
            // ルームの参加処理をここに追加
            printf("room join\n");
        } else if (p.status == STATUS_REQ_SEND_HAND) {
            printf("received hand: %d\n", p.cmd);
            // 勝敗判定
            Judge(&p, &cpu);
        }

        /* Echo message back to client */
        if (send(clntSocket, &p, recvMsgSize, 0) != recvMsgSize)
            DieWithError("send() failed");

        /* See if there is more data to receive */
        if ((recvMsgSize = recv(clntSocket, &p, sizeof(player), 0)) < 0)
            DieWithError("recv() failed");
    }

    close(clntSocket);    /* Close client socket */
}
