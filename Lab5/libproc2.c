#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"
void run_proc2(int read_fd) {
    srand(time(NULL) * getpid());
    int random = rand() % 3;
    write(read_fd, &random, sizeof(random));
    game player = {
        random,
        {"Kamień", "Papier", "Nożyce"}
    };
    printf("Gracz 2: %s\n", player.symbol[player.myNum]);

}