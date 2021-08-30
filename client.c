#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // <winsock2.h> <windows.h>
#include <pthread.h>
#include "client_files/str.h"
#include "client_files/commands.h"
#include "client_files/color.h"

// Global variables
volatile sig_atomic_t flag = 0;
int sockfd = 0;
char name[31] = {};
char password[20] = {};
char loginOrRegister[101] = {};
info *data;

//server_ip->45.79.126.30

void catch_ctrl_c_and_exit(int sig)
{
    flag = 1;
}

void recv_msg_handler()
{
    char receiveMessage[201] = {};
    while (1)
    {
        int receive = recv(sockfd, receiveMessage, 201, 0);
        if (receive > 0)
        {
            if (strcmp(receiveMessage, "[Either Username or Password didn't match!]") == 0 || strcmp(receiveMessage, "[Username already exists!]") == 0 || strncmp(receiveMessage, "[You have been kicked by ", 25) == 0 || strcmp(receiveMessage, "Connection Closed!") == 0 || strcmp(receiveMessage, "[\033[0;31m Server is Closed!\033[0m]") == 0)
            {

                printf("\r%s\n", receiveMessage);
                close(sockfd);
                exit(EXIT_FAILURE);
                break;
            }
            else if (strncmp(receiveMessage, " !!!", 4) == 0)
            {
                system("xdg-open https://www.youtube.com/watch?v=dQw4w9WgXcQ"); //win->start
                printf("\r%s\n", receiveMessage);
                str_overwrite_stdout();
            }
            else
            {
                printf("\r%s\n", receiveMessage);
                str_overwrite_stdout();
            }
        }
        else if (receive == 0)
        {
            break;
        }
        else
        {
            // -1
        }
    }
}

void send_msg_handler()
{
    char message[101] = {};
    while (1)
    {
        str_overwrite_stdout();
        while (fgets(message, 101, stdin) != NULL)
        {
            str_trim_lf(message, 101);
            if (strlen(message) == 0)
            {
                str_overwrite_stdout();
            }
            else
            {
                break;
            }
        }

        if (strcmp(message, "/exit") == 0)
        {
            break;
        }
        else if (message[0] == '/') // commands
        {
            strcpy(data->message, message);
            command(data);
        }
        else
        {
            send(sockfd, message, 101, 0);
        }
    }
    catch_ctrl_c_and_exit(2);
}

int main()
{
    signal(SIGINT, catch_ctrl_c_and_exit); //catch_ctrl_c_and_exit modifies flag value to 1 for exit
    char ip[16];
    int leave_flag = 0, success = 0, err;
    char opt[5]; //for login or registration or exit
    char *pass;

    //WSADATA Data
    //WSAStartup(MAKEWORD(2,2),&DATA);

    system("clear"); //for windwos -> cls
    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        red();
        printf("Fail to create a socket.");
        exit(EXIT_FAILURE);
    }

    // Socket information
    struct sockaddr_in server_info, client_info;
    int s_addrlen = sizeof(server_info);
    int c_addrlen = sizeof(client_info);
    memset(&server_info, 0, s_addrlen);
    memset(&client_info, 0, c_addrlen);
    server_info.sin_family = PF_INET;
    printf("Enter the server ip: ");
    scanf("%s", ip);
    fflush(stdin);
    server_info.sin_addr.s_addr = inet_addr(ip);
    server_info.sin_port = htons(8888);

    // Connect to Server
    err = connect(sockfd, (struct sockaddr *)&server_info, s_addrlen);
    if (err == -1)
    {
        red();
        printf("Connection to Server error!\n");
        exit(EXIT_FAILURE);
    }

    // Names
    getsockname(sockfd, (struct sockaddr *)&client_info, (socklen_t *)&c_addrlen);
    getpeername(sockfd, (struct sockaddr *)&server_info, (socklen_t *)&s_addrlen);
    green();
    printf("Connect to Server: %s:%d\n", inet_ntoa(server_info.sin_addr), ntohs(server_info.sin_port));
    printf("You are: %s:%d\n\n", inet_ntoa(client_info.sin_addr), ntohs(client_info.sin_port));

    //Login or Register:
    green();
    printf("\t\033[1m\033[4m\033[7mWelcome To ChatChamber:");

    while (!success)
    {
        printf("\t\033[0m\033[0;32m\n\t\t1)Login\n\t\t2)Register\n\t\t3)Exit\n");
        green();
        fflush(stdin);
        printf("You: ");
        reset();
        scanf("%s", opt);
        switch (atoi(opt)) //atoi->alphabet to integer
        {
        case 1: //login
            green();
            printf("\tLogin\nUsername: ");
            reset();
            if (scanf("%s", name))
            {
                fflush(stdin);
                str_trim_lf(name, 31);
                success = 1;
            }
            if (strlen(name) < 2 || strlen(name) >= 30)
            {
                red();
                system("clear"); //win -> cls
                printf("\nName must be more than one and less than thirty characters.\n");
                success = 0;
                break;
            }
            green();
            pass = getpass("Password: ");
            reset();
            if (pass != NULL)
            {
                strcpy(password, pass);
                fflush(stdin);
                str_trim_lf(password, 20);
                success = 1;
            }
            if (strlen(password) < 2 || strlen(password) >= 19)
            {
                red();
                system("clear"); //win -> cls
                printf("\nPassword must be more than one and less than twenty characters.\n");
                success = 0;
                break;
            }
            sprintf(loginOrRegister, "1 %s %s", name, password);
            break;
        case 2:
            green();
            printf("\tRegister\nUsername: ");
            reset();
            if (scanf("%s", name))
            {
                fflush(stdin);
                str_trim_lf(name, 31);
                success = 1;
            }
            if (strlen(name) < 2 || strlen(name) >= 30)
            {
                red();
                system("clear"); //win -> cls
                printf("\nName must be more than one and less than thirty characters.\n");
                success = 0;
                break;
            }
            green();
            printf("\nPassword: ");
            reset();
            if (scanf("%s", password))
            {
                fflush(stdin);
                str_trim_lf(password, 20);
                success = 1;
            }
            if (strlen(password) < 2 || strlen(password) >= 19)
            {
                red();
                system("clear"); //win -> cls
                printf("\nPassword must be more than one and less than twenty characters.\n");
                success = 0;
                break;
            }
            sprintf(loginOrRegister, "2 %s %s", name, password);
            break;
        case 3:
            success = 1;
            strcpy(loginOrRegister, "3 exit exit");
            break;
        default:
            red();
            printf("\nOut of Option\n");
            success = 0;
            break;
        }
    }

    if (leave_flag == 0)
    {
        system("clear");                                           // win->cls()
        send(sockfd, loginOrRegister, sizeof(loginOrRegister), 0); //sent the info about login or registration

        //storing info for data
        data = (info *)malloc(sizeof(info));

        strcpy(data->name, name);
        data->sockfd = sockfd;

        pthread_t send_msg_thread;
        if (pthread_create(&send_msg_thread, NULL, (void *)send_msg_handler, NULL) != 0)
        {
            red();
            printf("Create pthread error!\n");
            exit(EXIT_FAILURE);
        }

        pthread_t recv_msg_thread;
        if (pthread_create(&recv_msg_thread, NULL, (void *)recv_msg_handler, NULL) != 0)
        {
            red();
            printf("Create pthread error!\n");
            exit(EXIT_FAILURE);
        }

        while (1)
        {
            if (flag)
            {
                yellow();
                printf("\nBye\n");
                break;
            }
        }
    }

    close(sockfd);
    exit(EXIT_SUCCESS);
    return 0;
}
