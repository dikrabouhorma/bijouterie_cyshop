#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Structure des produits:
typedef struct{
	char Nom[20];
	int Num;
	int prix;
	int taille;
	int stock;
}Produit;

// Procedure pour sauvegarder un produit dans un fichier
void sauvegarder_produit(Produit* produit) {
    //Ajouter les produits dans le fichier un par un
    FILE* fichier = fopen("P.txt", "a");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }
    fprintf(fichier, "%s,%d,%d,%d,%d\n", produit->Nom, produit->Num, produit->prix, produit->taille, produit->stock);
    fclose(fichier);
}

// Fonction pour récupérer les produits à partir d'un fichier
Produit* recuperer_produits(int* nb_produits) {
    FILE* fichier = fopen("P.txt", "r");
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
    Produit* produits = (Produit*)malloc(count * sizeof(Produit));
    if (produits == NULL) {
        printf("Erreur lors de l'allocation de mémoire.\n");
        fclose(fichier);
        return NULL;
    }

    // Lecture des lignes et remplissage du tableau de produits
    int i = 0;
    while (fgets(ligne, sizeof(ligne), fichier)) {
        char nom[50];
        int num, prix, taille, stock;
        sscanf(ligne, "%[^,],%d,%d,%d,%d", nom, &num, &prix, &taille, &stock);
        strcpy(produits[i].Nom, nom);
        produits[i].Num = num;
        produits[i].prix = prix;
        produits[i].taille = taille;
        produits[i].stock = stock;
        i++;
    }

    fclose(fichier);
    *nb_produits = count;
    return produits;
}

// Fonction pour mettre à jour le prix d'un produit par son reference
void update_produit_REF(int ref, int nouveau_stock) {
    int nb_produits, i;
    Produit* produits = recuperer_produits(&nb_produits);

    for (i = 0; i < nb_produits; i++) {
        if (produits[i].Num == ref) {
            produits[i].stock += nouveau_stock;
            sauvegarder_produit(&produits[i]);
            break;
        }
    }
    //Vider le fichier
    FILE* fichier = fopen("P.txt", "w");
    fprintf(fichier, "%s", "");
    fclose(fichier);

    //Mettre à jour les produits dans le fichier
    for (i = 0; i < nb_produits; i++){
        sauvegarder_produit(&produits[i]);
    }
    printf("\n\n\n ============>>>>>>Mise a jour du stock avec succes .. ");
}

void update_produit_NOM(char Nom[20], int nouveau_stock) {
    int nb_produits, i;
    Produit* produits = recuperer_produits(&nb_produits);

    for (i = 0; i < nb_produits; i++) {
        if (strcmp(produits[i].Nom, Nom)==0) {
            produits[i].stock += nouveau_stock;
            break;
        }
    }

    //Vider le fichier
    FILE* fichier = fopen("P.txt", "w");
    fprintf(fichier, "%s", "");
    fclose(fichier);

    //Mettre à jour les produits dans le fichier
    for (i = 0; i < nb_produits; i++){
        sauvegarder_produit(&produits[i]);
    }
    printf("\n\n\n ============>>>>>>Mise a jour du stock avec succes .. ");
}

