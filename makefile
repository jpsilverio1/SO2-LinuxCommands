hello: main.c grepCommand.c catCommand.c cdCommand.c echoCommand.c manCommand.c wccCommand.c pwdCommand.c

	gcc -c comandos main.c grepCommand.c catCommand.c cdCommand.c echoCommand.c manCommand.c wccCommand.c pwdCommand.c
	ar crv library.a main.o grepCommand.o catCommand.o cdCommand.o echoCommand.o manCommand.o wccCommand.o pwdCommand.o
	ranlib library.a