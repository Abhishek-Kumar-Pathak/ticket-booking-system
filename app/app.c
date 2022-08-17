#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <conio.h>

#define MAX_TICKET 50
#define TICKET_START 400

// tickets data
static char **ticketsResult = NULL;
int *rowcols = NULL; //note start pos 0
int numofrow = 0;

// users data
static char **usersResult = NULL;
int totalUsersRows; //note start pos 0

// transport data
static char **TransResult = NULL;
int TransNumOfRow = 0;

// availability
char availability[15] = {0};
int is_booking_allowed = 1;

// mode
char SelectedMode[12] = {0};

// to store a user details
struct user
{
    int uid;
    long long phone;
    int pin;
    char name[30];
} userdata;

// to store a ticket data
struct ticket
{
    int id;
    int totalticket;
    char type[10];
    char status[15];
    int price;
    int typenum;
    long long phone;
    char name[30];
    short int age;
    char bdate[18];
    char jdate[15];

} ticketdata;

// to store a transport (mode) data
struct transport
{
    int num;
    char transportType[10];
    int price;
    char from[20];
    char to[20];
    char FromTime[15];
    char ToTime[15];
} transportdata;

// diagram function Declaration
void printDIAGRAM(int type);

/*
    this function will use tickets.csv file to retrieve all ticket data.
    Used Global vars:-
    rowcols - for count num of items per row.
    numofrow - to store total num of rows in the file.
    ticketsResult - all ticket data will be stored in it.
*/
int retriveTicketsData()
{

    FILE *fp = fopen("tickets.csv", "r");
    if (fp == NULL)
    {
        printf("unable to open file\n");
    }

    char buffer[sizeof(char) * 200];
    int totalcols = 0;
    numofrow = 0; //reset if called twice

    rowcols = (int *)malloc(MAX_TICKET * sizeof(int)); 
    if (rowcols == NULL)
        printf("unable to locate memory for rowcols\n");

    ticketsResult = (char **)malloc(sizeof(char *) * (totalcols + 5));
    if (ticketsResult == NULL)
        printf("unable to locate memory for ticketsResult\n");

    while (fgets(buffer, sizeof(buffer), fp))
    {

        size_t ln = strlen(buffer) - 1;
        buffer[strcspn(buffer, "\n")] = 0;
        if (*buffer && buffer[ln] == '\n')
        {
            printf("newline present, please remove\n");
            buffer[ln] = 0;
        }

        char *token = strtok(buffer, ",");
        numofrow++;
        int temp = 0;

        while (token != NULL)
        {
            rowcols[numofrow - 1] = temp;
            temp = temp + 1;

            ticketsResult[totalcols] = malloc(sizeof(char) * ((strlen(token) + 2)));
            strcpy(ticketsResult[totalcols], token);
            totalcols++;
            free(token);
            token = strtok(NULL, ",");

            if (token != NULL)
            {
                char **newticketsResult;
                newticketsResult = (char **)realloc(ticketsResult, sizeof(char *) * (totalcols + 2));
                // only if realloc was successful then use it.
                if (newticketsResult != NULL)
                {
                    ticketsResult = newticketsResult;
                }
                else
                {
                    printf("unable to locate memory for newticketsResult\n");
                }
            }
        }
    }
    fclose(fp);
    // printf("%d and %d\n", TotalConfirmed, TotalCancelled);
    return 1;
}


