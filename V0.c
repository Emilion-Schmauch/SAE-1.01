#include <stdio.h>
#include <string.h>
#pragma warning(disable:4996 6031)

enum { max_participants = 100, max_concours = 20, max_caractère = 31 };
typedef struct {
    char prenom[max_caractère];
    char nom[max_caractère];
    unsigned int identifiant;
} Participant;

typedef struct {
    char nom[max_caractère];
    unsigned int identifiant;
    unsigned int score[max_participants]; // Index de score correspond aux identifiants des participants-1
    int etat_bilan[max_participants]; // Les index corespondent aux identifians des participants-1. Leurs valeurs correspondent au classement des participants.
}Concours;

/*
* Prend en paramètre la liste de participants(tableau de structures Participant, un prenom (str), un nom (str) et un identifiant(unsigned int)
* Renvoie 1 si l'inscription est possible, sinon 0.
*/
int test_validite_inscrire(const Participant liste_participants[], const char prenom[],
    const char nom[], const unsigned int nombre_participants) {
    for (unsigned int i = 0; i < nombre_participants; ++i) {
        if (strcmp(liste_participants[i].prenom, prenom) == 0 &&
            strcmp(liste_participants[i].nom, nom) == 0) {
            return 0;
        }
    }
    return 1;
}
/*
* Prend en paramètre un prenom (str), un nom (str), le nombre de participants déjà inscrits (unsigned int), un pointeur vers un participant,
la liste des concours (tableau de structures Concours) et le nombre de concours (unsigned int)
* Remplis la structure pointé avec les informations donnés en paramètre
* Ne renvoie rien
*/
void inscrire(char prenom[], char nom[], unsigned int nombre_participants, Participant* part_p, Concours liste_concours[], unsigned int nombre_concours) {
    part_p->identifiant = nombre_participants;
    strcpy(part_p->prenom, prenom);
    strcpy(part_p->nom, nom);
    for (unsigned int i = 0;i < nombre_concours;++i) {
        int plus_haut_rang = 0;
        for (unsigned int j = 0;j < nombre_participants-1;++j) {
            if (liste_concours[i].etat_bilan[j] > plus_haut_rang) {
                plus_haut_rang = liste_concours[i].etat_bilan[j];
            }
        }
        liste_concours[i].etat_bilan[nombre_participants - 1] = plus_haut_rang+1;
    }
    return;
}


void participants(const Participant liste_participants[], const Concours liste_concours[],
    unsigned int nombre_participants, unsigned int nombre_concours) {
    if (nombre_participants == 0) {
        printf("Aucun participant inscrit\n");
        return;
    }
    for (unsigned int i = 0; i < nombre_participants; ++i) {
        unsigned nombre_concours_part_actuel = 0;
        for (unsigned int j = 0; j < nombre_concours; ++j) {
            if (liste_concours[j].score[i] > 0) {
                nombre_concours_part_actuel += 1;
            }
        }
        printf("(%u) %s %s : %u concours\n", liste_participants[i].identifiant, liste_participants[i].prenom,
            liste_participants[i].nom, nombre_concours_part_actuel);
    }
    return;
}

int test_validite_creer(const Concours liste_concours[], const char nom_concours[], unsigned int nombre_concours) {
    for (unsigned int i = 0; i < nombre_concours; ++i) {
        if (strcmp(liste_concours[i].nom, nom_concours) == 0) {
            return 0;
        }
    }
    return 1;
}

void creer(const char nom_concours[], unsigned int identifiant, Concours* concours, unsigned int nombre_participants) {
    strcpy(concours->nom, nom_concours);
    concours->identifiant = identifiant;
    for (int i = 0; i < max_participants; ++i) {
        concours->score[i] = 0;
    }
    for (unsigned int i = 0;i < nombre_participants;++i) {
        concours->etat_bilan[i] = 1;
    }
    return;

}

void concours(const Concours liste_concours[], unsigned int nombre_concours) {
    if (nombre_concours == 0) {
        printf("Aucun concours cree\n");
        return;
    }

    for (unsigned int i = 0; i < nombre_concours; ++i) {
        unsigned int nombre_participants_score_non_nul = 0;
        for (unsigned int j = 0; j < max_participants; ++j) {
            if (liste_concours[i].score[j] > 0) {
                nombre_participants_score_non_nul++;
            }
        }
            printf("(%u) %s : %u %s\n", liste_concours[i].identifiant, liste_concours[i].nom, 
                nombre_participants_score_non_nul, nombre_participants_score_non_nul > 1 ? "paticipants":"participant");
    }
    return;
}

