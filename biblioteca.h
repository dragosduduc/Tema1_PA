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

void form_list(Team**, int, FILE*);
void write_list(Team*, FILE*);

#endif // BIBLIOTECA_H_INCLUDED