/*
    this function helps to retrieve a ticket data by ticket number and store in the ticketdata (ticket struct var)
    params - ticketNum, isShowData
    isShowData = 0, will not print ticket data
    isShowData = 1, to print data
*/
int retriveTicket(int ticketNum, int isShowData)
{
    int temp = 0;
    for (int i = 0; i < numofrow; i++)
    {
        int find = atoi(ticketsResult[temp]);

        if (ticketNum == find)
        {
            int col = 0;
            for (size_t a = 0; a <= rowcols[i]; a++)
            {
                int pos = temp + a; //matched pos + upcoming pos

                if (col == 0)
                    ticketdata.id = atoi(ticketsResult[pos]);
                if (col == 1)
                    ticketdata.totalticket = atoi(ticketsResult[pos]);
                if (col == 2)
                    strcpy(ticketdata.type, ticketsResult[pos]);
                if (col == 3)
                    strcpy(ticketdata.status, ticketsResult[pos]);
                if (col == 4)
                    ticketdata.price = atoi(ticketsResult[pos]);
                if (col == 5)
                    ticketdata.typenum = atoi(ticketsResult[pos]);
                if (col == 6)
                    ticketdata.phone = atoll(ticketsResult[pos]);
                if (col == 7)
                    strcpy(ticketdata.name, ticketsResult[pos]);
                if (col == 8)
                    ticketdata.age = atoi(ticketsResult[pos]);
                if (col == 9)
                    strcpy(ticketdata.bdate, ticketsResult[pos]);
                if (col == 10)
                    strcpy(ticketdata.jdate, ticketsResult[pos]);

                col++;
            }
            break;
        }
        //   temp : 0 then 9 then 18 , jump to next line
        temp += rowcols[i] + 1;
    }
    if (userdata.phone == ticketdata.phone)
    {
        if (isShowData == 1)
        {
            if(strcmp(ticketdata.type, "Bus") == 0)
                printDIAGRAM(1);
            else if(strcmp(ticketdata.type, "Train") == 0)
                printDIAGRAM(2);
            else
                printDIAGRAM(3);

            printf("\n========== Ticket ID: %d ==========\n", ticketdata.id);
            printf("\nTicket Type: %s Ticket\n", ticketdata.type);
            printf("Booked On: %s\n", ticketdata.bdate);
            printf("Journey Date: %s\n", ticketdata.jdate);
            printf("Booked Seat(s): %d\n", ticketdata.totalticket);
            printf("Ticket Status: ");
            if (strcmp(ticketdata.status, "confirmed") == 0)
            {
                printf("\033[0;32m");
            }
            else
            {
                printf("\033[0;31m");
            }
            printf("%s\n", ticketdata.status);
            printf("\033[0m");
            printf("%s No: %d\n", ticketdata.type, ticketdata.typenum);
            printf("Total Fare: %d\n", ticketdata.price);
            printf("\n========== Passanger Details ==========\n");
            printf("\nPassanger Name: %s\n", ticketdata.name);
            printf("Passanger Age: %hi\n", ticketdata.age);
            printf("Passanger Phone Number: %lld\n", ticketdata.phone);
            printf("\n");
            // getModeData(ticketdata.typenum);
        }
    }
    else
    {
        printf("You don't have acces to read this ticket");
    }
    return 1;
}

/*
    This function walks in the ticketsResult and find no of confirmed/WL tickets
    params: DateMonth (journey date), typeNum (bus/train/flight number)
*/

int Ticket_availability(char *DateMonth, int typeNum)
{
    // printf("\n%s %d\n", DateMonth, typeNum);
    int ConfirmedTickets = 0;
    int TotalWL = 0;

    int availSeats = 0;
    int pos = 10;

    for (int i = 0; i < numofrow; i++)
    {
        // printf("%d\n", strncmp(ticketsResult[pos], DateMonth, 6));
        if (strncmp(ticketsResult[pos], DateMonth, 6) == 0)
        {
            int getTypeNum = atoi(ticketsResult[pos - 5]); //Typenum
            // printf("%d %d\n", getTypeNum, typeNum);

            if (getTypeNum == typeNum)
            {
                //         //go to next col::status
                // printf("%s\n", ticketsResult[pos - 7]);
                if (strcmp(ticketsResult[pos - 7], "confirmed") == 0)
                {
                    ConfirmedTickets++;
                }
                else if (strncmp(ticketsResult[pos - 7], "WL", 2) == 0)
                { //for Trains

                    TotalWL++;
                }else{
                    continue;
                }
            }
        }
        //   pos :  jump to next line for TransportType
        pos += (rowcols[i] + 1);
    }

    // printf("%s  %d\n", SelectedMode, ConfirmedTickets);

    if (strcmp(SelectedMode, "Train") == 0)
    {
        availSeats = MAX_TICKET - ConfirmedTickets;
        if (availSeats <= 0)
        {
            //Current WL
            availSeats = availSeats + TotalWL;
            sprintf(availability, "%s %d", "WL", abs(availSeats) + 1);
            is_booking_allowed = 1;
        }
        else
        {
            snprintf(availability, sizeof(availability), "%d", availSeats);
        }
    }
    else
    {
        //for bus and flight only !
        availSeats = MAX_TICKET - ConfirmedTickets;
        if (availSeats <= 0)
        {
            snprintf(availability, sizeof(availability), "%s %d", "Not Avilable", 0);
            is_booking_allowed = 0;
        }
        else
        {
            snprintf(availability,sizeof(availability), "%d", availSeats);
        }
    }

    printf("\n\n========== %s No. %d Seat Availability Check For Date: %s ==========\n", SelectedMode, typeNum, DateMonth);
    printf("Current : %s\n\n\n", availability);
    // printf("%s  %d  %s\n", modeType, ConfirmedTickets, availability);
    return 1;
}

