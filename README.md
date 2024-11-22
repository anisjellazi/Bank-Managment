Explanation for each function:

Add Account with the two structures CompteBancaire and Banque
This function allows adding a new account to the Banque structure. There is a "counter" field that displays the number of accounts in the bank. This counter must not exceed the limit defined in the variable MAX_ACCOUNTS.

Delete Account (SuppCompte)
This function allows deleting a bank account. First, the account to be deleted is searched for using a for loop. The target account's position is overwritten with the next account in the list, effectively removing it, and the size of the array is reduced by 1.

Update Balance (miseajourSolde)
Since the program already includes two essential operations (adding and deleting accounts), it was deemed important to create a function to update the account balance. This function first searches for the account, then adds the desired amount to the balance.

Structures Transaction and TransactionList
The Transaction structure is used to store details of transactions performed, including the amount and their status. The second structure, TransactionList, is used to store transactions that are either pending or in progress.

Money Withdrawal (Retrait D’argent)
This function allows access to the customer's account using the account number. Once the account is located, it checks if the amount to be withdrawn is available. If not, it displays an "insufficient balance" message. If the account cannot be found, an error message is displayed.

Money Deposit (Dépôt d’argent)
This function works similarly to the withdrawal function but does not need to check the account balance since the operation involves adding an amount without subtraction.
