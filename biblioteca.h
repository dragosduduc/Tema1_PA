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
    int height;
};
typedef struct echipaInArbore Node;

void createInitialList(Team**, int, FILE*);
int teamsRemaining(int numberOfTeams);
void freeTeam(Team*);
void eliminateWorstTeams(Team**, int);
void writeList(Team*, FILE*);
void writeListWithPoints(Team*, FILE*);
Queue* createQueue();
void enQueue(Queue*, Match*);
int queueIsEmpty(Queue*);
Match* deQueue(Queue*);
void freeQueue(Queue*);
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
Node* createNode(Team*);
Node* insertInBST(Node*, Team*);
void inorderReverse(Node*, FILE*);
void fprintLevelInTree(FILE*, Node*, int);
int max(int, int);
int nodeHeight(Node*);
Team* copyTeamFromNode(Node* node);
void createListFromTree(Node*, Team**);
Node* rightRotation(Node*);
Node* leftRotation(Node*);
Node* LRRotation(Node*);
Node* RLRotation(Node*);
Node* insertInAVL(Node*, Team*);
void freeNode(Node*);
void freeTree(Node*);

#endif // BIBLIOTECA_H_INCLUDED