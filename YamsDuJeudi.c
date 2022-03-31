/**
 * @file YamsDuJeudi.c
 * @author Matéo GUEZENNEC
 * @brief Programme de Yams
 * @version 1.1
 * @date 2021-11-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LONGEUR_TABLEAU 17
#define NOMBRE_DES 5
#define NOMBRE_MAX_DES 6
#define LONGEUR_NOM 9

typedef int T_des[NOMBRE_DES];
typedef char T_nom[LONGEUR_NOM]; // String des nom dans le tableau
typedef int T_Score[LONGEUR_TABLEAU]; // Tableau des scores
typedef int T_ScoreDes[LONGEUR_TABLEAU][3]; // Score temporaire mise a jour pour chaque lancer de dès avec les possibilités du lancé

void afficheScore(T_nom nom1, T_nom nom2, T_Score score_1, T_Score score_2);
void saisieNom(T_nom nom1, T_nom nom2);
void changeEtat(T_des desRelance, int NumDes); // Change l'etat de la relance d'un des 1 ou 0
void afficheDes(T_des des); // Affichage Graphique des dès
void tirageDes(T_des des, T_des desRelance); // Donne un chiffre aléatoire au dès qui doivent être relancé
void reinitialiseDes(T_des des, T_des desRelance); // reset les dès est met que tout les dès sont a relancer
void selectionDes(T_des des, T_des desRelance, T_nom nom, int *lancer); // permet de selection quels des sont a relancer
int CalculeScoreChiffre(int nb, T_des des); // fonction qui retourn le score en fonction du nombre de dès de celui ci
void SelectionDuScore(T_Score scoreFinal, T_ScoreDes ScoreTemp, T_des des); // Demande au joueurs dans quel case il veut rentrer un score, si ce n'est pas possible il lui demande de barrer une case
void resetScoretmp(T_ScoreDes ScoreTemp); // reset le tableau de Valeur temporaire
void CalculeScoreBrelan(T_ScoreDes ScoreTemp, T_des des); // Calcule si le brelan est possible pour le joueur et pour le lancé actuel
void CalculeScoreCarre(T_ScoreDes ScoreTemp, T_des des); // Calcule si le Carre est possible pour le joueur et pour le lancé actuel
void CalculeScoreFullhouse(T_ScoreDes ScoreTemp, T_des des); // Calcule si le FullHouse est possible pour le joueur et pour le lancé actuel
void CalculeScorePetitesuite(T_ScoreDes ScoreTemp, T_des des); // Calcule si le Petite Suite est possible pour le joueur et pour le lancé actuel
void CalculeScoreGrandesuite(T_ScoreDes ScoreTemp, T_des des); // Calcule si le Grande Suite est possible pour le joueur et pour le lancé actuel
void CalculeScoreYams(T_ScoreDes ScoreTemp, T_des des); // Calcule si le Yams est possible pour le joueur et pour le lancé actuel
void CalculeScoreChance(T_ScoreDes ScoreTemp, T_des des); // Calcule le total des dès et regarde si la Chance est encore disponible
void AjoutBonus(T_Score score); // Calcule si le score du joueur remplie la condition pour avoir le bonus
void CalculeTotalScore(T_Score score); // Calcule Les 3 totaux du tableau des scores (total supérieur, total inférieur et total)
void tri(T_des desIn, T_des desOut); // tri les dès de avec un tableau d'entré est un tableau de sortie (pour garder l'ordre des dès)
void AfficheVainqueur(T_nom joueur1, T_nom joueur2, T_Score scoreJoueur1, T_Score scoreJoueur2); // Affiche le nom du vainqueur ainsi que les score des 2 joueurs

/**
 * @brief Fonction principale du programme
 * 
 * @return int 
 */
