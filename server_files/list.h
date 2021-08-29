#ifndef LIST
#define LIST

typedef struct Nodes
{
    int socket;
    struct Nodes *next;
    struct Nodes *prev;
    char ip[16];
    char name[31];
    char role[10];
    int leave_flag; //admin->1,member->1
} ClientList;

ClientList *addNode(int sockfd, char *ip)
{
    ClientList *client = (ClientList *)malloc(sizeof(ClientList));
    
    client->socket = sockfd;
    client->next = NULL;
    client->prev = NULL;
    client->leave_flag = 0;
    strncpy(client->role,"member",10);
    strncpy(client->ip,ip,16);
    strncpy(client->name,"NULL",5);
    return client;
}
#endif
ClientList *head, *curr;