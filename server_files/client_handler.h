#ifndef Client
#define Client
#include "list.h"
#include <string.h>

struct user_data
{
    char username[31];
    char password[31];
    char role[10];
} user_info;

void sendAll(ClientList *, char *);

void catch_ctrl_c_and_exit(int sig)
{
    ClientList *tmp;
    char alert[] = "[\033[0;31m Server is Closed!\033[0m]";
    while (head != NULL)
    {
        printf("\nClose socketfd: %d\n", head->socket);
        sendAll(head, alert);
        close(head->socket); // close all socket include server_sockfd
        tmp = (ClientList *)head;
        head = head->next;

        free(tmp);
    }
    printf("Bye\n");
    exit(EXIT_SUCCESS);
}

void makeAdmin(ClientList *client, char username[])
{
    FILE *fp = fopen("server_files/user_info.dat", "r+b");
    ClientList *tmp = (ClientList *)head->next;
    int found = 0, curr_pos;
    char msg[101];
    if (strcasecmp(client->name, username) == 0)
    {

        strcpy(msg, "[\033[0;33m You can't admin yourself!\033[0m]");
        send(client->socket, msg, sizeof(msg), 0);
        return;
    }
    if (strcmp(client->role, "admin") == 0)
    {
        while (tmp != NULL)
        {
            if (strcasecmp(tmp->name, username) == 0)
            {
                found = 1;
                break;
            }
            else
            {
                tmp = tmp->next;
            }
        }
        if (found)
        {
            if (strcmp(tmp->role, "admin") == 0)
            {
                sprintf(msg, "[\033[0;33m %s is already an admin!\033[0m ]", tmp->name);
                send(client->socket, msg, sizeof(msg), 0);
            }
            else
            {
                strcpy(tmp->role, "admin");
                sprintf(msg, "[\033[0;32m You have been made admin by %s!\033[0m]", client->name);
                send(tmp->socket, msg, sizeof(msg), 0);
                sprintf(msg, "[\033[0;36m %s has been made admin by %s!\033[0m]", tmp->name, client->name);
                sendAll(tmp, msg);
                while (fread(&user_info, sizeof(user_info), 1, fp) == 1)
                {
                    if (strcmp(user_info.username, tmp->name) == 0)
                    {
                        curr_pos = ftell(fp);
                        fseek(fp, curr_pos - sizeof(user_info), SEEK_SET);
                        strcpy(user_info.username, tmp->name);
                        strcpy(user_info.password, user_info.password);
                        strcpy(user_info.role, "admin");
                        printf("%s %s %s", user_info.username, user_info.password, user_info.role);
                        fwrite(&user_info, sizeof(user_info), 1, fp);
                        break;
                    }
                }
                fclose(fp);
            }
        }
        else
        {
            sprintf(msg, "[\033[0;33m %s is not online!\033[0m]", username);
            send(client->socket, msg, sizeof(msg), 0);
        }
    }
    else
    {
        strcpy(msg, "[\033[0;33mYou donot have the permssion. Please ask admin for help!\033[0m]");
        send(client->socket, msg, sizeof(msg), 0);
    }
}

void sendAll(ClientList *client, char *msg)
{
    ClientList *tmp = (ClientList *)head->next;
    while (tmp != NULL)
    {
        if (client->socket != tmp->socket && tmp->leave_flag == 0)
        {
            printf("Send to socket %d: \"%s\" \n", tmp->socket, msg);
            send(tmp->socket, msg, 201, 0);
        }
        tmp = tmp->next;
    }
}

void online(ClientList *client)
{
    char msg[201];
    memset(msg, 0, sizeof(msg));
    int i, count = 1;
    ClientList *tmp = (ClientList *)head->next;
    msg[0] = '\r';
    while (tmp != NULL)
    {
        for (i = 0; i < strlen(tmp->name); i++)
        {
            msg[count++] = tmp->name[i];
        }
        msg[count++] = ' ';
        msg[count++] = '[';
        for (i = 0; i < strlen(tmp->role); i++)
        {
            msg[count++] = tmp->role[i];
        }
        msg[count++] = ']';
        msg[count++] = '\n';
        tmp = tmp->next;
    }
    send(client->socket, msg, sizeof(msg), 0);
}

