#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <limits.h> // Add this line to include INT_MAX
#define MAX_ACCOUNTS 100



const char *RECORDS = "./data/records.txt";
const char *USERS_FILE = "./data/users.txt";


int getAccountFromFile(FILE *ptr, char name[50], struct Record *r)
{
    return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r->id,
		          &r->userId,
		          r->name,
                  &r->accountNbr,
                  &r->deposit.month,
                  &r->deposit.day,
                  &r->deposit.year,
                  r->country,
                  &r->phone,
                  &r->amount,
                  r->accountType) != EOF;
}

int getNewOwnerId(const char *username) {
    FILE *userFile = fopen(USERS_FILE, "r");
    if (userFile != NULL) {
        int newOwnerId;
        char existingUsername[50], existingPassword[50];
        while (fscanf(userFile, "%d %s %s", &newOwnerId, existingUsername, existingPassword) == 3) {
            if (strcmp(username, existingUsername) == 0) {
                fclose(userFile);
                return newOwnerId;
            }
        }
        fclose(userFile);
    }
    return -1; // Invalid username
}

void saveAccountToFile(FILE *ptr, struct User u, struct Record r)
{
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
            r.id,
	        u.id,
            u.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
}

void stayOrReturn(int notGood, void f(struct User u), struct User u)
{
    int option;
    if (notGood == 0)
    {
        system("clear");
        printf("\n✖ Record not found!!\n");
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
        scanf("%d", &option);
        if (option == 0)
            f(u);
        else if (option == 1)
            mainMenu(u);
        else if (option == 2)
            exit(0);
        else
        {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    }
    else
    {
        printf("\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &option);
    }
    if (option == 1)
    {
        system("clear");
        mainMenu(u);
    }
    else
    {
        system("clear");
        exit(1);
    }
}

void success(struct User u)
{
    int option;
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &option);
    system("clear");
    if (option == 1)
    {
        mainMenu(u);
    }
    else if (option == 0)
    {
        exit(1);
    }
    else
    {
        printf("Insert a valid operation!\n");
        goto invalid;
    }
}

void createNewAcc(struct User u)
{
    struct Record r;
    struct Record cr;
    char userName[50];
    FILE *pf = fopen(RECORDS, "a+");

    while (1)
    {
        system("clear");
        printf("\t\t\t===== New record =====\n");

        printf("\nEnter today's date(mm/dd/yyyy):");
        scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);

        // Increment id for each user's account
        int highestAccountId = 0;
        while (getAccountFromFile(pf, userName, &cr))
        {
            if (cr.id > highestAccountId)
            {
                highestAccountId = cr.id;
            }
        }
        r.id = highestAccountId + 1;

        printf("\nEnter the account number:");
        scanf("%d", &r.accountNbr);

        // Check if the account number is already used by any user
        rewind(pf);
        int accountExists = 0;
        while (getAccountFromFile(pf, userName, &cr))
        {
            if (cr.accountNbr == r.accountNbr)
            {
                printf("\n✖ This Account number is already used\n\n");
                accountExists = 1;
                break;
            }
        }

        if (!accountExists)
        {
            break; // If the account number is unique, break out of the loop
        }

        // Pause execution for 3 seconds
        sleep(3);
    }

    printf("\nEnter the country:");
    scanf("%s", r.country);
    printf("\nEnter the phone number:");
    scanf("%d", &r.phone);
    printf("\nEnter amount to deposit: $");
    scanf("%lf", &r.amount);
    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
    scanf("%s", r.accountType);

    saveAccountToFile(pf, u, r);

    fclose(pf);
    success(u);
}



void checkAllAccounts(struct User u)
{
    char userName[100];
    struct Record r;

    FILE *pf = fopen(RECORDS, "r");

    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u.name);

    int foundAccounts = 0; // Variable to track if any accounts were found for the user

    while (getAccountFromFile(pf, userName, &r))
    {
        if (r.userId == u.id) // Compare user_id of the account with the id of the logged-in user
        {
            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);

            foundAccounts = 1; // Mark that accounts were found for the user
        }
    }

    fclose(pf);

    if (!foundAccounts)
    {
        printf("\nNo accounts found for user %s. Create a new account to get started!\n", u.name);
    }

    success(u);
}

