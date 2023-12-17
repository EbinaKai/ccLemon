#include "ccLemon.h"

HandMapping handMappings[] = {
    {'c', HAND_CHARGE},
    {'b', HAND_BLOCK},
    {'a', HAND_ATTACK},
    {'k', HAND_KATTACK},
    {'q', HAND_QUIT},
    {'u', HAND_UNKNOWN}  // 'u' は未知の手を表す
};

void PlayerInit(player *p) {
    p->cost = 0;
    p->cmd = HAND_CHARGE;
    p->status = STATUS_INIT;
    p->roomID = -1;
}

char* getHandName(GameHand hand) {
    switch (hand) {
        case HAND_QUIT:
            return "QUIT";
        case HAND_CHARGE:
            return "CHARGE";
        case HAND_BLOCK:
            return "BLOCK";
        case HAND_ATTACK:
            return "ATTACK";
        case HAND_KATTACK:
            return "KATACK";
        default:    // ここは使われない
            return "Unknown Hand";
    }
}

// コマンド文字からGameHandへの変換
GameHand getHandCode(char command) {
    for (int i = 0; i < sizeof(handMappings) / sizeof(handMappings[0]); i++) {
        if (handMappings[i].command == command) {
            return handMappings[i].hand;
        }
    }
    return HAND_UNKNOWN;
}

// GameHandからコマンド文字への変換
char getHandChar(GameHand hand) {
    for (int i = 0; i < sizeof(handMappings) / sizeof(handMappings[0]); i++) {
        if (handMappings[i].hand == hand) {
            return handMappings[i].command;
        }
    }
    return 'u';  // 未知の手の場合
}

// コマンドを入力
void InputHand(player *p) {
    int hand;
    char code;
    char buf[100];

    fgets(buf, sizeof(buf), stdin);
    if (sscanf(buf, "%c", &code) != 1) {
        // char型でない入力の場合
        printf("コードで入力してください[%c/%c/%c/%c/%c](%d): ", 
                                    getHandChar(HAND_CHARGE), 
                                    getHandChar(HAND_BLOCK), 
                                    getHandChar(HAND_ATTACK), 
                                    getHandChar(HAND_KATTACK), 
                                    getHandChar(HAND_QUIT),
                                    p->cost);
        InputHand(p);
        
    } else {
        hand = getHandCode(code);
        if (hand < HAND_CHARGE || hand > HAND_QUIT ) {
            // 想定されていない入力があった場合
            printf("無効な入力です[%c/%c/%c/%c/%c](%d): ", 
                                    getHandChar(HAND_CHARGE), 
                                    getHandChar(HAND_BLOCK), 
                                    getHandChar(HAND_ATTACK), 
                                    getHandChar(HAND_KATTACK), 
                                    getHandChar(HAND_QUIT),
                                    p->cost);
            InputHand(p);

        } else {
            switch (hand) {
                case HAND_CHARGE:
                    // コストをチャージする
                    p->cost += 1;
                    p->cmd = hand;
                    break;

                case HAND_BLOCK:
                case HAND_QUIT:
                    p->cmd = hand;
                    break;
                    
                case HAND_ATTACK:
                    // コストが足りているか
                    if (p->cost > 0) {
                        p->cost -= 1;
                        p->cmd = hand;
                    } else {
                        printf("コストが足りません: ");
                        InputHand(p);
                    }
                    break;

                case HAND_KATTACK:
                    // コストが足りているか
                    if (p->cost > 2) {
                        p->cost -= 3;
                        p->cmd = hand;
                    } else {
                        printf("コストが足りません: ");
                        InputHand(p);
                    }
                    break;

                default:
                    break;                
            }
        }
    }
    
    // コマンドを決定
    if (p->cmd != HAND_QUIT) {
        p->status = STATUS_REQ_SEND_HAND;
    } else {
        p->status = STATUS_REQ_GAME_QUIT;
    }
}

void CommandHelp() {
    printf("HAND   | COMMAND | COST\n");
    printf("-------|---------|------\n");
    printf("%6s | %7c |  +1\n", getHandName(HAND_CHARGE), getHandChar(HAND_CHARGE));
    printf("%6s | %7c |   0\n", getHandName(HAND_BLOCK), getHandChar(HAND_BLOCK));
    printf("%6s | %7c |  -1\n", getHandName(HAND_ATTACK), getHandChar(HAND_ATTACK));
    printf("%6s | %7c |  -3\n", getHandName(HAND_KATTACK), getHandChar(HAND_KATTACK));
    printf("%6s | %7c |  --\n", getHandName(HAND_QUIT), getHandChar(HAND_QUIT));
}

void InputMode(player *p) {
    char buf[100];
    fgets(buf, sizeof(buf), stdin);
    if (buf[0] == 'y') {
        p->status = STATUS_REQ_ROOM_CREATE;
    } else if (buf[0] == 'n') {
        p->status = STATUS_REQ_ROOM_JOIN;
    } else if (buf[0] == 'q') {
        p->status = STATUS_REQ_GAME_QUIT;
    } else {
        printf("無効な入力です(y/n/q): ");
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
