//to make the first admin in the server

#include <string.h>
#include <stdio.h>
#include <unistd.h>

struct user_data
{
    char username[31];
    char password[31];
    char role[10];
} user_info;

void main()
{
    FILE *fp = fopen("user_info.dat", "rb");
    char *pass;
    int success = 0;
    if (fp == NULL)
    {
        fp = fopen("user_info.dat", "wb");
        while (!success)
        {
            printf("The First Admin Name: ");
            scanf("%s", user_info.username);
            if (strlen(user_info.username) < 2 || strlen(user_info.username) > 30)
            {
                printf("Enter the user name that is in range of 3 to 30 characters!\n");
                continue;
            }
            pass = getpass("Password: ");
            fflush(stdin);
            strcpy(user_info.password, pass);
            if (strlen(user_info.password)< 4 || strlen(user_info.password) > 20)
            {
                printf("Enter a password that is in range of 4 to 20 characters");
                continue;
            }
            success = 1;
            strcpy(user_info.role, "admin");
            fwrite(&user_info,sizeof(user_info),1,fp);
        }
    }
    fclose(fp);
}