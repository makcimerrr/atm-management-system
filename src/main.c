#include "header.h"
#include <stdbool.h>

char loggedInUsername[50] = "";

void mainMenu(struct User u)
{
    int option;
    system("clear");
    printf("\n\n\t\t======= ATM Maxime's Bank System =======\n\n");
    printf("\n\t\t-->> Feel free to choose one of the options below <<--\n");
    printf("\n\t\t[1]- Create a new account\n");
    printf("\n\t\t[2]- Update account information\n");
    printf("\n\t\t[3]- Check accounts\n");
    printf("\n\t\t[4]- Check list of owned account\n");
    printf("\n\t\t[5]- Make Transaction\n");
    printf("\n\t\t[6]- Remove existing account\n");
    printf("\n\t\t[7]- Transfer ownership\n");
    printf("\n\t\t[8]- Exit\n");
    scanf("%d", &option);

    switch (option)
    {
    case 1:
        createNewAcc(u);
        break;
    case 2:
        // student TODO : add your **Update account information** function
        // here
        updateAccountInfo(u);
        break;
    case 3:
        // student TODO : add your **Check the details of existing accounts** function
        // here
        checkInterest(u);
        break;
    case 4:
        checkAllAccounts(u);
        break;
    case 5:
        // student TODO : add your **Make transaction** function
        // here
        break;
    case 6:
        // student TODO : add your **Remove existing account** function
        // here
        break;
    case 7:
        // student TODO : add your **Transfer owner** function
        // here
        transferOwner(u);
        break;
    case 8:
        exit(1);
        break;
    default:
        printf("Invalid operation!\n");
    }
};

int initMenu(struct User *u)
{
    int r = 0;
    int option;

    system("clear");
    printf("\n\n\t\t======= ATM Maxime's Bank System=======\n");
    printf("\n\t\t-->> Feel free to login / register :\n");
    printf("\n\t\t[1]- login\n");
    printf("\n\t\t[2]- register\n");
    printf("\n\t\t[3]- exit\n");

    while (!r)
    {
        scanf("%d", &option);
        switch (option)
        {
        case 1:
            if (loginMenu(u, u))
            {
                r = 1;
            }
            else
            {
                printf("\nLogin failed. Exiting...\n");
                exit(1);
            }
            break;
        case 2:
            registration(u);
            r = 1;
            break;
        case 3:
            exit(1);
            break;
        default:
            printf("Insert a valid operation!\n");
        }
    }

    return 1; // Return 1 to indicate successful login
}



// Function prototypes
int initMenu(struct User *u);
int loginMenu(char a[50], char pass[50]); // Add the prototype for loginMenu


int main()
{
    struct User u;
    
    initMenu(&u);
    mainMenu(u);
    return 0;
}

