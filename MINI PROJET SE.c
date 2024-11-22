#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_ACCOUNTS 100
#define MAX_TRANSACTIONS 100

typedef struct {
    int Numcompte;
    float Solde;
} CompteBancaire;

typedef struct {
    CompteBancaire Comptes[MAX_ACCOUNTS];
    int Compteur;
} banque;

void ajoutCompte(banque *banque, int Numcompte, float Soldeinitial) {
    if (banque->Compteur < MAX_ACCOUNTS) {
        CompteBancaire Nvcompte;
        Nvcompte.Numcompte = Numcompte;
        Nvcompte.Solde = Soldeinitial;
        banque->Comptes[banque->Compteur] = Nvcompte;
        banque->Compteur++;
        printf("Compte bancaire ajouté avec succès.\n");
    } else {
        printf("Impossible d'ajouter un nouveau compte bancaire. Nombre maximum de comptes atteint.\n");
    }
}

void Suppcompte(banque *banque, int Numcompte) {
    int i, test = 0;
    for (i = 0; i < banque->Compteur; i++) {
        if (banque->Comptes[i].Numcompte == Numcompte) {
            test = 1;
            break;
        }
    }

    if (test) {
        for (int j = i; j < banque->Compteur - 1; j++) {
            banque->Comptes[j] = banque->Comptes[j + 1];
        }
        banque->Compteur--;
        printf("Compte supprimé.\n");
    } else {
        printf("Compte bancaire introuvable.\n");
    }
}

void miseajourSolde(banque *banque, int Numcompte, float nvSolde) {
    int i, test = 0;
    for (i = 0; i < banque->Compteur; i++) {
        if (banque->Comptes[i].Numcompte == Numcompte) {
            test = 1;
            break;
        }
    }

    if (test) {
        banque->Comptes[i].Solde = nvSolde;
        printf("Mise à jour terminée.\n");
    } else {
        printf("Compte bancaire introuvable.\n");
    }
}

typedef struct {
    int CompteSource;
    int destinationC;
    float Montant;
    int etat;
} Transaction;

typedef struct {
    Transaction transactions[MAX_TRANSACTIONS];
    int Compteur;
} TransactionList;

void Retrait(banque *banque, int Numcompte, float Montant) {
    int i, test = 0;
    for (i = 0; i < banque->Compteur; i++) {
        if (banque->Comptes[i].Numcompte == Numcompte) {
            test = 1;
            break;
        }
    }

    if (test) {
        if (banque->Comptes[i].Solde >= Montant) {
            banque->Comptes[i].Solde -= Montant;
            printf("Retrait de %.2f effectué avec succès depuis le compte %d.\n", Montant, Numcompte);
        } else {
            printf("Solde insuffisant sur le compte %d.\n", Numcompte);
        }
    } else {
        printf("Compte bancaire introuvable.\n");
    }
}

void depot(banque *banque, int Numcompte, float Montant) {
    int i, test = 0;
    for (i = 0; i < banque->Compteur; i++) {
        if (banque->Comptes[i].Numcompte == Numcompte) {
            test = 1;
            break;
        }
    }

    if (test) {
        banque->Comptes[i].Solde += Montant;
        printf("Dépôt de %.2f effectué avec succès sur le compte %d.\n", Montant, Numcompte);
    } else {
        printf("Compte bancaire introuvable.\n");
    }
}

void transfer(banque *banque, int CompteSource, int destinationC, float Montant) {
    int i, testSource = 0, testDestination = 0;
    for (i = 0; i < banque->Compteur; i++) {
        if (banque->Comptes[i].Numcompte == CompteSource) {
            testSource = 1;
        }
        if (banque->Comptes[i].Numcompte == destinationC) {
            testDestination = 1;
        }
        if (testSource && testDestination) {
            break;
        }
    }

    if (testSource && testDestination) {
        if (banque->Comptes[i].Solde >= Montant) {
            banque->Comptes[i].Solde -= Montant;
            for (i = 0; i < banque->Compteur; i++) {
                if (banque->Comptes[i].Numcompte == destinationC) {
                    break;
                }
            }
            banque->Comptes[i].Solde += Montant;
            printf("Virement de %.2f effectué avec succès du compte %d vers le compte %d.\n", Montant, CompteSource, destinationC);
        } else {
            printf("Solde insuffisant sur le compte %d.\n", CompteSource);
        }
    } else {
        printf("Compte bancaire introuvable.\n");
    }
}

