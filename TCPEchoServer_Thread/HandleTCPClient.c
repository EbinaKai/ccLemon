#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h>     /* for close() */
#include <netinet/in.h>  // for ntohl()
#include <pthread.h>        /* for POSIX threads */
#include "../common/ccLemon.h"
#include "GameRoom.h"

void DieWithError(char *errorMessage);  /* Error handling function */

void HandleTCPClient(int clntSocket, pthread_t threadID, Room *roomList)
{
    ssize_t recvMsgSize;                    /* Size of received message */
    int receivedInt;
    player p;
    Room room;

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
        switch(p.status) {
            case STATUS_REQ_ROOM_CREATE:
                // ルームの作成処理
                printf("room created\n");
                room = *createRoom(roomList);
                room.player1 = threadID;
                p.roomID = room.roomID;
                drawRoom(roomList);
                p.status = STATUS_RES_ROOM_CREATED;
                break;
            
            case STATUS_REQ_ROOM_JOIN:
                // ルームの参加処理
                drawRoom(roomList);
                room = *getRoom(roomList, p.roomID);
                if (room.roomID == -1) {        // ルームが存在しない場合
                    printf("room not found\n");
                    p.status = STATUS_RES_ROOM_NOT_FOUND;
                } else if (room.isRoomFull) {   // ルームが満員の場合
                    printf("room is full\n");
                    p.status = STATUS_RES_ROOM_IS_FULL;
                } else {                        // ルームに参加
                    printf("room joined\n");
                    room.player2 = threadID;
                    p.status = STATUS_RES_ROOM_JOINNED;
                }
                break;
            
            case STATUS_REQ_SEND_HAND:
                room = *getRoom(roomList, p.roomID);
                printf("received hand: %d\n", p.cmd);
                Judge(&p, &cpu);    // 勝敗判定
                p.enemyCmd = cpu.cmd;
                break;  
            
            default:
                break;
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
