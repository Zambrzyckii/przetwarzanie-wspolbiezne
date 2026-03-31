all: main libproc1.so libproc2.so
main: main.c
	gcc -Wall main.c -o main -ldl
libproc1.so: libproc1.c game.h
	gcc -Wall -fPIC -shared libproc1.c -o libproc1.so
libproc2.so: libproc2.c game.h
	gcc -Wall -fPIC -shared libproc2.c -o libproc2.so
clean:
	rm -f main
	rm -f libproc1.so
	rm -f libproc2.so
