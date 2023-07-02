//ATM |-> --Saeed mazahery--

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include <windows.h>

//defines
#define USER_MAX 17
#define PASS_MAX 5
#define SLEEP 900
#define NAME_MAX 15
#define PHONE_MAX 12
#define CODE_MAX 11
#define ID_MAX 17
#define LOGIN_CHANCE 3
#define LIST_MAX 100

//structs.
typedef struct employee {
    char userName[USER_MAX];
    char password[PASS_MAX];
} EMP;
typedef struct customer {
    int row;
    char firstName[NAME_MAX];
    char lastName[NAME_MAX];
    char phoneNumber[PHONE_MAX];
    char nationalCode[CODE_MAX];
    char idCard[ID_MAX];
    char password[PASS_MAX];
    float balance;
} CUS;
struct listUpdate {
    char mod[10];
    float money;
    char id[ID_MAX];
};
typedef struct cashFile {
    int row;
    struct listUpdate list[11];
    int number;
} CASH;

//Total prototypes.
void startProgram(void);//Show prompt of start the program.

void endProgram(void);  //Show prompt of end the program.

int mainMenu(char *);   //function of main menu.

int employee(int (*checkLogIn)(char *, char *));    //Employees section.

int checkEmp(char *, char *);   //For checking the username and password.

int customer(int (*checkLogIn)(char *, char *));    //Customer section.

int checkCus(char *, char *);   //For checking the username and password.

int readingList(CUS *);    //It reads customer information from the file and stores it in a struct.

void saveToFile(CUS *, int);    //It reads customer information from the struct and save it in the file.

//Employee prototype.
int createAccount(void);    //Employee->Create an account.

int EditAccount(void);    //Employee->Edit accounts.

void showRichest(void);    //Employee->The information of the customer with the most inventory is displayed.

void showTotalBalance(void);    // The total balance of all accounts is printed.

//Customer prototype.
void accountBalance(char *);    //customer->Customer account balance is displayed.

void cashTransfer(char *);    //customer->In this section, money is transferred.

void cashWithdrawal(char *);    //customer->In this section, money is withdrawn from the account.

void recentAccountUpdates(char *);    //customer->The last ten transactions of the account are displayed.

void changePassword(char *);    //customer->In this section, the password is changed.

//Function of cashTransfer, cashWithdrawal, recentAccountUpdates.
void saveToCashFile(char *, float, char *,
                    char *);    //It reads recent account updates from the struct and save it in the file.

void readCashFile(CASH *);    //It reads recent account updates from the file and stores it in a struct.

///////////////////////////////////////////////////////////////////
int main() {
    atexit(endProgram); //A function that is executed at the end of the program.
    startProgram();
    char choice[10];

    while (1) {
        switch (mainMenu(choice)) {
            case 1:
                employee(checkEmp);
                break;
            case 2:
                customer(checkCus);
                break;
            case 3:
                system("cls");
                printf("\nDo you want to exit the program?\n ~Press (y) to exit the program~\n");
                if (tolower(getch()) == 'y')
                    exit(EXIT_SUCCESS);
                else
                    break;
            default:
                printf("Your choice was wrong!!\n"
                       "Try again\n");
                Sleep(SLEEP);
        }
        system("Cls");
    }
}

/////////////////////////////////////////////////////////////////
void startProgram(void) {
     int i;
    for (i = 0; i < 24; ++i)
        printf("_*");
    puts("");
    printf("%32s", "....Welcome....\n");
    for (i = 0; i < 24; ++i)
        printf("_*");
    puts("");
    getchar();
    system("cls");
}

/////////////////////////////////////////////////////////////////
int mainMenu(char choice[]) {

    printf("\n___________________*Main Menu*___________________\n");
    Sleep(SLEEP);
    printf("::Enter the desired option number to enter each section::\n");
    printf("%s%s%s",
           "1.Log in employee.\n",
           "2.Log in customer.\n",
           "3.Exit.\n");
    printf("\nEnter your choose->");
    scanf("%s", choice);//User selection.
    return choice[0] - 48;
}