int test_validite_gain(
    unsigned int id_participant,
    unsigned int id_concours,
    int nb_points,
    const Concours liste_concours[],
    const Participant liste_participants[],
    unsigned int nombre_concours,
    unsigned int nombre_participants
) {
    int id_p_trouve = 0;
    int id_c_trouve = 0;
    unsigned int retour = 1;

    for (unsigned int i = 0; i < nombre_participants; ++i) {
        if (liste_participants[i].identifiant == id_participant) {
            id_p_trouve = 1;
            break;
        }
    }

    if (!id_p_trouve) {
        printf("Participant incorrect\n");
        retour = 0;
    }

    for (unsigned int i = 0; i < nombre_concours; ++i) {
        if (liste_concours[i].identifiant == id_concours) {
            id_c_trouve = 1;
            break;
        }
    }

    if (!id_c_trouve) {
        printf("Concours incorrect\n");
        retour = 0;
    }

    if (nb_points <= 0) {
        printf("Points incorrects\n");
        retour = 0;
    }
    return retour;
}

void gain(unsigned int id_participant, unsigned int id_concours, int nb_points, Concours liste_concours[]) {
    liste_concours[id_concours - 1].score[id_participant - 1] += nb_points;
    printf("Gain enregistre\n");
    return;
}

int test_validite_scores(const Participant liste_participants[], unsigned int id_participant, unsigned int nombre_participants) {
    for (unsigned int i = 0; i < nombre_participants; ++i) {
        if (liste_participants[i].identifiant == id_participant) {
            return 1;
        }
    }
    printf("Identifiant incorrect\n");
    return 0;
}

void scores(unsigned int id_participant, const Concours liste_concours[], const Participant liste_participants[],
    const unsigned int nombre_concours, const unsigned int nombre_participants) {
    printf("%s %s\n", liste_participants[id_participant - 1].prenom, liste_participants[id_participant - 1].nom);
    unsigned int id_concours;
    char nom_concours[max_caractère];
    unsigned int nb_points;
    unsigned int point_gagne = 0;
    for (unsigned int i = 0; i < nombre_concours; ++i) {
        nb_points = liste_concours[i].score[id_participant - 1];
        if (nb_points != 0) {
            point_gagne = 1;
            id_concours = liste_concours[i].identifiant;
            strcpy(nom_concours, liste_concours[i].nom);
            printf("(%u) %s : %u %s\n", id_concours, nom_concours, nb_points, nb_points > 1 ? "points":"point");
        }
        if (!point_gagne || nombre_concours == 0) {
            printf("Aucun gain\n");
        }
    }
    return;
}

