#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "biblioteca.h"

void form_list(Team** head, int numberOfTeams, FILE* in){
    int i,j;
    for(i = 0; i < numberOfTeams; i++){
        Team* team = (Team*)malloc(sizeof(Team));
        team->next = NULL;
        team->points = 0;
        fscanf(in, "%d", &team->numberOfPlayers);
        //se citește separat spațiul dintre numărul de membri și numele echipei
        fgetc(in);
        fgets(team->name, 30, in);
        //se formatează corect numele echipei (s-a citit împreună cu \n)
        team->name[strlen(team->name)-2] = '\0';
        team->members = (Player*)malloc(team->numberOfPlayers * sizeof(Player));
        for(j = 0; j < team->numberOfPlayers; j++){
            fscanf(in, "%s %s %d", team->members[j].firstName, team->members[j].secondName, &team->members[j].points);
            team->points += team->members[j].points;
        }
        //se calculează media aritmetică specifică fiecărei echipe
        team->points = team->points / (team->numberOfPlayers * 1.0);
        //se adaugă echipa la începutul listei
        team->next = *head;
        *head = team;
    }
}

int teamsRemaining(int numberOfTeams){
    int bestTeams = 1;
    //se folosesc operațiile pe biți pentru a obține cea mai mare putere a lui 2 mai mică decât numărul total de echipe
    while(bestTeams < numberOfTeams)
        bestTeams = bestTeams << 1;
    if(bestTeams > numberOfTeams)
        bestTeams = bestTeams >> 1;
    return bestTeams;
}

void eliminateWorstTeams(Team** head, int worstTeams){
    int i;
    Team* iter;
    //se repetă ciclul căutare minim + ștergere minim pentru câte elemente trebuie șterse
    for(i = 0; i < worstTeams; i++){
        float minTeamPoints = INT_MAX;
        char minTeamPointsName[30] = "";
        iter = *head;
        //căutare minim
        while(iter != NULL){
            if(iter->points < minTeamPoints){
                minTeamPoints = iter->points;
                strcpy(minTeamPointsName, iter->name);
            }
            iter = iter->next;
        }
        //ștergere minim, dacă minimul este primul element din listă
        if(strcmp((*head)->name, minTeamPointsName) == 0){
            Team* headcopy = *head;
            *head = (*head)->next;
            free(headcopy);
        }else{//ștergere minim, dacă minimul NU este primul element din listă
            //elementul precedent elementului curent
            Team* iter_prev = *head;
            iter = *head;
            int deleted = 0;
            //nu se mai continuă parcurgerea listei dacă am șters deja elementul
            while(iter != NULL && deleted == 0)
                if(strcmp(iter->name, minTeamPointsName) != 0){
                    iter_prev = iter;
                    iter = iter->next;
                }else{
                    //ștergerea efectivă
                    iter_prev->next = iter->next;
                    free(iter);
                    deleted = 1;
                }
        }
    }
}

void write_list(Team* head, FILE *out){
    Team* iter = head;
    //cât timp elementul curent NU este NULL, i se afișează numele și se trece la next
    while(iter != NULL){
        fprintf(out, "%s\n", iter->name);
        iter = iter->next;
    }
}