/* shell.c : Main file for shell
 * Project 1
 * By: David Taylor
 * Builds dsh
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#define BUFFSIZE 512

extern char **get_line(void);

/*void sig_handler(int signo){
  wait(&status);
}*/

int main(int argc, char *argv[]) {
  int i;
  int status = 0;
  char redir_out, redir_in, bkg, parse_err, in_call;
  char** args;
  char* comm_args[BUFFSIZE];
  char* redir_ins;
  char* redir_outs;
  pid_t proc, pid_err_check;
  /*FILE* redir_outf, redir_inf;*/

  /* Might not be necessary
    if(signal(SIGCHLD, handler)) {
    printf("Could not set signal handler\n");
    exit(-1);
  }*/
  fprintf(stderr, "got past setup\n");
  while(1) {
    /* Check to see if any background processes completed */
    while(!waitpid(-1, &status, WNOHANG) /*!= 0*/){
      sleep(1);
      fprintf(stderr, "waiting for background, stat is %d\n", status);
      if(pid_err_check == -1){
        fprintf(stderr, "\nWaiting for child process failed.");
        exit(1);
      }
    }
    /* Initialize values and print prompt */
    redir_out = 0;
    redir_in = 0;
    /*redir_outf = NULL;
    redir_inf = NULL;*/
    redir_ins = "";
    redir_outs = "";
    bkg = 0;
    parse_err = 0;
    in_call = 1;
    memset(comm_args, 0, BUFFSIZE);
    printf("\ndsh$");

    /* Gets the line from stdin, and parses input */
    args = get_line();
    if(!args) continue;

    if(strcmp(args[0], "$?") == 0){
      printf("\n%d", status);
      continue;
    }

    if(strcmp(args[0], "exit") == 0) break;
    for(i = 0; args[i] != NULL; i++) {
      if(strcmp(args[i],"<") == 0) {
        if(args[i+1] == NULL){
          fprintf(stderr, "\nError: no file given to <");
          parse_err = 1;
          break;
        }
        redir_in = 1;
        in_call = 0;
        /* file redir in here */
        redir_ins = args[i+1];
        
      }

      if(strcmp(args[i],">") == 0) {
        if(args[i+1] == NULL){
          fprintf(stderr, "\nError: no file given to >");
          parse_err = 1;
          break;
        }
        redir_out = 1;
        in_call = 0;
        /* file redir out here */
        redir_outs = args[i+1];
        
      }
      if(strcmp(args[i],"&") == 0){
        bkg = 1;
        in_call = 0;
      }
      if(in_call && i > 0){
        comm_args[i-1] = args[i];
      }
    }
    if(parse_err) continue;

    /* Forks the process. The child does the command, the parent
       either waits, or continues going with an interupt signal */
    proc = fork();

    if(proc == -1){
      fprintf(stderr, "\nFork failed, got %s\n",strerror(errno));
      exit(2);
      /*continue;*/
    }
    else if(proc == 0){
      /* run child process here */
      
      /* redirection here */
      if(redir_in){
        if(!redir_ins){
          fprintf(stderr, "\nFile name is NULL. You should not see this");
          continue;
        }
        freopen(redir_ins, "r", stdin);
      }
      if(redir_out){
        if(!redir_ins){
          fprintf(stderr, "\nFile name is NULL. You should not see this");
          continue;
        }
        freopen(redir_outs, "w", stdout);
      }
      

      execvp(args[0], comm_args);
      fprintf(stderr, "\nexecvp returned to shell. You should not see this.\n");
      exit(1);
    }
    else {
      /* do parent here */
      if(!bkg){
        pid_err_check = wait(&status);
        if(pid_err_check == -1){
          fprintf(stderr, "\nWaiting for child process failed.\n");
          exit(1);
        }
      }
      
    }
  }
  return 0;
}