/*
    this function will use users.csv file to retrieve all users data.
    Used Global vars:-
    totalUsersRows - to store total num of rows in the file.
    usersResult - all users data will be stored in it.
*/
int retriveUsersData()
{

    FILE *fp = fopen("users.csv", "r");
    if (fp == NULL)
    {
        printf("unable to open file\n");
    }

    char buffer[sizeof(char) * 200];
    int totalcols = 0;
    totalUsersRows = 0; //reset

    usersResult = (char **)malloc(sizeof(char *) * (totalcols + 5));
    if (usersResult == NULL)
        printf("unable to locate memory for userResult ptr\n");

    char *token;
    while (fgets(buffer, sizeof(buffer), fp))
    {

        size_t ln = strlen(buffer) - 1;
        buffer[strcspn(buffer, "\n")] = 0;
        if (*buffer && buffer[ln] == '\n')
        {
            printf("newline present, please remove in retriveUsersData\n");
            buffer[ln] = 0;
        }

        token = strtok(buffer, ",");
        totalUsersRows++;

        while (token != NULL)
        {

            usersResult[totalcols] = malloc(sizeof(char) * ((strlen(token) + 2)));
            strcpy(usersResult[totalcols], token);
            totalcols++;

            token = strtok(NULL, ",");

            if (token != NULL)
            {
                char **newusersResult;
                newusersResult = (char **)realloc(usersResult, sizeof(char *) * (totalcols + 2));
                // only if realloc was successful then use it.
                if (newusersResult != NULL)
                {
                    usersResult = newusersResult;
                }
                else
                {
                    printf("unable to locate memory for newuserResult\n");
                }
            }
        }
    }
    fclose(fp);
    free(token);
    // printf("%d and %d\n", TotalConfirmed, TotalCancelled);
    return 1;
}


/*
    This function helps to retrieve a user data  and store in the userdata (user struct var)
    params: phone (user phone number), pin(User secret pin), isShow(0 = do not show data, 1 = show user data)
    return: 0 = no user found, 1= user found, 2 = user pin does not matched
*/
int getUser(long long phone, int pin, int isshow)
{
    int temp = 0;
    int returnVal = 0;
    for (size_t i = 0; i < totalUsersRows; i++)
    {
        long long getPhone = atoll(usersResult[temp]);
        int getPin = atoi(usersResult[temp + 1]);
        if (phone == getPhone)
        {
            returnVal == 2;
            if (pin == getPin)
            {

                returnVal = 1;
                userdata.uid = i;
                int col = 0;
                for (size_t j = 0; j < 3; j++)
                {
                    int pos = temp + j;

                    if (col == 0)
                        userdata.phone = atoll(usersResult[pos]);
                    if (col == 1)
                        userdata.pin = atoi(usersResult[pos]);
                    if (col == 2)
                        strcpy(userdata.name, usersResult[pos]);

                    col++;
                }
                break;
            }
        }
        temp += 3;
    }

    if (isshow == 1)
    {
        printf("\n\n========== Account Details ==========\n");
        printf("\nUser Name: %s\n", userdata.name);
        printf("Secret Pin: %hi\n", userdata.pin);
        printf("Phone Number: %lld\n", userdata.phone);
        printf("\n\n");
    }

    return returnVal;
}

/*
    this function helps to retrive and print all tickets of a user by using ticketsResult global var.
*/

int getAllTicketsofUser()
{

    int pos = 6;
    printf("\n\n======================================================== BOOKING HISTORY ========================================================\n\n");
    printf("\n\nTicket ID          Type          Status             Fare         Type Number            Booked On                Journey Date\n\n");

    for (int i = 0; i < numofrow; i++)
    {
        long long getphone = atoll(ticketsResult[pos]);

        if (getphone == userdata.phone)
        {
            // just for design
            // bus
            if (strlen(ticketsResult[pos - 4]) == 3)
            {
                printf("%d                %s           %s          %d          %d                  %s           %s\n", atoi(ticketsResult[pos - 6]), ticketsResult[pos - 4], ticketsResult[pos - 3], atoi(ticketsResult[pos - 2]), atoi(ticketsResult[pos - 1]), ticketsResult[pos + 3], ticketsResult[pos + 4]);
            } //train
            else if (strlen(ticketsResult[pos - 4]) == 5)
            {
                printf("%d                %s         %s            %d          %d                  %s           %s\n", atoi(ticketsResult[pos - 6]), ticketsResult[pos - 4], ticketsResult[pos - 3], atoi(ticketsResult[pos - 2]), atoi(ticketsResult[pos - 1]), ticketsResult[pos + 3], ticketsResult[pos + 4]);
            } //flight
            else if (strlen(ticketsResult[pos - 4]) == 6)
            {
                printf("%d                %s        %s            %d          %d                  %s           %s\n", atoi(ticketsResult[pos - 6]), ticketsResult[pos - 4], ticketsResult[pos - 3], atoi(ticketsResult[pos - 2]), atoi(ticketsResult[pos - 1]), ticketsResult[pos + 3], ticketsResult[pos + 4]);
            }
        }
        //   pos : 6 then 17 then 28 , jump to next line for phone
        pos += (rowcols[i] + 1);
    }
    printf("\n\n");
    return 1;
}

