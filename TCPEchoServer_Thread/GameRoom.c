#include "GameRoom.h"

// ルームリストを作成
Room* createRoomList() {
  Room* roomList = (Room*)malloc(sizeof(Room));
  roomList->next = NULL;
  roomList->roomID = -1;
  return roomList;
}

// ルームIDを発行 
int generateRoomID(Room* roomList) {
  int roomID = 0;
  if (roomList == NULL) {
    return roomID;
  } else {
    Room* current = roomList;
    while (current->next != NULL) {
      current = current->next;
      roomID++;
    }
    return roomID;
  }
}

// 新しいルームを作成してリストに追加する関数
Room *createRoom(Room* roomList) {
  Room* newRoom = (Room*)malloc(sizeof(Room));
  newRoom->roomID = generateRoomID(roomList);
  newRoom->isRoomFull = 0;
  newRoom->next = NULL;

  if (roomList == NULL) {
    roomList = newRoom;
  } else {
    Room* current = roomList;
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = newRoom;
  }
  return newRoom;
}

Room *getRoom(Room* roomList, int id) {
  Room* current = roomList;
  while (current != NULL) {
    if (current->roomID == id) {
      printf("found room with id: %d\n", id);
      return current;
    }
    current = current->next;
  }
  return roomList;
}

// ルームを削除する関数
void deleteRoom(Room* roomList, Room* room) {
  Room* current = roomList;
  while (current->next != NULL) {
    if (current->next == room) {
      current->next = room->next;
      free(room);
      break;
    }
    current = current->next;
  }
}

void drawRoom(Room* roomList) {
  Room* current = roomList;
  while (current != NULL) {
    printf("roomID: %d\n", current->roomID);
    current = current->next;
  }
}
