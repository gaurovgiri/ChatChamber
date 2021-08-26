#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
//#include <sys/types.h>
/*#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>*/

#include <pthread.h>
#include "client_files/str.h"

#include <winsock2.h>
#include <windows.h>
//#pragma comment(lib,"ws2_32.lib")




// Global variables
volatile sig_atomic_t flag = 0;
SOCKET sockfd = 0;
char name[31] = {};

void catch_ctrl_c_and_exit(int sig) {
    flag = 1;
}

void recv_msg_handler() {
    char receiveMessage[201] = {};
    while (1) {
        int receive = recv(sockfd, receiveMessage, 201, 0);
        if (receive > 0) {
            printf("\r%s\n", receiveMessage);
            str_overwrite_stdout();
        } else if (receive == 0) {
            break;
        } else { 
            // -1 
        }
    }
}

void send_msg_handler() {
    char message[101] = {};
    while (1) {
        str_overwrite_stdout();
        while (fgets(message, 101, stdin) != NULL) {
            str_trim_lf(message, 101);
            if (strlen(message) == 0) {
                str_overwrite_stdout();
            } else {
                break;
            }
        }
        send(sockfd, message, 101, 0);
        if (strcmp(message, "exit") == 0) {
            break;
        }
    }
    catch_ctrl_c_and_exit(2);
}

int main()
{
    signal(SIGINT, catch_ctrl_c_and_exit);
    WSADATA Data;
    WSAStartup(MAKEWORD(2,2),&Data);

    // Naming
    printf("Please enter your name: ");
    if (fgets(name, 31, stdin) != NULL) {
        str_trim_lf(name, 31);
    }
    if (strlen(name) < 2 || strlen(name) >= 31-1) {
        printf("\nName must be more than one and less than thirty characters.\n");
        exit(EXIT_FAILURE);
    }

    // Create socket
    sockfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sockfd == -1) {
        printf("Fail to create a socket.");
        exit(EXIT_FAILURE);
    }

    // Socket information
    struct sockaddr_in server_info, client_info;
    int s_addrlen = sizeof(server_info);
    int c_addrlen = sizeof(client_info);
    memset(&server_info, 0, s_addrlen);
    memset(&client_info, 0, c_addrlen);
    server_info.sin_family = AF_INET;
    server_info.sin_addr.S_un.S_addr=inet_addr("45.79.126.30");
  //  server_info.sin_addr.s_addr = inet_addr("45.79.126.30");
    server_info.sin_port = htons(8888);

    // Connect to Server
    int err = connect(sockfd, (struct sockaddr *)&server_info, s_addrlen);
    if (err == -1) {
        printf("Connection to Server error!\n");
        exit(EXIT_FAILURE);
    }
    
    // Names
    getsockname(sockfd, (struct sockaddr*) &client_info,&c_addrlen);
    getpeername(sockfd, (struct sockaddr*) &server_info,&s_addrlen);
  
    printf("Connect to Server: %s:%d\n", inet_ntoa(server_info.sin_addr), ntohs(server_info.sin_port));
    printf("You are: %s:%d\n", inet_ntoa(client_info.sin_addr), ntohs(client_info.sin_port));

    send(sockfd, name, 31, 0);

    pthread_t send_msg_thread;
    if (pthread_create(&send_msg_thread, NULL, (void *) send_msg_handler, NULL) != 0) {
        printf ("Create pthread error!\n");
        exit(EXIT_FAILURE);
    }

    pthread_t recv_msg_thread;
    if (pthread_create(&recv_msg_thread, NULL, (void *) recv_msg_handler, NULL) != 0) {
        printf ("Create pthread error!\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        if(flag) {
            printf("\nBye\n");
            break;
        }
    }

    close(sockfd);
    return 0;
}