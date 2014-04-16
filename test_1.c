/* Test program 1 : test background processes */
/* By David Taylor */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char** argv){
    sleep(20);
    printf("I waited 20 secs. exiting 0\n");
    return 0;
}