/////////////////////////////////////////////////////////////////
int employee(int (*checkLogIn)(char *, char *)) {
    int chance = 0;
    while (chance < LOGIN_CHANCE) { //Here, the user has the ability to enter
        // the information three times in case of wrong entry.
        EMP employee;
        system("cls");
        printf("\n\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\LOGIN//////////////////// \n");
        Sleep(SLEEP);
        printf("Enter your user name :");
        scanf("%s", employee.userName);
        printf("Enter your password :");
        scanf("%s", employee.password);
        system("Cls");

        int check = checkLogIn(employee.userName, employee.password); //to check username and password
        if (check == 2) {
            char choice[10];

            while (1) {
                //Menu list.
                system("Cls");
                printf("\n___________________*Employee Menu*___________________\n");
                Sleep(SLEEP);
                printf("%s%s%s%s%s",
                       "1.Create an account.\n",
                       "2.Edit an account.\n",
                       "3.Show the richest customer.\n",
                       "4.Show total balance of all accounts.\n",
                       "5.Back to the main menu.\n");
                printf("\nEnter your choose->");
                scanf("%s", choice);//User selection.
                switch (choice[0]) {
                    case '1':
                        createAccount();
                        break;
                    case '2':
                        EditAccount();
                        break;
                    case '3':
                        showRichest();
                        break;
                    case '4':
                        showTotalBalance();
                        break;
                    case '5':
                        return 0;
                    default:
                        printf("Your choice was wrong!!\n"
                               "Try again\n");
                        getchar();
                        Sleep(SLEEP);
                }
            }
        } else if (check == 0 || check == 1) {
            chance++;
            if (chance != 3) {
                puts("Your username or password is wrong!!\n"
                     "           ::Try again::");
                getchar();
                Sleep(SLEEP);
            }
        }
    }
    puts("***your chance is over***");
    getchar();
    Sleep(SLEEP);
    return 0;
}

/////////////////////////////////////////////////////////////////
int checkEmp(char userName[], char password[]) {
    enum check {
        ErrorUser, ErrorPass, CorrectExpression
    };
    int final;
    FILE *login;
    if ((login = fopen("loginEmp.txt", "r")) == NULL) {
        puts("File could not be opened");
    } else {
        while (!feof(login)) {
            EMP Check = {"", ""}; // Create struct employee loginCheck with default information.

            int result = fscanf(login, "%s %s", Check.userName, Check.password);
            if (result != 0) { //For the function fscanf to run correctly
                if (strcmp(userName, Check.userName) == 0) {
                    if (strcmp(password, Check.password) == 0) {
                        fclose(login);
                        return CorrectExpression;
                    } else
                        final = ErrorPass;
                } else
                    final = ErrorUser;
            }
        }
        fclose(login);
        return final;
    }
}

/////////////////////////////////////////////////////////////////
int checkCus(char userName[], char password[]) {
    enum check {
        ErrorUser, ErrorPass, CorrectExpression
    };
    int final;
    FILE *login;
    if ((login = fopen("CustomerInformation.txt", "r")) == NULL) {
        puts("File could not be opened");
    } else {
        while (!feof(login)) {
            CUS Check = {0, "", "", "", "", "", "", 0}; // Create struct employee loginCheck with default information.

            int result = fscanf(login, "%d %s %s %s %s %s %s %f", //Read customer structure from file
                                &Check.row,
                                Check.firstName,
                                Check.lastName,
                                Check.phoneNumber,
                                Check.nationalCode,
                                Check.idCard,
                                Check.password,
                                &Check.balance
            );
            if (result != 0) { //For the function fscanf to run correctly
                if (strcmp(userName, Check.idCard) == 0) {
                    if (strcmp(password, Check.password) == 0) {
                        fclose(login);
                        return CorrectExpression;
                    } else
                        final = ErrorPass;
                } else
                    final = ErrorUser;
            }
        }
        fclose(login);
        return final;
    }
}