void kick(ClientList *client, char username[])
{
    char msg[201];
    int found = 0;
    ClientList *tmp = (ClientList *)head->next;
    if (strcasecmp(client->name, username) == 0)
    {
        strcpy(msg, "[\033[0;33m You cannot kick yourself.\033[0m ]");
        send(client->socket, msg, sizeof(msg), 0);
        return;
    }
    else if (strcmp(client->role, "admin") == 0)
    {
        sprintf(msg, "[\033[0;31m You have been kicked by %s\033[0m]", client->name);
        while (tmp != NULL)
        {
            if (strcasecmp(tmp->name, username) == 0)
            {
                found = 1;
                send(tmp->socket, msg, sizeof(msg), 0);
                tmp->leave_flag = 1;
                break;
            }
            else
                tmp = tmp->next;
        }
        if (!found)
        {
            sprintf(msg, "[\033[0;33m%s is not online!\033[0m]", username);
            send(client->socket, msg, sizeof(msg), 0);
        }
        else
        {
            sprintf(msg, "[\033[0;36m%s have been kicked by %s\033[0m]", username, client->name);
            sendAll(tmp, msg);
        }
    }
    else
    {
        strcpy(msg, "[\033[0;33mYou donot have the permission to kick out. Please ask admin.\033[0m]");
        send(client->socket, msg, sizeof(msg), 0);
    }
}
void whispered(ClientList *client, char username[], char msg[])
{
    ClientList *tmp = (ClientList *)head->next;
    char whisper[201];
    int found = 0;
    if (strcasecmp(client->name, username) == 0)
    {
        strcpy(whisper, "You cannot whisper yourself!");
        send(client->socket, whisper, sizeof(whisper), 0);
        return;
    }
    while (tmp != NULL)
    {
        if (strcasecmp(tmp->name, username) == 0)
        {
            sprintf(whisper, "|%s whispered you: %s|", client->name, msg);
            send(tmp->socket, whisper, sizeof(whisper), 0);
            found = 1;
            break;
        }
    }
    if (found == 0)
    {
        sprintf(whisper, "[%s is not online]", username);
        send(client->socket, whisper, sizeof(whisper), 0);
    }
}