void checkInterest(struct User u)
{
    system("clear");
    printf("\t\t====== Check Interest from user, %s =====\n\n", u.name);
    int accountNumber;
    printf("\nEnter the account number you want to check: ");
    scanf("%d", &accountNumber);

    double interest;
    double interest2;

    struct Record r;
    char userName[100];
    FILE *pf = fopen(RECORDS, "r");

    int found = 0;

    while (getAccountFromFile(pf, userName, &r))
    {
        if (r.accountNbr == accountNumber)
        {
            found = 1;
            printf("\n=========================\n");
            printf("Account number: %d\n", r.accountNbr);
            printf("Deposit Date: %d/%d/%d\n", r.deposit.month, r.deposit.day, r.deposit.year);
            printf("Country: %s\n", r.country);
            printf("Phone number: %d\n", r.phone);
            printf("Amount deposited: $%.2f\n", r.amount);
            printf("Type Of Account: %s\n", r.accountType);

            double interestRate = 0.0;
            int timePeriod = 0;
            int timeY = 0;

            if (strcmp(r.accountType, "saving") == 0)
            {
                printf("Interest Rate: 0.07%% per year\n");
                interestRate = 0.07;
                timePeriod = 1; // 1 month (interest will be calculated monthly)  
            }
            else if (strcmp(r.accountType, "fixed01") == 0)
            {
                printf("Interest Rate: 0.04%% per year\n");
                interestRate = 0.04;
                timePeriod = 12; // 1 year (interest will be calculated monthly)
                timeY = 1;
            }
            else if (strcmp(r.accountType, "fixed02") == 0)
            {
                printf("Interest Rate: 0.05%% per year\n");
                interestRate = 0.05;
                timePeriod = 24; // 2 years (interest will be calculated monthly)
                timeY = 2;
            }
            else if (strcmp(r.accountType, "fixed03") == 0)
            {
                printf("Interest Rate: 0.08%% per year\n");
                interestRate = 0.08;
                timePeriod = 36; // 3 years (interest will be calculated monthly)
                timeY = 3;
            }
            else if (strcmp(r.accountType, "current") == 0)
            {
                printf("You will not get interests because the account is of type current\n");
            }

            if (timePeriod > 11)
            {
                interest = r.amount * interestRate;
                interest2 = interest * timeY;

                printf("Interest Earned after %d months: $%.2f\n", timePeriod, interest2);
            }
            else if (timePeriod == 1)
            {
                interest = r.amount * interestRate;
                interest2 = interest * (1. / 12.);
                printf("Interest Earned on day %d of every month: $%.2f,\n", r.deposit.day, interest2);
            }

            printf("=========================\n");
            break;
        }
    }

    fclose(pf);

    if (!found)
    {
        printf("\n✖ Account not found.\n");
    }

    success(u);
}

void registration(struct User *u)
{
    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management Register\n\t\t\t\t\t Enter username:");
    scanf("%s", u->name);

    // Check if the username is already taken
    FILE *userFile = fopen(USERS_FILE, "r");
    if (userFile != NULL)
    {
        int existingUserId;
        char existingUsername[50];
        char existingPassword[50];
        int isUsernameTaken = 0; // Flag to indicate if the username is taken

        while (fscanf(userFile, "%d %s %s", &existingUserId, existingUsername, existingPassword) == 3)
        {
            if (strcmp(u->name, existingUsername) == 0)
            {
                isUsernameTaken = 1;
                break; // The username is taken, no need to continue checking
            }
        }
        fclose(userFile);

        if (isUsernameTaken)
    {
        printf("\nError: Username already taken. Please choose a different username.\n");
        sleep(4); // Add a 3-second delay
        system("clear"); // Clear the screen after the delay
        registration(u); // Repeat the registration process
        return; // Return to the previous registration call
    }
    }

    printf("\n\n\n\n\n\t\t\t\tEnter a password: ");
    scanf("%s", u->password);

    // Find the highest existing ID in the file and add 1 to create a new ID.
    int highestId = 0;
    int currentId;
    userFile = fopen(USERS_FILE, "r");
    if (userFile != NULL)
    {
        while (fscanf(userFile, "%d %*s %*s", &currentId) == 1)
        {
            if (currentId > highestId)
                highestId = currentId;
        }
        fclose(userFile);
    }
    u->id = highestId + 1;

    userFile = fopen(USERS_FILE, "a");
    if (userFile != NULL)
    {
        // Save the new user to the file.
        fprintf(userFile, "%d %s %s\n", u->id, u->name, u->password);
        fclose(userFile);

        printf("\nRegistration successful! Your ID is: %d\n", u->id);
    }
    else
    {
        printf("\nError: Unable to open users file.\n");
    }
}

