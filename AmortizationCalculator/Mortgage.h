//
//  Mortgage.h - Defines structs for mortgage, monthly payment, and mortgage result details.
//  AmortizationCalculator
//
//  Created by Stephen Choate.
//

#ifndef Mortgage_h
#define Mortgage_h

#include <stdio.h>

/* Loan - Struct for loan, holds the user input attributes to be shared across multiple processing functions */
extern struct Loan{
    float homeValue, loanAmount, pmiAmount, hoaAmount, interestRate, yearlyTax, yearlyInsurance, mortgageTerm, yearlyTaxInc;
}Loan;

/* Payment - Struct for a month's payment, holds the details of payment, remaining balance, and the associated month. Holds pointer to the next month payment structure (if exists and is assigned).
 */
extern struct Payment{
    float payment, principal, interest, taxes, fees, pmi, remaining, month;
    struct Payment *nextmo;
}Payment;

/* Mortgage - Struct for mortgage details, holds pointers to mortgage attributes and first payment in linked list. Holds total payment attributes: total paid, interest, tax, fees and PMI (float) */
extern struct Mortgage{
    struct Loan *loan;
    struct Payment *firstPmt;
    float totalPaid, totalInterest, totalTax, totalFee, totalPMI;
}Mortgage;

/* getLoan - Accepts home value, loan amount, yearly tax, insurance, mortgage term, interest rate, pmi and hoa values, yearly tax increase percentage as parameters (all float). Returns pointer to Loan struct. */
extern struct Loan *getLoan(float homeVal, float loanAmt, float yTax, float yIns, float yMort, float iRate, float pmi, float hoa, float yTaxInc);

/* getMortgage - Accepts Loan (struct) and monthly PTI (double) as input. Initializes and returns mortgage  struct. */
extern struct Mortgage *getMortgage (struct Loan *loan, double pti);

#endif /* Mortgage_h */
