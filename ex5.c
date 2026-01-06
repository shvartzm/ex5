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

void expandDB();
void compressDB();
void compressValuesDB();


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

void sortLexiDB();
int isDBFull();
int takenSpotsInDB();
int isStringNumeric(char *s);


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
char *getString(){
    char *input = NULL;
    char c;
    int size = 0;
    scanf(" %c", &c);
    input = (char*)malloc(sizeof(char));
    input[size] = c;
    size++;
    
    while((c = getchar()) != '\n' && c != EOF){
        input =(char*) realloc(input,(size + 1) * sizeof(char));
        input[size] = c;
        size++;
    }
    input = (char*)realloc(input,(size +1)* sizeof(char));
    input[size] = '\0';
    return input;
    
}

void expandDB(){
    if (database == NULL){
        database = (TVShow***)(malloc(sizeof(TVShow**)));
        database[0] = (TVShow**)(malloc(sizeof(TVShow*)));
        database[0][0] = NULL;
    }
    else{
        database = (TVShow***)(realloc(database,(dbSize + 1) * sizeof(TVShow**)));
        for(int i = 0; i < dbSize; i++){
            database[i] = (TVShow**)(realloc((database)[i],(dbSize + 1) * sizeof(TVShow*)));
            database[i][dbSize] = NULL;
        }
        database[dbSize] = (TVShow**)malloc((dbSize + 1) * sizeof(TVShow*));
        for(int j = 0; j < dbSize + 1; j++){
            database[dbSize][j] = NULL;
        }
    }
    dbSize++;
}

void compressDB(){
     free(database[dbSize - 1]); // free last row
     for (int i = 0 ; i < dbSize -1; i++){
        database[i] = (TVShow**)realloc(database[i],(dbSize - 1) * sizeof(TVShow*));
     }
     if (dbSize - 1 == 0 ){
        free(database);
        database = NULL;
     }else{
        database = (TVShow***)realloc(database, (dbSize - 1) * sizeof(TVShow**));
     }
     dbSize--;
}

void compressValuesDB(){
    TVShow* temp[dbSize * dbSize];
    int arrayIndex = 0;
    for (int i = 0; i < dbSize; i++){ // convert to 1d array
        for (int j = 0; j < dbSize; j++){
            if(database[i][j] != NULL){
                temp[arrayIndex] = database[i][j];
                arrayIndex++;
                database[i][j] = NULL;
            }
        }
    }
    compressDB();
    arrayIndex = 0;
    for (int i = 0; i < dbSize; i++){ // convert back to 2d array
        for (int j = 0; j < dbSize; j++){
             database[i][j] = temp[arrayIndex];
             arrayIndex++;
        }
    }
}

void sortLexiDB(){
    TVShow* temp[dbSize * dbSize];
    int arrayIndex = 0;
    for (int i = 0; i < dbSize; i++){ // convert to 1d array
        for (int j = 0; j < dbSize; j++){
             temp[arrayIndex] = database[i][j];
             arrayIndex++;
        }
    }
    int indexMin;
    for (int i = 0; i < (dbSize * dbSize)-1; i++){ // selection sort
        indexMin = i;
        for(int j = i + 1; j < (dbSize * dbSize); j++){
            if (temp[j] != NULL) {
                if (temp[indexMin] == NULL) {
                    indexMin = j;
                }
                else if (strcmp(temp[j]->name, temp[indexMin]->name) < 0) {
                    indexMin = j;
                }
            }
        }
        if (indexMin != i){
            TVShow* swap = temp[i];
            temp[i] = temp[indexMin];
            temp[indexMin] = swap;
        }
    }
    arrayIndex =0;
    for (int i = 0; i < dbSize; i++){ // convert back to 2d array
        for (int j = 0; j < dbSize; j++){
             database[i][j] = temp[arrayIndex];
             arrayIndex++;
        }
    }
}

int isDBFull(){
    for (int i = 0; i < dbSize;i++){
        for (int j= 0; j < dbSize; j++){
            if (database[i][j] == NULL){
               return 0;
            }
        }
    }
    return 1;
}

int takenSpotsInDB(){
    int counter = 0;
    for (int i = 0; i < dbSize;i++){
        for (int j= 0; j < dbSize; j++){
            if (database[i][j] != NULL){
               counter++;
            }
        }
    }

    return counter;
}