void *c_handler(void *client_t)
{
    ClientList *client = (ClientList *)client_t;
    char chat_name[31] = {};
    char password[20] = {};
    char recv_msg[101] = {};
    char send_msg[201] = {};
    int data, command_flag = 0;
    char *user, *pass, *user_name, *whisp;
    char kicks[36];
    char admin[36];
    char whisper[36];
    char loginOrRegister[101];
    char *logOrReg;
    int found = 0;
    FILE *fp;

    // char opt[4]; // check the option

    //login or register
    recv(client->socket, loginOrRegister, sizeof(loginOrRegister), 0);
    logOrReg = strtok(loginOrRegister, " ");
    user = strtok(NULL, " ");
    pass = strtok(NULL, " ");

    switch (atoi(logOrReg)) //atoi(opt)
    {
    case 1: //login
        fp = fopen("server_files/user_info.dat", "rb");

        while (fread(&user_info, sizeof(user_info), 1, fp) == 1)
        {
            
            if ((strcasecmp(user_info.username, user) == 0) && (strcmp(user_info.password, pass) == 0))
            {
                found = 1;
                break;
            }
        }
        if (found)
        {
            strcpy(send_msg, "\033[0;32m[Your login was Successful.\033[0m]");
            send(client->socket, send_msg, sizeof(send_msg), 0);
            strcpy(client->name, user);
            strcpy(client->role, user_info.role);
            printf("%s->(%s)(%d) joined the chatroom\n", client->name, client->ip, client->socket);
            sprintf(send_msg, "[\033[0;35m %s joined the chatroom\033[0m]", client->name);
            sendAll(client, send_msg);
        }
        else
        {
            strcpy(send_msg, "[\033[0;31mEither Username or Password didn't match!\033[0m]");
            send(client->socket, send_msg, sizeof(send_msg), 0);
            client->leave_flag = 1;
        }
        fclose(fp);
        break;

    case 2: //register
        fp = fopen("server_files/user_info.dat", "rb");
        while (fread(&user_info, sizeof(user_info), 1, fp) == 1)
        {
            if ((strcasecmp(user_info.username, user) == 0))
            {
                found = 1;
                break;
            }
        }
        if (found)
        {
            strcpy(send_msg, "[\033[0;31mUsername already exists!\033[0m]");
            send(client->socket, send_msg, sizeof(send_msg), 0);
            client->leave_flag = 1;
        }
        else
        {
            fclose(fp);
            fp = fopen("server_files/user_info.dat", "ab");
            strcpy(user_info.username, user);
            strcpy(user_info.password, pass);
            strcpy(user_info.role, "member");
            strcpy(client->name, user_info.username);
            strcpy(client->role, user_info.role);
            fwrite(&user_info, sizeof(user_info), 1, fp);
            printf("%s->(%s)(%d) joined the chatroom FRESH\n", client->name, client->ip, client->socket);
            sprintf(send_msg, "[\033[0;35m %s joined the chatroom \033[0;32m|FRESH|\033[0m]", client->name);
            sendAll(client, send_msg);
            strcpy(send_msg, "[\033[0;32mYou are registered successfully!\033[0m]");
            send(client->socket, send_msg, sizeof(send_msg), 0);
        }
        fclose(fp);
        break;
    case 3:
        client->leave_flag = 1;
        break;
    default:
        break;
    }

    //Chat
    while (client->leave_flag == 0)
    {
        if (client->leave_flag == 0)
            data = recv(client->socket, recv_msg, sizeof(recv_msg), 0);
        else
            break;
        if (client->leave_flag == 1) //double checking for kicked users
            break;
        if (data > 0)
        {
            strcpy(kicks, recv_msg);
            strcpy(admin, recv_msg);
            strcpy(whisper, recv_msg);

            if (strlen(recv_msg) == 0)
                continue;
            else if (strcmp(recv_msg, "/online") == 0)
            {
                online(client);
                command_flag = 1;
            }
            else if (strcmp(recv_msg, "/role") == 0)
            {
                sprintf(send_msg, "[\033[0;34mYou are \033[0;32m\"%s\"\033[0m]", client->role);
                send(client->socket, send_msg, sizeof(send_msg), 0);
                command_flag = 1;
            }
            else if (strcmp(user_name = strtok(kicks, " "), "/kick") == 0)
            {
                command_flag = 1;
                if (user_name != NULL)
                {
                    user_name = strtok(NULL, " ");
                    kick(client, user_name);
                }
            }
            else if (strcmp(user_name = strtok(admin, " "), "/admin") == 0)
            {
                command_flag = 1;
                if (user_name != NULL)
                {
                    user_name = strtok(NULL, " ");
                    makeAdmin(client, user_name);
                }
            }
            else if (strcmp(user_name = strtok(whisper, " "), "/whisper") == 0)
            {
                command_flag = 1;
                if (user_name != NULL)
                {
                    user_name = strtok(NULL, " ");
                    whisp = strtok(NULL, "\n");
                    whispered(client, user_name, whisp);
                }
            }
            else
                sprintf(send_msg, "\033[0m\033[41;1m%s:\033[0m\033[44;1m %s\033[0m", client->name, recv_msg);
        }
        else if (data == 0 || strcmp(recv_msg, "exit") == 0)
        {
            printf("%s->(%s)(%d) left the chatroom\n", client->name, client->ip, client->socket);
            sprintf(send_msg, "[\033[0;33m%s left the chatroom\033[0m]", client->name);
            sendAll(client, send_msg);
            client->leave_flag = 1;
        }
        else
        {
            printf("Something Occured Wrong!\n");
            client->leave_flag = 1;
        }
        if (client->leave_flag == 0)
            printf("%s(%s): %s\n", client->name, client->ip, recv_msg);

        if (command_flag == 0 && client->leave_flag != 1)
            sendAll(client, send_msg);
        else
            command_flag = 0;
    }

    // if leaves, truncate the client from the list

    strcpy(send_msg,"Connection Closed!");
    printf("\tConnection Closed with %s\n",client->ip);
    send(client->socket,send_msg,sizeof(send_msg),0);
    close(client->socket);
    if (client == curr)
    {
        client->prev->next = NULL;
        curr = client->prev;
    }
    else
    {
        client->prev->next = client->next;
        client->next->prev = client->prev;
    }
    free(client);
}

#endif