int main() {
    // Ramdom init
    srand( time ( NULL ));
    T_ScoreDes Scoretmp1; // Score temporaire mise a jour pour chaque lancer de dès avec les possibilités du lancé
    T_ScoreDes Scoretmp2;
    T_nom joueur1, joueur2; // Nom des joueurs
    T_Score score_1; // Score1
    T_Score score_2; // Score2
    T_des des; // Valeurs des dès
    T_des desRelance; //Quel des sont a relancer 
    int tour, joueur, lancer;
    tour = 0;
    joueur = 0;
    lancer = 0;
    saisieNom(joueur1, joueur2);
    // initialise les tableaux des score a 0 . POSSIBILITE DE FAIRE UNE FONCTION
    for (int i = 0; i < LONGEUR_TABLEAU; i++) {
        score_1[i] = 0;
        score_2[i] = 0;
        Scoretmp1[i][0] = 1;
        Scoretmp1[i][1] = 0;
        Scoretmp1[i][2] = 0;
        Scoretmp2[i][0] = 1;
        Scoretmp2[i][1] = 0;
        Scoretmp2[i][2] = 0;
    }
    printf("Bienvenue dans le jeux du Yams : \n");
    while (tour < 13) {
        printf("Début du tour N°%d : \n", tour+1);
        joueur = 0;// remet la valeur du joueur a 0;
        while (joueur < 2) {
            afficheScore(joueur1, joueur2, score_1, score_2);
            reinitialiseDes(des, desRelance);
            lancer = 0;
            if(joueur % 2 == 0) {
                while (lancer < 3){
                    tirageDes(des, desRelance);
                    afficheDes(des);
                    selectionDes(des, desRelance, joueur1, &lancer);
                    lancer++;
                }
                SelectionDuScore(score_1, Scoretmp1, des);
            } else {
                while (lancer < 3){
                    tirageDes(des, desRelance);
                    afficheDes(des);
                    selectionDes(des, desRelance, joueur2, &lancer);
                    lancer++;
                }
                SelectionDuScore(score_2, Scoretmp2, des);
            }
            joueur++;
        }
        tour++;
    }
    AfficheVainqueur(joueur1, joueur2, score_1, score_2);
    return EXIT_SUCCESS;
}

/*
            TOUTES LES FONCTIONS ET PROCEDURE SONT EN DESSOUS
*/

/**
 * @brief Demande le nom des 2 joueurs afin de les enregistrer
 * 
 * @param nom1 
 * @param nom2 
 */
void saisieNom(T_nom nom1, T_nom nom2) {
    printf("Les noms doivent faire au MAXIMUM 9 caractères\n");
    printf("Donnez le nom du joueur 1: ");
    fgets(nom1, LONGEUR_NOM, stdin);
    strtok(nom1, "\n");
    printf("Donnez le nom du joueur 2: ");
    fgets(nom2, LONGEUR_NOM, stdin);
    strtok(nom2, "\n");
}
/**
 * @brief Affiche le tableau graphiquement
 * 
 * @param des 
 */
void afficheDes(T_des des) {
    for (size_t i = 0; i < NOMBRE_DES; i++) {
        printf(" ┏━━━┓ ");
    }
    printf("\n");
    for (size_t i = 0; i < NOMBRE_DES; i++) {
        printf(" ┃ %1d ┃ ", des[i]);
    }
    printf("\n");
    for (size_t i = 0; i < NOMBRE_DES; i++) {
        printf(" ┗━━━┛ ");
    }
    printf("\n");
    for (int i = 0; i < NOMBRE_DES; i++)
    {
        printf("   %c   ", 65+i); // Met une lettre de l'aphabet ASCII de A à Z
    }
    printf("\n");
}
/**
 * @brief Selection quel des sont à garder (remetre un des le relancera)
 * 
 * @param des 
 * @param desRelance 
 * @param nom 
 */
void selectionDes(T_des des, T_des desRelance, T_nom nom, int *lancer) {
    char choix;
    printf("Quel des voulez vous relancer %s ?(0 pour relance | 1 pour renter un score) : ", nom);
    while (choix != '0' && choix != '1') {
        scanf("%c", &choix);
        switch (choix) {
        case 'A':
            changeEtat(desRelance, 0);
            break;
        case 'B':
            changeEtat(desRelance, 1);
            break;
        case 'C':
            changeEtat(desRelance, 2);
            break;
        case 'D':
            changeEtat(desRelance, 3);
            break;
        case 'E':
            changeEtat(desRelance, 4);
            break;
        case '1':
            *lancer = 3;
            break;
        default:
            break;
        }
    }
}
/**
 * @brief donne une valeur au des
 * 
 * @param des 
 * @param desRelance 
 */
