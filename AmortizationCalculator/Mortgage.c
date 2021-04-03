//
//  Mortgage.c
//  AmortizationCalculator
//
//  Created by Stephen Choate.
//

#include <math.h>
#include <stdlib.h>
#include "Mortgage.h"

//Take in mortgage attributes from input, initialize and return pointer to Mortgage structure
struct Loan *getLoan(float homeVal, float loanAmt, float yTax, float yIns, float yMort, float iRate, float pmi, float hoa, float yTaxInc){
    struct Loan *loan;
    loan = (struct Loan*) malloc(sizeof(Loan));
    
    loan->homeValue = homeVal;
    loan->loanAmount = loanAmt;
    loan->pmiAmount = pmi;
    loan->hoaAmount = hoa;
    loan->interestRate = iRate;
    loan->yearlyTax = yTax;
    loan->yearlyInsurance = yIns;
    loan->mortgageTerm = yMort;
    loan->yearlyTaxInc = yTaxInc;
    
    return loan;
}

//Take in mortgage struct, calculated PTI, create linked list of payments, set up and return mortgage detail structure
struct Mortgage *getMortgage (struct Loan *loan, double pti){
    struct Mortgage *mort = (struct Mortgage*)malloc(sizeof(Mortgage));
    struct Payment *firstMo = malloc(sizeof(Payment));
    struct Payment *latestMo = malloc(sizeof(Payment));
    double totalPay = 0, totalInt = 0, totalTax = 0, totalFee = 0, totalPMI = 0;
    double remainBal = loan->loanAmount;
    double curTax = loan->yearlyTax;
    double fees = loan->hoaAmount;
    double pmi = loan->pmiAmount;
    double r = (loan->interestRate/12)/100;
    int months = (int)(loan->mortgageTerm*12);
    int cm = 1;
    int activePMI = (loan->pmiAmount > 0) ? 1 : 0;
    
    //Loop through months in mortgage term, calculate payments and decrement remaining balance
    for(cm=1; cm<=months; cm++){
        //Initialize structure for the current month, increment taxes by multiplier if the start of a new year.
        struct Payment *curMonth = (struct Payment*)malloc(sizeof(Payment));
        curTax = ((cm % 13) == 0) ? curTax*(1+(loan->yearlyTaxInc/100)) : curTax;
        
        //Determine if PMI can be dropped (LTV below 80%)
        activePMI = (remainBal/loan->homeValue < .8 && activePMI) ? 0 : activePMI;
        
        //Calculate payment attributes for the current month
        double i = (remainBal*r);
        double p = pti-i;
        double t = curTax/12;
        double m = i+p+t+fees;
        m += (activePMI) ? pmi : 0;
        
        remainBal -= p;
        curMonth->interest = i;
        curMonth->principal = p;
        curMonth->taxes = t;
        curMonth->fees = fees;
        curMonth->pmi = (activePMI) ? pmi : 0;
        curMonth->month = cm;
        curMonth->payment = m;
        curMonth->nextmo=NULL;
        curMonth->remaining = fabs(remainBal);
        
        totalPay+=m;
        totalInt+=i;
        totalTax+=t;
        totalFee+=fees;
        totalPMI+=pmi;
        
        //If first month run, set first month pointer to current month struct, otherwise, set latest month's nextmo pointer to the current month
        if(cm==1){
            firstMo=curMonth;
        }else{
            latestMo->nextmo = (cm<=months) ? curMonth : NULL;
        }
        //Update latestMo pointer to point at newest struct
        latestMo = curMonth;
    }
    
    //Set mortgage detail totals after running through all monthly payments
    mort->loan = loan;
    mort->totalPaid = totalPay;
    mort->totalInterest = totalInt;
    mort->totalTax = totalTax;
    mort->totalFee = totalFee;
    mort->totalPMI = totalPMI;
    mort->firstPmt = firstMo;
    
    return mort;
}