/////////////////////////////////////////////////////////////////
int customer(int (*checkLogIn)(char *, char *)) {
    int chance = 0;

    while (chance < LOGIN_CHANCE) {//Here, the user has the ability to enter
        // the information three times in case of wrong entry.
        system("Cls");
        printf("\n\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\LOGIN//////////////////// \n");
        Sleep(SLEEP);
        CUS customer;
        printf("Enter your ID card :");
        scanf("%s", customer.idCard);
        printf("Enter your password :");
        scanf("%s", customer.password);
        system("Cls");

        int check = checkLogIn(customer.idCard, customer.password);// to check ID card and password
        if (check == 2) {
            char choice[10];
            while (1) {
                //Menu list.
                system("Cls");
                printf("\n___________________*customer Menu*___________________\n");
                Sleep(SLEEP);
                printf("%s%s%s%s%s%s",
                       "1.Account balance.\n",
                       "2.Cash transfer.\n",
                       "3.Cash withdrawal.\n",
                       "4.Recent account updates.\n",
                       "5.Change password.\n",
                       "6.Back to the main menu.\n");
                printf("\nEnter your choose->");
                scanf("%s", choice);//User selection.
                switch (choice[0]) {
                    case '1':
                        accountBalance(customer.idCard);
                        break;
                    case '2':
                        cashTransfer(customer.idCard);
                        break;
                    case '3':
                        cashWithdrawal(customer.idCard);
                        break;
                    case '4':
                        recentAccountUpdates(customer.idCard);
                        break;
                    case '5':
                        changePassword(customer.idCard);
                        break;
                    case '6':
                        return 0;
                    default:
                        printf("Your choice was wrong!!\n"
                               "***Try again***\n");
                        getchar();
                        Sleep(SLEEP);
                }
            }
        } else if (check == 0 || check == 1) {
            chance++;
            if (chance != 3) {
                puts("Your username or password is wrong!!\n"
                     "           ::Try again::");
                getchar();
                Sleep(SLEEP);
            }
        }
    }
    puts("***your chance is over***");
    getchar();
    Sleep(SLEEP);
    return 0;
}

