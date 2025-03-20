#define LINETAM 26
#define COLUMNTAM 26
#define MAPTAM ((COLUMNTAM*LINETAM) + 1)

//Main
int checkingInitial(int, char*[]);
char** verify_argv(int, char*[]);

//Conection
void definingPorts(char*, char*);
int startingGame(int, char[]);

//Game
int mainGame(int, char*[], int**, char[]);
int myturn(char[],char*[], int**, int*);
int histurn(char[], char*[], int**, int*);
int ver_myturn(char[], char*[], int**, char*, int, int*);
int ver_histurn(char[], char*[], int**, char*, int, int);
void destroyed(int);

//Maps
char *creatingMap(void);
int acessingMap(char*, int);
void changingMap(char*, int, int);
int inputToPosition(char[]);
void printMaps(char*[]);

//Talker
char *talker(char[], char[], float);

//Listener
char *listener();

//ProcessingFile
int readingFile(char[], char*, int**);
int building_islands(char*, int**, char[], char[], char[]);
int makingIsland(char*, int**, int, char[], char[], int, int);
int square(char*, int**, int);
int triangle(char*, int**, int, char[]);
int positionToColumn(int);
int positionToLine(int);
char *uppercase(char*);

//life
int **creatingLife();
int registringIsland(int**, int, int[], int);
int searchEmpty();
int searchIsland();
void printLife(int**);
int verifyTotalLife(int**);
int destroyIsland(int**, int);

//autogame
int autoGame(int, char*[], int**, char[]);
int botturn(char[],char*[], int**, int*, int*[], int*[]);
int verifyPositions(int, int*[]);
void registerPosition(int, int*[]);
void registerAttack(int, int*[]);
char *clockAttack(int* [], int*[]);
int nextClockAttackPosition(int*[]);
int lastPositionAttack(int*[]);
void countStepsClock(int*[]);
char* positionToInput(int);
