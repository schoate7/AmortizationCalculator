//
//  main.c
//  AmortizationCalculator - A basic mortgage amortization calculator.
//
//  Created by Stephen Choate.
//

#include <stdlib.h>
#include <malloc/_malloc.h>
#include <tgmath.h>
#include <ctype.h>
#include "Mortgage.h"

#define HOME_VAL "Home value: $"
#define LOAN_AMT "Loan amount: $"
#define TAXES_Y "Yearly property tax: $"
#define INSURANCE_Y "Yearly insurance: $"
#define HOA_EXIST "Does the house have an HOA (y/n): "
#define HOA_AMT "What is the monthly HOA fee?: $"
#define MORT_Y "Mortgage Term (years): "
#define INTEREST_R "Interest rate (%): "
#define PMI_AMT "LTV ratio above 80%% detected, PMI may be required!\nPlease monthly enter PMI (0 if none): $"
#define TAX_INC_Y "Annual property tax increase (%): "
#define AMRT_SCHED "\nDo you wish to print the amorization schedule (y/n)?: "
#define RUN_AGAIN "Do you wish to run again (y/n)?: "

//Generic function to get a character value, take in char array for user prompt text.
char getChar(char stringIn[]){
    char cIn = ' ';
    char *inp = (char*)malloc(64);
    printf("%s", stringIn);
    scanf(" ");
    fgets(inp, 64, stdin);
    if(inp!=NULL){
        cIn = toupper(inp[0]);
    }
    return cIn;
}

//Generic function to get a float value, take in char array for user prompt text.
float getFloat(char stringIn[]){
    float flIn;
    char *inp = (char*)malloc(64);
    printf("%s",stringIn);
    scanf(" ");
    fgets(inp, 64, stdin);
    flIn = atof(inp);
    free(inp);
    return flIn;
}

//Free memory taken by last run's mort struct & linked list of monthly payments.
void clearLastRun(struct Mortgage *mort){
    struct Payment *cm = mort->firstPmt;
    struct Payment *lm = 0;
    free(mort->loan);
    
    //Clear all payments in linked list, reserve pointer to be freed, increment current month pointer, free latest month
    while(cm!=NULL){
        lm = cm;
        cm = (cm->nextmo != NULL) ? cm->nextmo : NULL;
        free(lm);
    }
    free(mort); //Free the mortgage struct once payment linked list freed
}

//Print mortgage details from struct, if user selects, print amortization schedule (detect PMI drop), call fn to clear memory.
void printMortgage(struct Mortgage *mort){
    char printSched=' ';
    struct Payment *cm = mort->firstPmt;
    int pmiActive = (cm->pmi > 0) ? 1 : 0;
    
    //Print output details from MortDetails struct
    printf("\n\nMortgage Cost Totals:\n");
    printf("Total Paid: $%'.2f\n", mort->totalPaid);
    printf("Total Interest: $%'.2f\n", mort->totalInterest);
    printf("Total Tax: $%'.2f\n", mort->totalTax);
    printf("Total Fees: $%'.2f\n", mort->totalFee);
    printf("Total PMI: $%'.2f\n", mort->totalPMI);
    
    do{
        printSched = getChar(AMRT_SCHED);
    }while(printSched!='Y' && printSched!='N');
    
    //Loop through linked list of monthly payments to print full amortization schedule
    while(printSched=='Y' && cm!=NULL){
        printf("Month: %i - Principal Paid: $%.2f, Interest Paid: $%.2f, Remaining Balance: $%.2f\n", (int)cm->month, cm->principal, cm->interest, cm->remaining);
        if(pmiActive){
            pmiActive = (cm->pmi == cm->nextmo->pmi);
            (cm->pmi > cm->nextmo->pmi) ? printf("**---PMI Dropped!---**\n") : 0;
        }
        cm = (cm->nextmo!=NULL) ? cm->nextmo : NULL;
    }
    clearLastRun(mort); //Call function to clear memory allocated to mortgage objects and linked list
}

//Calculate monthly principal and interest payment based on mortgage attributes.
double getPayment(struct Loan *loan){
    double a = loan->loanAmount;
    double r = ((loan->interestRate)/12)/100;
    double n = (loan->mortgageTerm)*12;
    return a/((pow((1+r),n)-1)/(r*(pow((1+r),n))));
}

//Prompt user for HOA, return value if yes, 0 if no.
float getHOA(){
    float hoaIn = 0;
    char isHOA=' ';
    
    while(isHOA == ' '){
        isHOA = getChar(HOA_EXIST);
        isHOA = (isHOA == 'Y' || isHOA == 'N') ? isHOA : ' ';
    }
    if (isHOA == 'Y'){
        hoaIn = getFloat(HOA_AMT);
    }
    return hoaIn;
}

//Main menu, prompt for details, call funcs to get mortgage pmt and make linked list, print and prompt for multiple runs.
int mainMenu(){
    float homeVal, loanAmt, pmi, hoa, iRate, yTax, yIns, yMort, yTaxInc;
    double mPmt;
    char runAgain;
    struct Loan *loan;
    struct Mortgage *md;
    homeVal = getFloat(HOME_VAL);
    loanAmt = getFloat(LOAN_AMT);
    yTax = getFloat(TAXES_Y);
    yIns = getFloat(INSURANCE_Y);
    hoa = getHOA();
    yMort = getFloat(MORT_Y);
    iRate = getFloat(INTEREST_R);
    pmi = ((int)loanAmt/(int)homeVal >.8) ? getFloat(PMI_AMT) : 0;
    yTaxInc = getFloat(TAX_INC_Y);
    
    loan = getLoan(homeVal, loanAmt, yTax, yIns, yMort, iRate, pmi, hoa, yTaxInc);
    mPmt = getPayment(loan);
    md = getMortgage(loan, mPmt);
    printMortgage(md);
    
    do{
        runAgain = getChar(RUN_AGAIN);
    }while (runAgain!='Y' && runAgain!='N');
    return (runAgain=='Y') ? 1 : 0;
}

//Main function, print heading, loop on menu until quit code.
int main(int argc, const char * argv[]) {
    int runAgain;
    printf("Welcome to the mortgage amortization calculator!\n");
    do{
        runAgain=mainMenu();
    }while(runAgain);
    return 0;
}
