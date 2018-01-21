server: server.o  
	gcc -o server server.o 

server.o: server.c header.h prototype.h declaration.h
	gcc -c server.c

clean:
	rm *.o
