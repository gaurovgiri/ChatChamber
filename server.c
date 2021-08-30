#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>
#include "server_files/list.h"
#include "server_files/client_handler.h"
#include <signal.h>

void main()
{
    int server_sockfd, server_addrlen, client_addrlen, client_sockfd;
    struct sockaddr_in server_addr, client_addr;
    ClientList *client;
    char msg[101];
    FILE *fp;
    
    signal(SIGINT, catch_ctrl_c_and_exit);

    server_addrlen = sizeof(server_addrlen);
    client_addrlen = sizeof(client_addrlen);

    //creating a socket
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    //feeding address for socket
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8888);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    //binding and listening
    bind(server_sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_sockfd, 5);

    //printing the server address
    getsockname(server_sockfd, (struct sockaddr *)&server_addr, (socklen_t *)&server_addrlen);
    printf("Listening on %s:%d\n", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));

    //initializing the linked list
    head = addNode(server_sockfd, inet_ntoa(server_addr.sin_addr));
    curr = head;

    //creating user_info data file if doesn't exists
    fp = fopen("server_files/user_info.dat", "rb");
    if (fp == NULL)
    {
        fp = fopen("server_files/user_info.dat", "wb");
    }
    fclose(fp);

    //handling connections
    while (1)
    {
        client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_addr, (socklen_t *)&client_addrlen);

        //printing the client ip
        getpeername(client_sockfd, (struct sockaddr *)&client_addr, (socklen_t *)&client_addrlen);
        printf("Connecting with %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        //adding client to list
        client = addNode(client_sockfd, inet_ntoa(client_addr.sin_addr));
        client->prev = (ClientList *)curr;
        curr->next = client;
        curr = client;

        //assingning a thread to the client
        pthread_t t_id;

        if (pthread_create(&t_id, NULL, c_handler, client) != 0)
        {
            perror("Creating Thread Unsuccesful");
            exit(EXIT_FAILURE);
        }
    }
}