/*
    this function will use transports.csv file to retrieve all Bus/Train/Flight data.
    Used Global vars:-
    TransNumOfRow - to store total num of rows in the file.
    TransResult - all transport data will be stored in it.
*/

int retriveTransportData()
{

    FILE *fp = fopen("transports.csv", "r");
    if (fp == NULL)
    {
        printf("unable to open file\n");
    }

    char buffer[sizeof(char) * 200];
    int totalcols = 0;
    TransNumOfRow = 0; //reset

    TransResult = (char **)malloc(sizeof(char *) * (totalcols + 5));
    if (TransResult == NULL)
        printf("unable to locate memory for userResult ptr\n");

    while (fgets(buffer, sizeof(buffer), fp))
    {

        size_t ln = strlen(buffer) - 1;
        buffer[strcspn(buffer, "\n")] = 0;
        if (*buffer && buffer[ln] == '\n')
        {
            printf("newline present, please remove in retriveTransportData\n");
            buffer[ln] = 0;
        }

        char *token = strtok(buffer, ",");
        TransNumOfRow++;

        while (token != NULL)
        {

            TransResult[totalcols] = malloc(sizeof(char) * ((strlen(token) + 2)));
            strcpy(TransResult[totalcols], token);
            totalcols++;
            free(token);
            token = strtok(NULL, ",");

            if (token != NULL)
            {
                char **newTransResult;
                newTransResult = (char **)realloc(TransResult, sizeof(char *) * (totalcols + 2));
                // only if realloc was successful then use it.
                if (newTransResult != NULL)
                {
                    TransResult = newTransResult;
                }
                else
                {
                    printf("unable to locate memory for newTransResult\n");
                }
            }
        }
    }
    // printf("%d and %d\n", TotalConfirmed, TotalCancelled);
    fclose(fp);
    return 1;
}

/*
    This function will retrieve Bus/Train/Flight by using number
    params: modeNum (number)
    return: 0 = not found, 1 = found
*/

int getModeData(int modeNum)
{
    // printf("%d\n", modeNum);
    int pos = 0;
    int isfound = 0;
    for (int i = 0; i < TransNumOfRow; i++)
    {
        isfound++;
        // printf("%d\n", i);
        int getTransNum = atoi(TransResult[pos]);
        // printf("%d\n", getTransNum);
        if (getTransNum == modeNum)
        {

            transportdata.num = atoi(TransResult[(pos + 0)]);

            strcpy(transportdata.transportType, TransResult[(pos + 1)]);

            transportdata.price = atoi(TransResult[(pos + 2)]);

            strcpy(transportdata.from, TransResult[(pos + 3)]);

            strcpy(transportdata.to, TransResult[(pos + 4)]);

            strcpy(transportdata.FromTime, TransResult[(pos + 5)]);

            strcpy(transportdata.ToTime, TransResult[(pos + 6)]);

            // break;
        }
        pos += 7;
    }
    if (isfound > 0)
    {
        printf("\n========== %s Time Table ==========\n", transportdata.transportType);
        printf("\n%s Number: %d\n", transportdata.transportType, transportdata.num);
        printf("From: %s at %s\n", transportdata.from, transportdata.FromTime);
        printf("To: %s at %s\n", transportdata.to, transportdata.ToTime);
        printf("Runs: Daily\n");
        printf("\n");
        return 1;
    }
    else
    {
        return 0;
    }
}

