#ifndef CCLEMON_H
#define CCLEMON_H
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    HAND_CHARGE,
    HAND_BLOCK,
    HAND_ATTACK,
    HAND_KATTACK,
} GameHand;

typedef enum {
    // リクエストステータス
    STATUS_REQ_ROOM_CREATE,
    STATUS_REQ_ROOM_JOIN,
    STATUS_REQ_SEND_HAND,

    // レスポンスステータス
    STATUS_RES_ROOM_CREATED,
    STATUS_RES_ROOM_JOINNED,
    STATUS_RES_ROOM_NOT_FOUND,
    STATUS_RES_ROOM_IS_FULL,
    STATUS_RES_GAME_UNDECIDED,
    STATUS_RES_GAME_WIN,
    STATUS_RES_GAME_LOSE,
} GameStatus;

typedef struct {
    int cost;           // コスト
    GameHand cmd;       // コマンド
    GameHand enemyCmd;  // 相手のコマンド
    GameStatus status;  // 通信の状態
    int roomID;         // ゲームの部屋番号
    char message[100];  // メッセージ
} player;

void PlayerInit(player *p);
const char* getHandName(GameHand hand);
void InputMode(player *p);
void InputRoomId(player *p);
void InputHand(player *p);
void CommandHelp();
void Judge(player *p1, player *p2);

#endif // CCLEMON_H