int isStringNumeric(char *s){
    for (int i = 0; i < (int)strlen(s); i++){
        if (!(s[i] >= '0' && s[i] <= '9')){
            return 0;
        }
    }
    return 1;
}

int validLength(char *s){
    if (s == NULL) return 0;
    char *temp = (char*)malloc((strlen(s)+1) * sizeof(char));
    strcpy(temp,s);
    char *organized;
    organized = strtok(temp,":");
    int num;
    if (organized == NULL){
        free(temp);
        return 0;
    }
    if (strlen(organized) != 2) {
        free(temp);
        return 0;
    }
    if (!isStringNumeric(organized)){
        free(temp);
        return 0;
    }
    
    num = atoi(organized);
    if(!(num >= 0 && num < 100)){
        free(temp);
        return 0;
    }

    organized = strtok(NULL, ":");
    if (organized == NULL){
        free(temp);
        return 0;
    }
    if (strlen(organized) != 2){
        free(temp);
        return 0;
    }
    if (!isStringNumeric(organized)){
        free(temp);
        return 0;
    }
    num = atoi(organized);
    if(!(num >= 0 && num < 60)){
        free(temp);
        return 0;
    }

    organized = strtok(NULL, ":");
    if (organized == NULL){
        free(temp);
        return 0;
    }
    if (strlen(organized) != 2){
        free(temp);
        return 0;
    }
    if (!isStringNumeric(organized)){
        free(temp);
        return 0;
    }
    num = atoi(organized);
    if(!(num >= 0 && num < 60)){
            free(temp);
        return 0;
    }

    organized = strtok(NULL, ":");
    if (organized != NULL){
        free(temp);
        return 0;
    }
    free(temp);
    return 1;
}

void printArray(){
    for(int i = 0 ; i < dbSize; i++){
        for (int j = 0; j < dbSize; j++){
            if(database[i][j] != NULL){
                printf("[%s] ", database[i][j] ->name);
            }else{
                printf("[NULL] ");
            }
        }
        printf("\n");
    }
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
    char* name = NULL;
    printf("Enter the name of the show:\n");
    name = getString();
    if (findShow(name) != NULL){
        printf("Show already exists.\n");
        free(name);
        return;
    }
    TVShow* pshow = (TVShow*)malloc(sizeof(TVShow));
    pshow ->name = name;
    pshow -> seasons = NULL;
    int foundSpot = 0;
    if (isDBFull()){
        expandDB();
        sortLexiDB();
    }
    for(int i = 0; i < dbSize && (!foundSpot); i++){
            for(int j = 0; j < dbSize  && (!foundSpot); j++){
                if(database[i][j] == NULL){
                    database[i][j] = pshow;
                    foundSpot = 1;
                }
            }
    }
    sortLexiDB();
}
void deleteShow(){
    char* name = NULL;
    do
    {
        if (name != NULL){
            free(name);
        }
        printf("Enter the name of the show:\n");
        name = getString();
        if (findShow(name) == NULL){
            printf("Show not found.");
        }
    } while (findShow(name) == NULL);
    for (int i = 0; i < dbSize; i++){
        for (int j = 0; j < dbSize; j++){
            if (database[i][j] != NULL && strcmp(database[i][j] -> name, name) == 0)
            {
                freeShow(database[i][j]);
                database[i][j] = NULL;
                if (takenSpotsInDB() == (dbSize - 1) * (dbSize - 1)){
                    compressValuesDB();
                }  
                sortLexiDB();
                free(name);
                return;
            }
            
        }
    }
    free(name);
}

void printShow(){
    char* nameshow = NULL;
    printf("Enter the name of the show:\n");
    nameshow = getString();
    if (findShow(nameshow) == NULL){
        printf("Show not found.\n");
        free(nameshow);
        return;
    }
    TVShow *show = findShow(nameshow);
    printf("Name: %s\n", nameshow);
    free(nameshow);
    Season *seasonrunner = show -> seasons;
    printf("Seasons:\n");
    for(int i = 0; seasonrunner != NULL; i++){
       
        printf("\tSeason %d: %s\n", i, seasonrunner->name);
        Episode* episoderunner = seasonrunner -> episodes;
        for(int j =0; episoderunner != NULL; j++){
            printf("\t\tEpisode %d: %s (%s)\n", j,episoderunner -> name, episoderunner -> length);
            episoderunner = episoderunner -> next;
        }
        seasonrunner = seasonrunner -> next;
    }
}