int test_validite_classement_bilan(unsigned int id_concours, Concours liste_concours[], unsigned int nombre_concours) {
    unsigned int id_trouve = 0;
    for (unsigned int i = 0; i < nombre_concours; ++i) {
        if (liste_concours[i].identifiant == id_concours) {
            id_trouve = 1;
        }
    }
    if (!id_trouve) {
        printf("Identifiant incorrect\n");
        return 0;
    }
    return 1;
}
int test_participants_triees(const unsigned int liste_scores[], unsigned int nombre_participants) {
    for (unsigned int i = 0;i < nombre_participants-1;++i) {
        if (liste_scores[i] < liste_scores[i + 1]) {
            return 0;
        }
    }
    return 1;
}
void classement_bilan(Concours liste_concours[], const Participant liste_participants[]
    , unsigned int nombre_participants, unsigned int id_concours, unsigned int est_bilan) {

    printf("%s\n", liste_concours[id_concours - 1].nom);
    unsigned int score_positif_trouve = 0;
    for (unsigned int i = 0; i < nombre_participants; ++i) {
        if (liste_concours[id_concours - 1].score[i] > 0) {
            score_positif_trouve = 1;
        }
    }
    if (!score_positif_trouve) {
        printf("Aucun participant\n");
        return;
    }

    unsigned int index = 0;
    Participant liste_participants_triees[max_participants];
    unsigned int liste_scores_triees[max_participants];
    for (unsigned int i = 0;i < nombre_participants;++i) {
        liste_participants_triees[i] = liste_participants[i];
        liste_scores_triees[i] = liste_concours[id_concours-1].score[i];
    }
    while (!(test_participants_triees(liste_scores_triees,nombre_participants))) {
        if (liste_scores_triees[index] < liste_scores_triees[index+1]){
            unsigned int ancien= liste_scores_triees[index];
            liste_scores_triees[index] = liste_scores_triees[index + 1];
            liste_scores_triees[index + 1] = ancien;

            Participant ancien_participant=liste_participants_triees[index];
            liste_participants_triees[index] = liste_participants_triees[index + 1];
            liste_participants_triees[index + 1] = ancien_participant;
            }
        index == nombre_participants - 2 ? index = 0 : index++;
    }
    int ex = 0;
    int position;
    for (unsigned int i = 0; i < nombre_participants; ++i) {
        if(liste_scores_triees[i]>0){
        position = i + 1 - ex;
        if (liste_scores_triees[i] == liste_scores_triees[i + 1] || ex) {

            if (liste_scores_triees[i] > 1)
                printf("%uex -> (%u) %s %s : %u %s", position, liste_participants_triees[i].identifiant, liste_participants_triees[i].prenom,
                    liste_participants_triees[i].nom, liste_scores_triees[i], liste_scores_triees[i] > 1 ? "points" : "point");
            if (liste_scores_triees[i] == liste_scores_triees[i + 1]) ex++;
            else ex = 0;
        }
        else {
                printf("%u -> (%u) %s %s : %u %s", position, liste_participants_triees[i].identifiant, liste_participants_triees[i].prenom,
                    liste_participants_triees[i].nom, liste_scores_triees[i],liste_scores_triees[i]>1?"points":"point");
            ex = 0;
        }
        if (est_bilan) {

            int ancien_rang = liste_concours[id_concours - 1].etat_bilan[liste_participants_triees[i].identifiant - 1];
            int difference = ancien_rang - position;

            if (difference >= 0) {
                printf(" : +%d %s\n", difference, difference > 1 ? "places" : "place");
            }
            else {
                printf(" : %d %s\n", difference, difference < -1 ? "places" : "place");

            }
        }
                liste_concours[id_concours - 1].etat_bilan[liste_participants_triees[i].identifiant - 1] = position;
            }
            else printf("\n");
        
    }
    return;
}

int test_validite_comparaison(unsigned int id_concours,unsigned int id_concours2, const Concours liste_concours[], unsigned int nombre_concours, 
    unsigned int nombre_participants) {
    unsigned int id_trouve = 0;
    unsigned int participants_trouve = 0;
    for (unsigned int i = 0; i < nombre_concours; ++i) {
        if (liste_concours[i].identifiant == id_concours){
            id_trouve = 1;
        }
    }
    if (!id_trouve) {
        printf("Identifiant incorrect\n");
        return 0;
    }
    id_trouve = 0;
    for (unsigned int i = 0; i < nombre_concours; ++i) {
        if (liste_concours[i].identifiant == id_concours2) {
            id_trouve = 1;
        }
    }
    if (!id_trouve) {
        printf("Identifiant incorrect\n");
        return 0;
    }
    for (unsigned int i = 0; i < nombre_participants;++i) {
        if (liste_concours[id_concours - 1].score[i] > 0 || liste_concours[id_concours2 - 1].score[i] > 0) {
            participants_trouve += 1;
        }
    }
    if (participants_trouve==0) {
        printf("Aucun participant\n");
        return 0;
    }
    for (unsigned int i = 0; i < nombre_participants;++i) {
        if (liste_concours[id_concours - 1].score[i] == 0 && liste_concours[id_concours2 - 1].score[i] > 0) {
            printf("Concours incompatibles\n");
            return 0;
        }
    }
    return 1;
    
}

