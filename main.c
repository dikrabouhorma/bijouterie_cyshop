#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Clients.h"
#include "Gestion.h"

int main() {
    //Variables
	int mode=0;
	int compte=0;
	char id[30];
	int option=0;
	int recherche=0;
	int i;
	int achat = 0;
	int total = 0;
	int cnx;

	int nb_produits;
    Produit* produits = recuperer_produits(&nb_produits);

    int nb_clients;
    Client* clients = recuperer_clients(&nb_clients);

	printf("\nBienvenu/e a notre Bijouterie ! :)\n");

	while(mode!=1 && mode!=2){
		printf("\n1.Mode Achat");
		printf("\n2.Mode Gestion");
		printf("\nChoisissez le mode que vous souhaitez:  ");
		scanf("%d",&mode);
	}
	if(mode==1){
		// Il s'agit du Mode Achat
		// Le client va nous indiquer si il est nouveau ou il possède deja un compte
		system("cls"); // pour vider le terminal en Windows
		printf("\n============== Bienvenu/e sur le Mode Achat ! ================= \n");
		while(compte!=1 && compte!=2){
			printf("1.J'ai deja un compte? Se connecter\n");
			printf("2.Nouveau Client ? Creer un compte\n");
			printf("Choisissez votre mode d'identification: ");
			scanf("%d",&compte);
		}
		if(compte==1){
			// Le client a deja un compte
			system("cls");
			printf("*********** ESPACE CLIENT ***********");
			printf("\nIdentifiant: ");
			scanf("%s",id);
		    // Verification de l'identifiant et affichage de l'historique d'achats
			cnx = connexion(&nb_clients, id);
			if (cnx == 1){
                printf("Connexion avec succes\n");
                //Espace achats
                char n_f[10];
                strcpy(n_f, id);
                strcat(n_f, ".txt");
                //=================================================================
                //Afficher historique d'achats
                //le fichier contient toute l'historique du client,
                //nous voulons uniquement les trois derniers !!
                FILE* fichier = fopen(n_f, "r");

                // Compter le nombre de lignes
                char ligne[100];
                int count = 0;
                while (fgets(ligne, sizeof(ligne), fichier)) {
                    count++;
                }
                rewind(fichier); // Réinitialisation de la position du curseur de lecture à la position initiale du fichier
                // Allocation dynamique du tableau de produits
                int *historique = (int*)malloc(count * sizeof(int));
                // Lecture des lignes et remplissage du tableau
                i = 0;
                int hist;
                while (fgets(ligne, sizeof(ligne), fichier)) {
                    sscanf(ligne, "%d", &hist);
                    historique[i] = hist;
                    i++;
                }
                fclose(fichier);
                if (count>=3){
                    printf("\n\n*********** Historique d'achat ***********\n");
                    //Afficher uniquement les 3 dernier achats
                    for (i=count-3; i< count; i++){
                        printf("\t%d\n", historique[i]);
                    }
                }else if (count == 0){
                    printf("\n\n*********** Historique VIDE ***********\n");
                }

                else {
                    printf("\n\n*********** Historique d'achat ***********\n");
                    //Afficher uniquement les 3 dernier achats
                    for (i=0; i< count; i++){
                        printf("\t%d\n", historique[i]);
                    }
                }
                //===========================================
                //créer ou mettre  à jour le fichier qui contient l'historique du client
                fichier = fopen(n_f, "a+");

                int sous_total;
                while(achat == 0){
                    sous_total = achats(id);
                    if (sous_total==-1) break;
                    total = total + sous_total;
                    printf("\n\nSi vous voulez achetez un autre produit, tapez 0, sinon tapez 1 \t");
                    scanf("%d", &achat);
                }
                printf("\nTotal a payer : %d", total);
                // sauvegarder le total dans l'historique du client concerne

                fprintf(fichier, "%d\n", total);
                fclose(fichier);
			}
			else{
                printf("identifiant incorrecte !!\n");
            }
		}
		else {
			// Nouveau client
			system("cls");
			printf("Creation d'un nouveau compte :");
			char Nom[15], Prenom[15], id[30];
            printf("\n\tNom: ");
            scanf("%s",Nom);
            printf("\n\tPrenom: ");
            scanf("%s",Prenom);
            // id : Concatenation du nom et prénom
            int i = 0;
            while (Nom[i] != '\0') {
                id[i] = Nom[i];
                i++;
            }
            int j = 0;
            while (Prenom[j]!= '\0'){
                id[i] = Prenom[j];
                i++;
                j++;
            }
            id[i] = '\0';

            Client c1;
            strcpy(c1.Nom, Nom);
            strcpy(c1.Prenom, Prenom);
            strcpy(c1.id, id);
            creer_client(&c1);

			printf("\nIdentifiant: ");
			scanf("%s",id);
			// Connexion
			cnx = connexion(&nb_clients, id);
			if (cnx == 1){
                printf("Connexion avec succes\n");
                //Espace achats
                char n_f[10];
                strcpy(n_f, id);  // fonction permettant de copier l'identifiant dans la varible n_f
                strcat(n_f, ".txt");// concatene l'identifiant avec .txt
                //=================================================================
                //Afficher historique d'achats
                //le fichier contient tout l'historique du client,
                //nous voulons uniquement les trois derniers !!
                FILE* fichier = fopen(n_f, "r");

                // Compter le nombre de lignes
                char ligne[100];
                int count = 0;
                while (fgets(ligne, sizeof(ligne), fichier)) {
                    count++;
                }
                rewind(fichier); // Réinitialisation de la position du curseur de lecture à la position initiale du fichier
                // Allocation dynamique du tableau de produits
                int *historique = (int*)malloc(count * sizeof(int));
                // Lecture des lignes et remplissage du tableau
                i = 0;
                int hist;
                while (fgets(ligne, sizeof(ligne), fichier)) {
                    sscanf(ligne, "%d", &hist);
                    historique[i] = hist;
                    i++;
                }
                fclose(fichier);
                if (count>=3){
                    printf("\n\n*********** Historique d'achat ***********\n");
                    //Afficher uniquement les 3 dernier achats
                    for (i=count-3; i< count; i++){
                        printf("\t%d\n", historique[i]);
                    }
                }else if (count == 0){
                    printf("\n\n*********** Historique VIDE ***********\n");
                }

                else {
                    printf("\n\n*********** Historique d'achat ***********\n");
                    //Afficher uniquement les 3 dernier achats
                    for (i=0; i< count; i++){
                        printf("\t%d\n", historique[i]);
                    }
                }
                //===========================================

                //créer ou mettre  à jour le fichier qui contient l'historique du client
                fichier = fopen(n_f, "a+");
                int sous_total;
                while(achat == 0 ){
                    sous_total = achats(id);
                    if (sous_total==-1) break;
                    total = total + sous_total;
                    printf("\n\nSi vous voulez achetez un autre produit, tapez 0, sinon tapez 1 \t");
                    scanf("%d", &achat);
                }
                printf("\nTotal à payer : %d", total);
                // sauvegarder le total dans l'historique du client concerne

                fprintf(fichier, "%d\n", total);
                fclose(fichier);
			}
			else{
                printf("identifiant incorrecte !!\n");
            }

		}
	}
	else{
		//Il s'agit du Mode Gestion
		system("cls"); //vider le terminal en windows
		printf("\n=================== Bienvenu/e au Mode Gestion =================\n");
		// Afficher les produits en ruptures de stock ainsi que que les produits qui ont un stock très bas
		//Affichage des produits en rupture de stock
        printf("\n URGENT !!!!!!  PRODUITS EN RUPTURE DE STOCK:");
        for (i = 0; i < nb_produits; i++) {
            if (produits[i].stock == 0) {
                printf("\n  - %s , Ref=%d ", produits[i].Nom, produits[i].Num);
            }
        }
    	// Afficher les produits en ruptures de stock ainsi que que les produits qui ont un stock très bas
        stock_min();
		printf("\n\n ====================== MENU: ========================= \n");

		while(option!=1 && option!=2 ){
			printf("\n1. Rechercher le stock d'un produit");
			printf("\n2. Augmenter le stock d'un produit");
			printf("\n\n Choisissez une option:  ");
			scanf("%d",&option);
    	}
    	if(option==1){

			// Rechercher un produit
			printf("\n RECHERCHE DUN PRODUIT:");

			while(recherche!=1 && recherche!=2){
				printf("\n\n1. Chercher a l aide du Nom");
				printf("\n2. Chercher a l aide de la Reference ");
				printf("\n\n Choisissez une option:  ");
				scanf("%d",&recherche);
			}
			if(recherche==1){
                char produit_chercher[20];
                printf("\nLe nom du produit ? ");
                scanf("%s", &produit_chercher);
                int trouve = chercher_nom(produit_chercher);
                if (trouve >= 0 ){
                    printf("\tProduit Trouve");
                    printf("\n\tQuantite en stock : %d", trouve);
                }
                else
                     printf("\tIntrouvable !!");

			}
			else {
                int ref;
                printf("\nLa reference du produit ? ");
                scanf("%d", &ref);
                int trouve = chercher_ref(ref);
                if (trouve >= 0 ){
                    printf("\tTrouve");
                    printf("\n\tQuantite en stock : %d", trouve);
                }
                else
                     printf("\tIntrouvable !!");
			}
		}
		else if (option==2){
			// Augmentation du stock d'un produit
			system("cls");
            printf("*********** AUGMENTATION DU STOCK ***********");
			printf("\n\nQuel est le produit dont vous voulez augmenter le stock?");

			while(recherche!=1 && recherche!=2){
				printf("\n\n1. Chercher a l aide du Nom");
				printf("\n2. Chercher a l aide de la Reference ");
				printf("\n\n Choisissez une option:  ");
				scanf("%d",&recherche);
			}
			if(recherche==1){
                char produit_chercher[20];
                printf("\n\nLe nom du produit ? ");
                scanf("%s", &produit_chercher);
                int trouve = chercher_nom(produit_chercher);
                if (trouve >= 0 ){
                    int aug=0;
                    printf("\tProduit Trouve \nNombre de produits a ajouter ? \t");
                    scanf("%d", &aug );

                    update_produit_NOM(produit_chercher, aug);
                }
                else
                     printf("\tProduit Introuvable !!");
			}
			else {
                int ref;
                printf("\nLa reference du produit ? ");
                scanf("%d", &ref);
                int trouve = chercher_ref(ref);
                if (trouve >= 0 ){
                    int aug=0;
                    printf("\tProduit Trouve \nNombre de produits a ajouter ? \t");
                    scanf("%d", &aug );

                    update_produit_REF(ref, aug);
                }
                else
                     printf("\tIntrouvable !!");
			}
		}
	}
}
