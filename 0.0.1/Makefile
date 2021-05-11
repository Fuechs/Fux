exec = fux.out
sources = ${wildcard src/*.c}
objects = ${sources: .c=.o}
flags = -g

${exec}: ${objects}
	gcc ${objects} ${flags} -o ${exec}


%.o: %.c include/%.h
	gcc -c ${flags} $< -o $@

install:
	make
	cp ./fux.out /usr/local/bin/fuxlang

clean:	
	-rm *.out
	-rm *.o
	-rm src/*.o