/*
    This function will show all transport data between two location 
    params: from (origin), to (destination)
    return: 0 = not found, 1 = found
*/
int getListOfMode(char *from, char *to)
{

    printf("\n\n%s No.          From (Time)                    To (Time)                    Fare\n\n", SelectedMode);
    int pos = 1;
    int isFound = 0;
    for (int i = 0; i < TransNumOfRow; i++)
    {
        // printf("%s %s %s\n",TransResult[pos], TransResult[(pos + 4)], TransResult[(pos + 5)]); debug
        if (stricmp(SelectedMode, TransResult[pos]) == 0 && stricmp(from, TransResult[(pos + 2)]) == 0 && stricmp(to, TransResult[(pos + 3)]) == 0)
        {
            isFound++;
            printf("%d              %s (%s)               %s (%s)             %d\n", atoi(TransResult[pos - 1]), TransResult[(pos + 2)], TransResult[pos + 4], TransResult[pos + 3], TransResult[pos + 5], atoi(TransResult[pos + 1]));
        }
        //   pos : 1 then 7 then 15 , jump to next line for trans Type
        pos += 7;
    }

    printf("\n\n");
    return (isFound > 0);
}


/*
    This function helps to add data in the csv file
    params: type (0 for add ticket data, 1 = for add user data)
    return: 1 
*/

int addData(int type)
{
    // type = 1 for add new user
    // other for aad ticket
    FILE *fp;
    int res;
    if (type == 1)
    {
        fp = fopen("users.csv", "a");
        fprintf(fp, "\n%lld,%d,%s", userdata.phone, userdata.pin, userdata.name);
        fclose(fp);

        if (retriveUsersData() == 1)
        {
            return 1;
        }
    }
    else
    {
        fp = fopen("tickets.csv", "a");
        fprintf(fp, "\n%d,%d,%s,%s,%d,%d,%lld,%s,%hi,%s,%s", ticketdata.id, ticketdata.totalticket, ticketdata.type, ticketdata.status, ticketdata.price, ticketdata.typenum, ticketdata.phone, ticketdata.name, ticketdata.age, ticketdata.bdate, ticketdata.jdate);
        // fprintf(fp, "%ld,%ld,%lld,%s,%hi,%s", ticketdata.price, ticketdata.typenum, ticketdata.phone, ticketdata.name, ticketdata.age);
        fclose(fp);
    
        if (retriveTicketsData() == 1)
        {
            return 1;
        }
        // update numofrow and ticketresult
    }
}



/*
    This function helps to update csv file data
    params: type (0 for update user data, 1 for ticket data)
    return: 1
*/
int updateData(int type){
    char buffer[256];
    char *restStr;
    FILE *tempFP = fopen("temp.csv", "w");

    if(type == 1){
        FILE *ticketFP = fopen("tickets.csv", "r");

        while (fgets(buffer, 254, ticketFP) != NULL)
        {

            char *tok = strtok_r(buffer, ",", &restStr);
            if(atoi(tok) == ticketdata.id){
                fprintf(tempFP, "%d,%d,%s,%s,%d,%d,%lld,%s,%hi,%s,%s", ticketdata.id, ticketdata.totalticket, ticketdata.type, ticketdata.status, ticketdata.price, ticketdata.typenum, ticketdata.phone, ticketdata.name, ticketdata.age, ticketdata.bdate, ticketdata.jdate);

            }else{
                fprintf(tempFP,"%s,%s",tok,restStr);
            }
        }
        fclose(ticketFP);
        fclose(tempFP);
        remove("tickets.csv");
        rename("temp.csv", "tickets.csv");

        // if(strcmp(ticketdata.type, "Train") == 0){
        //     // update_Other_Train_tickets();
        // }else{
        //     rename("temp.csv", "tickets.csv");
        // }
        return 1;
    }
    else if (type == 0)
    {
        FILE *UserFP = fopen("users.csv", "r");

        int countRow = 0;
        while (fgets(buffer, 254, UserFP) != NULL)
        {
            if(countRow == userdata.uid){
                fprintf(tempFP, "%lld,%d,%s", userdata.phone, userdata.pin, userdata.name);

            }else{
                fprintf(tempFP,"%s",buffer);
            }
            countRow ++;
        }
        fclose(UserFP);
        fclose(tempFP);
        unlink("users.csv");
        rename("temp.csv", "users.csv");
        return 1;
    }

}

