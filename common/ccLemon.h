#include <stdio.h>
#include <stdlib.h>

typedef enum {
    HAND_CHARGE,
    HAND_BLOCK,
    HAND_ATACK,
    HAND_KATACK,
} GameHand;

typedef enum {
    STATUS_REQ_ROOM_CREATE,
    STATUS_REQ_ROOM_JOIN,
    STATUS_REQ_SEND_HAND,
    STATUS_RES_GAME_UNDECIDED,
    STATUS_RES_GAME_WIN,
    STATUS_RES_GAME_LOSE,
} GameStatus;

typedef struct {
    int cost;           // コスト
    GameHand cmd;       // コマンド
    GameStatus status;  // 通信の状態
    int room;           // ゲームの部屋番号
    char message[100];  // メッセージ
} player;

void PlayerInit(player *p);
void InputMode(player *p);
void InputRoomId(player *p);
void InputHand(player *p);
void com_help();
void Judge(player *p1, player *p2);