#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "../Card/card.h"
#include "../Terminal/terminal.h"
#include "server.h"


 static ST_accountsDB_t ST_AccountsDB[255] = {
{ 100000,RUNNING,"1122334455667788990"},
{ 250000.15,RUNNING,"678132873214669726"},
{ 100001568.5,RUNNING,"9873213456789931352" },
{ 2000,BLOCKED,"9873134566546666"},
{ 50000,RUNNING,"878973243546265799"},
{ 20,RUNNING,"0548479434679879998"},
{ 0.25,RUNNING,"9834135459514756657"},
{ 2454.5,BLOCKED,"8798781354333464446"},
{ 10,BLOCKED,"9879434357812367543"},
{ 20.5,RUNNING,"97877822546684846"}
};


static ST_transaction_t TransDataDB[255];
static strCompare CompareStr(char* str1, char* str2)
{
	strCompare state = notmatched;
	uint8_t Itrator = 0;
	do
	{
		if(str1[Itrator] != str2[Itrator])	break;
		if (!str1[Itrator]) state = matched;
	}while(str2[Itrator++]);
		return state;
}

EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	EN_transState_t ErrorState = INTERNAL_SERVER_ERROR;
	transData->transState = INTERNAL_SERVER_ERROR;
	ST_accountsDB_t* ReferanceAccount;
	if (isValidAccount(&transData->cardHolderData, &ReferanceAccount) == TERMINAL_OK)
		if (isBlockedAccount(ReferanceAccount) == TERMINAL_OK)
		{
			if (isAmountAvailable(&transData->terminalData, ReferanceAccount) == TERMINAL_OK)
			{
				ReferanceAccount->balance -= transData->terminalData.transAmount;
				transData->transState = APPROVED;
				if (saveTransaction(transData) == TERMINAL_OK)
					if (getTransaction(transData->transactionSequenceNumber, transData) == TERMINAL_OK)
					{
						ErrorState = APPROVED;
						printf("Balance=%.1f\nPrimary Account Number =%s", ReferanceAccount->balance, ReferanceAccount->primaryAccountNumber);
						printf("\n Holder Name :%s\n", transData->cardHolderData.cardHolderName);
					}
					else	ErrorState = INTERNAL_SERVER_ERROR;
				else ErrorState = INTERNAL_SERVER_ERROR;
			}
			else	ErrorState = DECLINED_INSUFFECIENT_FUND;
		}
		else	ErrorState = DECLINED_STOLEN_CARD;
	else	ErrorState= FRAUD_CARD;
	transData->transState = ErrorState;
	return ErrorState;
}


EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t** accountRefrence)
{
	EN_serverError_t ErrorState = ACCOUNT_NOT_FOUND;
	uint8_t i = 0;
	while(i < 255 && ST_AccountsDB[i].primaryAccountNumber[0]
		&& ErrorState != APPROVED)
	{
		if (CompareStr(cardData->primaryAccountNumber, ST_AccountsDB[i].primaryAccountNumber))
		{
			ErrorState = APPROVED;
			*accountRefrence = &ST_AccountsDB[i];
		}
		i++;
	}
	return ErrorState;
}
EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence)
{
	EN_serverError_t ErrorState = BLOCKED_ACCOUNT;
	if (RUNNING == accountRefrence->state)	ErrorState = TERMINAL_OK;
	return ErrorState;
}
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData,ST_accountsDB_t * accountRefrence)
{
	EN_serverError_t ErrorState = LOW_BALANCE;
	if (termData->transAmount <= accountRefrence->balance)	ErrorState = TERMINAL_OK;
	return ErrorState;
}

EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	EN_serverError_t ErrorState = SAVING_FAILED;
	transData->transactionSequenceNumber=0;

	if (!TransDataDB[254].transactionSequenceNumber)
	{
		while (TransDataDB[transData->transactionSequenceNumber].transactionSequenceNumber
			&& transData->transactionSequenceNumber++ < 255);
		if (transData->transactionSequenceNumber < 255)
		{
			transData->transactionSequenceNumber++;
			TransDataDB[transData->transactionSequenceNumber - 1] = *transData;
			ErrorState = SERVER_OK;
		}
	}

	else
	{
		uint8_t LowIndex = 0;
		uint8_t i;
		for ( i = 1; i < 255; i++)
		{
			if (TransDataDB[LowIndex].transactionSequenceNumber >
				TransDataDB[i].transactionSequenceNumber)
				LowIndex = i;
		}

		uint8_t BiggestIND = 0;

		for (i = 1; i < 255; i++)
		{
			if (TransDataDB[BiggestIND].transactionSequenceNumber <
				TransDataDB[i].transactionSequenceNumber)
				BiggestIND = i;
		}
		transData->transactionSequenceNumber = TransDataDB[BiggestIND].transactionSequenceNumber + 1;
		TransDataDB[LowIndex] = *transData;
		ErrorState = TERMINAL_OK;

	}

	return ErrorState;
}



EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData)
{
	EN_serverError_t ErrorState = TRANSACTION_NOT_FOUND;
	uint8_t i = 0;
	while (i < 255 && ErrorState)
	{
		if (transactionSequenceNumber == TransDataDB[i].transactionSequenceNumber)
		{
			*transData = TransDataDB[i];
			ErrorState = TERMINAL_OK;
		}
		i++;
	}

	return ErrorState;
}
void listSavedTransactions(void)
{   uint8_t i;
	for ( i = 0; i < 255; i++)
		printf("index=%d,,seq=%d\n", i, TransDataDB[i].transactionSequenceNumber);
}
/*

int main (){
    // recieveTransactionData function test
    ST_cardData_t MY_CARD;
    ST_terminalData_t MY_TERMINAL ;
    ST_transaction_t MY_TRANSACTION ;
    ST_accountsDB_t MY_ACCOUNT;
    uint8_t i ;
    //arr[20] = "4567 6546 1425 4321"
    for (i =0 ; i< 16 ; i++){
    MY_CARD.primaryAccountNumber[i] = MY_ACCOUNT.primaryAccountNumber[i];
    }

    uint8_t recieveTransactionData_Result ,isValidAccount_Result,isAmountAvailable_Result, saveTransaction_Result  = 0;
    recieveTransactionData_Result  = recieveTransactionData(&MY_TRANSACTION);
    if(APPROVED == recieveTransactionData_Result)
    printf("DATA_APPROVED\n");
    else if (DECLINED_STOLEN_CARD == recieveTransactionData_Result)
    printf("DECLINED_STOLEN_CARD\n");
    else if (DECLINED_INSUFFECIENT_FUND == recieveTransactionData_Result)
    printf("LOW_BALANCE\n");
    else if (INTERNAL_SERVER_ERROR == recieveTransactionData_Result)
    printf("SAVING_FAILED\n");
    // isValidAccount function test
    isValidAccount_Result  = isValidAccount(&MY_CARD,&MY_ACCOUNT);
    if(SERVER_OK == isValidAccount_Result )
    printf("ACCOUNT_EXISTED\n");
    else if (ACCOUNT_NOT_FOUND == isValidAccount_Result)
    printf("ACCOUNT_NOT_FOUND\n");
    // isAmountAvailable function test
    isAmountAvailable_Result  = isAmountAvailable(&MY_TERMINAL,&MY_ACCOUNT);
    if(LOW_BALANCE == isAmountAvailable_Result )
    printf("LOW_BALANCE\n");
    else if (TERMINAL_OK == isAmountAvailable_Result)
    printf("TERMINAL_OK\n");
    // saveTransaction function test
    saveTransaction_Result  = saveTransaction(&MY_TRANSACTION);
    if(SAVING_FAILED == saveTransaction_Result )
    printf("SAVING_FAILED\n");
    else if (SERVER_OK == saveTransaction_Result)
    printf("SAVING_DONE\n");

    return 0 ;
}
*/