void TX(banque *banque, TransactionList *transactionList) {
    int i;
    for (i = 0; i < transactionList->Compteur; i++) {
        pid_t Pridnum = fork();

        if (Pridnum == -1) {
            fprintf(stderr, "Erreur lors de la création du processus fils.\n");
            exit(1);
        } else if (Pridnum == 0) {
            Transaction *transaction = &transactionList->transactions[i];
            transaction->etat = 1; // Marquer la transaction comme en attente

            // Rechercher les comptes source et destination
            CompteBancaire *CompteSource = NULL;
            CompteBancaire *destinationC = NULL;

            int j;
            for (j = 0; j < banque->Compteur; j++) {
                if (banque->Comptes[j].Numcompte == transaction->CompteSource) {
                    CompteSource = &banque->Comptes[j];
                }
                if (banque->Comptes[j].Numcompte == transaction->destinationC) {
                    destinationC = &banque->Comptes[j];
                }
                if (CompteSource && destinationC) {
                    break;
                }
            }

            // Effectuer l'opération bancaire appropriée
            if (CompteSource != NULL && destinationC != NULL) {
                if (transaction->Montant > 0) {
                    Retrait(banque, transaction->CompteSource, transaction->Montant);
                    depot(banque, transaction->destinationC, transaction->Montant);
                } else {
                    printf("Montant invalide pour la transaction.\n");
                }
            } else {
                printf("Compte bancaire introuvable.\n");
            }

            // Changer l'état de la transaction
            transaction->etat = 0; // Marquer la transaction comme finalisée
            exit(0);
        }
    }

    // Attendre la fin de tous les processus fils
    for (i = 0; i < transactionList->Compteur; i++) {
        wait(NULL);
    }
}


void affichagetransaction(TransactionList *transactionList, banque *banque) {
    int i;

    printf("Transactions en cours :\n");
    for (i = 0; i < transactionList->Compteur; i++) {
        if (transactionList->transactions[i].etat == 0) {
            Transaction transaction = transactionList->transactions[i];

            CompteBancaire *CompteSource = NULL;
            CompteBancaire *destinationC = NULL;

            int j;
            for (j = 0; j < banque->Compteur; j++) {
                if (banque->Comptes[j].Numcompte == transaction.CompteSource) {
                    CompteSource = &banque->Comptes[j];
                }
                if (banque->Comptes[j].Numcompte == transaction.destinationC) {
                    destinationC = &banque->Comptes[j];
                }
                if (CompteSource && destinationC) {
                    break;
                }
            }

            if (CompteSource != NULL && destinationC != NULL) {
                printf("Compte Source: %d | Compte Destination: %d | Montant: %.2f | État: en cours\n",
                    transaction.CompteSource, transaction.destinationC, transaction.Montant);
            } else {
                printf("Erreur lors de la transaction : Compte bancaire introuvable.\n");
            }
        }
    }

    printf("\nTransactions en cours de traitement :\n");
    for (i = 0; i < transactionList->Compteur; i++) {
        if (transactionList->transactions[i].etat == 1) {
            Transaction transaction = transactionList->transactions[i];

            CompteBancaire *CompteSource = NULL;
            CompteBancaire *destinationC = NULL;

            int j;
            for (j = 0; j < banque->Compteur; j++) {
                if (banque->Comptes[j].Numcompte == transaction.CompteSource) {
                    CompteSource = &banque->Comptes[j];
                }
                if (banque->Comptes[j].Numcompte == transaction.destinationC) {
                    destinationC = &banque->Comptes[j];
                }
                if (CompteSource && destinationC) {
                    break;
                }
            }

            if (CompteSource != NULL && destinationC != NULL) {
                printf("Compte Source: %d | Compte Destination: %d | Montant: %.2f | État: en cours\n",
                    transaction.CompteSource, transaction.destinationC, transaction.Montant);
            } else {
                printf("Erreur lors de la transaction : Compte bancaire introuvable.\n");
            }
        }
    }
}

