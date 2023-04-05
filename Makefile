CC=gcc
SRC=life.c
OUT=life
CFLAGS = -Wall -O2 -fopenmp
demo:
	${CC} ${CFLAGS} ${SRC} -o ${OUT}