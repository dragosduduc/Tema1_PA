#include <stdio.h>
#include <stdlib.h>
#include "biblioteca.h"
int main(int argc, char* argv[]){

    //se deschide fișierul de intrare c.in și se extrag cerințele
    int task[5];
    FILE* c_in;
    if((c_in = fopen(argv[1], "rt")) == NULL){
        printf("Fisierul de intrare nu a putut fi deschis.\n");
        exit(1);
    }
    fscanf(c_in, "%d %d %d %d %d", &task[0], &task[1], &task[2], &task[3], &task[4]);
    fclose(c_in);

    //se deschide fișierul de intrare d.in
    FILE* d_in;
    if((d_in = fopen(argv[2], "rt")) == NULL){
        printf("Fisierul de intrare nu a putut fi deschis.\n");
        exit(1);
    }

    //dacă cerința 1 NU se cere, nu mai are sens continuarea execuției programului
    if(task[0] != 1) return 0;

    //se citește numărul de echipe din fișierul de intrare
    int numberOfTeams;
    fscanf(d_in, "%d", &numberOfTeams);

    //se formează lista de echipe
    Team* head = NULL;
    createInitialList(&head, numberOfTeams, d_in);

    fclose(d_in);

    //se rezolvă cerința 2, dacă se cere
    int maxPower;
    if(task[1] == 1){

        //se calculează cea mai mare putere a lui 2 mai mică decât numărul total de echipe
        maxPower = teamsRemaining(numberOfTeams);

        int teamsToEliminate = numberOfTeams - maxPower;

        //se elimină echipele cu cele mai mici punctaje, pentru a se ajunge la un număr de echipe - putere a lui 2
        eliminateWorstTeams(&head, teamsToEliminate);
    }

    //se deschide fișierul de ieșire
    FILE* out;
    if((out = fopen("/home/dragosduduc/TEMA1_PA/r.out", "wt")) == NULL){
        printf("Fisierul de iesire nu a putut fi deschis.\n");
        exit(1);
    }
    
    //se scrie lista formată în fișierul de ieșire
    writeList(head, out);
    
    //se rezolvă cerința 3, dacă se cere
    Team* quarterFinalists = NULL;
    if(task[2] == 1){

        //se creează și se populează coada de meciuri
        Queue* q;
        q = createQueue();
        putMatchesFromListToQueue(q, &head, maxPower);

        //se stabilește numărul rundei curente
        int roundNumber = 0;

        //se inițializează stivele pentru câștigători și pierzători și lista pentru ultimele opt echipe
        Team* winnerStackTop;
        Team* loserStackTop;

        //cât timp au mai rămas meciuri/runde de jucat, se joacă
        while(maxPower > 1){

            roundNumber++;
            winnerStackTop = loserStackTop = NULL;

            fprintf(out, "\n--- ROUND NO:%d\n", roundNumber);
            playMatches(q, &winnerStackTop, &loserStackTop, out);

            //dacă am ajuns la ultimele opt echipe, se trec pierzătorii rundei curente în lista ultimelor opt echipe (doar pierzătorii pentru că restul se vor pune în listă după următoarele runde, după actualizarea punctajului)
            if(maxPower <= 8)
                putTeamsFromStackToList(&loserStackTop, &quarterFinalists);
            
            //se înjumătățește numărul de echipe (rămân doar câștigătorii), se afișează și se pun înapoi în coada de meciuri
            maxPower = maxPower >> 1;
            fprintf(out, "\nWINNERS OF ROUND NO:%d\n", roundNumber);
            putMatchesFromStackToQueue(q, &winnerStackTop, maxPower, out);
        }

        //echipa câștigătoare, care a rămas în stivă (la ultimul ciclu, maxPower intră în funcția putMatchesFromStackToQueue cu valoarea 0), se scoate din stivă și se pune în lista ultimelor opt echipe
        Team* champs = pop(&winnerStackTop);
        champs->next = quarterFinalists;
        quarterFinalists = champs;
        fprintf(out, "%-33s -  %.2f\n", champs->name, champs->points);
    }

    //se rezolvă cerința 4, dacă se cere
    if(task[3] == 1){
        fprintf(out, "\nTOP 8 TEAMS:\n");
        writeListWithPoints(quarterFinalists, out);
    }

    fclose(out);

    return 0;
}