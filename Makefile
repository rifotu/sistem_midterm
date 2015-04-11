all:
	gcc -c server.c
	gcc server.o -o server
	gcc -c client.c
	gcc client.o -o client
	
clean:
	rm server.o
	rm server
	rm client.o
	rm client
	rm mainFIFO
