#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include "header.h"

char *USERS = "./data/users.txt";

// Global variable to store the original terminal settings
struct termios orig_flags;

// Cleanup function to restore original terminal settings
void restoreTerminal()
{
    tcsetattr(fileno(stdin), TCSANOW, &orig_flags);
}

int loginMenu(struct User *u)
{
    struct termios nflags;

    // Store the original terminal settings
    tcgetattr(fileno(stdin), &orig_flags);

    system("clear");
    printf("\n\n\n\t\t\t  Bank Management System\n\t\t\t\t\t");

    int loginSuccessful = 0; // Flag to track successful login
    FILE *fp;
    struct User userChecker;

    while (!loginSuccessful)
    {
        printf("\n\n\n\t\t\t\t   User Login:");
        scanf("%s", u->name);

        // disabling echo
        tcgetattr(fileno(stdin), &nflags);
        nflags.c_lflag &= ~ECHO;
        nflags.c_lflag |= ECHONL;

        if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
        {
            perror("tcsetattr");
            return 0; // Return 0 to indicate login failure
        }

        if ((fp = fopen(USERS, "r")) == NULL)
        {
            printf("Error! Opening file");
            return 0; // Return 0 to indicate login failure
        }

        int userFound = 0; // Flag to track if the entered username exists

        while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) == 3)
        {
            if (strcmp(userChecker.name, u->name) == 0)
            {
                userFound = 1;
                break;
            }
        }

        fclose(fp);

        // restore terminal
        if (tcsetattr(fileno(stdin), TCSANOW, &orig_flags) != 0)
        {
            perror("tcsetattr");
            return 0; // Return 0 to indicate login failure
        }

        if (!userFound)
        {
            printf("\nUsername not found. Please try again.\n");
            // Clear the input buffer to remove any leftover characters
            char buffer[100];
            fgets(buffer, sizeof(buffer), stdin);
            continue;
        }

        printf("\n\n\n\t\t\t\t   User Password:");
        scanf("%s", u->password);

        // disabling echo again for password entry
        tcgetattr(fileno(stdin), &nflags);
        nflags.c_lflag &= ~ECHO;
        nflags.c_lflag |= ECHONL;

        if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
        {
            perror("tcsetattr");
            return 0; // Return 0 to indicate login failure
        }

        if ((fp = fopen(USERS, "r")) == NULL)
        {
            printf("Error! Opening file");
            return 0; // Return 0 to indicate login failure
        }

        while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) == 3)
        {
            if (strcmp(userChecker.name, u->name) == 0)
            {
                if (strcmp(userChecker.password, u->password) == 0)
                {
                    u->id = userChecker.id; // Update the ID with the correct value
                    loginSuccessful = 1;
                    break;
                }
            }
        }

        fclose(fp);

        // restore terminal
        if (tcsetattr(fileno(stdin), TCSANOW, &orig_flags) != 0)
        {
            perror("tcsetattr");
            return 0; // Return 0 to indicate login failure
        }

        if (!loginSuccessful)
        {
            printf("\nIncorrect password. Please try again.\n");
            // Clear the input buffer to remove any leftover characters
            char buffer[100];
            fgets(buffer, sizeof(buffer), stdin);
        }
    }

    // Register the cleanup function to restore terminal settings on program exit
    atexit(restoreTerminal);

    return 1; // Return 1 to indicate login success
}

const char *getPassword(struct User u)
{
    FILE *fp;
    struct User userChecker;

    if ((fp = fopen("./data/users.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    while (fscanf(fp, "%s %s", userChecker.name, userChecker.password) != EOF)
    {
        if (strcmp(userChecker.name, u.name) == 0)
        {
            fclose(fp);
            char *buff = userChecker.password;
            return buff;
        }
    }

    fclose(fp);
    return "no user found";
}   