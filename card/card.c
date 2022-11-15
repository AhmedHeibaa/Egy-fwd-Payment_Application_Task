#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "card.h"


EN_cardError_t getCardHolderName(ST_cardData_t *cardData) {
    uint8_t clientName[25] ={0};
    uint8_t cilentNameLength =0;
    uint8_t returnState = CARD_OK ;
printf("Please enter your name: \n");
gets(clientName);

cilentNameLength = strlen(clientName);
if (clientName == NULL || cilentNameLength < 20 ||cilentNameLength > 24 ){
       // printf("NAME_Error" );
    returnState = WRONG_NAME;
     //  wrong name
}
else {

    strcpy(cardData->cardHolderName,clientName);
    //printf("NAME_OK" );

}
        return returnState;
};
EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData){
    uint8_t cardExpiryDate[10]={0};
    uint8_t dateLength;
printf("Please enter your card expiry date: \n");
gets(cardExpiryDate);
dateLength = strlen(cardExpiryDate);
if (cardExpiryDate == NULL || dateLength != 5  || cardExpiryDate[2] != '/'){
    return WRONG_EXP_DATE; // Wrong expiry date
  }
  else {

    strcpy(cardData->cardExpirationDate,cardExpiryDate);
    //printf("EXPIRY_DATE_OK");
    return CARD_OK;
  }
};

EN_cardError_t getCardPAN(ST_cardData_t *cardData){
    uint8_t panNumber[20] = {0};
printf("Please enter PAN number: \n");
gets(panNumber);
uint8_t panLength = strlen(panNumber);

// PAN = cardData->primaryAccountNumber;
if (panNumber == NULL ||panLength < 16 || panLength > 20 )
{
return WRONG_PAN; // wrong PAN
}
else
{
  strcpy(cardData->primaryAccountNumber,panNumber);
  //printf("PAN_OK");
  return CARD_OK;
}
};
/*
int main (){
    // getCardHolderName function test
    ST_cardData_t MY_CARD;
    uint8_t getCardHolderName_Result ,getCardExpiryDate_Result,getCardPAN_Result  = 0;
    getCardHolderName_Result  = getCardHolderName(&MY_CARD);
    if(WRONG_NAME == getCardHolderName_Result )
    printf("WRONG_NAME\n");
    else if (CARD_OK == getCardHolderName_Result)
    printf("Name_OK\n");
    // getCardExpiryDate function test
    getCardExpiryDate_Result  = getCardExpiryDate(&MY_CARD);
    if(WRONG_EXP_DATE == getCardExpiryDate_Result )
    printf("WRONG_EXP_DATE\n");
    else if (CARD_OK == getCardExpiryDate_Result)
    printf("ExpiryDate_OK\n");
    // getCardPAN function test
    getCardPAN_Result  = getCardPAN(&MY_CARD);
    if(WRONG_PAN == getCardPAN_Result )
    printf("WRONG_PAN\n");
    else if (CARD_OK == getCardPAN_Result)
    printf("PAN_OK\n");

    return 0 ;
}
*/
