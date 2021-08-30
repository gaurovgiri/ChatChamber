#ifndef COMMAND
#define COMMAND

typedef struct DATA
{
    char name[31];
    int sockfd;
    char message[101];
} info;

void command(info *data) //commands are written here
{

    char *user_name, *whisp;
    char kick[36], admin[36], whisper[201];

    strcpy(kick, data->message);
    strcpy(admin, data->message);
    memset(whisper,0,sizeof(whisper));
    strcpy(whisper, data->message);

    if (strcmp(data->message, "/help") == 0)
    {
        printf("Command List:\n");
        printf("/whoami -> Displays your name\n");
        printf("/clear -> To clear screen\n");
        printf("/dev -> To display creators' names\n");
        printf("/role -> To display user's role\n");
        printf("/online -> To display online users\n");
        printf("/admin -> To make admin. Syntax: /admin <username>\n");
        printf("/kick -> To kick user from the server. Syntax: /kick <username>\n");
        printf("/whisper -> To send private message to a user. Syntax: /whisper <username> <message>\n");



    }

    else if (strcmp(data->message, "/whoami") == 0)
    {
        printf("You are \"%s\".\n", data->name);
    }
    else if (strcmp(data->message, "/rickroll") == 0)
    {
        send(data->sockfd, data->message, sizeof(data->message), 0);
    }
    else if (strcmp(data->message, "/clear") == 0)
    {
        system("clear"); // for windows -> cls , cls;
    }

    else if (strcmp(data->message, "/dev") == 0)
    {
        printf("\r Developers' Name:\n\tGaurav Giri\n\tAnup Bhattarai\n\tKeshab Kamal\n\tAakash Budhathoki\n");
    }

    else if (strcmp(data->message, "/online") == 0)
    {
        send(data->sockfd, data->message, sizeof(data->message), 0);
    }
    else if (strcmp(data->message, "/role") == 0)
    {
        send(data->sockfd, data->message, sizeof(data->message), 0);
    }
    else if (strcmp(user_name = strtok(admin, " "), "/admin") == 0)
    {
        user_name = strtok(NULL, " ");
        if (user_name == NULL)
        {
            printf("\rPlease specify the user name. Syntax : /admin <username>\n");
        }
        else
        {
            send(data->sockfd, data->message, sizeof(data->message), 0);
        }
    }
    else if (strcmp(user_name = strtok(kick, " "), "/kick") == 0)
    {
        user_name = strtok(NULL, " ");
        if (user_name == NULL)
        {
            printf("\rPlease specify the user name. Syntax : /kick <username>\n");
        }
        else
        {
            send(data->sockfd, data->message, sizeof(data->message), 0);
        }
    }
    else if (strcmp(user_name = strtok(whisper, " "), "/whisper") == 0) // fix the multi words whisper
    {
        user_name = strtok(NULL, " ");
        whisp = strtok(NULL, "\n");
        if (user_name == NULL || whisp == NULL)
        {
            printf("\rPlease specify the user name. Syntax : /whisper <username> <msg>\n");
        }
        else
        {

            send(data->sockfd, data->message, sizeof(data->message), 0);
        }
    }

    else
    {
        printf("\rUnknown Command. Use '/help' to show commands.\n");
    }
}

#endif