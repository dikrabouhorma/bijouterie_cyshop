#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Structure Client:
typedef struct{
	char Nom[15];
	char Prenom[15];
	char id[30];
}Client;

// Cette fonction permet de creer  un client et de l'ajouter dans le fichier
void creer_client(Client* c) {
    FILE* fichier = fopen("C.txt", "a");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }
    fprintf(fichier, "%s,%s,%s\n", c->Nom, c->Prenom, c->id);
    printf("\nCompte cree avec succes, vous pouvez se connecter avec votre identifiant : \t %s", c->id);
    fclose(fichier);
}

// Cette fonction permet de récupérer l'ensemble des clients du fichier dans un tableau des clients
Client* recuperer_clients(int* nb_clients) {
    FILE* fichier = fopen("C.txt", "r");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return NULL;
    }
    // Comptage du nombre de lignes/produits dans le fichier
    char ligne[100];
    int count = 0;
    while (fgets(ligne, sizeof(ligne), fichier)) {
        count++;
    }
    rewind(fichier); // Réinitialisation de la position du curseur de lecture à la position initiale du fichier
    // Allocation dynamique du tableau de produits
    Client* clients = (Client*)malloc(count * sizeof(Client));
    if (clients == NULL) {
        printf("Erreur lors de l'allocation de mémoire.\n");
        fclose(fichier);
        return NULL;
    }

    // Lecture des lignes et remplissage du tableau de produits
    int i = 0;
    while (fgets(ligne, sizeof(ligne), fichier)) {
        char Nom[15], Prenom[15], id[30];
        sscanf(ligne, "%[^,],%[^,],%[^,]", Nom, Prenom, id);
        strcpy(clients[i].Nom, Nom);
        strcpy(clients[i].Prenom, Prenom);
        strcpy(clients[i].id, id);
        i++;
    }
    fclose(fichier);
    *nb_clients = count;
    return clients;
}

//Procedure permettant de se connecter:
int connexion(int* nb_clients, char id[30]){
	int i;
    Client* clients = recuperer_clients(nb_clients);
    if (clients != NULL) {
        for (i = 0; i < *nb_clients; i++) {
            // enlever le saut de la ligne
            size_t longueur = strcspn(clients[i].id, "\n");
            clients[i].id[longueur] = '\0';
            // comparer id avec les id des clients existants
            if (strcmp(clients[i].id, id)==0){
                //printf("Connexion avec succes\n");
                return 1;
            }
        }
    }
    return -1;
}
