# Makefile: A makefile for project 1
# by: David Taylor

MKFILE    = Makefile
CC        = cc

EXECFILE  = dsh
GEN_IN    = lex.l
GEN_OUT   = lex.yy.c
CSRC      = shell.c ${GEN_OUT}
CSUBMITS  = shell.c ${GEN_IN}     
OBJECTS	  = ${CSRC:.c=.o}
SOURCES   = ${CSUBMITS} ${MKFILE} README LICENSE design.pdf .gitignore

all : ${EXECFILE}

${EXECFILE} : ${OBJECTS}
	${CC} ${OBJECTS} -o ${EXECFILE}

${OBJECTS} : ${CSRC}
	${CC} -c ${CSRC}

${GEN_OUT} : ${GEN_IN}
	flex ${GEN_IN}

clean :
	- rm *.o

spotless : clean
	- rm ${EXECFILE}

bundle : ${SOURCES}
	tar -czvf dshell.tgz ${SOURCES}


