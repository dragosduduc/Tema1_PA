#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    ///printf("%d %d %d %d %d\n", task[0], task[1], task[2], task[3], task[4]);

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
    form_list(&head, numberOfTeams, d_in);

    fclose(d_in);

    //se deschide fișierul de ieșire
    FILE* out;
    if((out = fopen("/home/dragosduduc/TEMA1_PA/r.out", "wt")) == NULL){
        printf("Fisierul de iesire nu a putut fi deschis.\n");
        exit(1);
    }
    
    //se scrie lista formată în fișierul de ieșire
    write_list(head, out);
    
    fclose(out);

    return 0;
}