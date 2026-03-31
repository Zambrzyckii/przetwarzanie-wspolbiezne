#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>
#include <sys/wait.h>

int main() {
    int p1[2], p2[2];
    char* win[] = {"Remis", "Pierwszy", "Drugi"};
    pipe(p1);
    pipe(p2);
    void* h1 = dlopen("./libproc1.so", RTLD_LAZY);
    void* h2 = dlopen("./libproc2.so", RTLD_LAZY);

    void (*proc1)(int) = dlsym(h1, "run_proc1");
    void (*proc2)(int) = dlsym(h2, "run_proc2");
    if (fork() == 0) {
        close(p1[0]);
        close(p2[1]);
        close(p2[0]);
        proc1(p1[1]);
        exit(0);
    }

    if (fork() == 0) {
        close(p2[0]);
        close(p1[1]);
        close(p1[0]);
        proc2(p2[1]);
        exit(0);
    }
    close(p1[1]);
    close(p2[1]);
    int val1, val2;
    read(p1[0], &val1, sizeof(val1));
    read(p2[0], &val2, sizeof(val2));
    int diff = (val1 - val2 + 3) % 3;
    printf("Wynik: %s\n", win[diff]);
    wait(NULL);
    wait(NULL);
    dlclose(h1);
    dlclose(h2);
    return 0;
}