
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "terminal.h"



EN_terminalError_t getTransactionDate(ST_terminalData_t *termData){
  uint8_t currentDate[15] = {0};
  printf("Please enter current date: \n");
  gets(currentDate);
  uint8_t currentDateLength = strlen(currentDate);

  if (currentDate == NULL || currentDateLength < 10 ||currentDate[2] != '/' || currentDate[5] != '/' )
  {
    //printf("Wrong Date");
    return WRONG_DATE;
  }
  else
  {
    strcpy(termData->transactionDate,currentDate);
    //printf("Date Ok");
    return TERMINAL_OK;
  }
}
/*
EN_terminalError_t isCardExpired(ST_cardData_t *cardData, ST_terminalData_t *termData){
 uint8_t Return_Value = TERMINAL_OK;
  uint8_t CardExpiryMonth = ((cardData->cardExpirationDate[0] - '0') * 10) + (cardData->cardExpirationDate[1] - '0'); //   15/25
    uint8_t CardExpiryYear  = ((cardData->cardExpirationDate[3] - '0') * 10) + (cardData->cardExpirationDate[4] - '0');

  uint8_t transactionMonth =  ((termData->transactionDate[3] - '0' * 10) + termData->transactionDate[4] - '0');//  15/10/2022
  uint8_t transcationYear =  ((termData->transactionDate[8] - '0' * 10) + termData->transactionDate[9] - '0');

  if (transcationYear > CardExpiryYear) {
    Return_Value  = EXPIRED_CARD ;
  }
  else if (transcationYear == CardExpiryYear){
    if(transactionMonth >CardExpiryMonth || transactionMonth == CardExpiryMonth){
      //printf("Expired card");
      Return_Value  = EXPIRED_CARD  ;
    }
  }
    return Return_Value;

}
*/
EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData)
{
	EN_terminalError_t ErrorState = EXPIRED_CARD;

	uint8_t CardYY = (cardData->cardExpirationDate[3] - '0') * 10 + (cardData->cardExpirationDate[4] - '0');
	uint8_t TermYY = (termData->transactionDate[8] - '0') * 10 + (termData->transactionDate[9] - '0');
	if (CardYY == TermYY)
	{
		uint8_t CardMM = (cardData->cardExpirationDate[0] - '0') * 10 + (cardData->cardExpirationDate[1] - '0');
		uint8_t TermMM = (termData->transactionDate[3] - '0') * 10 + (termData->transactionDate[4] - '0');
		if(CardMM>= TermMM)		ErrorState = TERMINAL_OK;
	}
	else if (CardYY > TermYY)	ErrorState = TERMINAL_OK;
	return ErrorState;
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData){
  float transactionAmount = 0;
  printf("Please enter the transaction amount: \n");
  scanf("%f", &transactionAmount);

  if (transactionAmount <= 0){
    //printf("invalid amount");
    return INVALID_AMOUNT;
  }
  else {
    //printf("Date Ok");
    termData->transAmount = transactionAmount;
    return TERMINAL_OK;
  }
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData){
  if (termData->transAmount > termData->maxTransAmount) {
    //printf("Exceeded amount");
    return EXCEED_MAX_AMOUNT ;
  }
  else
  //printf("Date Ok");
  return TERMINAL_OK;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t *termData, float maxAmount){
  // float maxAmount = 0;
  // printf ("Please enter maximum allowed amount: \n ")
  // scanf("%f", &maxAmount);
  if (maxAmount <=0){
    //printf("Invalid amount");
    return INVALID_MAX_AMOUNT;
  }
  else
  {
    termData->maxTransAmount = maxAmount;
    //printf("Date Ok");
    return TERMINAL_OK;

  }
}

/*
int main (){
    ST_terminalData_t MY_TERMINAL;
    ST_cardData_t MY_CARD;
    // getTransactionDate function test
    uint8_t getTransactionDate_Result ,isCardExpired_Result,getTransactionAmount_Result,setMaxAmount_Result,isBelowMaxAmount_Result  = 0;
    getTransactionDate_Result  = getTransactionDate(&MY_TERMINAL);
    if(WRONG_DATE == getTransactionDate_Result )
    printf("WRONG_DATE\n");
    else if (TERMINAL_OK == getTransactionDate_Result)
    printf("DATE_OK\n");
    // isCardExpired function test
    isCardExpired_Result  = isCardExpired(&MY_CARD,&MY_TERMINAL);
    if(EXPIRED_CARD == isCardExpired_Result )
    printf("EXPIRED_CARD\n");
    else if (TERMINAL_OK == isCardExpired_Result)
    printf("DATE_OK\n");
    // getTransactionAmount function test
    getTransactionAmount_Result  = getTransactionAmount(&MY_TERMINAL);
    if(INVALID_AMOUNT == getTransactionAmount_Result )
    printf("INVALID_AMOUNT\n");
    else if (CARD_OK == getTransactionAmount_Result)
    printf("AMOUNT_OK\n");
    // setMaxAmount function test
    setMaxAmount_Result  = setMaxAmount(&MY_TERMINAL, 5000);
    if(INVALID_MAX_AMOUNT == setMaxAmount_Result )
    printf("INVALID_MAX_AMOUNT\n");
    else if (TERMINAL_OK == setMaxAmount_Result)
    printf("MAX_AMOUNT_SET_SUCCESSFULLY\n");

     // isBelowMaxAmount function test
    isBelowMaxAmount_Result  = isBelowMaxAmount(&MY_TERMINAL);
    if(EXCEED_MAX_AMOUNT == isBelowMaxAmount_Result )
    printf("The transaction amount exceeded the max amount\n");
    else if (TERMINAL_OK == isBelowMaxAmount_Result)
    printf("TRANSACTION_SET_SUCCESSFULLY\n");
    return 0 ;
}

*/
