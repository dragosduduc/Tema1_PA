#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void write_list(Team* head, FILE *out){
    Team* iter = head;
    //cât timp elementul curent NU este NULL, i se afișează numele și se trece la next
    while(iter != NULL){
        fprintf(out, "%s\n", iter->name);
        iter = iter->next;
    }
}