/*
    This function helps to get current date and month and time or verify date/month, month name by using month number
    params: VerifyDay, VerifyMonth
    uses: getAndVerifyTime(0, 0) // current date month and time
          getAndVerifyTime(12, 9) = 12 Sep
    return: use timedata global array
*/
char *getAndVerifyTime(int VerifyDay, int VerifyMonth)
{

    // 24H Clock
    // printf("now %d %d %d and %dH : %dm\n", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900, tm.tm_hour, tm.tm_min);
    // printf("%d:%d %s", hour, tm.tm_min, amOrpm);

    // get month name by monthnumber also the the num of days

    int monNum = 0;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int hour = tm.tm_hour;
    char amOrpm[10];
    if (VerifyDay == 0 && VerifyMonth == 0)
    {

        //convert to 12h
        //hour
        if (tm.tm_hour > 12)
        {
            hour = tm.tm_hour - 12;
        }

        // am or pm ?
        if (tm.tm_hour >= 0 && tm.tm_hour < 12)
            strcpy(amOrpm, "AM");

        if (tm.tm_hour >= 12 && tm.tm_hour <= 23)
            strcpy(amOrpm, "PM");

        monNum = tm.tm_mon + 1;
    }
    else
    {
        monNum = VerifyMonth;
    }
    // monNum =
    // scanf("%d/%d", &day, &monNum);

    char month[15] = {0};
    int monthDays = 0;

    switch (monNum)
    {
    case 1:
        snprintf(month, sizeof(month), "%s", "Jan");
        monthDays = 31;
        break;
    case 2:
        snprintf(month, sizeof(month), "%s", "Feb");
        monthDays = 28;
        break;
    case 3:
        snprintf(month, sizeof(month), "%s", "Mar");
        monthDays = 31;
        break;
    case 4:
        snprintf(month, sizeof(month), "%s", "Apr");
        monthDays = 30;
        break;
    case 5:
        snprintf(month, sizeof(month), "%s", "May");
        monthDays = 31;
        break;
    case 6:
        snprintf(month, sizeof(month), "%s", "Jun");
        monthDays = 30;
        break;
    case 7:
        snprintf(month, sizeof(month), "%s", "Jul");
        monthDays = 31;
        break;
    case 8:
        snprintf(month, sizeof(month), "%s", "Aug");
        monthDays = 31;
        break;
    case 9:
        snprintf(month, sizeof(month), "%s", "Sep");
        monthDays = 30;
        break;
    case 10:
        snprintf(month, sizeof(month), "%s", "Oct");
        monthDays = 31;
        break;
    case 11:
        snprintf(month, sizeof(month), "%s", "Nov");
        monthDays = 30;
        break;
    case 12:
        snprintf(month, sizeof(month), "%s", "Dev");
        monthDays = 31;
        break;

    default:
        break;
    }


    static char timeData[25] = {0}; 
    if (VerifyDay == 0 && VerifyMonth == 0)
    {
        snprintf(timeData, sizeof(timeData), "%d %s %d:%d %s", tm.tm_mday, month, hour, tm.tm_min, amOrpm);
        // printf("%d", hour);
    }
    else if (VerifyDay != 0 && VerifyDay <= monthDays && month[0] != 0 && VerifyMonth >= (tm.tm_mon + 1))
    {
        if(VerifyMonth == (tm.tm_mon + 1) ){
            if( VerifyDay >= tm.tm_mday ){
                snprintf(timeData, sizeof(timeData), "%d %s", VerifyDay, month);
            }else{
                //oh! , entered past day, do nothing :)
            }

        }else{
            snprintf(timeData, sizeof(timeData), "%d %s", VerifyDay, month);
        }
    }
    return timeData;
    
}





/* 
    this function helps to process ticket booking by using user inputs
    return : 1 = successfully booked, 2 = error
    required : make sure SelectedMode have mode type value
*/

int bookTicket()
{
    char Inputfrom[50];
    char Inputto[50];
    int Inputmodenum = 0;
    int InputDay = 0, InputMonth = 0;

ReDate:
    printf("Enter Date and month (Eg: 19/10): ");
    scanf("%d/%d", &InputDay, &InputMonth);
    strcpy(ticketdata.jdate, getAndVerifyTime(InputDay, InputMonth));
    if (ticketdata.jdate[0] == 0)
    {
        printf("Incorrect Date and month\n");
        goto ReDate;
    }

    printf("From: ");
    scanf("%s", Inputfrom);
    printf("To: ");
    scanf("%s", Inputto);
    retriveTransportData();

    if (!getListOfMode(Inputfrom, Inputto))
    {
        printf("No %s availble from %s to %s\n", SelectedMode, Inputfrom, Inputto);
        return 0;
    }
    else
    {
    ReStatus:
        printf("Enter Number to check Status: ");
        scanf("%d", &Inputmodenum);
        getModeData(Inputmodenum);
        // printf("%s\n", timeData);
        Ticket_availability(ticketdata.jdate, Inputmodenum);
        if (is_booking_allowed == 0)
        {
            printf("No %s Seat Available for date: %s\n", SelectedMode, ticketdata.jdate);
            printf("Please Check for another one\n");
            goto ReStatus;
        }
        printf("\n\n============ Passanger DETAILS ==============\n\n");
        printf("Enter Your Name: ");
        scanf("%s", ticketdata.name);
        printf("Enter Your Age: ");
        scanf("%hi", &ticketdata.age);

        printf("\nPlease Wait....\n");

        ticketdata.id = TICKET_START + numofrow;
        ticketdata.totalticket = 1;
        strcpy(ticketdata.type, transportdata.transportType);
        if (isdigit(availability[0]) != 0)
        { // check it empty or it is wl ?addData
            strcpy(ticketdata.status, "confirmed");
        }
        else
        {
            strcpy(ticketdata.status, availability);
        }
        
        ticketdata.price = transportdata.price;
        ticketdata.typenum = transportdata.num;
        ticketdata.phone = userdata.phone;
        strcpy(ticketdata.bdate, getAndVerifyTime(0, 0));
        if (addData(0) == 1)
        {
            printf("\nBooked Successfully. Ticket ID: %d\n", ticketdata.id);
            return 1;
        }
        else
        {
            printf("\nUnable to process\n");
            return 2;
        }
    }
}


