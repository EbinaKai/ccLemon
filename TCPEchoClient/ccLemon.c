#include "ccLemon.h"

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
}

void com_help() {
    printf("HAND   | No | COST\n");
    printf("-------|----|------\n");
    printf("CHARGE | %-2d |  +1\n", HAND_CHARGE);
    printf("BLOCK  | %-2d |   0\n", HAND_BLOCK);
    printf("ATACK  | %-2d |  -1\n", HAND_ATACK);
    printf("KATACK | %-2d |  -3\n", HAND_KATACK);
}
