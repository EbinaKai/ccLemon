#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h>     /* for close() */
#include <netinet/in.h>  // for ntohl()
#include <pthread.h>        /* for POSIX threads */
#include "../common/ccLemon.h"
#include "GameRoom.h"

void DieWithError(char *errorMessage);  /* Error handling function */

void HandleTCPClient(int clntSocket, Room *roomList)
{
    ssize_t recvMsgSize;                    /* Size of received message */
    int receivedInt;
    player p, enemy;
    Room *room;

    /* Receive message from client */
    if ((recvMsgSize = recv(clntSocket, &p, sizeof(player), 0)) < 0)
        DieWithError("recv() failed");

    /* Send received string and receive again until end of transmission */
    while (recvMsgSize > 0)      /* zero indicates end of transmission */
    {
        switch(p.status) {
            case STATUS_REQ_ROOM_CREATE:    // ルームの作成処理
                printf("room created\n");
                room = createRoom(roomList);
                p.roomID = room->roomID;
                p.status = STATUS_RES_ROOM_CREATED;
                break;
            
            case STATUS_REQ_ROOM_JOIN:  // ルームへの参加処理
                room = getRoom(roomList, p.roomID);

                if (room->roomID == -1) {   // ルームが存在しない場合
                    printf("room not found\n");
                    p.status = STATUS_RES_ROOM_NOT_FOUND;
                } 
                else if (room->isRoomFull) {  // ルームが満員の場合
                    printf("room is full\n");
                    p.status = STATUS_RES_ROOM_IS_FULL;
                } 
                else {    // ルームに参加
                    printf("room joined\n");
                    room->isRoomFull = 1;
                    p.status = STATUS_RES_ROOM_JOINNED;
                }
                break;

            case STATUS_REQ_GAME_QUIT: // ゲームを終了する場合
                printf("game quit\n");
                if (p.roomID != -1) {
                    room = getRoom(roomList, p.roomID);
                    deleteRoom(roomList, room);
                }
                p.status = STATUS_RES_GAME_QUIT;
                break;
            
            case STATUS_REQ_SEND_HAND:  // コマンドの処理
                printf("received hand: %d\n", p.cmd);
                room = getRoom(roomList, p.roomID);
                
                if (room->isRoomFull == 0) {     // ルームが満員でない場合
                    printf("room is not full\n");
                    p.status = STATUS_RES_ROOM_IS_NOT_FULL;
                    break;
                }

                if (room->waitingFlag == 0) {   // 相手のコマンド待ちの場合
                    printf("waiting for opponent's hand\n");
                    room->waitingFlag = 1;
                    room->player1 = &p;
                    while (room->waitingFlag);  // 相手のコマンドが確定するまで待機
                    enemy.cmd = room->player2->cmd;
                } 
                else {  // 相手のコマンド待ちでない場合
                    room->player2 = &p;
                    room->waitingFlag = 0;
                    enemy.cmd = room->player1->cmd;
                }

                Judge(&p, &enemy);  // 勝敗判定
                p.enemyCmd = enemy.cmd;

                if (p.status == STATUS_RES_GAME_WIN) // 勝利した場合はルームを削除
                    deleteRoom(roomList, room);
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