int main() {
    int NumCompte, c;
    float Montant;
    banque maBanque;
    maBanque.Compteur = 0;

    // Ajouter des comptes bancaires de test
    ajoutCompte(&maBanque, 5000, 50000.0);
    ajoutCompte(&maBanque, 2000, 100.0);

    // Déclaration et initialisation de TransactionList
    TransactionList transactionList;
    transactionList.Compteur = 0;

    printf("------------------MENU-----------------\n");
    printf("Donner votre choix\n");
    printf("1: Retrait d'argent\n");
    printf("2: Dépôt d'argent\n");
    printf("3: Faire un virement bancaire\n");
    printf("4: Ajouter un compte\n");
    printf("5: Supprimer votre compte\n");
    printf("6: Affichage de toutes les transactions\n");
    printf("7: Mettre à jour votre solde\n");
    printf("0: Fin du programme\n");

    scanf("%d", &c);

    while (c != 0) {
        switch (c) {
            case 1:
                printf("Donner votre Numéro de compte : ");
                scanf("%d", &NumCompte);
                printf("Donner le montant à retirer : ");
                scanf("%f", &Montant);
                Retrait(&maBanque, NumCompte, Montant);
                TX(&maBanque, &transactionList);
                break;
            case 2:
                printf("Donner votre Numéro de compte : ");
                scanf("%d", &NumCompte);
                printf("Donner le montant à déposer : ");
                scanf("%f", &Montant);
                depot(&maBanque, NumCompte, Montant);
                TX(&maBanque, &transactionList);
                break;
            case 3:
                printf("Donner votre Numéro de compte source : ");
                scanf("%d", &NumCompte);
                int destinationC;
                printf("Donner le numéro de compte de destination : ");
                scanf("%d", &destinationC);
                printf("Donner le montant à transférer : ");
                scanf("%f", &Montant);
                transfer(&maBanque, NumCompte, destinationC, Montant);
                TX(&maBanque, &transactionList);
                break;
            case 4:
                printf("Donner votre Numéro de compte : ");
                scanf("%d", &NumCompte);
                printf("Donner le montant initial du compte : ");
                scanf("%f", &Montant);
                ajoutCompte(&maBanque, NumCompte, Montant);
                break;
            case 5:
                printf("Donner votre Numéro de compte : ");
                scanf("%d", &NumCompte);
                Suppcompte(&maBanque, NumCompte);
                break;
            case 6:
                affichagetransaction(&transactionList, &maBanque);
                break;
            case 7:
                printf("Donner votre Numéro de compte : ");
                scanf("%d", &NumCompte);
                printf("Donner le montant à ajouter : ");
                scanf("%f", &Montant);
                miseajourSolde(&maBanque, NumCompte, Montant);
                break;
            default:
                printf("Choix invalide!\n");
                break;
        }

        printf("------------------MENU-----------------\n");
        printf("Donner votre choix\n");
        printf("1: Retrait d'argent\n");
        printf("2: Dépôt d'argent\n");
        printf("3: Faire un virement bancaire\n");
        printf("4: Ajouter un compte\n");
        printf("5: Supprimer votre compte\n");
        printf("6: Affichage de toutes les transactions\n");
        printf("7: Mettre à jour votre solde\n");
        printf("0: Fin du programme\n");

        scanf("%d", &c);
    }

    return 0;
}


