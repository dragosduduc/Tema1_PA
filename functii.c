#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "biblioteca.h"

void createInitialList(Team** head, int numberOfTeams, FILE* in){
    int i,j;
    char buffer1[50], buffer2[50];
    for(i = 0; i < numberOfTeams; i++){
        Team* team = (Team*)malloc(sizeof(Team));
        team->next = NULL;
        team->points = 0;
        fscanf(in, "%d", &team->numberOfPlayers);
        //se citește separat spațiul dintre numărul de membri și numele echipei
        fgetc(in);
        //se citește numele echipei într-un buffer și se formatează
        fgets(buffer1, 50, in);
        buffer1[strlen(buffer1)-2] = '\0';
        if(buffer1[strlen(buffer1) - 1] == ' ')
            buffer1[strlen(buffer1) - 1] = '\0';
        //se alocă memorie la pointer-ul din structură și se copiază string-ul din buffer în memoria de la pointer
        team->name = (char*)malloc((strlen(buffer1) + 1) * sizeof(char));
        strcpy(team->name, buffer1);
        //se citesc, în mod asemănător, informațiile legate de jucători
        team->members = (Player*)malloc(team->numberOfPlayers * sizeof(Player));
        for(j = 0; j < team->numberOfPlayers; j++){
            fscanf(in, "%s %s %d", buffer1, buffer2, &team->members[j].points);
            if(buffer1[strlen(buffer1) - 1] == ' ')
                buffer1[strlen(buffer1) - 1] = '\0';
            if(buffer2[strlen(buffer2) - 1] == ' ')
                buffer2[strlen(buffer2) - 1] = '\0';
            team->members[j].firstName = (char*)malloc((strlen(buffer1) + 1) * sizeof(char));
            team->members[j].secondName = (char*)malloc((strlen(buffer2) + 1) * sizeof(char));
            strcpy(team->members[j].firstName, buffer1);
            strcpy(team->members[j].secondName, buffer2);
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

//aceeași funcție ca și writeList(functia precedentă), dar se afișează și punctajul echipelor
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

//scriere(cu format) din coadă
void writeQueue(Queue* q, FILE* out){
    while(!queueIsEmpty(q)){
        Match* cnt = (Match*)malloc(sizeof(Match));
        cnt = deQueue(q);
        fprintf(out, "%-32s - %32s\n", cnt->team1->name, cnt->team2->name);
        free(cnt);
    }
}

//push din curs, modificată pentru tipul Team
void push(Team** top, Team* team){
    team->next = *top;
    *top = team;
}

//isEmpty pentru stive
int stackIsEmpty(Team* top){
    return (top == NULL);
}

//pop din curs, modificată pentru tipul Team(returnează chiar elementul - același bloc de memorie)
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

//deleteStack din curs
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
    //pe baza punctajului pe echipă, se stabilesc câștigătorii și pierzătorii, se pun în stivele corespunzătoare și se actualizează punctajele
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

//se COPIAZĂ(dintr-un bloc de memorie în altul) echipele dintr-o stivă într-o listă
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

//se scot meciurile din coadă și se stabilește rezultatul lor folosind funcția matchResult
void playMatches(Queue* q, Team** win, Team** lose, FILE* out){
    while(!queueIsEmpty(q)){
                Match* cntMatch;
                cntMatch = deQueue(q);
                fprintf(out, "%-32s - %32s\n", cntMatch->team1->name, cntMatch->team2->name);
                matchResult(cntMatch->team1, cntMatch->team2, win, lose);
            }
}

//se creează un nod pe baza informațiilor echipei date ca parametru
Node* createNode(Team* team){
    int i;
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->numberOfPlayers = team->numberOfPlayers;
    newNode->members = (Player*)malloc(newNode->numberOfPlayers * sizeof(Player));
    newNode->points = team->points;
    newNode->name = (char*)malloc((strlen(team->name) + 1) * sizeof(char));
    strcpy(newNode->name, team->name);
    for(i = 0; i < team->numberOfPlayers; i++){
        newNode->members[i].firstName = (char*)malloc((strlen(team->members[i].firstName) + 1) * sizeof(char));
        newNode->members[i].secondName = (char*)malloc((strlen(team->members[i].secondName) + 1) * sizeof(char));
        strcpy(newNode->members[i].firstName, team->members[i].firstName);
        strcpy(newNode->members[i].secondName, team->members[i].secondName);
        newNode->members[i].points = team->members[i].points;
    }
    return newNode;
}

Node* insertInBST(Node* root, Team* team){
    if(root == NULL){
        //se creează un nou nod și se pune la finalul BST-ului
        Node* newNode = createNode(team);
        newNode->left = newNode->right = NULL;
        return newNode;
    }
    //dacă key < (sau >) points, se apelează funcția pentru subarborele stâng (sau drept)
    if(team->points < root->points)
        root->left = insertInBST(root->left, team);
    else if(team->points > root->points)
        root->right = insertInBST(root->right, team);
    //dacă key == points, se compară lexicografic numele echipelor și se apelează funcția pentru subarborele corespunzător
    else if(team->points == root->points){
        if(strcmp(team->name, root->name) < 0)
            root->left = insertInBST(root->left, team);
        else if(strcmp(team->name, root->name) > 0)
            root->right = insertInBST(root->right, team);
    }
    return root;
}

//inorder din curs, dar elementele se scriu în ordine descrescătoare (right, root, left)
void inorderReverse(Node* root, FILE* out){
    if(root != NULL){
        inorderReverse(root->right, out);
        fprintf(out, "%-33s -  %.2f\n", root->name, root->points);
        inorderReverse(root->left, out);
    }
}

//printLevel din curs - ROOT SE CONSIDERĂ PE NIVEL 0
void fprintLevelInTree(FILE* out, Node* root, int level){
    if(root == NULL) return;
    if(level == 0) fprintf(out, "%s\n", root->name);
    else if(level > 0){
        fprintLevelInTree(out, root->right, level - 1);
        fprintLevelInTree(out, root->left, level - 1);
    }
}

//max din curs
int max(int a, int b){
    return ((a > b) ? a : b);
}

//nodeHeight din curs
int nodeHeight(Node* root){
    if(root == NULL) return -1;
    else return root->height;
}

//se creează o echipă pe baza informațiilor nodului dat ca parametru
Team* copyTeamFromNode(Node* node){
    int i;
    Team* newTeam = (Team*)malloc(sizeof(Team));
    newTeam->numberOfPlayers = node->numberOfPlayers;
    newTeam->members = (Player*)malloc(newTeam->numberOfPlayers * sizeof(Player));
    newTeam->points = node->points;
    newTeam->name = (char*)malloc((strlen(node->name) + 1) * sizeof(char));
    strcpy(newTeam->name, node->name);
    for(i = 0; i < node->numberOfPlayers; i++){
        newTeam->members[i].firstName = (char*)malloc((strlen(node->members[i].firstName) + 1) * sizeof(char));
        newTeam->members[i].secondName = (char*)malloc((strlen(node->members[i].secondName) + 1) * sizeof(char));
        strcpy(newTeam->members[i].firstName, node->members[i].firstName);
        strcpy(newTeam->members[i].secondName, node->members[i].secondName);
        newTeam->members[i].points = node->members[i].points;
    }
    return newTeam;
}

//se parcurge lista în inordine și elementele se copiază într-o listă separată (elementele vor fi sortate crescător în listă)
void createListFromTree(Node* root, Team** newList){
    if(root != NULL){
        createListFromTree(root->left, newList);
        Team* newTeam = copyTeamFromNode(root);
        newTeam->next = *newList;
        *newList = newTeam;
        createListFromTree(root->right, newList);
    }
}

//rightRotation din curs
Node* rightRotation(Node* z){
    Node* y = z->left;
    Node* T3 = y->right;
    y->right = z;
    z->left = T3;
    z->height = max(nodeHeight(z->left), nodeHeight(z->right)) + 1;
    y->height = max(nodeHeight(y->left), nodeHeight(y->right)) + 1;
    return y;
}

//leftRotation din curs
Node* leftRotation(Node* z){
    Node* y = z->right;
    Node* T2 = y->left;
    y->left = z;
    z->right = T2;
    z->height = max(nodeHeight(z->left), nodeHeight(z->right)) + 1;
    y->height = max(nodeHeight(y->left), nodeHeight(y->right)) + 1;
    return y;
}

//LRRotation din curs
Node* LRRotation(Node* z){
    z->left = leftRotation(z->left);
    return rightRotation(z);
}

//RLRotation din curs
Node* RLRotation(Node* z){
    z->right = rightRotation(z->right);
    return leftRotation(z);
}

Node* insertInAVL(Node* root, Team* team){
    if(root == NULL){
        //se creează un nou nod și se pune la finalul AVL-ului
        Node* newNode = createNode(team);
        newNode->height = 0;
        newNode->left = newNode->right = NULL;
        return newNode;
    }
    //dacă key < (sau >) points, se apelează funcția pentru subarborele stâng (sau drept)
    if(team->points < root->points)
        root->left = insertInAVL(root->left, team);
    else if(team->points > root->points)
        root->right = insertInAVL(root->right, team);
    else if(team->points == root->points){
        //dacă key == points, se compară lexicografic numele echipelor și se apelează funcția pentru subarborele corespunzător
        if(strcmp(team->name, root->name) < 0)
            root->left = insertInAVL(root->left, team);
        else if(strcmp(team->name, root->name) > 0)
            root->right = insertInAVL(root->right, team);
    }
    //se stabilesc înălțimea și factorul de echilibru pe fiecare nod
    root->height = max(nodeHeight(root->left), nodeHeight(root->right)) + 1;
    int balance = nodeHeight(root->left) - nodeHeight(root->right);
    //rotațiile, când punctajele sunt diferite
    if(balance > 1 && team->points < root->left->points)
        return rightRotation(root);
    if(balance < -1 && team->points > root->right->points)
        return leftRotation(root);
    if(balance > 1 && team->points > root->left->points)
        return LRRotation(root);
    if(balance < -1 && team->points < root->right->points)
        return RLRotation(root);
    //rotațiile, când punctajele sunt egale (aceleași rotații, dar se stabilesc pe baza numelor echipelor)
    if(balance > 1 && strcmp(team->name, root->left->name) < 0)
        return rightRotation(root);
    if(balance < -1 && strcmp(team->name, root->right->name) > 0)
        return leftRotation(root);
    if(balance > 1 && strcmp(team->name, root->left->name) > 0)
        return LRRotation(root);
    if(balance < -1 && strcmp(team->name, root->right->name) < 0)
        return RLRotation(root);
    return root;
}