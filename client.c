#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>  
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

char* fifo_name1;

struct client_msg_s{
	
	char* client_name;
	int wait_time;
	char* op_name;
};

typedef struct main_fifo_msg_s{
   
     int   START_ID;
     char source[15];
     char destination[15];
     struct client_msg_s message[80];
     // durum bilgisi exception falan
     int   STOP_ID;
} main_fifo_msg_t;

void sig_handler(int signo);
void write_to_fifo(char *, struct main_fifo_msg_s, int);

int main(int argc, char* argv[])
{   
	int wait_time;
	char* op_name;
    char* client_name;
    struct main_fifo_msg_s main_fifo_msg;
    struct client_msg_s client_msg;
    int counter = 1;
    
    if (signal(SIGINT, sig_handler) == SIG_ERR);

	if(argc == 1){
		printf("You should give more parameters!\n");
		printf("Example usage: ./clientX [FIFO_NAME] [WAITING_TIME] [OPERATION NAME]\n");
	} else {
		
		client_msg.client_name = (char*)malloc(sizeof(char) * strlen(argv[0]) + 1);
		strcpy(client_msg.client_name, argv[0]);
		
		fifo_name1 = (char*)malloc(sizeof(char) * strlen(argv[1]) + 1);
		strcpy(fifo_name1, argv[1]);
		
		client_msg.wait_time = atoi(argv[2]);
		
		client_msg.op_name = (char*)malloc(sizeof(char) * strlen(argv[3]) + 1);
		strcpy(client_msg.op_name, argv[3]);
	}
	printf("FIFO_NAME: %s\n", fifo_name1);
	
	main_fifo_msg.message[0] = client_msg;
	
	while(1){
		write_to_fifo(fifo_name1, main_fifo_msg, counter);
		counter++;
	}
	
    printf("client exit successfully\n");
    return EXIT_SUCCESS;
}

void write_to_fifo(char * fifo_name, struct main_fifo_msg_s main_fifo_msg,
					int counter){
	
    int s2c;
    char msg[80];
    char* buf;
    
    s2c = open(fifo_name, O_WRONLY);
    
    buf = (char*)malloc(sizeof(char) * 10 * sizeof(struct main_fifo_msg_s));
	// start sending messages, with 3s interval
	sprintf(main_fifo_msg.source, "Client %d", counter);
	
	strcpy(main_fifo_msg.destination, "Server");
	printf("Message #%d to %s\n", counter, main_fifo_msg.destination);

	strcpy(msg, "Message #"); 
	sprintf(buf, "%d", counter);
	strcat(msg, buf);
	strcat(msg, " from ");
	strcat(msg, main_fifo_msg.source);
	strcat(msg, " to ");
	strcat(msg, main_fifo_msg.destination);
	strcat(msg, " OPNAME: ");
	strcat(msg, main_fifo_msg.message->op_name);
	strcat(msg, " CLIENT_NAME: ");
	strcat(msg, main_fifo_msg.message->client_name);
	strcat(msg, "\0");

	write(s2c, msg, strlen(msg)+1);

	sleep(main_fifo_msg.message->wait_time);
}

void sig_handler(int signo)
{
	if (signo == SIGINT)
		printf("received Ctrl + C! Program Exiting.\n");
	exit(0);
}