void tirageDes(T_des des, T_des desRelance) {
    for (int i = 0; i < NOMBRE_DES; i++) {
        if (desRelance[i] == 0) {
            des[i] = 1 + rand() % NOMBRE_MAX_DES;            
        }
    }
}
/**
 * @brief Change l'etat de la relance du des, si il étais relancé il passe en mode garder et si il était en mode garder il passe en mode relnace
 * 
 * @param desRelance 
 * @param NumDes 
 */
void changeEtat(T_des desRelance,int NumDes) {
    if (desRelance[NumDes] == 0) {
        desRelance[NumDes] = 1;
    } else {
        desRelance[NumDes] = 0;
    }
}
/**
 * @brief Affiche la fiche de score du Yams
 * 
 * @param nom1 
 * @param nom2 
 * @param score_1 
 * @param score_2 
 */
void afficheScore(T_nom nom1, T_nom nom2, T_Score score_1, T_Score score_2) {
    printf("/-------------------------------------\\\n");
    printf("|     Joueurs     |%9s|%9s|\n", nom1, nom2);
    printf("|-----------------|---------|---------|\n");
    printf("|  1 [total de 1] |%9d|%9d|\n", score_1[0], score_2[0]);
    printf("|  2 [total de 2] |%9d|%9d|\n", score_1[1], score_2[1]);
    printf("|  3 [total de 3] |%9d|%9d|\n", score_1[2], score_2[2]);
    printf("|  4 [total de 4] |%9d|%9d|\n", score_1[3], score_2[3]);
    printf("|  5 [total de 5] |%9d|%9d|\n", score_1[4], score_2[4]);
    printf("|  6 [total de 6] |%9d|%9d|\n", score_1[5], score_2[5]);
    printf("| Bonus si > à 62 |%9d|%9d|\n", score_1[6], score_2[6]);
    printf("| Total supérieur |%9d|%9d|\n", score_1[7], score_2[7]);
    printf("|-----------------|---------|---------|\n");
    printf("| Brelan   [total]|%9d|%9d|\n", score_1[8], score_2[8]);
    printf("| Carré    [total]|%9d|%9d|\n", score_1[9], score_2[9]);
    printf("| Full House  [25]|%9d|%9d|\n", score_1[10], score_2[10]);
    printf("| Petite suite[30]|%9d|%9d|\n", score_1[11], score_2[11]);
    printf("| Grande suite[40]|%9d|%9d|\n", score_1[12], score_2[12]);
    printf("| Yams        [50]|%9d|%9d|\n", score_1[13], score_2[13]);
    printf("| Chance   [total]|%9d|%9d|\n", score_1[14], score_2[14]);
    printf("|-----------------|---------|---------|\n");
    printf("| Total inférieur |%9d|%9d|\n", score_1[15], score_2[15]);
    printf("|=================|=========|=========|\n");
    printf("|      Total      |         |         |\n");
    printf("|                 |%9d|%9d|\n", score_1[16], score_2[16]);
    printf("\\-------------------------------------/\n");
}

/**
 * @brief Réinitialise les des et leurs Etat de Relance
 * 
 * @param des 
 * @param desRelance 
 */
void reinitialiseDes(T_des des, T_des desRelance) {
    for (int i = 0; i < NOMBRE_DES; i++) {
        des[i] = 0;
        desRelance[i] = 0;
    }
}

/**
 * @brief Calcule et retourne le score du chiffre n.
 * 
 * @param nombre 
 * @param tableau_des 
 * @return int 
 */
