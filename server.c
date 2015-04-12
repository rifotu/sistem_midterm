#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

char* fifo_name1;
char* server_log_file = "server.log";

typedef struct main_fifo_msg_s{
   
     int   START_ID;
     char source[15];
     char destination[15];
     int    message[80];
     int   STOP_ID;
     
} main_fifo_msg_t;

void sig_handler(int signo);
void read_from_fifo(char *, main_fifo_msg_t, int);

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
    
    // if no fifos, create them
    if (stat(fifo_name1, &st) != 0)
        mkfifo(fifo_name1, 0666);
        
    //while'ı çek buraya
    while(1){
		read_from_fifo(fifo_name1, server_data, counter);
	}
	
	// delete fifos
    unlink(fifo_name1);

    printf("server exit successfully\n");
    return EXIT_SUCCESS;
}  

void read_from_fifo(char * fifo_name, main_fifo_msg_t server_data,
					int counter){
	int s2c;
    char buf[80];
	
	s2c = open(fifo_name, O_RDONLY);
	
    // receive messages
	if (read(s2c, &buf, sizeof(char) * 10 * sizeof(main_fifo_msg_t)) > 0)
	{
		counter = 0;
		printf("%s", buf);
		printf("\n");
	}
	sleep(1);
	counter++;
	if (counter>3) 
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
