/* shell.c : Main file for shell
 * Project 1
 * By: David Taylor
 * Builds dsh
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

extern char **get_line(void);

int main(int argc, char *argv[]) {
  int i, rarg_pos;
  char redir_out, redir_in, bkg, parse_err, in_call;
  char** args;

  while(1) {
    redir_out = 0;
    redir_in = 0;
    bkg = 0;
    rarg_pos = 0;
    parse_err = 0;
    in_call = 1;
    printf("\ndsh$");
    args = get_line();
    if(!args) continue;
    if(strcmp(args[0],"exit") == 0) break;
    for(i = 0; args[i] != NULL; i++) {
      if(args[i]=='<'){
        if(args[i+1] == NULL){
          fprintf(stderr, "\nError: no file given to <");
          parse_err = 1;
          break;
        }
        redir_in = 1;
        /* file redir in here */
        
      }
      if(args[i]=='>'){
        if(args[i+1] == NULL){
          fprintf(stderr, "\nError: no file given to >");
          parse_err = 1;
          break;
        }
        redir_out = 1;
        /* file redir out here */
        
      }
      if(args[i] == '&'){
        bkg = 1;
        in_call = 0;
      }
      if(in_call) rarg_pos++;
    }
    if(parse_err) continue;

    pid_t proc;

    proc = fork();
    if(proc = -1){
      fprintf(stderr, "\nFork failed, got %s\n",strerror(errno));
      exit(2);
    }
    else if(proc = 0){
      /* run child process here */
    }
    else {
      /* do parent here */
    }
  }
  return 0;
}
