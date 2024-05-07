#ifndef BIBLIOTECA_H_INCLUDED
#define BIBLIOTECA_H_INCLUDED

struct jucator{
    char firstName[50];
    char secondName[50];
    int points;
};
typedef struct jucator Player;

struct echipa{
    int numberOfPlayers;
    char name[50];
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

void createInitialList(Team**, int, FILE*);
int teamsRemaining(int numberOfTeams);
void eliminateWorstTeams(Team**, int);
void writeList(Team*, FILE*);
void writeListWithPoints(Team*, FILE*);
Queue* createQueue();
void enQueue(Queue*, Match*);
int queueIsEmpty(Queue*);
Match* deQueue(Queue*);
void putMatchesFromListToQueue(Queue*, Team**, int);
void writeQueue(Queue*, FILE*);
void push(Team**, Team*);
int stackIsEmpty(Team*);
Team* pop(Team**);
void deleteStack(Team**);
void putMatchesFromStackToQueue(Queue*, Team**, int, FILE*);
void matchResult(Team*, Team*, Team**, Team**);
void putTeamsFromStackToList(Team**, Team**);
void playMatches(Queue*, Team**, Team**, FILE*);

#endif // BIBLIOTECA_H_INCLUDED