#ifndef GAMEROOM_H
#define GAMEROOM_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>        /* for POSIX threads */
#include "../common/ccLemon.h"

// ルームの構造体
typedef struct Room Room;

struct Room {
    int roomID;
    int isRoomFull;
    int waitingFlag;
    player* player1;
    player* player2;
    Room* next;
};

Room* createRoomList();
int generateRoomID(Room *roomList);
Room *createRoom(Room *roomList);
Room *getRoom(Room *roomList, int id);
void deleteRoom(Room *roomList, int roomID);
void drawRoom(Room* roomList);

#endif // GAMEROOM_H
