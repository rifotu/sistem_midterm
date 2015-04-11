#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>  
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

struct data {
	char* sender;
	char* receiver;
};

typedef struct main_fifo_msg_s{
   
     int   START_ID;
     char* source;
     char* destination;
     int    message[80];
     int   STOP_ID;
     
} main_fifo_msg_t;

void sig_handler(int signo);
void write_to_fifo(char *, main_fifo_msg_t, int, char*);

int main(int argc, char* argv[])
{   
	int wait_time;
	char* op_name;
    char* fifo_name1;
    //struct data client_data;
    main_fifo_msg_t client_data;

	if(argc == 1){
		printf("You should give more parameters!\n");
		printf("Example usage: ./server [FIFO_NAME] [WAITING_TIME] [OPERATION NAME]\n");
	} else {
		fifo_name1 = (char*)malloc(sizeof(char) * strlen(argv[1]) + 1);
		strcpy(fifo_name1, argv[1]);
		
		op_name = (char*)malloc(sizeof(char) * strlen(argv[3]) + 1);
		strcpy(op_name, argv[3]);
		
		wait_time = atoi(argv[2]);
	}
	printf("FIFO_NAME: %s\n", fifo_name1);
	
	write_to_fifo(fifo_name1, client_data, wait_time, op_name);

    printf("client exit successfully\n");
    return EXIT_SUCCESS;
}

void write_to_fifo(char * fifo_name, main_fifo_msg_t client_data,
					int wait_time, char* op_name){
	
    int s2c, i;
    char msg[80];
    char* buf;
    
    s2c = open(fifo_name, O_WRONLY);
    
    if (signal(SIGINT, sig_handler) == SIG_ERR);
    
    buf = (char*)malloc(sizeof(char) * 10 * sizeof(main_fifo_msg_t));
	// start sending messages, with 3s interval
    for (i=0; i<5; i++)
    {
		client_data.source = (char*)malloc(sizeof(char) * 10);
		sprintf(client_data.source, "Client %d", i);
		
		client_data.destination = (char*)malloc(sizeof(char) * 10);
		strcpy(client_data.destination, "Server");
        printf("Message #%d to %s\n", i, client_data.destination);

        strcpy(msg, "Message #"); 
        sprintf(buf, "%d", i);
        strcat(msg, buf);
        strcat(msg, " from ");
        strcat(msg, client_data.source);
        strcat(msg, " to ");
        strcat(msg, client_data.destination);
        strcat(msg, " OPNAME: ");
        strcat(msg, op_name);
        strcat(msg, "\0");

        write(s2c, msg, strlen(msg)+1);

        sleep(wait_time);
    }
}

void sig_handler(int signo)
{
	if (signo == SIGINT)
		printf("received Ctrl + C! Program Exiting.\n");
	exit(0);
}
