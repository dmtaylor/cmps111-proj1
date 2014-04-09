%option noyywrap

%{
#define YY_PROTO(proto) proto
#define YY_DECL char **yylex YY_PROTO(( void ))
int _numargs = 10;
char *_args[10];
int _argcount = 0;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char *strdup(char* src)
{
    char *dst = malloc(strlen(src)+1);
    if(dst) strcpy(dst, src);
    return dst;
}
%}

WORD	[a-zA-Z0-9\/\.-]+
SPECIAL	[()><|&;*]

%%
	_argcount = 0; 
	_args[0] = NULL; 

{WORD}|{SPECIAL} {  
	  if(_argcount < _numargs-1) {
	    _args[_argcount++] = (char *)strdup(yytext);
	    _args[_argcount] = NULL;
	  }
	}

\n	return _args;

[ \t]+

.

%%

char **get_line() {
  return (char **)yylex();
}
