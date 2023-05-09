#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include <stdbool.h>

void ticketGenerator(int numOfTickets);
void sort(int whiteBall[5]); //sorting function (Insertion Sort)
void generatingDraw();
void lotteryResult(int numOfTickets);

//Global Variables to hold the winning numbers of the draw:
int goldBall_result;
int whiteBalls_result[5];
int megaplier_result;

int main(void)
{
    int numOfTickets = 0;
    bool validInput = false; //Variable used for error handling.

    printf("Welcome to Mega Millions Lottery!\n");
    printf("How many tickets would you like to generate?\n");
    while (validInput == false)
    {
        scanf("%d", &numOfTickets);
        if (numOfTickets >= 0)
        {
            validInput = true;
        }
        else
        {
            printf("Invalid input received! Please enter how many tickets would you like to generate?\n");
        }
    }

    printf("You have requested %d tickets.\n", numOfTickets);

    ticketGenerator(numOfTickets);
    generatingDraw();
    lotteryResult(numOfTickets);

    return 0;
}


void ticketGenerator(int numOfTickets)
{
    int whiteBall[5], goldBall, megaplier;
    int i, j;
    bool validNum;

    FILE *cfPtr;
    cfPtr = fopen("customerTickets.txt", "w");

    srand(time(NULL));

        j = 0;
        while(j < numOfTickets) //Loop to generate tickets:
        {
            fprintf(cfPtr, "%d, ", j+1);

            i=0;
            while(i<5) //Loop to generate white balls:
            {
                validNum = true;

                int x; //Variable to temporary store the white ball until it is checked that it is not repeated.
                x = (rand()%70) + 1;
                for (int k=(i-1); k!=-1; k--) //Loop to check for the repetition of white ball.
                {
                    if (x == whiteBall[k])
                    {
                        validNum = false;
                    }
                }

                if (validNum == true) //If no repetition is found then the value of x is stored in whiteBall[i]:
                {
                    whiteBall[i] = x;
                    i++;
                }

            }
            sort(whiteBall); //Function to sort the white balls.

            for (i=0; i<5; i++) //Loop to write white ball numbers in customerTickets.txt:
            {
                fprintf(cfPtr, "%d, ", whiteBall[i]);
            }

            goldBall = (rand()%25) + 1;
            fprintf(cfPtr, "%d, ", goldBall); //Writes a gold ball

            megaplier = rand() % 2;
            fprintf(cfPtr, "%d\n", megaplier); //Writes the megaplier


            j++; //One full ticket is written in the file, therefore j is incremented to write the next ticket through the loop.
        }
        fclose(cfPtr);
}


 void generatingDraw()
{
    int i, megaplierBall;
    bool validNum;


    i = 0;
    while(i<5) //Loop to pick winning numbers from white balls.
    {
        validNum = true;

        int x; //Variable to temporary store the white ball winning number until it is checked that it is not repeated.
        x = (rand()%70) + 1;
        for (int k=(i-1); k!=-1; k--)  //Loop to check for the repetition of white ball.
        {
            if (x == whiteBalls_result[k])
            {
                validNum = false;
            }
        }

        if (validNum == true) //If no repetition is found then the value of x is stored in whiteBalls_result[i]:
        {
            whiteBalls_result[i] = x;
            i++;
        }
    }

    sort(whiteBalls_result); //Function to sort the white balls.

    printf("Winning numbers: ");
    for (i=0; i<5; i++)
    {
        printf("%d ", whiteBalls_result[i]);
    }
    printf("| ");

    srand(time(NULL));
    goldBall_result = (rand()%25) + 1;
    printf("%d\n\n", goldBall_result);

    megaplierBall = (rand() % 16) + 1; //Generates a number between 1 and 15.

    if (megaplierBall <= 5) //The first 5 balls from 15, are marked with 2x.
    {
        megaplier_result = 2;
    }
    else if (megaplierBall <= 11) //The next 6 balls are marked with 3x.
    {
      megaplier_result = 3;
    }
    else if (megaplierBall <= 14) //The next 3 balls are marked with 4x.
    {
        megaplier_result = 4;
    }
    else {
        megaplier_result = 5; //And the final ball is marked with 5x.
    }

}

