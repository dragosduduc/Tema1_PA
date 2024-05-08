#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "biblioteca.h"

void createInitialList(Team** head, int numberOfTeams, FILE* in){
    int i,j;
    for(i = 0; i < numberOfTeams; i++){
        Team* team = (Team*)malloc(sizeof(Team));
        team->next = NULL;
        team->points = 0;
        fscanf(in, "%d", &team->numberOfPlayers);
        //se citește separat spațiul dintre numărul de membri și numele echipei
        fgetc(in);
        fgets(team->name, 50, in);
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
        char minTeamPointsName[50] = "";
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

void writeList(Team* head, FILE* out){
    Team* iter = head;
    //cât timp elementul curent NU este NULL, i se afișează numele și se trece la next
    while(iter != NULL){
        fprintf(out, "%s\n", iter->name);
        iter = iter->next;
    }
}

void writeListWithPoints(Team* head, FILE* out){
    Team* iter = head;
    while(iter != NULL){
        fprintf(out, "%-33s -  %.2f\n", iter->name, iter->points);
        iter = iter->next;
    }
}

//createQueue din curs
Queue* createQueue(){
    Queue* q;
    q = (Queue*)malloc(sizeof(Queue));
    if(q == NULL) return NULL;
    q->front = NULL;
    q->rear = NULL;
    return q;
}

//enQueue din curs, modificată pentru elemente de tip Match
void enQueue(Queue* q, Match* game){
    if(q->rear == NULL){
        q->rear = game;
        q->front = game;
    }else{
        (q->rear)->next = game;
        q->rear = game;
    }
    game->next = NULL;
}

//isEmpty pentru cozi
int queueIsEmpty(Queue* q){
    return (q->front == NULL);
}

//deQueue din curs, modificată pentru elemente de tip Match
Match* deQueue(Queue* q){
    Match* aux;
    if(queueIsEmpty(q)){
        aux = (Match*)malloc(sizeof(Match));
        aux->team1 = (Team*)malloc(sizeof(Team));
        aux->team2 = (Team*)malloc(sizeof(Team));
        strcpy(aux->team1->name, "Queue is empty.");
        strcpy(aux->team2->name, "Queue is empty.");
        printf("Queue is empty.\n");
        return aux;
    }
    aux = q->front;
    q->front = (q->front)->next;
    aux->next = NULL;
    if(q->front == NULL)
        q->rear = NULL;
    return aux;
}

void moveMatchesFromListToQueue(Queue* q, Team** head, int bestTeams){
    int i;
    //din listă se scot câte două echipe alăturate și se pun într-un meci în coadă
    for(i = 0; i < bestTeams / 2; i++){
            Match* game = (Match*)malloc(sizeof(Match));
            game->team1 = *head;
            *head = (*head)->next;
            game->team2 = *head;
            *head = (*head)->next;
            enQueue(q, game);
    }
}

void writeQueue(Queue* q, FILE* out){
    while(!queueIsEmpty(q)){
        Match* cnt = (Match*)malloc(sizeof(Match));
        cnt = deQueue(q);
        fprintf(out, "%-32s - %32s\n", cnt->team1->name, cnt->team2->name);
        free(cnt);
    }
}

void push(Team** top, Team* team){
    team->next = *top;
    *top = team;
}

//isEmpty pentru stive
int stackIsEmpty(Team* top){
    return (top == NULL);
}

Team* pop(Team** top){
    Team* aux;
    if(stackIsEmpty(*top)){
        aux = (Team*)malloc(sizeof(Team));
        strcpy(aux->name, "Stack is empty.");
        printf("Stack is empty.\n");
        return aux;
    }
    aux = *top;
    *top = (*top)->next;
    aux->next = NULL;
    return aux;
}

void deleteStack(Team** top){
    Team* temp;
    while(!stackIsEmpty(*top)){
        temp = *top;
        *top = (*top)->next;
        free(temp);
    }
}

void moveMatchesFromStackToQueue(Queue* q, Team** stackTop, int bestTeams, FILE* out){
    int i;
    //din stivă se scot câte două echipe alăturate și se pun într-un meci în coadă
    for(i = 0; i < bestTeams / 2; i++){
            Match* game = (Match*)malloc(sizeof(Match));
            game->team1 = pop(stackTop);
            fprintf(out, "%-33s -  %.2f\n", game->team1->name, game->team1->points);
            game->team2 = pop(stackTop);
            fprintf(out, "%-33s -  %.2f\n", game->team2->name, game->team2->points);
            enQueue(q, game);
    }
}
void matchResult(Team* firstTeam, Team* secondTeam, Team** winners, Team** losers){
    int i;
    if(firstTeam->points > secondTeam->points){
        push(winners, firstTeam);
        push(losers, secondTeam);
        firstTeam->points++;
        for(i = 0; i < firstTeam->numberOfPlayers; i++)
            firstTeam->members[i].points++;
    }else{
        push(winners, secondTeam);
        push(losers, firstTeam);
        secondTeam->points++;
        for(i = 0; i < secondTeam->numberOfPlayers; i++)
            secondTeam->members[i].points++;
        }
}

void copyTeamsFromStackToList(Team* stack, Team** head ){
    while(stack != NULL){
        Team* temp = stack;
        stack = stack->next;
        Team* newElement = (Team*)malloc(sizeof(Team));
        *newElement = *temp;
        newElement->next = *head;
        *head = newElement;
    }
}

void playMatches(Queue* q, Team** win, Team** lose, FILE* out){
    while(!queueIsEmpty(q)){
                Match* cntMatch;
                cntMatch = deQueue(q);
                fprintf(out, "%-32s - %32s\n", cntMatch->team1->name, cntMatch->team2->name);
                matchResult(cntMatch->team1, cntMatch->team2, win, lose);
            }
}