void updateAccountInfo(struct User u) {
    int accountNumber;
    int option;

    system("clear");
    printf("\t\t====== Update Account Info =====\n\n");
    printf("Enter the account number you want to update: ");
    scanf("%d", &accountNumber);

    struct Record accounts[MAX_ACCOUNTS];
    int numAccounts = 0;

    FILE *pf = fopen(RECORDS, "r");

    while (getAccountFromFile(pf, accounts[numAccounts].name, &accounts[numAccounts])) {
        if (accounts[numAccounts].userId == u.id) {
            numAccounts++;
        }
    }

    fclose(pf);

    int accountIndex = -1;
    for (int i = 0; i < numAccounts; i++) {
        if (accounts[i].accountNbr == accountNumber) {
            accountIndex = i;
            break;
        }
    }

    if (accountIndex == -1) {
        printf("\n✖ Account not found or you don't have permission to update this account.\n");
        success(u);
    }

    printf("\n=========================\n");
    printf("Owner: %s\n", accounts[accountIndex].name);
    printf("Account number: %d\n", accounts[accountIndex].accountNbr);
    printf("Deposit Date: %d/%d/%d\n", accounts[accountIndex].deposit.day, accounts[accountIndex].deposit.month, accounts[accountIndex].deposit.year);
    printf("Country: %s\n", accounts[accountIndex].country);
    printf("Phone number: %d\n", accounts[accountIndex].phone);
    printf("Amount deposited: $%.2f\n", accounts[accountIndex].amount);
    printf("Type Of Account: %s\n", accounts[accountIndex].accountType);

    do {
        printf("\nWhich field do you want to update?\n");
        printf("1. Country\n");
        printf("2. Phone number\n");
        printf("3. Exit\n");
        printf("Enter your choice (1 or 2): ");
        scanf("%d", &option);

        if (option == 1) {
            printf("\nEnter the new country: ");
            scanf("%s", accounts[accountIndex].country);
        } else if (option == 2) {
            printf("\nEnter the new phone number: ");
            scanf("%d", &accounts[accountIndex].phone);
        } else {
            printf("Invalid option. Please enter 1, 2, or 3.\n");
        }
    } while (option != 1 && option != 2 );

    // Create a temporary file to store the updated content
    FILE *tempFile = fopen("temp_records.txt", "w");
    if (tempFile == NULL) {
        printf("Error creating temporary file.\n");
        return;
    }

    // Write the original content to the temporary file, updating the specified account
    pf = fopen(RECORDS, "r");
    char line[500];
    while (fgets(line, sizeof(line), pf) != NULL) {
        int id;
        sscanf(line, "%d", &id);
        if (id == accounts[accountIndex].id) {
            fprintf(tempFile, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n",
                    accounts[accountIndex].id,
                    accounts[accountIndex].userId,
                    accounts[accountIndex].name,
                    accounts[accountIndex].accountNbr,
                    accounts[accountIndex].deposit.month,
                    accounts[accountIndex].deposit.day,
                    accounts[accountIndex].deposit.year,
                    accounts[accountIndex].country,
                    accounts[accountIndex].phone,
                    accounts[accountIndex].amount,
                    accounts[accountIndex].accountType);
            accountIndex = -2; // Mark as written
        } else {
            fputs(line, tempFile);
        }
    }
    fclose(pf);

    fclose(tempFile);

    // Replace the original file with the temporary file
    remove(RECORDS);
    rename("temp_records.txt", RECORDS);

    if (accountIndex != -2) {
        printf("\n✖ Account not found in the file.\n");
    } else {
        printf("\n✔ Account information updated successfully!\n");
    }
    printf("=========================\n");

    success(u);
}