//Season Functions
Season *findSeason(TVShow *show, char *name){
    Season *seasons = show -> seasons; 
    while (seasons != NULL){
        if (strcmp(seasons -> name, name) == 0){
            return seasons;
        }
        seasons = seasons->next;
    }
    return NULL;
}

void addSeason(){
    char* nameshow = NULL;
    char* nameseason = NULL;
    TVShow* pshow;
    int pos;
    printf("Enter the name of the show:\n");
    nameshow = getString();
    if (findShow(nameshow) != NULL){
        printf("Show already exists.\n");
        free(nameshow);
        return;
    }
    pshow = findShow(nameshow);
    free(nameshow);
    printf("Enter the name of the season:\n");
    nameseason = getString();
    if(findSeason(pshow,nameseason) != NULL){
        printf("Season already exits.\n");
        free(nameseason);
        return;
    }
    printf("Enter the position:\n");
    scanf("%d", &pos);
    getchar();

    Season* newseason = (Season*)malloc(sizeof(Season));
    newseason -> episodes = NULL;
    newseason -> name = nameseason;
    newseason -> next = NULL;
    if(pshow -> seasons == NULL){
        pshow -> seasons = newseason;
        return;
    }

    if(pos == 0){
        newseason -> next = pshow ->seasons;
        pshow -> seasons = newseason;
        return;
    }

    Season* currSeason = pshow -> seasons;
    for (int i =0; i < pos - 1 && currSeason -> next != NULL; i++){
        currSeason = currSeason -> next;
    }
    newseason -> next = currSeason -> next;
    currSeason -> next = newseason;
}

void deleteSeason(){
    char* nameshow = NULL;
    char* nameseason = NULL;
    TVShow* pshow;
    printf("Enter the name of the show:\n");
    nameshow = getString();
    if (findShow(nameshow) == NULL){
        printf("Show not found.\n");
        free(nameshow);
        return;
    }
    pshow = findShow(nameshow);
    free(nameshow);
    printf("Enter the name of the season:\n");
    nameseason = getString();
    if(findSeason(pshow,nameseason) == NULL){
        printf("Season not found.\n");
        free(nameseason);
        return;
    }
    
    if (strcmp(pshow -> seasons -> name, nameseason) == 0){
        Season* temp = pshow -> seasons;
        pshow -> seasons = pshow -> seasons -> next;
        freeSeason(temp);
        free(nameseason);
        return;
    }
    Season* currSeason = pshow -> seasons;
    while(strcmp(currSeason -> next -> name, nameseason) != 0){
        currSeason = currSeason -> next;
    }
    Season* temp = currSeason -> next;
    currSeason -> next = temp -> next;
    freeSeason(temp);
    free(nameseason);
}

// Episode Functions
Episode *findEpisode(Season *season, char *name){
    Episode *episodes = season -> episodes;
    while (episodes != NULL){
        if (strcmp(episodes -> name, name) == 0){
            return episodes;
        }
        episodes = episodes -> next;
    }
    return NULL;
}

void addEpisode(){
    char* nameshow = NULL;
    char* nameseason = NULL;
    char* nameepisode = NULL;
    char* episodelength = NULL;
    int pos;
    Season* season;
    printf("Enter the name of the show:\n");
    nameshow = getString();
    if (findShow(nameshow) == NULL){
        printf("Show not found.\n");
        free(nameshow);
        return;
    }
    
    printf("Enter the name of the season:\n");
    nameseason = getString();
    if(findSeason(findShow(nameshow),nameseason) == NULL){
        printf("Season not found.\n");
        free(nameshow);
        free(nameseason);
        return;
    }
    season = findSeason(findShow(nameshow),nameseason);
    free(nameshow);
    free(nameseason);
    printf("Enter the name of the episode:\n");
    nameepisode = getString();
    if (findEpisode(season,nameepisode) != NULL){
        printf("Episode already exists.\n");
        free(nameepisode);
        return;
    }

    do
    {
        if (episodelength != NULL){
            free(episodelength);
        }
        printf("Enter the length (xx:xx:xx):\n");
        episodelength = getString();
        if (!validLength(episodelength)){
            printf("Invalid length, enter again:\n");
        }
    } while (!validLength(episodelength));
    printf("Enter the position:\n");
    scanf("%d", &pos);
    getchar();

    Episode* newepisode = (Episode*)malloc(sizeof(Episode));
    newepisode -> name = nameepisode;
    newepisode -> length = episodelength;
    newepisode -> next = NULL;
    if(season -> episodes == NULL){
        season -> episodes = newepisode;
        return;
    }

    if(pos == 0){
        newepisode -> next = season -> episodes;
        season -> episodes = newepisode;
        return;
    }

    Episode* currEpisode = season -> episodes;
    for (int i = 0; i < pos - 1 && currEpisode -> next != NULL; i++){
        currEpisode = currEpisode -> next;
    }
    newepisode -> next = currEpisode -> next;
    currEpisode -> next = newepisode;
}

