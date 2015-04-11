#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>  
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

void write_to_fifo(char *);

int main(int argc, char* argv[])
{   
    char* fifo_name1;

	if(argc != 2){
		printf("You should give 2 parameters!\n");
		printf("Example usage: ./server [FIFO_NAME]\n");
	} else {
		fifo_name1 = (char*)malloc(sizeof(char) * strlen(argv[1]) + 1);
		strcpy(fifo_name1, argv[1]);
	}
	printf("FIFO_NAME: %s\n", fifo_name1);
	
	write_to_fifo(fifo_name1);

    printf("client exit successfully\n");
    return EXIT_SUCCESS;
}

void write_to_fifo(char * fifo_name){
	
    int s2c, i;
    char msg[80], buf[10];
    
    s2c = open(fifo_name, O_WRONLY);
	// start sending messages, with 3s interval
    for (i=0; i<5; i++)
    {
        printf("Message #%d\n", i);

        strcpy(msg, "Message #"); 
        sprintf(buf, "%d", i);
        strcat(msg, buf);
        strcat(msg, "\0");

        write(s2c, msg, strlen(msg)+1);

        sleep(2);
    }
}