void transferOwner(struct User u) {
    int accountNumber;

    system("clear");
    printf("\t\t====== Transfer Owner =====\n\n");

    // Get account number
    printf("Enter the account number you want to update: ");
    scanf("%d", &accountNumber);

    struct Record accounts[MAX_ACCOUNTS];
    int numAccounts = 0;

    FILE *pf = fopen(RECORDS, "r");

    while (getAccountFromFile(pf, accounts[numAccounts].name, &accounts[numAccounts])) {
        if (accounts[numAccounts].userId == u.id) {
            numAccounts++;
        }
    }

    fclose(pf);

    int accountIndex = -1;
    for (int i = 0; i < numAccounts; i++) {
        if (accounts[i].accountNbr == accountNumber) {
            accountIndex = i;
            break;
        }
    }

    if (accountIndex == -1) {
        printf("\n✖ Account not found or you don't have permission to update this account.\n");
        success(u);
    }

    printf("\n=========================\n");
    printf("Owner: %s\n", accounts[accountIndex].name);
    printf("Account number: %d\n", accounts[accountIndex].accountNbr);
    printf("Deposit Date: %d/%d/%d\n", accounts[accountIndex].deposit.day, accounts[accountIndex].deposit.month, accounts[accountIndex].deposit.year);
    printf("Country: %s\n", accounts[accountIndex].country);
    printf("Phone number: %d\n", accounts[accountIndex].phone);
    printf("Amount deposited: $%.2f\n", accounts[accountIndex].amount);
    printf("Type Of Account: %s\n", accounts[accountIndex].accountType);

    // Get new owner's username
    char newOwnerUsername[50];
    int isValidUsername = 0;

    while (!isValidUsername) {
        printf("Enter the username of the new owner: ");
        scanf("%s", newOwnerUsername);

        // Check if new owner username is valid
        FILE *userFile = fopen(USERS_FILE, "r");
        if (userFile != NULL) {
            int newOwnerId;
            char existingUsername[50], existingPassword[50];
            while (fscanf(userFile, "%d %s %s", &newOwnerId, existingUsername, existingPassword) == 3) {
                if (strcmp(newOwnerUsername, existingUsername) == 0) {
                    isValidUsername = 1;
                    break;
                }
            }
            fclose(userFile);
        }

        if (!isValidUsername) {
            printf("\n✖ Invalid username. Please enter a valid username.\n");
        }
    }

    // Update account owner's name and ID
    FILE *pf_tmp = fopen("temp_records.txt", "w");
    if (pf_tmp == NULL) {
        printf("Error opening temporary file for writing.\n");
        return;
    }

    pf = fopen(RECORDS, "r");
    if (pf == NULL) {
        printf("Error opening file for reading.\n");
        fclose(pf_tmp);
        return;
    }

    char line[1000];
    while (fgets(line, sizeof(line), pf)) {
        int id, userId, accountNbr, phone;
        double amount;
        char name[50], country[50], accountType[50];
        int deposit_day, deposit_month, deposit_year;

        sscanf(line, "%d %d %s %d %d/%d/%d %s %d %lf %s",
               &id, &userId, name, &accountNbr, &deposit_month, &deposit_day, &deposit_year,
               country, &phone, &amount, accountType);

        if (accountNbr == accounts[accountIndex].accountNbr) {
            userId = getNewOwnerId(newOwnerUsername);
            fprintf(pf_tmp, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n",
                    id, userId, newOwnerUsername, accountNbr, deposit_month, deposit_day, deposit_year,
                    country, phone, amount, accountType);
            break; // Write the modified line and stop processing further
        } else {
            fputs(line, pf_tmp);
        }
    }

    // Write the remaining lines to the temporary file
    while (fgets(line, sizeof(line), pf)) {
        fputs(line, pf_tmp);
    }

    fclose(pf);
    fclose(pf_tmp);

    if (remove(RECORDS) != 0) {
        printf("Error deleting original records file.\n");
        return;
    }

    if (rename("temp_records.txt", RECORDS) != 0) {
        printf("Error renaming temporary file to records file.\n");
        return;
    }

    printf("\n✔ Owner transfer successful!\n");
    printf("=========================\n");

    success(u);
}

