#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argCount, char* args[]) {
if (argCount < 2) {
printf("Incorrect number of arguments!\n");
return 1;
}
int inputNum = atoi(args[argCount - 1]);
int outputNum = inputNum * 2;
printf("Twice: Current process id: %d, Current result: %d\n", (int) getpid(), outputNum);

char* cmdArgs[argCount];

for (int i = 0; i < argCount; i++) {
    cmdArgs[i] = args[i+1];
}
char outputNumStr[200];
sprintf(outputNumStr, "%d", outputNum);
cmdArgs[argCount-1] = NULL;
cmdArgs[argCount-2] = outputNumStr;

execvp(cmdArgs[0], cmdArgs);

return 0;
}