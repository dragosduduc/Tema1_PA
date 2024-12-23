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
    Team* mainList = NULL;
    createInitialList(&mainList, numberOfTeams, d_in);

    fclose(d_in);

    //se rezolvă cerința 2, dacă se cere
    int maxPower;
    if(task[1] == 1){

        //se calculează cea mai mare putere a lui 2 mai mică decât numărul total de echipe
        maxPower = teamsRemaining(numberOfTeams);

        int teamsToEliminate = numberOfTeams - maxPower;

        //se elimină echipele cu cele mai mici punctaje, pentru a se ajunge la un număr de echipe - putere a lui 2
        eliminateWorstTeams(&mainList, teamsToEliminate);
    }

    //se deschide fișierul de ieșire
    FILE* out;
    if((out = fopen(argv[3], "wt")) == NULL){
        printf("Fisierul de intrare nu a putut fi deschis.\n");
        exit(1);
    }
    
    //se scrie lista formată în fișierul de ieșire
    writeList(mainList, out);
    
    //se rezolvă cerința 3, dacă se cere
    Team* quarterFinalists = NULL;
    if(task[2] == 1){

        //se creează și se populează coada de meciuri
        Queue* q;
        q = createQueue();
        moveMatchesFromListToQueue(q, &mainList, maxPower);

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

            //când se ajunge la runda de 16 echipe, cele 8 echipe învingătoare se copiază într-o listă
            if(maxPower == 16)
                copyTeamsFromStackToList(winnerStackTop, &quarterFinalists);

            //se șterg echipele care au pierdut în runda curentă
            deleteStack(&loserStackTop);
            
            //se înjumătățește numărul de echipe - rămân doar câștigătorii -, se afișează și se pun înapoi în coada de meciuri
            maxPower = maxPower >> 1;
            fprintf(out, "\nWINNERS OF ROUND NO:%d\n", roundNumber);
            moveMatchesFromStackToQueue(q, &winnerStackTop, maxPower, out);
        }

        //echipa câștigătoare, care a rămas în stivă (la ultimul ciclu, maxPower intră în funcția putMatchesFromStackToQueue cu valoarea 0), se scoate din stivă și se afișează
        Team* champs = pop(&winnerStackTop);
        fprintf(out, "%-33s -  %.2f\n", champs->name, champs->points);

        //se eliberează spațiul de memorie alocat cozii de meciuri
        freeQueue(q);
    }

    //se rezolvă cerința 4, dacă se cere
    Node* BST = NULL;
    if(task[3] == 1){
        fprintf(out, "\nTOP 8 TEAMS:\n");

        //se parcurge lista celor mai bune 8 echipe și fiecare echipă se pune în BST
        Team* iter = quarterFinalists;
        while(iter != NULL){
            BST = insertInBST(BST, iter);
            iter = iter->next;
        }

        //se șterge lista celor mai bune 8 echipe
        iter = quarterFinalists;
        while(iter != NULL){
            Team* temp = iter;
            iter = iter->next;
            freeTeam(temp);
        }

        //se parcurge BST-ul în ordine descrescătoare
        inorderReverse(BST, out);
    }

    //se rezolvă cerința 5, dacă se cere
    Node* AVL = NULL;
    if(task[4] == 1){

        //se parcurge crescător BST-ul, iar echipele se adaugă la începului listei (care, la final, va conține echipele în ordine descrescătoare)
        Team* quarterFinalistsRanked = NULL;
        createListFromTree(BST, &quarterFinalistsRanked);

        //se eliberează memoria alocată BST-ului
        freeTree(BST);

        //echipele din lista sortată descrescător se pun, pe rând, în AVL
        Team* iter = quarterFinalistsRanked;
        while(iter != NULL){
            AVL = insertInAVL(AVL, iter);
            iter = iter->next;
        }

        //se afișează echipele de pe nivelul 2
        fprintf(out, "\nTHE LEVEL 2 TEAMS ARE:\n");
        fprintLevelInTree(out, AVL, 2);

        //se eliberează memoria alocată listei ordonate de echipe
        iter = quarterFinalistsRanked;
        while(iter != NULL){
            Team* temp = iter;
            iter = iter->next;
            freeTeam(temp);
        }

        //se eliberează memoria alocată AVL-ului
        freeTree(AVL);
    }

    fclose(out);

    return 0;
}