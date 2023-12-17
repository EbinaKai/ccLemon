#ifndef GAMEROOM_H
#define GAMEROOM_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>        /* for POSIX threads */

// ルームの構造体
typedef struct Room Room;

struct Room {
    int roomID;
    int isRoomFull;
    pthread_t player1;
    pthread_t player2;
    Room* next;
};

Room* createRoomList();
int generateRoomID(Room *roomList);
Room *createRoom(Room *roomList);
Room *getRoom(Room *roomList, int id);
void deleteRoom(Room *roomList, Room* room);
void drawRoom(Room* roomList);

#endif // GAMEROOM_H
