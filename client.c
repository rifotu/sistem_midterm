#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>  
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdint.h>

// definitions
#define  OPERATION_NOTIFIER  1
#define  CTRL_C_RECEIVED 2

#define  START_ID  0xBE
#define  STOP_ID   0xEF

// structure definitins
typedef struct client_msg_s{
	
	int wait_time;
	char op_name[15];
}client_msg_t;

typedef struct main_fifo_msg_s{
   
     int  start_id;
     char source[15];  // TODO: replace 15 with a #define
     char destination[15];
     int  dac;  // tells the program about how to interpret the payload
     char message[80];  // use uint8_t instead of this, as we using binary messages 
     // durum bilgisi exception falan
     int   stop_id;
} main_fifo_msg_t;

// Global Variables
char* fifo_name1;
int fifo_id;

// Function prototypes
void sig_handler(int signo);
void write_to_fifo(int s2c, struct main_fifo_msg_s *main_fifo_msg);


int main(int argc, char* argv[])
{   
    struct main_fifo_msg_s main_fifo_msg_tx;  // use this for sending
    struct main_fifo_msg_s main_fifo_msg_rx;  // use this for receiving
    struct client_msg_s client_msg;
    void *ptr = NULL;
    int counter = 1;
    
    if (signal(SIGINT, sig_handler) == SIG_ERR){
        fputs("An error occurred while setting the signal handler.\n", stderr);
        exit(0);
    }

	if(argc == 1){
		printf("You should give more parameters!\n");
		printf("Example usage: ./clientX [FIFO_NAME] [WAITING_TIME] [OPERATION NAME]\n");
	} else {
		
		fifo_name1 = (char*)malloc(sizeof(char) * strlen(argv[1]) + 1);
		strcpy(fifo_name1, argv[1]);

        fifo_id = open(fifo_name1, O_WRONLY);

		client_msg.wait_time = atoi(argv[2]);
        strncpy(client_msg.op_name, argv[3], 15);  // get operation name
        

        main_fifo_msg_tx.start_id = START_ID;
		strncpy(main_fifo_msg_tx.source, argv[0], 15);  // get clientX name
        strncpy(main_fifo_msg_tx.destination, "ss", 15);
        main_fifo_msg_tx.dac = OPERATION_NOTIFIER;
        memcpy(main_fifo_msg_tx.message, (uint8_t *)&(client_msg), sizeof(struct client_msg_s));
        main_fifo_msg_tx.stop_id = STOP_ID;
		
        
		//client_msg.op_name = (char*)malloc(sizeof(char) * strlen(argv[3]) + 1);
		//strcpy(client_msg.op_name, argv[3]);
	}
	printf("FIFO_NAME: %s\n", fifo_name1);
    printf("source name: %s\n", main_fifo_msg_tx.source);
    printf("destination name: %s\n", main_fifo_msg_tx.destination);
    printf("dac: %d\n", main_fifo_msg_tx.dac);
    ptr = (void *)&(main_fifo_msg_tx.message[0]);
    printf("wait time %d\n", ((struct client_msg_s *)ptr)->wait_time);
    printf("operation name: %s\n", ((struct client_msg_s *)ptr)->op_name);

	
//	main_fifo_msg_tx.message[0] = client_msg;
	
	while(1){
        sleep(5);
        
		write_to_fifo(fifo_id, &(main_fifo_msg_tx));
		counter++;
	}
	
    close(fifo_id);
    printf("client exit successfully\n");
    return EXIT_SUCCESS;
}

void write_to_fifo(int s2c, struct main_fifo_msg_s *main_fifo_msg)
{
	int len = 0;
    //int s2c;
    
    //s2c = open(fifo_name, O_WRONLY);
    
    len = write(s2c, main_fifo_msg, sizeof(struct main_fifo_msg_s));
    printf("wr len %d\n", len);
    usleep(10);

	
}

void sig_handler(int signo)
{
	if (signo == SIGINT)
		printf("received Ctrl + C! Program Exiting.\n");
	exit(0);
}
