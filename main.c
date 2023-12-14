#include <stdio.h>
#include <stdlib.h>

enum {
    HAND_CHARGE,
    HAND_BLOCK,
    HAND_ATACK,
    HAND_KATACK,
};

typedef struct {
    int cost;
    int cmd;
} player;

void input(player *p) {
    int value;
    char buf[100];

    fgets(buf, sizeof(buf), stdin);
    if (sscanf(buf, "%d", &value) != 1) {
        // int型でない入力の場合
        printf("整数で入力してください(%d): ", p->cost);
        input(p);

    } else if (value < HAND_CHARGE || value > HAND_KATACK ) {
        // 想定されていない入力があった場合
        input(p);

    } else if(value == HAND_CHARGE) {
        // コストをチャージする
        p->cost += 1;

    } else if(value == HAND_ATACK) {
        // コストが足りているか
        if (p->cost > 0) {
            p->cost -= 1;
        } else {
            printf("コストが足りません: ");
            input(p);
        }

    } else if(value == HAND_KATACK) {
        // コストが足りているか
        if (p->cost > 2) {
            p->cost -= 3;
        } else {
            printf("コストが足りません: ");
            input(p);
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

int main() {
    player *p[2];
    int result;

    // メモリの確保
    for(int i = 0; i < 2; i++) {
        p[i] = (player *)malloc(sizeof(player));
        if(p[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            return 1;
        }
    }

    // コマンドのヘルプを表示
    com_help();

    while (1) {
        // Player1の入力
        printf("プレイヤー1が入力してください(%d): ", p[0]->cost);
        input(p[0]);

        // Player2の入力
        printf("プレイヤー2が入力してください(%d): ", p[1]->cost);
        input(p[1]);

        // 勝敗のパターンの判別
        switch(p[0]->cmd | (p[1]->cmd << 2)){
            case(0x2):
            case(0x3):
            case(0x7):
            case(0xb):
                result = 1;
                break;
            case(0x8):
            case(0xc):
            case(0xd):
            case(0xe):
                result = 2;
                break;
            default: 
                result = 0;
            break;
        }

        // 勝敗がついた場合
        if (result != 0) {
            break;
        }
        
    }

    if(result == 1) {
        printf("プレイヤー1の勝利\n");
    } else {
        printf("プレイヤー2の勝利\n");
    }

    // メモリの開放
    for(int i = 0; i < 2; i++) {
        free(p[i]);
    }
    
    
    return 0;
}
