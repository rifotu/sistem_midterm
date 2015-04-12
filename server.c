#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

char* fifo_name1;
int fifo_id;
char* server_log_file = "server.log";

typedef struct main_fifo_msg_s{
   
     int  START_ID;
     char source[15];
     char destination[15];
     int  dac;
     int  message[80];
     int  STOP_ID;
     
} main_fifo_msg_t;

void sig_handler(int signo);
void read_from_fifo(int s2c, main_fifo_msg_t *server_data);

int main(int argc, char* argv[])
{
    struct stat st;
    main_fifo_msg_t server_data;
    int counter;
    
    if (signal(SIGINT, sig_handler) == SIG_ERR);
    
    if(argc != 2){
		printf("You should give 2 parameters!\n");
		printf("Example usage: ./server [FIFO_NAME]\n");
		exit(0);
	} else {
		fifo_name1 = (char*)malloc(sizeof(char) * strlen(argv[1]) + 1);
		strcpy(fifo_name1, argv[1]);
	}
	printf("FIFO_NAME: %s\n", fifo_name1);

	fifo_id = open(fifo_name1, O_RDONLY);
    
    // if no fifos, create them
    if (stat(fifo_name1, &st) != 0)
        mkfifo(fifo_name1, 0666);
        
    //while'ı çek buraya
    while(1){
        sleep(5);
		read_from_fifo(fifo_id, &server_data);
	}
	
	// delete fifos
    unlink(fifo_name1);

    printf("server exit successfully\n");
    return EXIT_SUCCESS;
}  

void read_from_fifo(int s2c, main_fifo_msg_t *server_data)
{
    int len = 0;
    // receive messages
	if (len = read(s2c, server_data, sizeof(main_fifo_msg_t)) > 0)
	{
		printf("read data: %d\n", len);
        printf("source name: %s\n", server_data->source);
	}
	sleep(1);
	return;
}

void sig_handler(int signo)
{
	FILE* fp;
	printf("fifo name: %s\n", fifo_name1);
	if (signo == SIGINT){
		printf("received Ctrl + C! Program Exiting.\n");
		
		fp = fopen(server_log_file, "w");
		fprintf(fp,"<CTRL+C> signal has arrived, all processes are terminated!\n");
	}
	fclose(fp);
	unlink(fifo_name1);
	exit(0);
}
