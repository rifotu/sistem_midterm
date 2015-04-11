#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[])
{
    int s2c, counter;
    struct stat st;
    char buf[10];
    char* fifo_name1;
    
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
        
	s2c = open(fifo_name1, O_RDONLY);

    // receive messages
    while (1)
    {
        if (read(s2c, &buf, sizeof(char) * 10) > 0)
        {
			counter = 0;
            printf("%s \n", buf);
        }
        sleep(1);
        counter++;
        if (counter>3) 
			break;
    }

	// delete fifos
    unlink(fifo_name1);

    printf("server exit successfully\n");
    return EXIT_SUCCESS;
}  