// free memory on exit
void Free_Memory()
{

// user data
    for (size_t i = 0; i < totalUsersRows; i++)
    {
        free(usersResult[i]);
    }
    free(usersResult);
    totalUsersRows = 0;

// ticket data
    for (size_t j = 0; j < numofrow; j++)
    {
        free(ticketsResult[j]);
    }
    free(ticketsResult);
    free(rowcols);
    numofrow = 0;

// transport Data
    for (size_t k = 0; k < TransNumOfRow; k++)
    {
        free(TransResult[k]);
    }
    free(TransResult);
    TransNumOfRow = 0;

}




int main()
{

    // retriveTicketsData();
    // retriveUsersData();
    // retriveTransportData();
    // strcpy(SelectedMode, "Bus");
    // getModeData(49876);
    // Ticket_availability("7 Oct", 49876);
    // printf("%d\n", numofrow);
    // printDIAGRAM(0);

    int userinput;
    int getUserResponse;


    //welcome
    printf("\nDe-Code Travel Agency\nBook your Bus/Train/Flight Tickets Easily\n\n");
login:
    printf("Hi Guest,\n1. Create Account\n2. Login to your Account\n\nEnter your option: ");
    scanf("%d", &userinput);

    if (userinput == 1)
    {
        printf("\nPlease enter Your First Name: ");
        scanf("%s", userdata.name);
        printf("Please enter Your Phone Number (+91): ");
        scanf("%lld", &userdata.phone);
        printf("Create a secret pin: ");
        scanf("%d", &userdata.pin);
        printf("Creating Account, Please wait ...");
        addData(1); // it will also update users.csv by calling retriveUsersData();
        getUserResponse = getUser(userdata.phone, userdata.pin, 1);
        switch (getUserResponse)
        {
        case 0:
            printf("\nAccount not Created, Please Try again.\n");
            goto login;
        case 1:
            printf("Account Created Successfully\n");
            goto afterLogin;
            break;
        default:
            goto login;
            break;
        }
    }
    else if (userinput == 2)
    {
        printf("\nEnter Your Registerd Phone Number (+91): ");
        scanf("%lld", &userdata.phone);
    REPIN:
        printf("Please enter secret pin: ");
        scanf("%d", &userdata.pin);
        retriveUsersData();
        printf("\nPlease wait ...\n");
        getUserResponse = getUser(userdata.phone, userdata.pin, 0);
        switch (getUserResponse)
        {
        case 0:
            printf("\nIncorrect Phone number or User does not exist, Try to create a new account.\n");
            goto login;
        case 1:
            printf("Login Successfully\n");
            goto afterLogin;
            break;
        case 2:
            printf("\nIncorrect Pin, Try again\n");
            goto REPIN;
            break;
        default:
            goto login;
        }
    }
    else{
        printf("\nWrong input\n\n");
        goto login;
    }



afterLogin:

    printf("\nWelcome %s\n\n(Service available b/w Mumbai-Patna, add more in transports.csv)\n\n1.Book Bus Ticket\n2.Book Train Ticket\n3.Book Flight Ticket\n4.Check Ticket status\n5.Cancel Ticket\n6.Booking History\n\nAccount\n7.View Account Details\n8.Update Details\n\nEnter your option: ", userdata.name);
    scanf("%d", &userinput);
    retriveTicketsData();
    switch (userinput)
    {
    case 1:
        strcpy(SelectedMode, "Bus");
        printDIAGRAM(1);
        bookTicket();
        break;
    case 2:
        strcpy(SelectedMode, "Train");
        printDIAGRAM(2);
        bookTicket();
        break;
    case 3:
        strcpy(SelectedMode, "Flight");
        printDIAGRAM(3);
        bookTicket();
        break;
    case 4:
        printf("\n\nPlease Enter Your Ticket ID: ");
        scanf("%d", &userinput);
        retriveTicket(userinput, 1);
        getModeData(ticketdata.typenum);
        break;
    case 5:
        printf("\n\nPlease Enter Your Ticket ID: ");
        scanf("%d", &userinput);
        retriveTicket(userinput, 0);
        strcpy(ticketdata.status, "cancelled");
        ticketdata.price = 0;
        if(updateData(1) == 1){
            printf("Successfully Cancelled");
        }else{
            printf("Something went wrong");
        }
        break;
    case 6:
        printf("\nPlease Wait ...\n");
        getAllTicketsofUser();
        break;
    case 7:
        printf("\n\n============ YOUR ACCOUNT DETAILS ==============\n\n");
        printf("Name: %s\nPhone Number: %lld\nSecret PIN: %d\n", userdata.name, userdata.phone, userdata.pin);
        break;
    case 8:
        printf("\n\n============ YOUR ACCOUNT DETAILS ==============\n\n");
        printf("Name: %s\nPhone Number: %lld\nSecret PIN: %d\n", userdata.name, userdata.phone, userdata.pin);
        printf("\n\n============ UPDATE  ACCOUNT DETAILS ==============\n\n");
        printf("\nPlease Enter NEW PIN: ");
        scanf("%d", &userinput);
        userdata.pin = userinput;
        printf("\nPlease Enter NAME: ");
        scanf("%s", userdata.name);
        if(updateData(0) == 1){
            printf("Successfully Updated name and secret pin.");
        }else{
            printf("Something went wrong");
        }
        break;

    default:
        printf("you have entered wrong option.\nPlease Try again\n");
        goto afterLogin;
        break;
    }

lastMenu:
    printf("\n\n1.Go Back\n2.Exit\nEnter your option: ");
    scanf("%d", &userinput);
    printf("\n\n");
    if (userinput == 1)
    {
        goto afterLogin;
    }
    else if (userinput == 2)
    {
        Free_Memory();
        exit(EXIT_SUCCESS);
    }
    else
    {
        printf("\nyou have entered wrong option.\nPlease Try again");
        goto lastMenu;
    }
    // getch();
    return 0;
}