int CalculeScoreChiffre(int nb, T_des des) {
    int res;
    res = 0;
    for (int i = 0; i < NOMBRE_DES; i++)
    {
        if (des[i] == nb)
        {
            res = res + nb;
        }    
    }
    return res;
}
/**
 * @brief Permet de selection le score que l'on souhaite faire et si on doit barre une case
 * 
 * @param scoreFinal 
 * @param ScoreTemporaire 
 * @param tableau_des  
 */
void SelectionDuScore(T_Score scoreFinal, T_ScoreDes ScoreTemp, T_des des) {
    char choixBarre;
    int scoreEntre, posibilite;
    char choix;
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");// Delimitation des possibilités
    resetScoretmp(ScoreTemp);
    for (int i = 0; i < NOMBRE_MAX_DES; i++) {
        if (ScoreTemp[i][0] == 1) {
            if (CalculeScoreChiffre(i+1, des) > 0) {
                ScoreTemp[i][1] = 1;
                ScoreTemp[i][2] = CalculeScoreChiffre(i+1, des);
                printf("%i - Vous pouez faire %d dans les %d\n", i+1, ScoreTemp[i][2], i+1);
            }
        }   
    }
    CalculeScoreBrelan(ScoreTemp, des);
    CalculeScoreCarre(ScoreTemp, des);
    CalculeScoreFullhouse(ScoreTemp, des);
    CalculeScorePetitesuite(ScoreTemp, des);
    CalculeScoreGrandesuite(ScoreTemp, des);
    CalculeScoreYams(ScoreTemp, des);
    CalculeScoreChance(ScoreTemp, des);
    //printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");// Fin delimitation des possibilités
    posibilite = 0;
    for (int i = 0; i < LONGEUR_TABLEAU; i++)
    {
        if (ScoreTemp[i][1] == 1)
        {
            posibilite++;
        }   
    }
    printf("━━━━━━━━━━━ Vous Avez %d Possibilités ━━━━━━━━━━━\n", posibilite);
    scoreEntre = 0;
    printf("Choisissez une lettre afin de rentrer un score parmit les choix ci-dessus\n");
    while (scoreEntre != 1) {
        if (posibilite > 0) {
            scanf("%c", &choix);
            switch (choix) {
                case '1':
                    if (ScoreTemp[0][0] == 1 && ScoreTemp[0][1] == 1)
                    {
                        ScoreTemp[0][0] = 0;
                        scoreFinal[0] = ScoreTemp[0][2];
                        scoreEntre = 1;
                    }            
                    break;
                case '2':
                    if (ScoreTemp[1][0] == 1 && ScoreTemp[1][1] == 1)
                    {
                        ScoreTemp[1][0] = 0;
                        scoreFinal[1] = ScoreTemp[1][2];
                        scoreEntre = 1;
                    }            
                    break;
                case '3':
                    if (ScoreTemp[2][0] == 1 && ScoreTemp[2][1] == 1)
                    {
                        ScoreTemp[2][0] = 0;
                        scoreFinal[2] = ScoreTemp[2][2];
                        scoreEntre = 1;
                    }            
                    break;
                case '4':
                    if (ScoreTemp[3][0] == 1 && ScoreTemp[3][1] == 1)
                    {
                        ScoreTemp[3][0] = 0;
                        scoreFinal[3] = ScoreTemp[3][2];
                        scoreEntre = 1;
                    }            
                    break;
                case '5':
                    if (ScoreTemp[4][0] == 1 && ScoreTemp[4][1] == 1)
                    {
                        ScoreTemp[4][0] = 0;
                        scoreFinal[4] = ScoreTemp[4][2];
                        scoreEntre = 1;
                    }            
                    break;
                case '6':
                    if (ScoreTemp[5][0] == 1 && ScoreTemp[5][1] == 1)
                    {
                        ScoreTemp[5][0] = 0;
                        scoreFinal[5] = ScoreTemp[5][2];
                        scoreEntre = 1;
                    }            
                    break;
                case 'B':
                    //printf("%d == 1 && %d == 1)",ScoreTemp[8][0], ScoreTemp[8][1]);
                    if (ScoreTemp[8][0] == 1 && ScoreTemp[8][1] == 1)
                    {
                        ScoreTemp[8][0] = 0;
                        scoreFinal[8] = ScoreTemp[8][2];
                        scoreEntre = 1;
                    }            
                    break;
                case 'C':
                    if (ScoreTemp[9][0] == 1 && ScoreTemp[9][1] == 1)
                    {
                        ScoreTemp[9][0] = 0;
                        scoreFinal[9] = ScoreTemp[9][2];
                        scoreEntre = 1;
                    }            
                    break;
                case 'F':
                    if (ScoreTemp[10][0] == 1 && ScoreTemp[10][1] == 1)
                    {
                        ScoreTemp[10][0] = 0;
                        scoreFinal[10] = ScoreTemp[10][2];
                        scoreEntre = 1;
                    }            
                    break;
                case 'P':
                    if (ScoreTemp[11][0] == 1 && ScoreTemp[11][1] == 1)
                    {
                        ScoreTemp[11][0] = 0;
                        scoreFinal[11] = ScoreTemp[11][2];
                        scoreEntre = 1;
                    }            
                    break;
                case 'G':
                    if (ScoreTemp[12][0] == 1 && ScoreTemp[12][1] == 1)
                    {
                        ScoreTemp[12][0] = 0;
                        scoreFinal[12] = ScoreTemp[12][2];
                        scoreEntre = 1;
                    }            
                    break;
                case 'Y':
                    if (ScoreTemp[13][0] == 1 && ScoreTemp[13][1] == 1)
                    {
                        ScoreTemp[13][0] = 0;
                        scoreFinal[13] = ScoreTemp[13][2];
                        scoreEntre = 1;
                    }            
                    break;
                case 'L':
                    if (ScoreTemp[14][0] == 1 && ScoreTemp[14][1] == 1)
                    {
                        ScoreTemp[14][0] = 0;
                        scoreFinal[14] = ScoreTemp[14][2];
                        scoreEntre = 1;
                    }            
                    break;
                default:
                    break;
            }
        } else {
            printf("Vous n'avez aucune possibilité, vous devez donc barrer une case (Choisiser une case):\n");
            if (ScoreTemp[0][0] == 1) {
                printf("1 - Vous pouvez barrer les 1\n");
            }
            if (ScoreTemp[1][0] == 1) {
                printf("2 - Vous pouvez barrer les 2\n");
            }
            if (ScoreTemp[2][0] == 1) {
                printf("3 - Vous pouvez barrer les 3\n");
            }
            if (ScoreTemp[3][0] == 1) {
                printf("4 - Vous pouvez barrer les 4\n");
            }
            if (ScoreTemp[4][0] == 1) {
                printf("5 - Vous pouvez barrer les 5\n");
            }
            if (ScoreTemp[5][0] == 1) {
                printf("6 - Vous pouvez barrer les 6\n");
            }
            // if (ScoreTemp[6][0] == 1) {
            //     printf("U - Vous pouvez barrer le Bonus\n");
            // }
            if (ScoreTemp[8][0] == 1) {
                printf("B - Vous pouvez barrer le Brelan\n");
            }
            if (ScoreTemp[9][0] == 1) {
                printf("C - Vous pouvez barrer le Carre\n");
            }
            if (ScoreTemp[10][0] == 1) {
                printf("F - Vous pouvez barrer le Full House\n");
            }
            if (ScoreTemp[11][0] == 1) {
                printf("P - Vous pouvez barrer la Petite Suite\n");
            }
            if (ScoreTemp[12][0] == 1) {
                printf("G - Vous pouvez barrer la Grande Suite\n");
            }
            if (ScoreTemp[13][0] == 1) {
                printf("Y - Vous pouvez barrer le Yams\n");
            }
            if (ScoreTemp[14][0] == 1) {
                printf("L - Vous pouvez barrer la Chance\n");
            }
            int abarre = 0;
            while (abarre == 0) {
                scanf("%c", &choixBarre);
                switch(choixBarre) {
                    case '1':
                        if (ScoreTemp[0][0] == 1) {
                            printf("1 - Vous avez barré les 1\n");
                            ScoreTemp[0][0] = 0;
                            abarre = 1;
                            scoreEntre = 1; 
                        }
                        break;
                    case '2':
                        if (ScoreTemp[1][0] == 1) {
                            printf("2 - Vous avez barré les 2\n");
                            ScoreTemp[1][0] = 0;
                            abarre = 1;
                            scoreEntre = 1; 
                        }
                        break;
                    case '3':
                        if (ScoreTemp[2][0] == 1) {
                            printf("3 - Vous avez barré les 3\n");
                            ScoreTemp[2][0] = 0;
                            abarre = 1;
                            scoreEntre = 1; 
                        }
                        break;
                    case '4':
                        if (ScoreTemp[3][0] == 1) {
                            printf("4 - Vous avez barré les 4\n");
                            ScoreTemp[3][0] = 0;
                            abarre = 1;
                            scoreEntre = 1; 
                        }
                        break;
                    case '5':
                        if (ScoreTemp[4][0] == 1) {
                            printf("5 - Vous avez barré les 5\n");
                            ScoreTemp[4][0] = 0;
                            abarre = 1;
                            scoreEntre = 1; 
                        }
                        break;
                    case '6':
                        if (ScoreTemp[5][0] == 1) {
                            printf("6 - Vous avez barré les 6\n");
                            ScoreTemp[5][0] = 0;
                            abarre = 1;
                            scoreEntre = 1; 
                        }
                        break;
                    // case 'U':
                    //     if (ScoreTemp[6][0] == 1) {
                    //         printf("U - Vous avez barré le Bonus\n");
                    //         ScoreTemp[6][0] = 0;
                    //         abarre = 1;
                    //         scoreEntre = 1; 
                    //     }
                    //     break;
                    case 'B':
                        if (ScoreTemp[8][0] == 1) {
                            printf("B - Vous avez barré le Brelan\n");
                            ScoreTemp[8][0] = 0;
                            abarre = 1;
                            scoreEntre = 1; 
                        }
                        break;
                    case 'C':
                        if (ScoreTemp[9][0] == 1) {
                            printf("C - Vous avez barré le Carre\n");
                            ScoreTemp[9][0] = 0;
                            abarre = 1;
                            scoreEntre = 1; 
                        }
                        break;
                    case 'F':
                        if (ScoreTemp[10][0] == 1) {
                            printf("F - Vous avez barré le Full House\n");
                            ScoreTemp[10][0] = 0;
                            abarre = 1;
                            scoreEntre = 1; 
                        }
                        break;
                    case 'P':
                        if (ScoreTemp[11][0] == 1) {
                            printf("P - Vous avez barré la Petite Suite\n");
                            ScoreTemp[11][0] = 0;
                            abarre = 1;
                            scoreEntre = 1; 
                        }
                        break;
                    case 'G':
                        if (ScoreTemp[12][0] == 1) {
                            printf("G - Vous avez barré la Grande Suite\n");
                            ScoreTemp[12][0] = 0;
                            abarre = 1;
                            scoreEntre = 1; 
                        }
                        break;
                    case 'Y':
                        if (ScoreTemp[13][0] == 1) {
                            printf("Y - Vous avez barré le Yams\n");
                            ScoreTemp[13][0] = 0;
                            abarre = 1;
                            scoreEntre = 1; 
                        }
                        break;
                    case 'L':
                        if (ScoreTemp[14][0] == 1) {
                            printf("L - Vous avez barré la Chance\n");
                            ScoreTemp[14][0] = 0;
                            abarre = 1;
                            scoreEntre = 1; 
                        }
                        break;
                }
            }   
        }
    }
    CalculeTotalScore(scoreFinal);
    AjoutBonus(scoreFinal); // Ajout du bonus si les conditions sont remplies
    CalculeTotalScore(scoreFinal);// recalcul si au cas ou il y a eu le Bonus ajouté
}

