/* test1.cpp
 * Brandon Ngo
 * 
 *
 */

#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <iostream>

using namespace std;

timespec diff(timespec start, timespec end);

int main(int argc, char *argv[])
{
    int pipefd[2];
    pid_t cpid;
    char buf;
    //long forkTime;
    timespec time1, time2;

    if (argc != 2) {
    fprintf(stderr, "Usage: %s <string>\n", argv[0]);
    exit(EXIT_FAILURE);
    }
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    cpid = fork();
    if (cpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    
    clock_gettime(CLOCK_PROCESS_CPUTTIME_ID, &time1);
    
    if (cpid != 0) {    /* Parent reads from pipe */
        close(pipefd[1]);          /* Close unused write end */
        while (read(pipefd[0], &buf, 1) > 0)
            write(STDOUT_FILENO, &buf, 1);
        write(STDOUT_FILENO, "\n", 1);
        close(pipefd[0]);
        _exit(EXIT_SUCCESS);
    } else {            /* Child writes argv[1] to pipe */
        close(pipefd[0]);          /* Close unused read end */
        write(pipefd[1], argv[1], strlen(argv[1]));
        close(pipefd[1]);          /* Reader will see EOF */
        wait(NULL);                /* Wait for child */
        exit(EXIT_SUCCESS);
        
        
    }
    clock_gettime(CLOCK_PROCESS_CPUTTIME_ID, &time2);
    //cout << diff(time1,time2).tv_sec << ":" << diff(time1,time2).tv_nsec  << endl;
    printf("Estimated Fork Time = %llu nanoseconds\n", (long long unsigned int) diff);
}

timespec diff(timespec start, timespec end)
{
    timespec temp;
    if ((end.tv_nsec-start.tv_nsec) < 0){
        temp.tv_sec = end.tv_sec-start.tv_sec-1;
	temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
    } else{
        temp.tv_sec = end.tv_sec - start.tv_sec;
	temp.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    return temp;

}