/////////////////////////////////////////////////////////////////
int createAccount(void) {
    CUS list[LIST_MAX];
    int numberOfCustomer = readingList(list);
    system("cls");
    printf("\n\\\\*******************CREATE ACCOUNT*******************//\n");
    // Getting account information from the employee.
    printf("::Enter the information in the following fields::\n");
    list[numberOfCustomer + 1].row = numberOfCustomer + 1;
    printf("First name->");
    scanf("%s", list[numberOfCustomer + 1].firstName);
    printf("Last name->");
    scanf("%s", list[numberOfCustomer + 1].lastName);

    int checkNum;
    int counter = 0;
    do { // Getting a phone number and checking its conditions.
        if (counter > 0)
            printf("This phone number is not acceptable\n");
        printf("Phone number->");
        scanf("%s", list[numberOfCustomer + 1].phoneNumber);
        counter++;

        //Checking the number of characters.
        checkNum = 0;
        int i;
        for (i = 0; i < PHONE_MAX - 1; ++i) {
            if (isdigit(list[numberOfCustomer + 1].phoneNumber[i]))
                checkNum++;
        }
    } while ((strlen(list[numberOfCustomer + 1].phoneNumber) != 11
              || checkNum != strlen(list[numberOfCustomer + 1].phoneNumber))
             || (list[numberOfCustomer + 1].phoneNumber[0] != '0'
                 || list[numberOfCustomer + 1].phoneNumber[1] != '9'));

    counter = 0;
    do { // Getting a national code and checking its conditions.
        if (counter > 0)
            printf("This national code is not acceptable\n");

        printf("National code->");
        scanf("%s", list[numberOfCustomer + 1].nationalCode);
        counter++;

        //Checking the number of characters.
        checkNum = 0;
        int i;
        for (i = 0; i < CODE_MAX - 1; ++i) {
            if (isdigit(list[numberOfCustomer + 1].nationalCode[i]))
                checkNum++;
        }
    } while (!(strlen(list[numberOfCustomer + 1].nationalCode) == 10
               && checkNum == strlen(list[numberOfCustomer + 1].nationalCode)));

    counter = 0;
    do { // Getting an ID card number and checking its conditions.
        if (counter > 0)
            printf("This ID card number is not acceptable\n");

        printf("ID card number->");
        scanf("%s", list[numberOfCustomer + 1].idCard);
        counter++;

        //Checking the number of characters.
        checkNum = 0;
        int i;
        for (i = 0; i < ID_MAX - 1; ++i) {
            if (isdigit(list[numberOfCustomer + 1].idCard[i]))
                checkNum++;
        }
    } while (!(strlen(list[numberOfCustomer + 1].idCard) == 16
               && checkNum == strlen(list[numberOfCustomer + 1].idCard)));

    counter = 0;
    do { // Getting a password and checking its conditions.
        if (counter > 0)
            printf("This Password is not acceptable\n");

        printf("Password->");
        scanf("%s", list[numberOfCustomer + 1].password);
        counter++;

        //Checking the number of characters.
        checkNum = 0;
        int i;
        for (i = 0; i < PASS_MAX - 1; ++i) {
            if (isdigit(list[numberOfCustomer + 1].password[i]))
                checkNum++;
        }
    } while (!(strlen(list[numberOfCustomer + 1].password) == 4
               && checkNum == strlen(list[numberOfCustomer + 1].password)));

    counter = 0;
    do { // Getting account balance and checking its conditions.
        if (counter > 0)
            printf("This value is not acceptable\n");

        printf("Account balance($)->");
        scanf("%f", &list[numberOfCustomer + 1].balance);
        counter++;

    } while (list[numberOfCustomer + 1].balance < 0);

    int i;
    for (i = 1; i <= numberOfCustomer; ++i) {
        if (strcmp(list[i].idCard, list[numberOfCustomer + 1].idCard) == 0) {
            printf("This number of ID card is already exist!\n");
            break;
        } else if (i == numberOfCustomer) {
            FILE *save;
            if ((save = fopen("RecentAccountUpdates.txt", "a")) == NULL) {
                puts("File could not be opened");
            } else {
                fprintf(save, "%d\n%c %.2f %c\n", numberOfCustomer + 1, 'e', 0.00, 'e');
            }
            fclose(save);

            printf("Create account was successful");
            saveToFile(list, numberOfCustomer + 1);
            break;
        }
    }
    getchar();
    Sleep(SLEEP + 500);
    return 0;

}

/////////////////////////////////////////////////////////////////
int EditAccount(void) {
    CUS list[LIST_MAX];
    int numberOfCustomer = readingList(list);
    char cardNumber[ID_MAX];
    system("cls");
    printf("\n\\\\*******************EDIT ACCOUNT*******************//\n");
    printf("Please enter the desired number of ID card->");
    scanf("%s", cardNumber);

    int j;
    for (j = 1; j <= numberOfCustomer; ++j) {
        if (strcmp(cardNumber, list[j].idCard) == 0) {
            printf("New first name->");
            scanf("%s", list[j].firstName);
            printf("New last name->");
            scanf("%s", list[j].lastName);

            int checkNum;
            int counter = 0;
            do { // Getting a phone number and checking its conditions.
                if (counter > 0)
                    printf("This phone number is not acceptable\n");
                printf("New phone number->");
                scanf("%s", list[j].phoneNumber);
                counter++;

                //Checking the number of characters.
                checkNum = 0;
                int i;
                for (i = 0; i < PHONE_MAX - 1; ++i) {
                    if (isdigit(list[j].phoneNumber[i]))
                        checkNum++;
                }
            } while ((strlen(list[j].phoneNumber) != 11
                      || checkNum != strlen(list[j].phoneNumber))
                     || (list[j].phoneNumber[0] != '0'
                         || list[j].phoneNumber[1] != '9'));

            counter = 0;
            do { // Getting a national code and checking its conditions.
                if (counter > 0)
                    printf("This national code is not acceptable\n");

                printf("New national code->");
                scanf("%s", list[j].nationalCode);
                counter++;

                //Checking the number of characters.
                checkNum = 0;
                int i;
                for (i = 0; i < CODE_MAX - 1; ++i) {
                    if (isdigit(list[j].nationalCode[i]))
                        checkNum++;
                }
            } while (!(strlen(list[j].nationalCode) == 10
                       && checkNum == strlen(list[j].nationalCode)));

            counter = 0;
            do { // Getting a password and checking its conditions.
                if (counter > 0)
                    printf("This Password is not acceptable\n");

                printf("New password->");
                scanf("%s", list[j].password);
                counter++;

                //Checking the number of characters.
                checkNum = 0;
                int i;
                for (i = 0; i < PASS_MAX - 1; ++i) {
                    if (isdigit(list[j].password[i]))
                        checkNum++;
                }
            } while (!(strlen(list[j].password) == 4
                       && checkNum == strlen(list[j].password)));

            saveToFile(list, numberOfCustomer);
            break;
        }
        if (j == numberOfCustomer)
            printf("There is no information for this ID card!\n");
    }
    getchar();
    Sleep(SLEEP + 500);
}

