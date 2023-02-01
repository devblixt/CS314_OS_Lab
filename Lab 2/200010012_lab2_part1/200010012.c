#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
int main() {
    char str[] = "Hello World";
    int pid;
    int t;
    for (int i = 0; i < 11; i++) {
        sleep(rand() % 4 + 1);
        pid = fork();
        if (pid == 0) {
            printf("%c %d\n", str[i], (int) getpid());
            exit(0);
        } else {
            wait(NULL);
        }
    }
    return 0;
}