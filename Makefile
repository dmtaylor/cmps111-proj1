# Makefile: A makefile for project 1
# by: David Taylor
# Original file.

MKFILE    = Makefile
CC        = cc

EXECFILE  = dsh
GEN_IN    = lex.l
GEN_OUT   = lex.yy.c
CSRC      = shell.c ${GEN_OUT}
CSUBMITS  = shell.c ${GEN_IN}     
OBJECTS	  = ${CSRC:.c=.o}
TESTS     = test_1.c
SOURCES   = ${CSUBMITS} ${MKFILE} README LICENSE design.pdf .gitignore

all : ${EXECFILE}

run : ${EXECFILE}
	./${EXECFILE}

${EXECFILE} : ${OBJECTS}
	${CC} ${OBJECTS} -o ${EXECFILE}

${OBJECTS} : ${CSRC}
	${CC} -c ${CSRC}

${GEN_OUT} : ${GEN_IN}
	flex ${GEN_IN}

clean :
	- rm *.o

tests: ${TESTS}
	${CC} -c ${TESTS}
	${CC} test_1.o -o test_1

spotless : clean
	- rm ${EXECFILE}

bundle : ${SOURCES}
	tar -czvf dshell.tgz ${SOURCES}