void comparaison(unsigned int id_concours, unsigned int id_concours2, const Concours liste_concours[], 
    unsigned int nombre_concours, unsigned int nombre_participants, Participant liste_participants[]) {
    printf("%s versus %s\n", liste_concours[id_concours2 - 1].nom, liste_concours[id_concours - 1].nom);
    for (unsigned int i = 0;i < nombre_participants;++i) {
        if (liste_concours[id_concours-1].score[i] > 0) {
            float pourcentage = (float)liste_concours[id_concours2 - 1].score[i] *100/ (float)liste_concours[id_concours - 1].score[i];
            printf("(%u) %s %s : %u points/%u points : %.2f%%\n", liste_participants[i].identifiant, liste_participants[i].prenom, liste_participants[i].nom,
                liste_concours[id_concours2 - 1].score[i], liste_concours[id_concours - 1].score[i], pourcentage);
        }
    }
    return;
}


int main()
{
    char commande[100];
    Participant liste_participants[max_participants];
    unsigned int nombre_participants = 0;
    char prenom[max_caractère];
    char nom[max_caractère];
    int validite;
    char nom_concours[max_caractère];
    Concours liste_concours[20];
    unsigned int nombre_concours = 0;
    unsigned int id_participant;
    unsigned int id_concours;
    unsigned int id_concours2;
    int nb_points;

    while (1)
    {
        scanf("%s", &commande);
        if (strcmp(commande, "EXIT") == 0)
        {
            break;
        }
        if (strcmp(commande, "INSCRIRE") == 0)
        {
            scanf(" %s %s", prenom, nom);
            validite = test_validite_inscrire(liste_participants, prenom, nom, nombre_participants);
            if (validite) {
                Participant part;
                inscrire(
                    prenom,nom,
                    nombre_participants + 1,&part,
                    liste_concours, nombre_concours);
                liste_participants[nombre_participants] = part;
                ++nombre_participants;
                printf("Inscription enregistree (%u)\n", nombre_participants);
            }
            else {
                printf("Nom incorrect\n");
            }
        }
        if (strcmp(commande, "PARTICIPANTS") == 0) {
            participants(liste_participants, liste_concours, nombre_participants, nombre_concours);
        }
        if (strcmp(commande, "CREER") == 0) {
            scanf(" %s", nom_concours);
            validite = test_validite_creer(liste_concours, nom_concours, nombre_concours);
            if (validite) {
                Concours concours;
                creer(nom_concours, nombre_concours + 1, &concours,nombre_participants);
                liste_concours[nombre_concours] = concours;
                nombre_concours++;
                printf("Creation enregistree (%u)\n", nombre_concours);

            }
            else {
                printf("Nom incorrect\n");
            }

        }
        if (strcmp(commande, "CONCOURS") == 0) {
            concours(liste_concours, nombre_concours);
        }
        if (strcmp(commande, "GAIN") == 0) {
            scanf(" %u %u %d", &id_participant, &id_concours, &nb_points);
            validite = test_validite_gain(id_participant, id_concours, nb_points, liste_concours
                , liste_participants, nombre_concours, nombre_participants);
            if (validite) {
                gain(id_participant, id_concours, nb_points, liste_concours);
            }

        }
        if (strcmp(commande, "SCORES") == 0) {
            scanf(" %u", &id_participant);
            validite = test_validite_scores(liste_participants, id_participant, nombre_participants);
            if (validite) {
                scores(id_participant, liste_concours, liste_participants
                    , nombre_concours, nombre_participants);
            }
        }
        if (strcmp(commande, "CLASSEMENT") == 0) {
            scanf(" %u", &id_concours);
            validite = test_validite_classement_bilan(id_concours, liste_concours, nombre_concours);
            if (validite) {
                classement_bilan(liste_concours, liste_participants
                    , nombre_participants, id_concours,0);
            }
        }
        if (strcmp(commande, "COMPARAISON") == 0) {
            scanf(" %u %u", &id_concours, &id_concours2);
            validite= test_validite_comparaison(id_concours,id_concours2, liste_concours, nombre_concours, nombre_participants);
            if (validite) {
                comparaison(id_concours, id_concours2, liste_concours, nombre_concours, nombre_participants, liste_participants);
            }
        }
        if (strcmp(commande, "BILAN") == 0) {
            scanf(" %u", &id_concours);
            validite = test_validite_classement_bilan(id_concours, liste_concours, nombre_concours);
            if (validite) {
                classement_bilan(liste_concours, liste_participants
                    , nombre_participants, id_concours, 1);
            }
        }
    }


    return 0;
}
