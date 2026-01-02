/***********
 ID: 216763235
 NAME: Maor Shvartz
***********/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Episode {
    char *name;
    char *length;
    struct Episode *next;
} Episode;

typedef struct Season {
    char *name;
    Episode *episodes;
    struct Season *next;
} Season;

typedef struct TVShow {
    char *name;
    Season *seasons;
} TVShow;

TVShow ***database = NULL;
int dbSize = 0;

char *getString();
int getInt();

int validLength(char *s);
int countShows();

void shrinkDB();
void expandDB();

void freeEpisode(Episode *e);
void freeSeason(Season *s);
void freeShow(TVShow *show);
void freeAll();

TVShow *findShow(char *name);
Season *findSeason(TVShow *show, char *name);
Episode *findEpisode(Season *season, char *name);

void addShow();
void addSeason();
void addEpisode();

void deleteShow();
void deleteSeason();
void deleteEpisode();

void printEpisode();
void printShow();
void printArray();

void addMenu() {
    int choice;
    printf("Choose an option:\n");
    printf("1. Add a TV show\n");
    printf("2. Add a season\n");
    printf("3. Add an episode\n");
    scanf("%d", &choice);
    getchar();
    switch (choice) {
        case 1: addShow(); break;
        case 2: addSeason(); break;
        case 3: addEpisode(); break;
    }
}

void deleteMenu() {
    int choice;
    printf("Choose an option:\n");
    printf("1. Delete a TV show\n");
    printf("2. Delete a season\n");
    printf("3. Delete an episode\n");
    scanf("%d", &choice);
    getchar();
    switch (choice) {
        case 1: deleteShow(); break;
        case 2: deleteSeason(); break;
        case 3: deleteEpisode(); break;
    }
}

void printMenuSub() {
    int choice;
    printf("Choose an option:\n");
    printf("1. Print a TV show\n");
    printf("2. Print an episode\n");
    printf("3. Print the array\n");
    scanf("%d", &choice);
    getchar();
    switch (choice) {
        case 1: printShow(); break;
        case 2: printEpisode(); break;
        case 3: printArray(); break;
    }
}

void mainMenu() {
    printf("Choose an option:\n");
    printf("1. Add\n");
    printf("2. Delete\n");
    printf("3. Print\n");
    printf("4. Exit\n");
}

int main() {
    int choice;
    do {
        mainMenu();
        scanf("%d", &choice);
        getchar();
        switch (choice) {
            case 1: addMenu(); break;
            case 2: deleteMenu(); break;
            case 3: printMenuSub(); break;
            case 4: freeAll(); break;
        }
    } while (choice != 4);
    return 0;
}

//All Around Functions
void expandDB(){
    if (database == NULL){
        database = (TVShow***)(malloc(sizeof(TVShow**)));
        *database = (TVShow**)(malloc(sizeof(TVShow*)));
        **database = (TVShow*)(malloc(sizeof(TVShow)));
    }
    else{
        *database = (TVShow**)(realloc(*database,(dbSize + 1) * sizeof(TVShow*)));
        for(int i = 0; i < dbSize; i++){
            (*database)[i] = (TVShow*)(realloc((*database)[i],(dbSize + 1) * sizeof(TVShow)));
        }
        (*database)[dbSize] = (TVShow*)malloc((dbSize + 1) * sizeof(TVShow));
    }
    dbSize++;
}


//TV Show Functions
TVShow *findShow(char *name){
    for(int i = 0; i <dbSize; i++){
        for (int j =0; j < dbSize; j++){
            if(strcmp(database[i][j] -> name, name) == 0){
                return database[i][j];
            }
        }
    }
    return NULL;
}

void addShow(){
    char* name;
    do
    {
        printf("Enter the name of the show:\n");
        name = scanf("%s", name);
        if(findShow(name) != NULL){
            printf("Show already exists");
        }
    } while (findShow(name) != NULL);
    
    
}



//Season Functions
Season *findSeason(TVShow *show, char *name){
    Season *seasons = show -> seasons; 
    while (seasons != NULL){
        if (strcmp(seasons -> name, name) == 0){
            return seasons;
        }
        seasons++;
    }
    return NULL;
}


// Episode Functions
Episode *findEpisode(Season *season, char *name){
    Episode *episodes = season -> episodes;
    while (episodes != NULL){
        if (strcmp(episodes -> name, name) == 0){
            return episodes;
        }
        episodes++;
    }
    return NULL;
}
