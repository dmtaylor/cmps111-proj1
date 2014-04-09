/* shell.c : Main file for shell
 * Project 1
 * By: David Taylor
 * Builds dsh
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>

extern char **get_line(void);

int main(int argc, char *argv[]) {
  int i, rarg_pos;
  char redir_out, redir_in, bkg;
  char** args;
  rarg_pos = 0;
  redir_out = 0;
  redir_in = 0;
  bkg = 0; 

  while(1) {
    printf{"\ndsh$");
    args = get_line();
    if(!args) continue;
    if(strcmp(args[0],"exit") == 0) break;
    for(i = 0; args[i] != NULL; i++) {
      /*printf("Argument %d: %s\n", i, args[i]);*/
    }
  }
  return 0;
}