/**
 * @brief 
 * 
 * @param ScoreTemporaire 
 * @param tableau_des  
 */
void CalculeScoreBrelan(T_ScoreDes ScoreTemp, T_des des) {
    int brelan;
    for (int i = 1; i <= NOMBRE_MAX_DES; i++)
    {
        //printf("test brelan de %d\n", i);
        brelan = 0;
        for (int y = 0; y < NOMBRE_DES; y++)
        {
            if (des[y] == i )
            {
                brelan = brelan + i;
            }
            
        }
        if(brelan >= 3*i  && ScoreTemp[8][0] == 1) {
           ScoreTemp[8][1] = 1; 
           ScoreTemp[8][2] = 3*i;
           printf("B - Vous pouez faire %d dans Brelan\n", ScoreTemp[8][2]);
        }
    }
}

/**
 * @brief 
 * 
 * @param ScoreTemporaire 
 * @param tableau_des  
 */
void CalculeScoreCarre(T_ScoreDes ScoreTemp, T_des des) {
    int Carre;
    for (int i = 1; i <= NOMBRE_MAX_DES; i++)
    {
        //printf("test Carre de %d\n", i);
        Carre = 0;
        for (int y = 0; y < NOMBRE_DES; y++)
        {
            if (des[y] == i )
            {
                Carre = Carre + i;
            }
            
        }
        if(Carre >= 4*i && ScoreTemp[9][0] == 1) {
           ScoreTemp[9][1] = 1; 
           ScoreTemp[9][2] = 4*i;
           printf("C - Vous pouez faire %d dans Carre\n", ScoreTemp[9][2]);
        }
    }
}

