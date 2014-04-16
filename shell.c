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

int main(int argc, char *argv[]) {
  /* Declarations */
  int i;
  int status, checkval, bkg;
  char redir_out, redir_in, parse_err, in_call, reprint;
  char** args;
  char* comm_args[BUFFSIZE];
  char* redir_ins;
  char* redir_outs;
  pid_t proc, pid_err_check;
  uid_t user_id;

  /* Prints banner and sets a few values that need to be initialized */
  printf("Welcome to d(ave's) sh(ell): a barebones shell\n");
  printf("Type exit to exit\n");
  status = 0;
  bkg = 0;
  while(1) {
    fflush(stdout);
    /* Check to see if any background processes completed         */
    /* Should be portable, checks for -1 and 0 in the event of no */
    /* children. This eschews the need for a signal handler       */
    checkval = waitpid(-1, &status, WNOHANG);
    while(checkval){
      if(errno == 10) break; /* errno 10 is no child processes */
      if(pid_err_check == -1){
        fprintf(stderr, "\nWaiting for child process failed.");
        exit(1);
      }
      checkval = waitpid(-1, &status, WNOHANG);
    }

    /* Initialize values and print prompt */
    redir_out = 0;
    redir_in = 0;
    redir_ins = "";
    redir_outs = "";
    bkg = 0;
    parse_err = 0;
    in_call = 1;
    user_id = getuid(); /* checks user id for prompt */
    memset(comm_args, 0, BUFFSIZE);
    printf("dsh");
    if(user_id) printf("$ ");
    else printf("# ");
    

    /* Gets the line from stdin, handles shell comands */
    args = get_line();
    if(args[0] == NULL) continue;

    /* Exits program */
    if(strcmp(args[0], "exit") == 0) break;

    /* Implementation of directory changing */
    if(strcmp(args[0], "cd") == 0){
      if(args[1] == NULL){
        chdir("~");
      }
      else{
        chdir(args[1]);
      }
      continue;
    }


    /* Prints exit status of last program to finish */
    if(strcmp(args[0], "$?") == 0){
      if(WIFEXITED(status)){
        printf("%d\n", WEXITSTATUS(status));
        fflush(NULL);
      }
      else if(WIFSIGNALED(status)){
        printf("exited with %s\n", strerror(WTERMSIG(status)));
        fflush(NULL);
      }
      else{
        printf("No prev status\n");
        fflush(NULL);
      }
      continue;
    }

    /* parses input line */
    for(i = 0; args[i] != NULL; i++) {

      /* handles file redirection */
      if(strcmp(args[i],"<") == 0) {
        if(args[i+1] == NULL){
          fprintf(stderr, "Error: no file given to <\n");
          parse_err = 1;
          break;
        }
        redir_in = 1;
        in_call = 0;
        redir_ins = args[i+1];
        
      }

      if(strcmp(args[i],">") == 0) {
        if(args[i+1] == NULL){
          fprintf(stderr, "Error: no file given to >\n");
          parse_err = 1;
          break;
        }
        redir_out = 1;
        in_call = 0;
        redir_outs = args[i+1];
        
      }

      /* Sets flags for background running */
      if(strcmp(args[i],"&") == 0){
        bkg = 1;
        in_call = 0;
      }
      
      /* gets arguments for program */
      if(in_call){
        comm_args[i] = args[i];
      }
    }
    if(parse_err){
    fprintf(stderr, "Has parse error\n");
    continue;
    }

    /* Forks the process. The child does the command, the parent
       either waits, or continues going */
    proc = fork();

    if(proc == -1){
      fprintf(stderr, "\nFork failed, got %s\n",strerror(errno));
      exit(2);
      /*continue;*/
    }

    /* run child process here */
    else if(proc == 0){
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

      /* Implementation of echo. NOTE: file redirection in does not */
      /* work with echo, only out.                                  */
      if(strcmp(args[0], "echo") == 0){
        for(i=1; comm_args[i] != NULL; i++){
          printf("%s ", comm_args[i]);
        }
        printf("\n");
        exit(0);
      }
      
      execvp(args[0], comm_args);
      fprintf(stderr, "\nexecvp returned to shell. It failed.\n");
      fprintf(stderr, "errno is %s\n", strerror(errno));
      exit(1);
    }

    /* do parent here */
    else {
      /* waits if background running is not ennabled */
      /* otherwise, loops back to beginnning         */
      if(!bkg){
        pid_err_check = wait(&status);
        fflush(stdout);
        if(pid_err_check == -1){
          fprintf(stderr, "\nWaiting for child process failed.\n");
          exit(1);
        }
      }
      
    }
  }
  return 0;
}
