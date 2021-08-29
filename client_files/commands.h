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

    char *user_name,*whisp;
    char kick[36];
    char admin[36];
    char whisper[201];

    strcpy(kick, data->message);
    strcpy(admin, data->message);
    strcpy(whisper,data->message);

    if (strcmp(data->message, "/help") == 0)
    {
        printf("Command List:\n");
        printf("/whoami -> Displays your name");
        printf("/clear -> To clear screen");
        printf("/dev -> To display creators' names");
    }

    else if (strcmp(data->message, "/whoami") == 0)
    {
        printf("You are \"%s\".\n", data->name);
    }
    else if (strcmp(data->message, "/rickroll") == 0)
    {
        sprintf(data->message, "\r%s RickRolled You: Never Gonna Give you Up.", data->name);
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
        whisp = strtok(NULL,"\0");
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