void makeTransaction(struct User u) {
    int accountNumber;
    char transactionType[10];

    system("clear");
    printf("\t\t====== Make Transaction =====\n\n");

    // Get account number
    printf("Enter the account number: ");
    scanf("%d", &accountNumber);

    struct Record accounts[MAX_ACCOUNTS];
    int numAccounts = 0;

    FILE *pf = fopen(RECORDS, "r");

    while (getAccountFromFile(pf, accounts[numAccounts].name, &accounts[numAccounts])) {
        if (accounts[numAccounts].userId == u.id) {
            numAccounts++;
        }
    }

    fclose(pf);

    int accountIndex = -1;
    for (int i = 0; i < numAccounts; i++) {
        if (accounts[i].accountNbr == accountNumber) {
            accountIndex = i;
            break;
        }
    }

    if (accountIndex == -1) {
        printf("\n✖ Account not found or you don't have permission to access this account.\n");
        success(u);
    }

    printf("\n=========================\n");
    printf("Owner: %s\n", accounts[accountIndex].name);
    printf("Account number: %d\n", accounts[accountIndex].accountNbr);
    printf("Current balance: $%.2f\n", accounts[accountIndex].amount);
    printf("Type Of Account: %s\n", accounts[accountIndex].accountType);

    if (strcmp(accounts[accountIndex].accountType, "fixed01") == 0 ||
        strcmp(accounts[accountIndex].accountType, "fixed02") == 0 ||
        strcmp(accounts[accountIndex].accountType, "fixed03") == 0) {
        printf("\n✖ Cannot perform transactions on fixed-term accounts.\n");
        success(u);
    }

    // Get transaction type (deposit or withdrawal)
    printf("Enter transaction type (deposit/withdraw): ");
    scanf("%s", transactionType);

    if (strcmp(transactionType, "deposit") == 0) {
        double amount;
        printf("Enter the amount to deposit: $");
        scanf("%lf", &amount);

        if (amount <= 0) {
            printf("\n✖ Invalid amount. Please enter a positive amount.\n");
            success(u);
        }

        accounts[accountIndex].amount += amount;
        printf("\n✔ Deposit successful! New balance: $%.2f\n", accounts[accountIndex].amount);
    } else if (strcmp(transactionType, "withdraw") == 0) {
        double amount;
        printf("Enter the amount to withdraw: $");
        scanf("%lf", &amount);

        if (amount <= 0) {
            printf("\n✖ Invalid amount. Please enter a positive amount.\n");
            success(u);
        }

        if (strcmp(accounts[accountIndex].accountType, "fixed01") == 0 ||
            strcmp(accounts[accountIndex].accountType, "fixed02") == 0 ||
            strcmp(accounts[accountIndex].accountType, "fixed03") == 0) {
            printf("\n✖ Cannot withdraw from fixed-term accounts.\n");
            success(u);
        }

        if (amount > accounts[accountIndex].amount) {
            printf("\n✖ Insufficient balance for withdrawal.\n");
            success(u);
        }

        accounts[accountIndex].amount -= amount;
        printf("\n✔ Withdrawal successful! New balance: $%.2f\n", accounts[accountIndex].amount);
    } else {
        printf("\n✖ Invalid transaction type. Please enter 'deposit' or 'withdraw'.\n");
        success(u);
    }

    // Update account balance
    FILE *pf_tmp = fopen("temp_records.txt", "w");
    if (pf_tmp == NULL) {
        printf("Error opening temporary file for writing.\n");
        return;
    }

    pf = fopen(RECORDS, "r");
    if (pf == NULL) {
        printf("Error opening file for reading.\n");
        fclose(pf_tmp);
        return;
    }

    char line[1000];
    while (fgets(line, sizeof(line), pf)) {
        int id, userId, accountNbr, phone;
        double amount;
        char name[50], country[50], accountType[50];
        int deposit_day, deposit_month, deposit_year;

        sscanf(line, "%d %d %s %d %d/%d/%d %s %d %lf %s",
               &id, &userId, name, &accountNbr, &deposit_month, &deposit_day, &deposit_year,
               country, &phone, &amount, accountType);

        if (accountNbr == accounts[accountIndex].accountNbr) {
            amount = accounts[accountIndex].amount;
            fprintf(pf_tmp, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n",
                    id, userId, name, accountNbr, deposit_month, deposit_day, deposit_year,
                    country, phone, amount, accountType);
            break; // Write the modified line and stop processing further
        } else {
            fputs(line, pf_tmp);
        }
    }

    // Write the remaining lines to the temporary file
    while (fgets(line, sizeof(line), pf)) {
        fputs(line, pf_tmp);
    }

    fclose(pf);
    fclose(pf_tmp);

    if (remove(RECORDS) != 0) {
        printf("Error deleting original records file.\n");
        return;
    }

    if (rename("temp_records.txt", RECORDS) != 0) {
        printf("Error renaming temporary file to records file.\n");
        return;
    }

    printf("=========================\n");

    success(u);
}