/////////////////////////////////////////////////////////////////
void showRichest(void) {
    getchar();
    system("cls");
    printf("\n\\\\*******************SHOW RICHEST CUSTOMER*******************//\n\n");
    CUS list[LIST_MAX];
    int numberOfCustomer = readingList(list);
    int max = 0, idMax;

    int i;
    for (i = 1; i <= numberOfCustomer; ++i) {
        if (list[i].balance > max) {
            max = list[i].balance;
            idMax = i;
        }
    }
    printf("  %-10s |  %-12s |  %-15s |  %-15s |  %-20s |  %-8s  |  %-8s\n",
           "First name",
           "Last name",
           "Phone number",
           "National code",
           "ID card",
           "Password",
           "Balance"
    );
    printf("_______________________________________________________________________________________________________________________\n");
    printf("  %-10s |  %-12s |  %-15s |  %-15s |  %-20s |  %-8s  |%8.2f$  \n",
           list[idMax].firstName,
           list[idMax].lastName,
           list[idMax].phoneNumber,
           list[idMax].nationalCode,
           list[idMax].idCard,
           list[idMax].password,
           list[idMax].balance);
    getchar();
}

/////////////////////////////////////////////////////////////////
void showTotalBalance(void) {
    getchar();
    system("cls");
    printf("\n\\\\*******************SHOW TOTAL BALANCE CUSTOMER*******************//\n\n");
    CUS list[LIST_MAX];
    int numberOfCustomer = readingList(list);
    float sum = 0.0;

    int i;
    for (i = 1; i <= numberOfCustomer; ++i) {
        sum += list[i].balance;
    }
    printf("______________________________________________________________\n");
    printf("     Total balance of all accounts is equal to ~ %.2f$ ~\n", sum);
    printf("--------------------------------------------------------------\n");
    getchar();
}

/////////////////////////////////////////////////////////////////
void accountBalance(char idCard[]) {
    getchar();
    system("cls");
    CUS list[LIST_MAX];
    int numberOfCustomer = readingList(list);
    int id;
    printf("\n\\\\*******************ACCOUNT BALANCE*******************//\n");
    
    int i;
    for (i = 1; i <= numberOfCustomer; ++i) {
        if (strcmp(idCard, list[i].idCard) == 0) {
            id = i;
        }
    }
    printf("___________________________________________________\n");
    printf("       Your account balance is ~ %.2f ~\n", list[id].balance);
    printf("---------------------------------------------------\n");

    getchar();
}

