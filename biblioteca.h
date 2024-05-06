#ifndef BIBLIOTECA_H_INCLUDED
#define BIBLIOTECA_H_INCLUDED

struct jucator{
    char firstName[30];
    char secondName[30];
    int points;
};
typedef struct jucator Player;

struct echipa{
    int numberOfPlayers;
    char name[30];
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

void createList(Team**, int, FILE*);
int teamsRemaining(int numberOfTeams);
void eliminateWorstTeams(Team**, int);
void writeList(Team*, FILE*);
Queue* createQueue();
void enQueue(Queue*, Match*);
int isEmpty(Queue*);
Match* deQueue(Queue*);
void putMatchesInQueue(Queue*, Team**, int);
void writeQueue(Queue*, FILE*);

#endif // BIBLIOTECA_H_INCLUDED