/*
    diagram function
    params: type (0 for print all, 1 for bus, 2 for train and pass 3 for fligt)
*/
void printDIAGRAM(int type){
            // bus
    printf("\n\n");


    if(type == 0 || type == 1){
    printf("----------------------------------\n");
    printf("|                                |\n");
    printf("|                                |\n");
    printf("|       === BUS TICKET ===       \\ \n");
    printf("|                                 \\-----\n");
    printf("|                                       |\n");
    printf("----------------------------------------\n");
    printf("      O                       O\n");

    printf("\n\n");
    }
     if(type == 0 || type == 2){




    printf("---------------------  ---------------------   ---------------------  -----------------------------------\n");
    printf("|                   |  |                   |  |                    |  |                                 |\n");
    printf("|                   |  |                   |  |                    |  |                                 |\n");
    printf("|                   |  |   === TRAIN       |  |      TICKET ===    |  |                                 \\ \n");
    printf("|                   |__|                   |__|                    |__|                                  \\-----\n");
    printf("|                   |  |                   |  |                    |  |                                        |\n");
    printf("---------------------  ---------------------   ---------------------  ------------------------------------------\n");
    printf("    O          O           O          O           O           O               O                       O\n");

    printf("\n\n");
    }
    if(type == 0 || type == 3){


    printf("    _________\n");
    printf("   |_________|                               ___\n");
    printf("         |   \\                              |   \\\n");
    printf("         |    \\                             |    \\\n");
    printf("         |     \\                            |     \\\n");
    printf("         |      \\                           |      \\\n");
    printf("         |       \\                          |       \\\n");
    printf("         |        \\                         |        \\\n");
    printf("      ---------------------------------------------------------------------------------------\\\n");
    printf("      \\                                                                                       \\\n");
    printf("       \\                                                                                       \\\n");
    printf("       (                                === FLIGHT TICKET ===                                   )\n");
    printf("       /                                                                                       /\n");
    printf("      /                                                                                       /\n");
    printf("      ---------------------------------------------------------------------------------------/\n");
    printf("                                            |        /\n");
    printf("                                            |       /\n");
    printf("                                            |      /\n");
    printf("                                            |     /\n");
    printf("                                            |    /\n");
    printf("                                            |___/\n");

    printf("\n\n");
    }
}