/////////////////////////////////////////////////////////////////
void cashTransfer(char idCard[]) {
    CUS list[LIST_MAX];
    system("cls");
    int numberOfCustomer = readingList(list);
    int senderId, receiverId;
    float money;
    char numCard[ID_MAX];
    printf("\n\\\\*******************CASH TRANSFER*******************//\n");
    
    int i;
    for (i = 1; i <= numberOfCustomer; ++i) {
        if (strcmp(idCard, list[i].idCard) == 0) {
            senderId = i;
            break;
        }
    }
    printf("The desired amount to transfer->");
    scanf("%f", &money);
    printf("The destination card number->");
    scanf("%s", numCard);

    if (money <= list[senderId].balance) {
        int i;      
        for (i = 1; i <= numberOfCustomer; ++i) {
            if (strcmp(numCard, list[i].idCard) == 0) {
                receiverId = i;

                list[senderId].balance -= money;
                list[receiverId].balance += money;
                saveToFile(list, numberOfCustomer);
                saveToCashFile("Transfer", money, idCard, list[receiverId].idCard);

                Sleep(SLEEP);
                printf("The transfer was successful\n");
                break;

            } else if (i == numberOfCustomer) {
                printf("The destination card number is not valid!\n");
                break;
            }
        }
    } else {
        printf("The desired amount is more than your balance!\n");
        getchar();
        Sleep(SLEEP);
    }
    getchar();
    Sleep(SLEEP);
}

/////////////////////////////////////////////////////////////////
void cashWithdrawal(char idCard[]) {
    CUS list[LIST_MAX];
    system("cls");
    int numberOfCustomer = readingList(list);
    int id;
    float money;
    printf("\n\\\\*******************CASH WITHDRAWAL*******************//\n");
    int i;
    for (i = 1; i <= numberOfCustomer; ++i) {
        if (strcmp(idCard, list[i].idCard) == 0) {
            id = i;
            break;
        }
    }

    printf("The desired amount to withdrawal->");
    scanf("%f", &money);

    if (money <= list[id].balance) {
        list[id].balance -= money;
        saveToFile(list, numberOfCustomer);
        saveToCashFile("Withdrawal", money, idCard, list[id].idCard);

        Sleep(SLEEP);
        printf("The Withdrawal was successful\n");
    } else {
        printf("The desired amount is more than your balance!\n");
    }
    getchar();
}

/////////////////////////////////////////////////////////////////
void recentAccountUpdates(char idCard[]) {
    system("cls");
    getchar();
    CUS list[LIST_MAX];
    int numberOfCustomer = readingList(list);
    CASH listCash[LIST_MAX];
    readCashFile(listCash);
    int id;
    printf("\n\\\\*******************CASH WITHDRAWAL*******************//\n\n");
    int i;
    for (i = 1; i <= numberOfCustomer; ++i) {
        if (strcmp(idCard, list[i].idCard) == 0) {
            id = i;
            break;
        }
    }

    if (listCash[id].number > 0) {
        printf("  %-17s|  %-8s|  %-23s\n", "Transaction mod", "Amount", "ID receiver or sender");
        printf("________________________________________________________\n");
        int i;
        for (i = 0; i < listCash[id].number; ++i) {
            if (strcmpi(listCash[id].list[i].mod, "Transfer") == 0 ||
                strcmpi(listCash[id].list[i].mod, "Withdrawal") == 0) {

                printf("%d  %-16s| -%-8.2f|  %-23s\n",
                       i + 1,
                       listCash[id].list[i].mod,
                       listCash[id].list[i].money,
                       listCash[id].list[i].id);

                printf("________________________________________________________\n");

            } else if (strcmpi(listCash[id].list[i].mod, "Deposit") == 0) {
                printf("%d  %-16s| +%-8.2f|  %-23s\n",
                       i + 1,
                       listCash[id].list[i].mod,
                       listCash[id].list[i].money,
                       listCash[id].list[i].id);
                printf("________________________________________________________\n");

            }

        }
    } else
        printf("                       <<Empty>>\n");

    getchar();
}