void removeAccount(struct User u) {
    int accountNumber;

    system("clear");
    printf("\t\t====== Remove Account =====\n\n");

    // Get account number
    printf("Enter the account number you want to remove: ");
    scanf("%d", &accountNumber);

    struct Record accounts[MAX_ACCOUNTS];
    int numAccounts = 0;

    FILE *pf = fopen(RECORDS, "r");

    while (getAccountFromFile(pf, accounts[numAccounts].name, &accounts[numAccounts])) {
        if (accounts[numAccounts].userId == u.id) {
            numAccounts++;
        }
    }

    fclose(pf);

    int accountIndex = -1;
    for (int i = 0; i < numAccounts; i++) {
        if (accounts[i].accountNbr == accountNumber) {
            accountIndex = i;
            break;
        }
    }

    if (accountIndex == -1) {
        printf("\n✖ Account not found or you don't have permission to access this account.\n");
        success(u);
    }

    printf("\n=========================\n");
    printf("Owner: %s\n", accounts[accountIndex].name);
    printf("Account number: %d\n", accounts[accountIndex].accountNbr);
    printf("Current balance: $%.2f\n", accounts[accountIndex].amount);
    printf("Type Of Account: %s\n", accounts[accountIndex].accountType);

    // Confirm account deletion
    char confirm[10];
    printf("Are you sure you want to remove this account? (yes/no): ");
    scanf("%s", confirm);

    if (strcmp(confirm, "yes") != 0) {
        printf("\nAccount removal cancelled.\n");
        success(u);
    }

    // Update records file
    FILE *pf_tmp = fopen("temp_records.txt", "w");
    if (pf_tmp == NULL) {
        printf("Error opening temporary file for writing.\n");
        return;
    }

    pf = fopen(RECORDS, "r");
    if (pf == NULL) {
        printf("Error opening file for reading.\n");
        fclose(pf_tmp);
        return;
    }

    char line[1000];
    while (fgets(line, sizeof(line), pf)) {
        int id, userId, accountNbr, phone;
        double amount;
        char name[50], country[50], accountType[50];
        int deposit_day, deposit_month, deposit_year;

        sscanf(line, "%d %d %s %d %d/%d/%d %s %d %lf %s",
               &id, &userId, name, &accountNbr, &deposit_month, &deposit_day, &deposit_year,
               country, &phone, &amount, accountType);

        if (accountNbr == accounts[accountIndex].accountNbr) {
            continue; // Skip writing the line for the deleted account
        } else {
            fputs(line, pf_tmp);
        }
    }

    fclose(pf);
    fclose(pf_tmp);

    if (remove(RECORDS) != 0) {
        printf("Error deleting original records file.\n");
        return;
    }

    if (rename("temp_records.txt", RECORDS) != 0) {
        printf("Error renaming temporary file to records file.\n");
        return;
    }

    printf("Account successfully removed.\n");
    printf("=========================\n");

    success(u);
}
