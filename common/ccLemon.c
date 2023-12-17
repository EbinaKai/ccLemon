#include "ccLemon.h"

void PlayerInit(player *p) {
    p->cost = 0;
    p->cmd = HAND_CHARGE;
    p->status = STATUS_REQ_ROOM_CREATE;
    p->roomID = -1;
}

const char* getHandName(GameHand hand) {
    switch (hand) {
        case HAND_CHARGE:
            return "CHARGE";
        case HAND_BLOCK:
            return "BLOCK";
        case HAND_ATTACK:
            return "ATTACK";
        case HAND_KATTACK:
            return "KATACK";
        default:
            return "Unknown Hand";
    }
}

void InputHand(player *p) {
    int value;
    char buf[100];

    fgets(buf, sizeof(buf), stdin);
    if (sscanf(buf, "%d", &value) != 1) {
        // int型でない入力の場合
        printf("整数で入力してください(%d): ", p->cost);
        InputHand(p);

    } else if (value < HAND_CHARGE || value > HAND_KATTACK ) {
        // 想定されていない入力があった場合
        printf("無効な入力です: ");
        InputHand(p);

    } else if(value == HAND_CHARGE) {
        // コストをチャージする
        p->cost += 1;
        p->cmd = value;

    } else if(value == HAND_BLOCK) {
        p->cmd = value;

    } else if(value == HAND_ATTACK) {
        // コストが足りているか
        if (p->cost > 0) {
            p->cost -= 1;
            p->cmd = value;
        } else {
            printf("コストが足りません: ");
            InputHand(p);
        }

    } else if(value == HAND_KATTACK) {
        // コストが足りているか
        if (p->cost > 2) {
            p->cost -= 3;
            p->cmd = value;
        } else {
            printf("コストが足りません: ");
            InputHand(p);
        }
    }

    // コマンドを決定
    p->status = STATUS_REQ_SEND_HAND;
}

void CommandHelp() {
    printf("HAND   | COMMAND | COST\n");
    printf("-------|---------|------\n");
    printf("%6s | %7d |  +1\n", getHandName(HAND_CHARGE), HAND_CHARGE);
    printf("%6s | %7d |   0\n", getHandName(HAND_BLOCK), HAND_BLOCK);
    printf("%6s | %7d |  -1\n", getHandName(HAND_ATTACK), HAND_ATTACK);
    printf("%6s | %7d |  -3\n", getHandName(HAND_KATTACK), HAND_KATTACK);
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
    if (sscanf(buf, "%d", &p->roomID) != 1) {
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
            break;
        case(0x8):
        case(0xc):
        case(0xd):
        case(0xe):
            p1->status = STATUS_RES_GAME_LOSE;
            break;
        default: 
            p1->status = STATUS_RES_GAME_UNDECIDED;
            break;
    }
}