/////////////////////////////////////////////////////////////////
void changePassword(char idCard[]) {
    system("cls");
    CUS list[LIST_MAX];
    int numberOfCustomer = readingList(list);
    int id;
    char pass[PASS_MAX];
    printf("\n\\\\*******************CHANGE PASSWORD*******************//\n");
    int i;
    for (i = 1; i <= numberOfCustomer; ++i) {
        if (strcmp(idCard, list[i].idCard) == 0) {
            id = i;
            break;
        }
    }
    printf("Please enter your current password->");
    scanf("%s", pass);
    if (strcmp(pass, list[id].password) == 0) {
        int counter = 0, checkNum;
        do { // Getting a password and checking its conditions.
            if (counter > 0)
                printf("This Password is not acceptable\n");

            printf("New password->");
            scanf("%s", list[id].password);
            counter++;

            //Checking the number of characters.
            checkNum = 0;
            int i;
            for (i = 0; i < PASS_MAX - 1; ++i) {
                if (isdigit(list[id].password[i]))
                    checkNum++;
            }
        } while (!(strlen(list[id].password) == 4
                   && checkNum == strlen(list[id].password)));

        saveToFile(list, numberOfCustomer);

    } else
        printf("The password you entered does not match your current password!\n");

    getchar();
    Sleep(SLEEP + 500);
}

/////////////////////////////////////////////////////////////////
int readingList(CUS *list) {
    FILE *read;
    int i;
    if ((read = fopen("CustomerInformation.txt", "r")) == NULL)
        puts("File could not be opened");
    else {
        for (i = 1; i <= LIST_MAX; ++i) {
            fscanf(read, "%d %s %s %s %s %s %s %f",
                   &list[i].row,
                   list[i].firstName,
                   list[i].lastName,
                   list[i].phoneNumber,
                   list[i].nationalCode,
                   list[i].idCard,
                   list[i].password,
                   &list[i].balance
            );
            if (list[i].row == 0) {
                break;
            }
        }
        fclose(read);
        return i - 1; // returns number of customer.
    }

}

/////////////////////////////////////////////////////////////////
void saveToFile(CUS list[], int size) {
    FILE *save;
    int i;
    if ((save = fopen("CustomerInformation.txt", "w")) == NULL)
        puts("File could not be opened");
    else {
        for (i = 1; i <= size; ++i) {

            fprintf(save, "%d %s %s %s %s %s %s %.2f\n",
                    list[i].row,
                    list[i].firstName,
                    list[i].lastName,
                    list[i].phoneNumber,
                    list[i].nationalCode,
                    list[i].idCard,
                    list[i].password,
                    list[i].balance
            );
        }
        fprintf(save, "%d", 0);
        fclose(save);
    }
}

