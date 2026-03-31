#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "game.h"
void run_proc1(int write_fd) {
    srand(time(NULL) * getpid());
    int random = rand() % 3;
    write(write_fd, &random, sizeof(random));
    game player = {
        random,
        {"Kamień", "Papier", "Nożyce"}
    };
    printf("Gracz 1: %s\n", player.symbol[player.myNum]);
}