void deleteEpisode(){
    char* nameshow = NULL;
    char* nameseason = NULL;
    char* nameepisode = NULL;
    Season* season;
    printf("Enter the name of the show:\n");
    nameshow = getString();
    if (findShow(nameshow) == NULL){
        printf("Show not found.\n");
        free(nameshow);
        return;
    }
    
    printf("Enter the name of the season:\n");
    nameseason = getString();
    if(findSeason(findShow(nameshow),nameseason) == NULL){
        printf("Season not found.\n");
        free(nameshow);
        free(nameseason);
        return;
    }
    season = findSeason(findShow(nameshow),nameseason);
    free(nameshow);
    free(nameseason);
    printf("Enter the name of the episode:\n");
    nameepisode = getString();
    if (findEpisode(season,nameepisode) == NULL){
        printf("Episode not found.\n");
        free(nameepisode);
        return;
    }

    if (strcmp(season -> episodes -> name, nameepisode) == 0){
        Episode* temp = season -> episodes;
        season -> episodes = season -> episodes -> next;
        freeEpisode(temp);
        free(nameepisode);
        return;
    }
    Episode* currEpisode = season -> episodes;
    while(strcmp(currEpisode -> next -> name, nameepisode) != 0){
        currEpisode = currEpisode -> next;
    }
    Episode* temp = currEpisode -> next;
    currEpisode -> next = temp -> next;
    freeEpisode(temp);
    free(nameepisode);
}

void printEpisode(){
    char* nameshow = NULL;
    char* nameseason = NULL;
    char* nameepisode = NULL;
    Season* season;
    printf("Enter the name of the show:\n");
    nameshow = getString();
    if (findShow(nameshow) == NULL){
        printf("Show not found.\n");
        free(nameshow);
        return;
    }
    
    printf("Enter the name of the season:\n");
    nameseason = getString();
    if(findSeason(findShow(nameshow),nameseason) == NULL){
        printf("Season not found.\n");
        free(nameshow);
        free(nameseason);
        return;
    }
    season = findSeason(findShow(nameshow),nameseason);
    free(nameshow);
    free(nameseason);
    printf("Enter the name of the episode:\n");
    nameepisode = getString();
    if (findEpisode(season,nameepisode) == NULL){
        printf("Episode not found.\n");
        free(nameepisode);
        return;
    }
    Episode *episode = findEpisode(season,nameepisode);
    free(nameepisode);
    printf("Name: %s\n", episode -> name);
    printf("Length: %s\n", episode -> length);
}

// Free Functions
void freeEpisode(Episode *e){
    Episode* current = e;
    Episode* nextEpisode;

    while(current != NULL){
        nextEpisode = current -> next;
        free(current -> name);
        free(current -> length);
        free(current);
        current = nextEpisode;
    }
}
void freeSeason(Season *s){
    Season* current = s;
    Season* nextSeason;

    while(current != NULL){
        nextSeason = current -> next;
        freeEpisode(current -> episodes);
        free(current -> name);
        free(current);
        current = nextSeason;
    }
}
void freeShow(TVShow *t){
    freeSeason(t -> seasons);
    free(t -> name);
    free(t);
}

void freeAll(){
    for(int i = 0; i < dbSize; i++){
        for(int j =0; j < dbSize; j++){
            if(database[i][j] != NULL){
                freeShow(database[i][j]);
            }
        }
        free(database[i]);
    }
    if(database!= NULL){
        free(database);
        database = NULL;
    }
    dbSize = 0;
}
