#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

typedef struct main_fifo_msg_s{
   
     int   START_ID;
     char source[15];
     char destination[15];
     int    message[80];
     int   STOP_ID;
     
} main_fifo_msg_t;

void sig_handler(int signo);

void read_from_fifo(char *, main_fifo_msg_t);

int main(int argc, char* argv[])
{
    struct stat st;
    char* fifo_name1;
    main_fifo_msg_t server_data;
    
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
        
    read_from_fifo(fifo_name1, server_data);

	// delete fifos
    unlink(fifo_name1);

    printf("server exit successfully\n");
    return EXIT_SUCCESS;
}  

void read_from_fifo(char * fifo_name, main_fifo_msg_t server_data){
	int s2c, counter;
    char buf[80];
	
	s2c = open(fifo_name, O_RDONLY);
	
	if (signal(SIGINT, sig_handler) == SIG_ERR);
    // receive messages
    while (1)
    {
        if (read(s2c, &buf, sizeof(char) * 10 * sizeof(main_fifo_msg_t)) > 0)
        {
			counter = 0;
            printf("%s", buf);
            printf("\n");
        }
        sleep(1);
        counter++;
        if (counter>3) 
			break;
    }

}

void sig_handler(int signo)
{
	if (signo == SIGINT)
		printf("received Ctrl + C! Program Exiting.\n");
	exit(0);
}
