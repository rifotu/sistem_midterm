void write_to_fifo(char * fifo_name, struct data client_data){
	
    int s2c, i;
    char msg[80];
    char* buf;
    
    s2c = open(fifo_name, O_WRONLY);
    
    if (signal(SIGINT, sig_handler) == SIG_ERR);
    
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