/////////////////////////////////////////////////////////////////
void saveToCashFile(char mod[], float money, char accountId[], char targetId[]) {
    CASH listCash[LIST_MAX];
    readCashFile(listCash);
    CUS list[LIST_MAX];
    int numberOfCustomer = readingList(list);

    if (strcmp(mod, "Transfer") == 0) { //transfer section
        int id, target, i;
        for (i = 1; i <= numberOfCustomer; ++i) {
            if (strcmp(accountId, list[i].idCard) == 0) { // Origin ID
                id = i;
                break;
            }
        }

        int j;
        for (j = 1; j <= numberOfCustomer; ++j) {
            if (strcmp(targetId, list[j].idCard) == 0) { // destination ID
                target = j;
                break;
            }
        }

        //To move the sender's account transactions and add a new transaction
        for (j = listCash[id].number - 1; j >= 0; --j) {
            strcpy(listCash[id].list[j + 1].mod, listCash[id].list[j].mod);
            listCash[id].list[j + 1].money = listCash[id].list[j].money;
            strcpy(listCash[id].list[j + 1].id, listCash[id].list[j].id);
        }
        strcpy(listCash[id].list[0].mod, "Transfer");
        listCash[id].list[0].money = money;
        strcpy(listCash[id].list[0].id, targetId);

        //To move the transactions of the recipient's account and add a new transaction
        for (j = listCash[id].number - 1; j >= 0; --j) {
            strcpy(listCash[target].list[j + 1].mod, listCash[target].list[j].mod);
            listCash[target].list[j + 1].money = listCash[target].list[j].money;
            strcpy(listCash[target].list[j + 1].id, listCash[target].list[j].id);
        }
        strcpy(listCash[target].list[0].mod, "Deposit");
        listCash[target].list[0].money = money;
        strcpy(listCash[target].list[0].id, accountId);

        //To store transaction information
        FILE *save;
        if ((save = fopen("RecentAccountUpdates.txt", "w")) == NULL)
            puts("File could not be opened");
        else {
             int i;
            for (i = 1; i <= numberOfCustomer; ++i) {
                fprintf(save, "%d\n", listCash[i].row);
                int j;
                for (j = 0; j < listCash[i].number + 1; ++j) {
                    fprintf(save, "%s %.2f %s\n",
                            listCash[i].list[j].mod,
                            listCash[i].list[j].money,
                            listCash[i].list[j].id);

                    if (listCash[i].number + 1 > 10)
                        break;
                }
                if (id == i)
                    fprintf(save, "%c %.2f %c\n", 'e', 0.00, 'e');
            }
        }
        fclose(save);

    } else if (strcmp(mod, "Withdrawal") == 0) { //withdrawal section
        int id;
        int t;
        for (t = 1; t <= numberOfCustomer; ++t) { // Account ID
            if (strcmp(accountId, list[t].idCard) == 0) {
                id = t;
                break;
            }
        }

        //To move the current transaction information and add a new transaction
        int i;
        for (i = listCash[id].number - 1; i >= 0; --i) {
            strcpy(listCash[id].list[i + 1].mod, listCash[id].list[i].mod);
            listCash[id].list[i + 1].money = listCash[id].list[i].money;
            strcpy(listCash[id].list[i + 1].id, listCash[id].list[i].id);
        }
        strcpy(listCash[id].list[0].mod, "Withdrawal");
        listCash[id].list[0].money = money;
        strcpy(listCash[id].list[0].id, targetId);

        //To store transaction information
        FILE *save;
        if ((save = fopen("RecentAccountUpdates.txt", "w")) == NULL)
            puts("File could not be opened");
        else {
             int k;
             for (k = 1; k <= numberOfCustomer; ++k) {
                fprintf(save, "%d\n", listCash[k].row);
                
                int j;
                for (j = 0; j < listCash[k].number + 1; ++j) {
                    fprintf(save, "%s %.2f %s\n",
                            listCash[k].list[j].mod,
                            listCash[k].list[j].money,
                            listCash[k].list[j].id);

                    if (listCash[k].number + 1 > 10)
                        break;
                }
                if (k == id)
                    fprintf(save, "%c %.2f %c\n", 'e', 0.00, 'e');
            }
        }
        fclose(save);
    }
}

/////////////////////////////////////////////////////////////////
void readCashFile(CASH listCash[]) {
    CUS list[LIST_MAX];
    int numberOfCustomer = readingList(list);

    FILE *read;
    if ((read = fopen("RecentAccountUpdates.txt", "r")) == NULL)
        puts("File could not be opened");
    else {
         
         int i;
        for (i = 1; i <= numberOfCustomer; ++i) {
            fscanf(read, "%d", &listCash[i].row); // row user

            int j;
            for (j = 0; j < 10; ++j) { //transactions
                fscanf(read, "%s %f %s\n",
                       listCash[i].list[j].mod,
                       &listCash[i].list[j].money,
                       listCash[i].list[j].id);

                if (listCash[i].list[j].mod[0] == 'e') { // the end of user transaction
                    listCash[i].number = j;
                    break;
                }
            }
        }
        fclose(read);
    }
}

/////////////////////////////////////////////////////////////////
void endProgram(void) {
    system("cls");
    puts("");
    int i;
    for (i = 0; i < 24; ++i)
        printf("_*");
    puts("");
    printf("%40s", "....Thanks for your confidence....\n");
    for (i = 0; i < 24; ++i)
        printf("_*");
    puts("");
    getch();
}
