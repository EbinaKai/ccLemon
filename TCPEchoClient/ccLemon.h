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

void InputHand(player *p);
void com_help();