/**
 * @brief Calcul si le fullHouse est possible
 * 
 * @param ScoreTemp 
 * @param des 
 */
void CalculeScoreFullhouse(T_ScoreDes ScoreTemp, T_des des) {
    int fullhouse;
    fullhouse = 0;
    T_des desTrie;
    tri(des, desTrie);
    if (desTrie[0] == desTrie[1] && desTrie[1] == desTrie[2] && desTrie[3] == desTrie[4]) {
        fullhouse = 1;
    } else if (desTrie[0] == desTrie[1] && desTrie[2] == desTrie[3] && desTrie[3] == desTrie[4]) {
        fullhouse = 1;
    }
    if (fullhouse == 1) {
        if (ScoreTemp[10][0] == 1){
            ScoreTemp[10][1] = 1; 
            ScoreTemp[10][2] = 25;
            printf("F - Vous pouez faire %d dans Full House\n", ScoreTemp[10][2]);
        }                        
    }
}

/**
 * @brief regarde si la petite suite est possible
 * 
 * @param ScoreTemp 
 * @param des 
 */
void CalculeScorePetitesuite(T_ScoreDes ScoreTemp, T_des des) {
   int suite;
	T_des destrie;
    suite = 0;
	tri(des, destrie);
	for (int i = NOMBRE_DES-1; i>0; i--) {
        if (destrie[i] == destrie[i-1] + 1) {
            suite++;
        }
    }
	if (suite >= 3 && ScoreTemp[11][0] == 1) {
            ScoreTemp[11][1] = 1;
            ScoreTemp[11][2] = 30;
            printf("P - Vous pouez faire %d dans Petite Suite\n", ScoreTemp[11][2]);
    }
}

