#include <stdio.h>
#include <stdlib.h>

typedef enum {
    HAND_CHARGE,
    HAND_BLOCK,
    HAND_ATACK,
    HAND_KATACK,
} GameHand;

typedef enum {
    STATUS_UNDECIDED,
    STATUS_WIN,
    STATUS_LOSE,
} GameStatus;

typedef struct {
    int cost;           // コスト
    GameHand cmd;       // コマンド
    GameStatus status;  // 通信の状態
    int room;           // ゲームの部屋番号
} player;

void InputHand(player *p);
void com_help();