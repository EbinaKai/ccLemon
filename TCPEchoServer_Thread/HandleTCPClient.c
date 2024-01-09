#include <stdio.h>      /* for log() and flog() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h>     /* for close() */
#include <netinet/in.h> // for ntohl()
#include <pthread.h>    /* for POSIX threads */
#include "../common/ccLemon.h"
#include "GameRoom.h"

void DieWithError(char *errorMessage);  /* Error handling function */
void logger(const char *format, ...);

void HandleTCPClient(int clntSocket, Room *roomList)
{
    ssize_t recvMsgSize;                    /* Size of received message */
    int receivedInt;
    player p, enemy;
    Room *room;
    time_t startTime, currentTime;

    /* Receive message from client */
    if ((recvMsgSize = recv(clntSocket, &p, sizeof(player), 0)) < 0)
        DieWithError("recv() failed");

    /* Send received string and receive again until end of transmission */
    while (recvMsgSize > 0)      /* zero indicates end of transmission */
    {
        switch(p.status) {
            case STATUS_REQ_ROOM_CREATE:    // ルームの作成処理
                logger("room created");
                room = createRoom(roomList);
                p.roomID = room->roomID;
                p.status = STATUS_RES_ROOM_CREATED;
                break;
            
            case STATUS_REQ_ROOM_JOIN:  // ルームへの参加処理
                room = getRoom(roomList, p.roomID);

                if (room == NULL) {   // ルームが存在しない場合
                    logger("room not found");
                    p.status = STATUS_RES_ROOM_NOT_FOUND;
                } 
                else if (room->isRoomFull) {  // ルームが満員の場合
                    logger("room is full");
                    p.status = STATUS_RES_ROOM_IS_FULL;
                } 
                else {    // ルームに参加
                    logger("room joined");
                    room->isRoomFull = 1;
                    p.status = STATUS_RES_ROOM_JOINNED;
                }
                break;

            case STATUS_REQ_GAME_QUIT: // ゲームを終了する場合
                logger("game quit");
                if (p.roomID != -1) {
                    room = getRoom(roomList, p.roomID);
                    logger("get room");
                    deleteRoom(roomList, room->roomID);
                    logger("delete room");
                }
                p.status = STATUS_RES_GAME_QUIT;
                break;
            
            case STATUS_REQ_SEND_HAND:  // コマンドの処理
                logger("received hand: %d", p.cmd);
                room = getRoom(roomList, p.roomID);

                if (room==NULL)
                {
                    p.status = STATUS_RES_ROOM_NOT_FOUND;
                    break;
                }
                
                
                if (room->isRoomFull == 0) {     // ルームが満員でない場合
                    logger("room is not full");
                    p.status = STATUS_RES_ROOM_IS_NOT_FULL;
                    break;
                }

                if (room->waitingFlag == 0) {   // 相手のコマンド待ちの場合
                    logger("waiting for opponent's hand");
                    room->waitingFlag = 1;
                    room->player1 = &p;

                    // 相手のコマンドが確定するまでor30秒経過まで待機
                    startTime = time(NULL);
                    while (room->waitingFlag) {
                        currentTime = time(NULL);
                        if (difftime(currentTime, startTime) >= 5) { // 5秒経過をチェック
                            logger("time out");
                            room->waitingFlag = 0;
                            p.status = STATUS_RES_GAME_TIMEOUT;
                            break; // 5秒経過したらループを抜ける
                        }
                    }

                    if (p.status == STATUS_RES_GAME_TIMEOUT)
                        break;
                    while (room->waitingFlag);
                    logger("break waiting loop");
                    enemy.cmd = room->player2->cmd;
                } 
                else {  // 相手のコマンド待ちでない場合
                    logger("skip waiting");
                    room->player2 = &p;
                    room->waitingFlag = 0;
                    enemy.cmd = room->player1->cmd;
                }

                Judge(&p, &enemy);  // 勝敗判定
                p.enemyCmd = enemy.cmd;

                if (p.status == STATUS_RES_GAME_WIN) // 勝利した場合はルームを削除
                    deleteRoom(roomList, room->roomID);
                break;  
            
            default:
                break;
        }

        logger(getStatusName(p.status));

        /* Echo message back to client */
        if (send(clntSocket, &p, recvMsgSize, 0) != recvMsgSize)
            DieWithError("send() failed");

        /* See if there is more data to receive */
        if ((recvMsgSize = recv(clntSocket, &p, sizeof(player), 0)) < 0)
            DieWithError("recv() failed");
    }

    close(clntSocket);    /* Close client socket */
}
