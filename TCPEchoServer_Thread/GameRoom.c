#include "GameRoom.h"

void logger(const char *format, ...);

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
      logger("found room with id: %d", id);
      return current;
    }
    current = current->next;
  }
  return NULL;
}

// ルームを削除する関数
void deleteRoom(Room* roomList, int roomID) {
  Room* current = roomList;
  Room* previous = NULL;
    // 対象のルームを検索
  while (current != NULL && current->roomID != roomID) {
    previous = current;
    current = current->next;
  }
    // ルームが見つからない場合
  if (current == NULL) {
    logger("Room with id: %d not found.", roomID);
    return;
  }

  previous->next = current->next;

  // ルームのリソースを解放
  logger("Room with id: %d deleted.", roomID);
  free(current);

}

void drawRoom(Room* roomList) {
  Room* current = roomList;
  while (current != NULL) {
    logger("roomID: %d", current->roomID);
    current = current->next;
  }
}