/**
 * @brief Regarde si la grande suite est possible
 * 
 * @param ScoreTemp 
 * @param des 
 */
void CalculeScoreGrandesuite(T_ScoreDes ScoreTemp, T_des des) {
    int suite;
	T_des destrie;
    suite = 0;
	tri(des, destrie);
	for (int i = NOMBRE_DES-1; i>0; i--) {
        if (destrie[i] == destrie[i-1] + 1) {
            suite++;
        }
    }
    if (suite >= 4 && ScoreTemp[12][0] == 1) {
        ScoreTemp[12][1] = 1;
        ScoreTemp[12][2] = 40;
        printf("G - Vous pouez faire %d dans Grande Suite\n", ScoreTemp[12][2]);
    }    
}

/**
 * @brief Tri le tableau de des en entré et redonne un tableau trié pour faire les comparaison(Garder un talbeau original pour garder l'ordre d'affichage des dès)
 * 
 * @param desIn 
 * @param desOut 
 */
void tri(T_des desIn, T_des desOut) {
    int tmp;
    for (int i = 0; i < NOMBRE_DES; i++) {
        desOut[i] = desIn[i];
    }
    
    for (int i = 1; i < 5; i++) {
        for (int y = 0; y < 5 - i; y++) {
            if (desOut[y] > desOut[y + 1]) {
                tmp = desOut[y];
                desOut[y] = desOut[y+1];
                desOut[y+1] = tmp;
            }
        }
    }
}

