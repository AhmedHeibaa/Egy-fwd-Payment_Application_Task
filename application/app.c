#include <stdio.h>


#include "../card/card.h"
#include "../server/server.h"
#include "../terminal/terminal.h"
#include "app.h"

void appStart(void){
  /* *Get the card data */
  ST_transaction_t Card_1;
  ST_terminalData_t Terminal_1;
  EN_cardError_t cardHolderName_State = getCardHolderName(&Card_1.cardHolderData);

  if (cardHolderName_State == WRONG_NAME){
    printf("\n DECLINED: Invalid card name.");
    ERROR_RETURN();
  }else{;}

  EN_cardError_t cardExpDate_State = getCardExpiryDate(&Card_1.cardHolderData);

  if (cardExpDate_State == WRONG_EXP_DATE){
    printf("\n DECLINED: Invalid card expiration date.");
    ERROR_RETURN();
  }else{;}


  EN_cardError_t cardPAN_State = getCardPAN(&Card_1.cardHolderData);

  if (cardPAN_State == WRONG_PAN){
    printf("\n DECLINED: Invalid card PAN.");
    ERROR_RETURN();
  }else{;}
  /*******************************************************/
  /* *Process the card data */
  //Terminal_1.maxTransAmount = 60000.00;
    //Terminal_1.transactionDate = "14/11/2022"
  EN_terminalError_t GetTransDate_State = getTransactionDate(&Card_1.terminalData);
    if(GetTransDate_State == WRONG_DATE){
      printf("\n DECLINED: Invalid transaction date.");
      ERROR_RETURN();
    }else{;}

  EN_terminalError_t expCard_State = isCardExpired(&Card_1.cardHolderData,&Card_1.terminalData);
  if(expCard_State == EXPIRED_CARD){
    printf("\n DECLINED: Card is expired.");
    ERROR_RETURN();
  }else{;}

  EN_terminalError_t transactionAmount_State = getTransactionAmount(&Card_1.terminalData);
  if(transactionAmount_State == INVALID_AMOUNT){
    printf("\n DECLINED: Invalid transaction amount.");
    ERROR_RETURN();
  }else{;}

  setMaxAmount(&Card_1.terminalData,5000);

  EN_terminalError_t transactionAmountLimit_State = isBelowMaxAmount(&Card_1.terminalData);
  if(transactionAmountLimit_State == EXCEED_MAX_AMOUNT){
    printf("\n DECLINED: Transaction amount exceeded.");
    ERROR_RETURN();
  }else{;}
/*******************************************************************************/
/* *Start the process */
//ST_transaction_t Transaction; //= {.cardHolderData = Card_1 , .terminalData = Terminal_1};

EN_transState_t transaction_State = recieveTransactionData(&Card_1);

if(transaction_State == DECLINED_STOLEN_CARD){
  printf("\n DECLINED: Invalid account.");
  ERROR_RETURN();
}
else if(transaction_State == DECLINED_INSUFFECIENT_FUND){
  printf("\n DECLINED: Insuffcient fund.");
  ERROR_RETURN();
}
else if(transaction_State == INTERNAL_SERVER_ERROR){
  printf("\n DECLINED: Internal server error.");
  ERROR_RETURN();
}else if(transaction_State == APPROVED)
{

printf("\n APPROVED: Successful transaction.\n");
//listSavedTransactions();

SUCCESSFUL_RETURN();
}

};


