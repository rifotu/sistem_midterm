#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>  
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

struct data {
	char* sender;
	char* receiver;
};

void write_to_fifo(char *, struct data);

int main(int argc, char* argv[])
{   
    char* fifo_name1;
    struct data client_data;

	if(argc != 2){
		printf("You should give 2 parameters!\n");
		printf("Example usage: ./server [FIFO_NAME]\n");
	} else {
		fifo_name1 = (char*)malloc(sizeof(char) * strlen(argv[1]) + 1);
		strcpy(fifo_name1, argv[1]);
	}
	printf("FIFO_NAME: %s\n", fifo_name1);
	
	write_to_fifo(fifo_name1, client_data);

    printf("client exit successfully\n");
    return EXIT_SUCCESS;
}

void write_to_fifo(char * fifo_name, struct data client_data){
	
    int s2c, i;
    char msg[80];
    char* buf;
    
    s2c = open(fifo_name, O_WRONLY);
    
    buf = (char*)malloc(sizeof(char) * 10 * sizeof(struct data));
	// start sending messages, with 3s interval
    for (i=0; i<5; i++)
    {
		client_data.sender = (char*)malloc(sizeof(char) * 10);
		sprintf(client_data.sender, "Client %d", i);
		client_data.receiver = (char*)malloc(sizeof(char) * 10);
		strcpy(client_data.receiver, "Server");
        printf("Message #%d to %s\n", i, client_data.receiver);

        strcpy(msg, "Message #"); 
        sprintf(buf, "%d", i);
        strcat(msg, buf);
        strcat(msg, " from ");
        strcat(msg, client_data.sender);
        strcat(msg, " to ");
        strcat(msg, client_data.receiver);
        strcat(msg, "\0");

        write(s2c, msg, strlen(msg)+1);

        sleep(2);
    }
}