/**
 * @brief Calcule si le Yams est possible
 * 
 * @param ScoreTemp 
 * @param des 
 */
void CalculeScoreYams(T_ScoreDes ScoreTemp, T_des des) {
    int Yams;
    for (int i = 1; i <= NOMBRE_MAX_DES; i++)
    {
        Yams = 0;
        for (int y = 0; y < NOMBRE_DES; y++)
        {
            if (des[y] == i )
            {
                Yams = Yams + i;
            }
        }
        if(Yams == 5*i) {
           ScoreTemp[13][1] = 1; 
           ScoreTemp[13][2] = 50;
           printf("Y - Vous pouez faire %d dans Yams\n", ScoreTemp[13][2]);
        }
        if(Yams == 5*i && ScoreTemp[10][0] == 1)/*Test si le full house est prit*/ {
           ScoreTemp[10][1] = 1; 
           ScoreTemp[10][2] = 25;
           printf("F - Vous pouez faire %d dans Full House\n", ScoreTemp[10][2]);
        }
    }
}

/**
 * @brief Fait la somme de tout les des pour la case chance
 * 
 * @param ScoreTemp 
 * @param des 
 */
void CalculeScoreChance(T_ScoreDes ScoreTemp, T_des des) {
    int chance;
    chance = 0;
    for (int i = 0; i < NOMBRE_DES; i++) {
        chance = chance + des[i];
    }

    if(ScoreTemp[14][0] == 1) {
        ScoreTemp[14][1] = 1; 
        ScoreTemp[14][2] = chance;
        printf("L - Vous pouez faire %d dans Chance\n", ScoreTemp[14][2]);
    }
}
/**
 * @brief Regarde si le score remplie la condition pour avoir le bonus
 * 
 * @param score 
 */
void AjoutBonus(T_Score score) {
    if (score[7] > 62)
    {
        score[6] = 35;
    }
    
}
/**
 * @brief Calcule les totaux du score du tableau en entrés
 * 
 * @param score 
 */
void CalculeTotalScore(T_Score score) {
    int totalsup;
    int totalinf;
    totalsup = 0;
    totalinf = 0;
    for (int i = 0; i < 7; i++)
    {
        totalsup = totalsup + score[i];
    }
    score[7] = totalsup;
    for (int i = 8; i <= 14; i++)
    {
        totalinf = totalinf + score[i];
    }
    score[15] = totalinf;
    score[16] = score[7] + score[15];
}

void resetScoretmp(T_ScoreDes Scoretmp) {
    for (int i = 0; i < LONGEUR_TABLEAU; i++)
    {
        Scoretmp[i][1] = 0;
        Scoretmp[i][2] = 0;
    }    
}

/**
 * @brief Affiche le nom du vainqueur ainsi que le score final des 2 joueurs
 * 
 * @param joueur1 
 * @param joueur2 
 * @param scoreJoueur1 
 * @param scoreJoueur2 
 */
void AfficheVainqueur(T_nom joueur1, T_nom joueur2, T_Score scoreJoueur1, T_Score scoreJoueur2){
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    if (scoreJoueur1[16] > scoreJoueur2[16]) {
        printf("Bienjoué à %s qui remporte la partie !\n", joueur1);
    } else {
        printf("Bienjoué à %s qui remporte la partie !\n", joueur1);
    }
    printf("%s a fait %d points\n", joueur1, scoreJoueur1[16]);
    printf("%s a fait %d points\n", joueur2, scoreJoueur2[16]);
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
}