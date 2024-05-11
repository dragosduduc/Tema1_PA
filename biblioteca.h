#ifndef BIBLIOTECA_H_INCLUDED
#define BIBLIOTECA_H_INCLUDED

struct jucator{
    char* firstName;
    char* secondName;
    int points;
};
typedef struct jucator Player;

struct echipa{
    int numberOfPlayers;
    char* name;
    Player* members;
    float points;
    struct echipa *next;
};
typedef struct echipa Team;

struct meci{
    Team* team1;
    Team* team2;
    struct meci *next;
};
typedef struct meci Match;

struct Q{
    Match* front;
    Match* rear;
};
typedef struct Q Queue;

struct echipaInArbore{
    int numberOfPlayers;
    char* name;
    Player* members;
    float points;
    struct echipaInArbore *left, *right;
};
typedef struct echipaInArbore Node;

void createInitialList(Team**, int, FILE*);
int teamsRemaining(int numberOfTeams);
void eliminateWorstTeams(Team**, int);
void writeList(Team*, FILE*);
void writeListWithPoints(Team*, FILE*);
Queue* createQueue();
void enQueue(Queue*, Match*);
int queueIsEmpty(Queue*);
Match* deQueue(Queue*);
void moveMatchesFromListToQueue(Queue*, Team**, int);
void writeQueue(Queue*, FILE*);
void push(Team**, Team*);
int stackIsEmpty(Team*);
Team* pop(Team**);
void deleteStack(Team**);
void moveMatchesFromStackToQueue(Queue*, Team**, int, FILE*);
void matchResult(Team*, Team*, Team**, Team**);
void copyTeamsFromStackToList(Team*, Team**);
void playMatches(Queue*, Team**, Team**, FILE*);
Node* insertInBST(Node*, Team*);
void inorderReverse(Node*, FILE*);

#endif // BIBLIOTECA_H_INCLUDED