void lotteryResult(int numOfTickets) {
    int userTicket[8], match[2]; //match[0] stores the number of white matched balls for a ticket.
                                 // and match[1] stores 1 if the gold ball was also matched, else it stays 0.
    int price, sales, totalProfit, annuityPayment;
    int numOfMegapliersIssued = 0, numOfTickets_above400Price = 0;
    int i;
    double averageAmountWon, numOfWonTickets = 0, totalAmountWon = 0;
    //Counter arrays for multiples of amounts less than 400:
    int c_ten[5] = {0}, c_four[5] = {0}, c_two[5]={0};
    int c_twoHundred = 0;


    FILE * cfPtr;
    cfPtr = fopen("customerTickets.txt", "r");

    for (long int n=0; n<numOfTickets; n++)
    {

        fscanf(cfPtr, "%d, %d, %d, %d, %d, %d, %d, %d", &userTicket[0], &userTicket[1], &userTicket[2], &userTicket[3], &userTicket[4], &userTicket[5], &userTicket[6], &userTicket[7]);
        fseek(cfPtr, 1, SEEK_CUR); //Moves the cursor to the next line in the file.

        if (userTicket[7] == 1)
        {
            numOfMegapliersIssued++; //This variable is further used in the program to calculate sales.
        }

        match[0] = 0; //Variable stores number matched white balls.
        for (int k = 0; k < 5; k++) //Loop to check how many white balls of generated ticket matches the winning numbers.
        {
            int m = 1;
            while (m < 6)
            {
                if (whiteBalls_result[k] == userTicket[m])
                {
                    match[0]++;
                }
                if (whiteBalls_result[k] < userTicket[m])
                {
                    break; //Because the white balls in the tickets are sorted, so
                           //if (whiteBalls_result[k] < userTicket[m]), it means that whiteBalls_result[k] will not be
                           //found in userTicket[>m]. Therefore, the inner while loop breaks.
                }
                m++;
            }
        }

        //Checking if the gold-ball matches the winning gold bold:
        if (userTicket[6] == goldBall_result)
        {
            match[1] = 1; //Initialised to 1, if the gold-ball matches.
        }
        else
        {
            match[1] = 0;
        }


        price = 0;
        if (match[0] > 2 || match[1] == 1)  //Prints out prices:
        {
            numOfWonTickets++;

            if (match[0] == 5 && match[1] == 1)
            {
                price = 100000000;
            }
            else if (match[0] == 5)
            {
                price = 1000000;
            }
            else if (match[0] == 4 && match[1] == 1)
            {
                price = 10000;
            }
            else if (match[0] == 4)
            {
                price = 500;
            }
            else if (match[0] == 3 && match[1] == 1)
            {
                price = 200;
                if (userTicket[6] == 0)
                {
                    //If no megaplier is bought, then the count for c_twoHundred is incremented.
                    c_twoHundred++;
                }
            }
            else if (match[0] == 3 || match[0] == 2)
            {
                price = 10;
                if (userTicket[7] == 0)
                {
                    c_ten[0]++;  //If no megaplier is bought, then the count for c_ten[0]
                                 //which keeps count for amounts won of $10, is incremented by 1.
                }
                else
                {
                    c_ten[megaplier_result-1]++; //For example, if megaplier is 3, then the third multiple of 10 is the price amount.
                                                 //c_ten[2] will keep count for prices won of $30
                }
            }
            else if (match[0] == 1)
            {
                price = 4;
                if (userTicket[7] == 0)
                {
                    c_four[0]++;
                }
                else
                {
                    c_four[megaplier_result-1]++;
                }
            }
            else  //Only the gold ball matches in the user ticket:
            {
                price = 2;
                if (userTicket[7] == 0)
                {
                    c_two[0]++;
                }
                else
                {
                    c_two[megaplier_result-1]++;
                }
            }


            if (userTicket[7] == 1) //If megaplier is bought:
            {
                price = price * megaplier_result;
            }


            if (price >= 400)
            {
                numOfTickets_above400Price++;
                if (numOfTickets_above400Price == 1)
                {
                    printf("Winning Tickets:\n");
                }
                printf("Ticket Number %d\n", userTicket[0]);
                printf("Numbers: ");
                for (i=1; i<6; i++)
                {
                    printf("%d ", userTicket[i]);
                }
                printf("| %d\n", userTicket[6]);
                printf("Won: $%d\n\n", price);
            }

        }

        totalAmountWon += price;
    }

    printf("In addition, without a Megaplier:\n");
    printf("%d won $2\n", c_two[0]);
    printf("%d won $4\n", c_four[0]);
    printf("%d won $10\n", c_ten[0]);
    printf("%d won $200\n", c_twoHundred);
    printf("With a Megaplier:\n");

    int megaplierAmounts = 0; //A counter to count the number of additional prices won with a megaplier.
    for (i=1; i<5; i++) //Printing out amounts won in addition with a megaplier:
    {
        if (c_two[i] > 0)
        {
            megaplierAmounts++;
            printf("%d won $%d\n", c_two[i], 2+(2*i));
        }
    }
    for (i=1; i<5; i++)
    {
        if (c_four[i] > 0)
        {
            megaplierAmounts++;
            printf("%d won $%d\n", c_four[i], 4+(4*i));
        }
    }
    for (i=1; i<5; i++)
    {
        if (c_ten[i] > 0)
        {
            megaplierAmounts++;
            printf("%d won $%d\n", c_ten[i], 10+(10*i));
        }
    }

    if (megaplierAmounts == 0)
    {
        printf("No additional amounts were won with a Megaplier.\n");
    }


    sales = (2*numOfTickets) + numOfMegapliersIssued;
    if (numOfWonTickets > 0)  //To ensure that the denominator is not zero, to avoid division by zero.
    {
        averageAmountWon = totalAmountWon/numOfWonTickets;
    }
    else
    {
        averageAmountWon = 0;
    }

    totalProfit = sales - totalAmountWon;

    printf("\n\nThere were %.0lf winners in total!\n", numOfWonTickets);
    printf("The total sales were: $%d\n", sales);
    printf("The total payout was: $%.0lf\n", totalAmountWon);
    printf("The average Amount won was $%.2lf\n", averageAmountWon);
    printf("The total profit was: $%d\n\n", totalProfit);
    printf("If the Jackpot is won:\n");
    printf("Cash Option: $100000000\n\n");
    printf("Annuity Option: Initial Payment: $1500000\n");
    annuityPayment = 1500000; //for year 1
    for (i=2; i<=30; i++) //Loop to print the annuity payments for the following 29 years:
    {
        annuityPayment = ((annuityPayment/100)*5) + annuityPayment;
        printf("Year %d: payment is $%d\n", i, annuityPayment);
    }

    fclose(cfPtr);
}


void sort(int whiteBall[5])
{
    int a, b, swap;
    for (a = 1; a < 5; a ++)
    {
        b = a;
        while (b > 0 && whiteBall[b] < whiteBall[b - 1])
        {
            swap = whiteBall[b];
            whiteBall[b] = whiteBall[b-1];
            whiteBall[b-1] = swap;
            b--;
        }
    }
}




