CC = gcc
CFLAGS = -c -std=c99 -g
LDFLAGS = -g
SRC = ${wildcard src/*c}
HDR = ${wildcard src/include/*.h}
OBJ = ${SRC:.c=.o}
EXEC = urr

all: ${SRC} ${OBJ} ${EXEC}

debug: all
debug: CFLAGS += -DDEBUG

${EXEC}: ${OBJ}
  ${CC} ${LDFLAGS} $^ -o $@
  
$.o: %.c ${HDR}
  ${CC} ${CFLAGS} $< -o $@
  
clean:
  rm src/*.o ${EXEC}
  
main:
  ./fux compile src/test/main.fux
