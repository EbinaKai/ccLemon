#include "ccLemon.h"

void PlayerInit(player *p) {
    p->cost = 0;
    p->cmd = HAND_CHARGE;
    p->status = STATUS_REQ_ROOM_CREATE;
    p->room = -1;
}

void InputHand(player *p) {
    int value;
    char buf[100];

    fgets(buf, sizeof(buf), stdin);
    if (sscanf(buf, "%d", &value) != 1) {
        // int型でない入力の場合
        printf("整数で入力してください(%d): ", p->cost);
        InputHand(p);

    } else if (value < HAND_CHARGE || value > HAND_KATACK ) {
        // 想定されていない入力があった場合
        printf("無効な入力です: ");
        InputHand(p);

    } else if(value == HAND_CHARGE) {
        // コストをチャージする
        p->cost += 1;

    } else if(value == HAND_ATACK) {
        // コストが足りているか
        if (p->cost > 0) {
            p->cost -= 1;
        } else {
            printf("コストが足りません: ");
            InputHand(p);
        }

    } else if(value == HAND_KATACK) {
        // コストが足りているか
        if (p->cost > 2) {
            p->cost -= 3;
        } else {
            printf("コストが足りません: ");
            InputHand(p);
        }
    }

    // コマンドを決定
    p->cmd = value;
    p->status = STATUS_REQ_SEND_HAND;
}

void com_help() {
    printf("HAND   | No | COST\n");
    printf("-------|----|------\n");
    printf("CHARGE | %-2d |  +1\n", HAND_CHARGE);
    printf("BLOCK  | %-2d |   0\n", HAND_BLOCK);
    printf("ATACK  | %-2d |  -1\n", HAND_ATACK);
    printf("KATACK | %-2d |  -3\n", HAND_KATACK);
}

void InputMode(player *p) {
    char buf[100];
    fgets(buf, sizeof(buf), stdin);
    if (buf[0] == 'y') {
        p->status = STATUS_REQ_ROOM_CREATE;
    } else if (buf[0] == 'n') {
        p->status = STATUS_REQ_ROOM_JOIN;
    } else {
        printf("無効な入力です(y/n): ");
        InputMode(p);
    }
}

void InputRoomId(player *p) {
    char buf[100];
    fgets(buf, sizeof(buf), stdin);
    if (sscanf(buf, "%d", &p->room) != 1) {
        // int型でない入力の場合
        printf("整数で入力してください: ");
        InputRoomId(p);
    }
}

void Judge(player *p1, player *p2) {
    // 勝敗のパターンの判別
    switch(p1->cmd | (p2->cmd << 2)){
        case(0x2):
        case(0x3):
        case(0x7):
        case(0xb):
            p1->status = STATUS_RES_GAME_WIN;
            p2->status = STATUS_RES_GAME_LOSE;
            break;
        case(0x8):
        case(0xc):
        case(0xd):
        case(0xe):
            p1->status = STATUS_RES_GAME_LOSE;
            p2->status = STATUS_RES_GAME_WIN;
            break;
        default: 
            p1->status = STATUS_RES_GAME_UNDECIDED;
            p2->status = STATUS_RES_GAME_UNDECIDED;
            break;
    }
}
