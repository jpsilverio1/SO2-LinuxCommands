hello: main.c grepCommand.c catCommand.c cdCommand.c echoCommand.c manCommand.c wccCommand.c pwdCommand.c

	gcc -c main.c grepCommand.c catCommand.c cdCommand.c echoCommand.c manCommand.c wccCommand.c pwdCommand.c
	gcc -o comandos main.o grepCommand.o catCommand.o cdCommand.o echoCommand.o manCommand.o wccCommand.o pwdCommand.o
	ar crv library.a main.o grepCommand.o catCommand.o cdCommand.o echoCommand.o manCommand.o wccCommand.o pwdCommand.o
	ranlib library.a
