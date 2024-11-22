# Bank Account Management System

This project implements a simple bank account management system using the structures `CompteBancaire` and `Banque`. It includes various functionalities such as adding, deleting, and updating accounts, as well as handling transactions.

## Features

### 1. Add Account
**Functionality**:  
Adds a new account to the `Banque` structure.  
- The `compteur` field keeps track of the total number of accounts.
- The counter must not exceed the limit defined in the variable `MAX_ACCOUNTS`.

---

### 2. Delete Account (`SuppCompte`)
**Functionality**:  
Deletes an account from the bank.  
- Searches for the account to be deleted using a loop.  
- Overwrites the target account with the next account in the list.  
- Reduces the size of the array by 1.

---

### 3. Update Balance (`miseajourSolde`)
**Functionality**:  
Updates the balance of an account.  
- Searches for the target account.  
- Adds the desired amount to the account balance.

---

### 4. Transaction Structures
**Structures**:  
- **`Transaction`**: Stores details of individual transactions, including the amount and their status.  
- **`TransactionList`**: Maintains a list of transactions that are either pending or in progress.

---

### 5. Money Withdrawal (`Retrait D’argent`)
**Functionality**:  
Withdraws money from an account.  
- Searches for the account using its account number.  
- Checks if the balance is sufficient before processing the withdrawal.  
- If the account is not found, an error message is displayed.  
- If the balance is insufficient, a warning is shown.

---

### 6. Money Deposit (`Dépôt d’argent`)
**Functionality**:  
Deposits money into an account.  
- Adds the specified amount to the account balance.  
- Does not require balance verification.