int chercher_ref(int ref) {
    int nb_produits, i;
    Produit* produits = recuperer_produits(&nb_produits);

    int s = -1;
    for (i = 0; i < nb_produits; i++) {
        if (produits[i].Num == ref) {
            //printf("produit trouvé");
            s = produits[i].stock;
            return s;
        }
    }
    return s;
}
int chercher_nom(char Nom[20]) {
    int nb_produits,i;
    Produit* produits = recuperer_produits(&nb_produits);

    int s = -1;

    for (i = 0; i < nb_produits; i++) {
        if (strcmp(produits[i].Nom, Nom)==0) {
                s = produits[i].stock;
                return s;
        }
    }
    return s;
}
//espace achat : permet aux clients d'acheter un ou plusieurs produits
//retourne le total des achats
int achats(char id[30]){
    int recherche;
    int quantite=0;
    int trouve;
    char produit_chercher[20];
    int i;
    int sousTotal=-1;
    int desabonner=0;

    int nb_produits;
    Produit* produits = recuperer_produits(&nb_produits);

    //system("cls"); // vider le terminal windows
    printf("\n\n\n*********** BIENVENU/E A VOTRE ESPACE ACHAT ***********");
    while(recherche!=1 && recherche!=2){
        printf("\n\n1. Chercher a l aide du Nom");
        printf("\n2. Cherchera l aide de la Reference ");
        scanf("%d",&recherche);
    }
    if(recherche==1){
        printf("\n\nLe nom du produit ? ");
        scanf("%s", &produit_chercher);
        trouve = chercher_nom(produit_chercher);
        // trouve : contient le stock du produit qu'on veut acheter
        if (trouve >= 0 ){
            printf("\tProduit Trouve \nQuantite ? \t");
            scanf("%d", &quantite );
            if ( quantite > trouve ){
                printf("\n\t La quantite demande n'est pas disponible");
                // Lui donnée le choix de désabonner !!!!

                printf("\n\n :( Est-ce que vous voulez vous desabonner ? :( \t tapez 1(OUI), tapez 2(NON)");
                scanf("%d", &desabonner);

                if (desabonner==2){
                    printf("Merci pour votre confiance !");
                }else{
                    supprimerClient(id);
                    printf("\n\n\n ============>>>>>> Au revoir .. ");
                    return sousTotal;
                }
            }
            else {
                for (i = 0; i < nb_produits; i++) {
                    if (strcmp(produits[i].Nom, produit_chercher)==0) {
                        produits[i].stock -= quantite;
                        printf("\n\tAjoute a votre panier !!");
                        sousTotal = quantite*produits[i].prix;
                        break;
                    }
                }
            }
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
            printf("\tProduit Trouve \nQuantite ? \t");
            scanf("%d", &quantite );
            if ( quantite > trouve ){
                printf("\n\t La quantite demande n'est pas disponible");
                // Lui donnée le choix de désabonner !!!!

                printf("\n\n :( Est-ce que vous voulez vous desabonner ? :( \t tapez 1(OUI), tapez 2(NON)");
                scanf("%d", &desabonner);

                if (desabonner==2){
                    printf("Merci pour votre confiance !");
                }else{
                    supprimerClient(id);
                    printf("\n\n\n ============>>>>>> Au revoir .. ");
                    return sousTotal;
                }
            }
            else {
                for (i = 0; i < nb_produits; i++) {
                    if (produits[i].Num == ref) {
                        produits[i].stock -= quantite;
                        printf("\n\tAjoute a votre panier !!");
                        sousTotal = quantite*produits[i].prix;
                        break;
                    }
                }
            }
        }
        else
             printf("\tIntrouvable !!");
        }
    //Vider le fichier
    FILE* fichier = fopen("P.txt", "w");
    fprintf(fichier, "%s", "");
    fclose(fichier);

    //Mettre à jour les produits dans le fichier: on sauvegarde le tableau avec les nouvelles valeurs dans l'ancien fichier
    for (i = 0; i < nb_produits; i++){
        sauvegarder_produit(&produits[i]);
    }
	return sousTotal;
}

void stock_min(){
    int nb_produits;
    Produit* produits = recuperer_produits(&nb_produits);

    //tri à bulles
    int i, j;
    Produit temp;

    for (i = 0; i < nb_produits - 1; i++) {
        for (j = 0; j < nb_produits - i - 1; j++) {
            if (produits[j].stock > produits[j + 1].stock) {
                temp = produits[j];
                produits[j] = produits[j + 1];
                produits[j + 1] = temp;
            }
        }
    }

    printf("\n\nLes 5 produits avec le stock le plus bas sont :\n");
    for (i = 0; i < 5; i++) {
        printf("\n  - %s , Ref=%d ", produits[i].Nom, produits[i].Num);
    }
}
//Fonction permettant au client de se désabonner

void supprimerClient(char id[30]) {
    // Ouvrir le fichier des clients en mode lecture
    FILE* fichierSource = fopen("C.txt", "r");
    if (fichierSource == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    // On va Créer un fichier temporaire pour stocker les données sans le client supprimé
    FILE* fichierTemp = fopen("temp.txt", "w");
    if (fichierTemp == NULL) {
        printf("Erreur lors de la création du fichier temporaire.\n");
        fclose(fichierSource);
        return;
    }

    // on va Lire les données ligne par ligne et copier dans le fichier temporaire sauf pour le client avec l'id spécifié
    char ligne[100];
    while (fgets(ligne, sizeof(ligne), fichierSource) != NULL) {
        // Extraire les données de la ligne
        char nom[15];
        char prenom[15];
        char clientId[30];
        sscanf(ligne, "%[^,],%[^,],%[^,]", nom, prenom, clientId);

        // enlever le saut de la ligne
        size_t longueur = strcspn(clientId, "\n");
        clientId[longueur] = '\0';

        // Vérifier si l'id correspond à celui du client à supprimer
        if (strcmp(clientId, id)!= 0) {
            fputs(ligne, fichierTemp);
        }
    }

    // Fermer les fichiers
    fclose(fichierSource);
    fclose(fichierTemp);

    // Supprimer l'ancien fichier
    remove("C.txt");
    // supprimer le fichier qui contient l'historique des achats du clients
    char n_f[30];
    strcpy(n_f, id);
    strcat(n_f, ".txt");
    printf("%s", n_f);


    remove(id);

    // Renommer le fichier temporaire avec le nom de l'ancien fichier
    rename("temp.txt", "C.txt");

    printf("Le client avec ID %s a ete supprime avec succes.\n", id);
}
