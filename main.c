#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// ESTRUCTURA DONDE SE ALMACENAN LOS DATOS DE LAS CARTAS
typedef struct card
{
    char name[50];
    char type[15];
    int LP; // LIFE POINTS
    int AP; // ATTACK POINTS
    int DP; // DEFENSE POINTS
    struct card *next;
}Card;

#define MAX_CARDS 60
#define MAX_LINE_LENGTH 200

int MAX_LP, MAX_AP, MAX_DP, MIN_LP, MIN_AP, MIN_DP, cardAmount;

// FUNCIÓN DONDE SE AÑADE UNA CARTA A LA LISTA
void addCard(Card **head, Card *newCard)
{
    if (*head == NULL)
    {
        *head = newCard;
    }
    else
    {
        Card *current = *head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = newCard;
    }
}

// LECTURA DEL ARCHIVO Y CREACIÓN DE LA LISTA
void readFile(Card **list)
{
    FILE *file = fopen("guardians.txt", "r");
    if (file == NULL) return;

    bool minAdded = false;
    cardAmount = 0, MAX_LP = 0, MAX_AP = 0, MAX_DP = 0;

    char line[MAX_LINE_LENGTH];
    while(fgets(line, MAX_LINE_LENGTH, file))
    {
        Card *newstruct = (Card*)malloc(sizeof(Card));

        char *text = strtok(line, ",");
        strcpy(newstruct->name, text);

        char *text2 = strtok(NULL, ",");
        strcpy(newstruct->type, text2);

        newstruct->LP = atoi(strtok(NULL, ","));
        newstruct->AP = atoi(strtok(NULL, ","));
        newstruct->DP = atoi(strtok(NULL, ","));
        newstruct->next = NULL;

        // ACTUALIZAR VALOR MÍNIMO
        if(minAdded == false)
        {
            MIN_LP = newstruct->LP;
            MIN_AP = newstruct->AP;
            MIN_DP = newstruct->DP;
            minAdded = true;
        }

        if(MIN_LP > newstruct->LP)
        {
            MIN_LP = newstruct->LP;
        }
        if(MIN_AP > newstruct->AP)
        {
            MIN_AP = newstruct->AP;
        }
        if(MIN_DP > newstruct->DP)
        {
            MIN_DP = newstruct->DP;
        }

        // ACTUALIZAR VALOR MÁXIMO
        if(MAX_LP < newstruct->LP)
        {
            MAX_LP = newstruct->LP;
        }
        if(MAX_AP < newstruct->AP)
        {
            MAX_AP = newstruct->AP;
        }
        if(MAX_DP < newstruct->DP)
        {
            MAX_DP = newstruct->DP;
        }

        cardAmount++;

        addCard(list, newstruct);
    }

    fclose(file);
}

// FUNCIÓN PARA LIBERAR MEMORIA RESERVADA
void freeCards(Card *head)
{
    Card *current = head;
    while (current != NULL)
    {
        Card *next = current->next;
        free(current);
        current = next;
    }
}

// FUNCIÓN PARA CREAR UNA CARTA, ALMACENANDO CADA DATO DENTRO DE UNA ESTRUCTURA
Card* createCard(char *name, char *type, int lp, int ap, int dp)
{
    Card *newCard = (Card*)malloc(sizeof(Card));
    strcpy(newCard->name, name);
    strcpy(newCard->type, type);
    newCard->LP = lp;
    newCard->AP = ap;
    newCard->DP = dp;
    return newCard;
}

// IMPRIMIR CARTAS
void printCards(Card *head)
{
    int i = 1;
    printf(">> Card List:\n");
    Card *current = head;
    while (current != NULL)
    {
        printf("%d. %s, %s, %d, %d, %d\n", i,current->name, current->type, current->LP, current->AP, current->DP);
        current = current->next;
        i++;
    }
}

/*
            [IMPORTANTE]

    A LO LARGO DEL CÓDIGO, DENTRO DE LOS ARREGLOS DE CARTAS, EL VALOR DE VIDA DE CADA
    CARTA (LP) SERÁ DEFINIDO COMO -1 EN REPRESENTACIÓN DE QUE ESA CARTA YA NO SE ENCUENTRA
    DISPONIBLE PARA USARSE.
    EJEMPLO:
    SI EN EL ARREGLO DE CARTAS DE LA MANO TRANSFORMO LOS PUNTOS DE VIDA EN -1 DE UNA CARTA ESPECÍFICA,
    ESA CARTA YA NO SE ENCONTRARÁ SIMBÓLICAMENTE DENTRO LA MANO.
    EN OTRAS PALABRAS, QUE UNA CARTA TENGA COMO ATRIBUTO -1 EN LP, SIGNIFICARÁ VACÍO.
*/
// INICIALIZAR VALORES DE LOS ARREGLOS A LO PREDETERMINADO
void initDeckArray(Card deck[], int limit)
{
    for(int i = 0; i < limit; i++)
    {
        strcpy(deck[i].name, "");
        strcpy(deck[i].type, "");
        deck[i].LP = -1;
        deck[i].AP = -1;
        deck[i].DP = -1;
    }
}

// FUNCIÓN QUE TRASPASA LAS CARTAS ESTRUCTURA A UN ARREGLO ESTRUCTURA
void listToArray(Card *head, Card deck[])
{
    int i=0;
    Card *current = head;
    while (current != NULL)
    {
        strcpy(deck[i].name, current->name);
        strcpy(deck[i].type, current->type);
        deck[i].LP = current->LP;
        deck[i].AP = current->AP;
        deck[i].DP = current->DP;
        current = current->next;
        i++;
    }
}

// SE TOMAN ELEMENTOS ALEATORIOS DEL ARREGLO STRUCT DEL MAZO PRINCIPAL Y SE GUARDAN SUS ÍNDICES EN UN ARREGLO DE INT
// SI UN NÚMERO ALEATORIO COINCIDE CON UNO GUARDADO PREVIAMENTE, NO SE REALIZA EL GUARDADO EN EL ARREGLO DE INT
// LA FUNCIÓN HARÁ SU TRABAJO HASTA QUE SE HAYAN GUARDADO 30 ÍNDICES DISTINTOS
void fillCardPool(int selectedCardsIndex[])
{
    bool saveCard = false;
    int pos = 0;
    while(pos < 30)
    {
        int randomIndex = rand() % cardAmount; // GENERA UN NÚMERO ALEATORIO ENTRE 0 Y EL NÚMERO DE CARTAS EN JUEGO

        for(int i = 0; i < 30; i++)
        {
            if(randomIndex == selectedCardsIndex[i]) // AQUÍ SE DETECTA SI UN NÚMERO ALEATORIO SE REPITE
            {
                saveCard = false;
                break; // POR LO QUE SE ROMPE EL CICLO PARA BUSCAR UN NUEVO NÚMERO
            }
            else
            {
                saveCard = true;
            }
        }

        // ÚNICAMENTE SI EL NÚMERO ALEATORIO ES NUEVO, SE HARÁ EL GUARDADO EN
        // EL ARREGLO Y SE AUMENTARÁ LA POSICIÓN PARA GUARDAR EN EL SIGUIENTE ESPACIO
        // ASÍ HASTA COMPLETAR TODOS LOS ESPACIOS
        if(saveCard == true)
        {
            selectedCardsIndex[pos] = randomIndex;
            pos++;
        }
    }
}

// FUNCIÓN QUE TOMA LOS 15 PRIMEROS VALORES DEL ARREGLO DE ÍNDICES
// ESTOS ÍNDICES PASARÁN LAS VALORES DE LAS ESTRUCTURAS AL NUEVO ARREGLO
void getPlayerCards(Card deck[], Card playerCardPool[],int selectedIndex[])
{
    int pos = 0;
    for(int i = 0; i < 15; i++)
    {
        strcpy(playerCardPool[i].name, deck[selectedIndex[pos]].name);
        strcpy(playerCardPool[i].type, deck[selectedIndex[pos]].type);
        playerCardPool[i].LP = deck[selectedIndex[pos]].LP;
        playerCardPool[i].AP = deck[selectedIndex[pos]].AP;
        playerCardPool[i].DP = deck[selectedIndex[pos]].DP;
        pos++;
    }

    printf(">> Your Cards: \n\n");
    for(int g = 0; g < 15; g++)
    {
        printf("%d. %s, %s, %d, %d, %d\n", g,playerCardPool[g].name, playerCardPool[g].type, playerCardPool[g].LP, playerCardPool[g].AP, playerCardPool[g].DP);
    }
    printf("\n");
}

// FUNCIÓN QUE TOMA LOS OTROS 15 VALORES RESTANTES Y LOS AÑADE AL ARREGLO DEL PC
void getPCCards(Card deck[], Card pcCardPool[],int selectedIndex[])
{
    int pos = 15;

    for(int i = 0; i < 15; i++)
    {
        strcpy(pcCardPool[i].name, deck[selectedIndex[pos]].name);
        strcpy(pcCardPool[i].type, deck[selectedIndex[pos]].type);
        pcCardPool[i].LP = deck[selectedIndex[pos]].LP;
        pcCardPool[i].AP = deck[selectedIndex[pos]].AP;
        pcCardPool[i].DP = deck[selectedIndex[pos]].DP;
        pos++;
    }
}

// FUNCIÓN DONDE EL JUGADOR ELIGE CON QUÉ CARTAS PARTIR EN LA MANO
void playerCardSelection(Card playerCardPool[], Card playerHand[])
{
    int c1=0, c2=0, c3=0;
    printf(">> Choose 3 cards from the pool by typing the index.\n");

    do
    {
        printf("\nCard number 1: ");
        scanf("%d", &c1);
        if(c1 < 0 || c1 > 14)
        {
            printf(">> Error. Index entered is not valid. Try Again.\n");
        }
    }while(c1 < 0 || c1 > 14);

    do
    {
        printf("Card number 2: ");
        scanf("%d", &c2);
        if(c2 < 0 || c2 > 14 || c2 == c1)
        {
            printf(">> Error. Index entered is not valid. Try Again.\n");
        }
    }while(c2 < 0 || c2 > 14 || c2 == c1);

    do
    {
        printf("Card number 3: ");
        scanf("%d", &c3);
        if(c3 < 0 || c3 > 14 || c3 == c2 || c3 == c1)
        {
            printf(">> Error. Index entered is not valid. Try Again.\n");
        }
    }while(c3 < 0 || c3 > 14 || c3 == c2 || c3 == c1);

    strcpy(playerHand[0].name, playerCardPool[c1].name);
    strcpy(playerHand[0].type, playerCardPool[c1].type);
    playerHand[0].LP = playerCardPool[c1].LP;
    playerHand[0].AP = playerCardPool[c1].AP;
    playerHand[0].DP = playerCardPool[c1].DP;

    strcpy(playerHand[1].name, playerCardPool[c2].name);
    strcpy(playerHand[1].type, playerCardPool[c2].type);
    playerHand[1].LP = playerCardPool[c2].LP;
    playerHand[1].AP = playerCardPool[c2].AP;
    playerHand[1].DP = playerCardPool[c2].DP;

    strcpy(playerHand[2].name, playerCardPool[c3].name);
    strcpy(playerHand[2].type, playerCardPool[c3].type);
    playerHand[2].LP = playerCardPool[c3].LP;
    playerHand[2].AP = playerCardPool[c3].AP;
    playerHand[2].DP = playerCardPool[c3].DP;

    playerCardPool[c1].LP = -1;
    playerCardPool[c2].LP = -1;
    playerCardPool[c3].LP = -1;

    printf("\n>> Done!");
}

// FUNCIÓN DONDE EL PC TOMA AL AZAR SUS PRIMERAS 3 CARTAS A LA MANO
void pcCardSelection(Card pcCardPool[], Card pcHand[])
{
    int randomNumb3=0,randomNumb2=0, randomNumb1 = rand() % 15; // Número random entre 0 y 14
    do
    {
        randomNumb2 = rand() % 15;
    }while(randomNumb2 == randomNumb1);
    do
    {
        randomNumb3 = rand() % 15;
    }while(randomNumb3 == randomNumb1 || randomNumb3 == randomNumb2);

    strcpy(pcHand[0].name, pcCardPool[randomNumb1].name);
    strcpy(pcHand[0].type, pcCardPool[randomNumb1].type);
    pcHand[0].LP = pcCardPool[randomNumb1].LP;
    pcHand[0].AP = pcCardPool[randomNumb1].AP;
    pcHand[0].DP = pcCardPool[randomNumb1].DP;

    strcpy(pcHand[1].name, pcCardPool[randomNumb2].name);
    strcpy(pcHand[1].type, pcCardPool[randomNumb2].type);
    pcHand[1].LP = pcCardPool[randomNumb2].LP;
    pcHand[1].AP = pcCardPool[randomNumb2].AP;
    pcHand[1].DP = pcCardPool[randomNumb2].DP;

    strcpy(pcHand[2].name, pcCardPool[randomNumb3].name);
    strcpy(pcHand[2].type, pcCardPool[randomNumb3].type);
    pcHand[2].LP = pcCardPool[randomNumb3].LP;
    pcHand[2].AP = pcCardPool[randomNumb3].AP;
    pcHand[2].DP = pcCardPool[randomNumb3].DP;

    pcCardPool[randomNumb1].LP = -1;
    pcCardPool[randomNumb2].LP = -1;
    pcCardPool[randomNumb3].LP = -1;

    printf("\n>> PC cards in hand!");
}

// EL JUGADOR ROBA UNA CARTA ALEATORIA DE SU MAZO DE 15 CARTAS
void drawPlayerCard(Card playerCardPool[], Card playerHand[], int p1HandPos)
{
    int randomNumb = 0;
    do
    {
        randomNumb = rand() % 15;

    } while(playerCardPool[randomNumb].LP == -1);

    strcpy(playerHand[p1HandPos].name, playerCardPool[randomNumb].name);
    strcpy(playerHand[p1HandPos].type, playerCardPool[randomNumb].type);
    playerHand[p1HandPos].LP = playerCardPool[randomNumb].LP;
    playerHand[p1HandPos].AP = playerCardPool[randomNumb].AP;
    playerHand[p1HandPos].DP = playerCardPool[randomNumb].DP;

    playerCardPool[randomNumb].LP = -1;
    printf("\n>> 1 CARD DRAWED [%s]\n", playerHand[p1HandPos].name);
}

// EL PC ROBA UNA CARTA ALEATORIA DE SU MAZO DE 15 CARTAS
void drawPCCard(Card pcCardPool[], Card pcHand[], int p2HandPos)
{
    int randomNumb = 0;
    do
    {
        randomNumb = rand() % 15;

    } while(pcCardPool[randomNumb].LP == -1);

    strcpy(pcHand[p2HandPos].name, pcCardPool[randomNumb].name);
    strcpy(pcHand[p2HandPos].type, pcCardPool[randomNumb].type);
    pcHand[p2HandPos].LP = pcCardPool[randomNumb].LP;
    pcHand[p2HandPos].AP = pcCardPool[randomNumb].AP;
    pcHand[p2HandPos].DP = pcCardPool[randomNumb].DP;

    pcCardPool[randomNumb].LP = -1;
    printf("\n>> PC DRAWED A CARD\n");
}

// FUNCIÓN DONDE EL JUGADOR INVOCA UNA CARTA DE LA MANO A LA MESA DE COMBATE
void playerSummon(Card playerHand[], Card playerTable[], int p1TablePos)
{
    int select = -1;
    bool canSummon = false;
    printf("\n>> Select a card to summon: ");
    do
    {
        scanf("%d", &select);
        if(playerHand[select].LP == -1)
        {
            printf(">> Error. Invalid index. Try Again.\n");
            canSummon = false;
        }
        else
        {
            canSummon = true;
        }
    }while(canSummon == false);

    strcpy(playerTable[p1TablePos].name, playerHand[select].name);
    strcpy(playerTable[p1TablePos].type, playerHand[select].type);
    playerTable[p1TablePos].LP = playerHand[select].LP;
    playerTable[p1TablePos].AP = playerHand[select].AP;
    playerTable[p1TablePos].DP = playerHand[select].DP;

    playerHand[select].LP = -1;

    printf("\n>> You have summoned %s!", playerTable[p1TablePos].name);
}

// FUNCIÓN DONDE EL PC INVOCA UNA CARTA DE LA MANO A LA MESA DE COMBATE DE MANERA ALEATORIA
void pcSummon(Card pcHand[], Card pcTable[], int p2TablePos)
{
    int randomNumb = -1;
    bool canSummon = false;

    do
    {
        randomNumb = rand() % 15;
        if(pcHand[randomNumb].LP == -1)
        {
            canSummon = false;
        }
        else
        {
            canSummon = true;
        }
    }while(canSummon == false);

    strcpy(pcTable[p2TablePos].name, pcHand[randomNumb].name);
    strcpy(pcTable[p2TablePos].type, pcHand[randomNumb].type);
    pcTable[p2TablePos].LP = pcHand[randomNumb].LP;
    pcTable[p2TablePos].AP = pcHand[randomNumb].AP;
    pcTable[p2TablePos].DP = pcHand[randomNumb].DP;

    pcHand[randomNumb].LP = -1;

    printf("\n>> PC summoned %s!", pcTable[p2TablePos].name);
}

// FUNCIÓN DONDE EL JUGADOR ELIGE A QUIEN ATACAR Y CON QUIEN, DENTRO DE LA MESA
int playerAttack(Card playerTable[], Card pcTable[], int pcHP)
{
    int select = -1, select2 = -1;
    bool canAttack = false, isAttackable = false;
    printf("\n>> Select a card to attack your opponent: ");
    do
    {
        scanf("%d", &select);
        if(playerTable[select].LP == -1)
        {
            printf(">> Error. Invalid index. Try Again.\n");
            canAttack = false;
        }
        else
        {
            canAttack = true;
        }
    }while(canAttack == false);

    printf("\n>> Select an opponent card to attack: ");
    do
    {
        scanf("%d", &select2);
        if(pcTable[select2].LP == -1)
        {
            printf(">> Error. Invalid index. Try Again.\n");
            isAttackable = false;
        }
        else
        {
            isAttackable = true;
        }
    }while(isAttackable == false);

    if(pcTable[select2].DP - playerTable[select].AP <= 0)
    {
        printf("\n>> You have destroyed %s!", pcTable[select2].name);
        pcTable[select2].LP = -1;
        pcHP--;
    }
    else
    {
        printf("\n>>%s has resisted the attack!", pcTable[select2].name);
    }
    return pcHP;
}

// FUNCIÓN DONDE EL PC ELIGE A QUIEN ATACAR Y CON QUIEN, DENTRO DE LA MESA. DE MANERA COMPLETAMENTE ALEATORIA
int pcAttack(Card playerTable[], Card pcTable[], int playerHP)
{
    int rand1 = 0, rand2 = 0;
    bool canAttack = false, isAttackable = false;

    // SELECCIÓN DE CARTA ATACANTE DEL PC
    do
    {
        rand1 = rand() % 15;
        if(pcTable[rand1].LP == -1)
        {
            canAttack = false;
        }
        else
        {
            canAttack = true;
        }
    }while(canAttack == false);

    // SELECCIÓN DE CARTA OBJETIVO DEL JUGADOR
    do
    {
        rand2 = rand() % 15;
        if(playerTable[rand2].LP == -1)
        {
            isAttackable = false;
        }
        else
        {
            isAttackable = true;
        }
    }while(isAttackable == false);

    // RESULTADOS
    printf("\n>>%s is going to attack %s", pcTable[rand1].name, playerTable[rand2].name);

    if(playerTable[rand2].DP - pcTable[rand1].AP <= 0)
    {
        printf("\n>> PC have destroyed %s!", playerTable[rand2].name);
        playerTable[rand2].LP = -1;
        playerHP--;
    }
    else
    {
        printf("\n>>%s has resisted the attack!", playerTable[rand2].name);
    }
    return playerHP;
}

// FUNCIÓN DONDE SE DESARROLLA TODO EL JUEGO, INTERACCIÓN ENTRE JUGADOR Y MESA Y PC
void gameStart(Card playerCardPool[], Card pcCardPool[], Card playerHand[], Card pcHand[], Card playerTable[], Card pcTable[])
{
    int playerHP = 5, pcHP = 5, turn = 1, p1HandPos = 3, p2HandPos = 3, p1TablePos = 0, p2TablePos = 0, inHandCounterP1 = 0, inHandCounterP2 = 0, inTableCounterP1 = 0, inTableCounterP2 = 0, action = 0;
    bool canSummon1 = true, canAttack1 = false, canSummon2 = true, canAttack2 = false, isOver = false;

    do
    {
        // MENÚ DE JUEGO ----------------------------------------

        printf("---------------------------------------------------\n");
        printf(">> Player Cards in Play: \n");

        for(int i = 0; i < 15; i++)
        {
            if(playerTable[i].LP != -1)
            {
                printf("%d. %s, %s. LP: %d. AP: %d. DP: %d\n", i, playerTable[i].name, playerTable[i].type, playerTable[i].LP, playerTable[i].AP, playerTable[i].DP);
                inTableCounterP1++;
            }
        }
        printf("\n----------------------- VS -----------------------\n");
        printf(">> PC Cards in Play: \n");

        for(int i = 0; i < 15; i++)
        {
            if(pcTable[i].LP != -1)
            {
                printf("%d. %s, %s. LP: %d. AP: %d. DP: %d\n", i, pcTable[i].name, pcTable[i].type, pcTable[i].LP, pcTable[i].AP, pcTable[i].DP);
                inTableCounterP2++;
            }
        }
        printf("\n---------------------------------------------------");
        printf("\n[Player HP: %d] [PC HP: %d] - [TURN %d", playerHP, pcHP, turn);

        // CUANDO EL TURNO SEA PAR, JUEGA EL PC. SI EL TURNO ES IMPAR, JUEGA EL USUARIO
        if(turn % 2 == 0) // TURNO DEL PC *********
        {
            printf(", PC turn]\n\n");

            // SI TODAVÍA QUEDAN CARTAS EN EL MAZO, SE ROBA 1 CARTA
            if(p2HandPos < 15)
            {
                drawPCCard(pcCardPool, pcHand, p2HandPos);
                p2HandPos++;
            }
            //-----------------------------------------------------
            for(int i = 0; i < 15; i++)
            {
                if(pcHand[i].LP != -1)
                {
                    inHandCounterP2++;
                }
            }
            if(inHandCounterP2 == 15)
            {
                canSummon2 = false;
            }
            if(inTableCounterP2 == 0)
            {
                canAttack2 = false;
            }

            action = (rand () % 2) + 1;

            // COMANDO ATACAR
            if(action == 1)
            {
                printf("\n>> PC is going to attack!");
                if(turn == 2 || canAttack2 == false)
                {
                    printf("\n>> PC can't attack a card. Summoning instead...");
                    action = 2;
                }
                else
                {
                    playerHP = pcAttack(playerTable, pcTable, playerHP);
                }
            }

            // COMANDO INVOCAR
            if(action == 2)
            {
                printf("\n>> PC is going to summon!");
                if(canSummon2 == false)
                {
                    printf("\n>> PC can't summon, no cards left in his hand.");
                }
                else
                {
                    pcSummon(pcHand, pcTable, p2TablePos);
                    p2TablePos++;
                }
            }
        }

        else // TURNO DEL USUARIO **********
        {
            printf(", YOUR turn]\n\n");

            // SI TODAVÍA QUEDAN CARTAS EN EL MAZO, SE ROBA 1 CARTA
            if(p1HandPos < 15)
            {
                drawPlayerCard(playerCardPool, playerHand, p1HandPos);
                p1HandPos++;
            }

            printf("\n>> CARDS IN HAND: \n\n");
            for(int i = 0; i < 15; i++)
            {
                if(playerHand[i].LP != -1)
                {
                    printf("%d. %s, %s, %d, %d, %d\n", i,playerHand[i].name, playerHand[i].type, playerHand[i].LP, playerHand[i].AP, playerHand[i].DP);
                    inHandCounterP1++;
                }
            }

            if(inHandCounterP1 == 15)
            {
                canSummon1 = false;
            }
            else {canSummon1 = true;}
            if(inTableCounterP1 == 0)
            {
                canAttack1 = false;
            }
            else{canAttack1 = true;}

            printf("\n>> What are you going to do?\n1. Attack\n2. Summon a Guardian from your hand\n\n>> Your option: ");
            do
            {
                scanf("%d", &action);
                if(action < 1 || action > 2)
                {
                    printf(">> Error. Enter a valid option. Try Again.\n");
                }
            } while (action < 1 || action > 2);

            // COMANDO ATACAR
            if(action == 1)
            {
                if(turn == 1 || canAttack1 == false)
                {
                    printf("\n>> You can't attack a card. Summoning instead...");
                    action = 2;
                }
                else
                {
                    pcHP = playerAttack(playerTable, pcTable, pcHP);
                }
            }

            // COMANDO INVOCAR
            if(action == 2)
            {
                if(canSummon1 == false)
                {
                    printf("\n>> You can't summon, you have no cards left in your hand.");
                }
                else
                {
                    playerSummon(playerHand, playerTable, p1TablePos);
                    p1TablePos++;
                }
            }
        }
        turn++;
        inHandCounterP1 = 0;
        inHandCounterP2 = 0;
        inTableCounterP1 = 0;
        inTableCounterP2 = 0;
        if(playerHP == 0 || pcHP == 0)
        {
            isOver = true;
        }
        printf("\n\n>> Pause.");
        getchar();
        getchar();
        system("cls");
    } while(isOver == false);

    if(playerHP == 0)
    {
        printf(">> You have lost all your lives. Your opponent WINS!");
    }
    if(pcHP == 0)
    {
        printf(">> Your opponent have lost all his lives. YOU WIN!");
    }
    getchar();
    getchar();
    system("cls");
}

int main()
{
    srand(time(NULL));

    Card *head = NULL;
    Card *newCard;

    char name[50], type[15];
    int option=0, ntype = 0, lp = 0, ap = 0, dp = 0;

    readFile(&head);

    // SE GUARDA EN ESTE ARREGLO EL ÍNDICE DE LAS CARTAS DEL MAZO PRINCIPAL, SERÁN IRREPETIBLES
    int selectedCardsIndex[30];
    for(int i = 0; i < 30; i++) {selectedCardsIndex[i]=-1;}

    // ARREGLO DEL MAZO PRINCIPAL
    Card deck[MAX_CARDS];
    initDeckArray(deck, MAX_CARDS);

    // ARREGLO DE CARTAS QUE ACTUARÁN COMO LAS 15 REPARTIDAS A CADA JUGADOR, AL AZAR
    Card playerCardPool[15];
    Card pcCardPool[15];

    // ARREGLO DE CARTAS QUE SE ENCONTRARÁN EN LA MANO DE LOS JUGADORES
    Card playerHand[15];
    Card pcHand[15];
    initDeckArray(playerHand, 15);
    initDeckArray(pcHand, 15);

    // ARREGLO DE CARTAS QUE SE ENCONTRARÁN EN LA MESA DE JUEGO PARA COMBATIR
    Card playerTable[15];
    Card pcTable[15];
    initDeckArray(playerTable, 15);
    initDeckArray(pcTable, 15);

    // MENU

    do
    {
        printf("THE CLASH OF THE GUARDIANS\n\n");
        printf("1. Play\n2. Create Card\n3. View History\n4. Exit Game\n\n");
        printf(">> Your option: ");
        scanf("%d", &option);
        switch(option)
        {
            // JUGAR
            case 1:
                system("cls");
                if(cardAmount >= 30) // SE NECESITAN MÍNIMO 30 CARTAS PARA REALIZAR LA REPARTICIÓN
                {
                    listToArray(head, deck); // LAS CARTAS PASAN DESDE LA LISTA HACIA EL ARREGLO

                    fillCardPool(selectedCardsIndex); // SE RELLENA EL ARREGLO DE LAS CARTAS QUE SERÁN REPARTIDAS A LOS JUGADORES

                    getPlayerCards(deck, playerCardPool,selectedCardsIndex); // SE OBTIENEN CARTAS PARA EL JUGADOR DESDE EL ARREGLO DE CARTAS REPARTIDAS
                    getPCCards(deck, pcCardPool,selectedCardsIndex); // SE OBTIENEN CARTAS PARA EL PC DESDE EL ARREGLO DE CARTAS REPARTIDAS

                    playerCardSelection(playerCardPool, playerHand); // EL JUGADOR ELIGE MANUALMENTE CON QUÉ CARTAS DESEA JUGAR
                    pcCardSelection(pcCardPool, pcHand); // LA MÁQUINA OBTIENE SUS PRIMERAS 3 CARTAS AL AZAR

                    printf("\n\n>> Your hand: \n");
                    for(int i = 0; i < 15; i++)
                    {
                        if(playerHand[i].LP != -1)
                        {
                            printf("%d. %s, %s, %d, %d, %d\n", i,playerHand[i].name, playerHand[i].type, playerHand[i].LP, playerHand[i].AP, playerHand[i].DP);
                        }
                    }

                    printf("\n>> Game Start [Press enter]");
                    getchar();
                    getchar();
                    system("cls");

                    gameStart(playerCardPool, pcCardPool, playerHand, pcHand, playerTable, pcTable); // FUNCIÓN DONDE SE DESARROLLA EL JUEGO

                    // LOS ARREGLOS SE VUELVEN A INICIALIZAR A SUS VALORES PREDETERMINADOS PARA PODER JUGAR UNA NUEVA PARTIDA
                    initDeckArray(playerHand, 15);
                    initDeckArray(pcHand, 15);
                    initDeckArray(playerTable, 15);
                    initDeckArray(pcTable, 15);
                }
                else
                {
                    printf("\n>> You need at least 30 cards to play. [%d/30]\n", cardAmount);
                }
                getchar();
                getchar();
                system("cls");
                break;
            case 2: // CREAR CARTA
                system("cls");
                if(cardAmount < MAX_CARDS) // SE CREAN MÁS CARTAS MIENTRAS LA CANTIDAD ACTUAL NO SOBREPASE EL LÍMITE PERMITIDO
                {
                    printf(">> Card Name: ");
                    gets(name);
                    gets(name);

                    do
                    {
                        printf(">> Card Type: \n1. Mage\n2. Viking\n3. Necromancer\n4. Beast\n");
                        scanf("%d", &ntype);
                        if(ntype == 1 || ntype == 2 || ntype == 3 || ntype == 4){}
                        else
                        {
                            printf(">> Error. Select a valid number. Try Again.\n");
                        }
                    }while(ntype < 1 || ntype > 4);
                    switch(ntype)
                    {
                    case 1:
                        strcpy(type, "Mage");
                        break;
                    case 2:
                        strcpy(type, "Viking");
                        break;
                    case 3:
                        strcpy(type, "Necromancer");
                        break;
                    case 4:
                        strcpy(type, "Beast");
                        break;
                    }

                    do
                    {
                        printf(">> Card Life Points: ");
                        scanf("%d", &lp);
                        if(lp < MIN_LP)
                        {
                            printf(">> Error. Life Points are below the limit [%d]. Try Again.\n", MIN_LP);
                        }
                        if(lp > MAX_LP)
                        {
                            printf(">> Error. Life Points exceeds the limit [%d]. Try Again.\n", MAX_LP);
                        }
                    }while(lp < MIN_LP || lp > MAX_LP);

                    do
                    {
                        printf(">> Card Attack Points: ");
                        scanf("%d", &ap);
                        if(ap < MIN_AP)
                        {
                            printf(">> Error. Attack Points are below the limit [%d]. Try Again.\n", MIN_AP);
                        }
                        if(ap > MAX_AP)
                        {
                            printf(">> Error. Attack Points exceeds the limit [%d]. Try Again.\n", MAX_AP);
                        }
                    }while(ap < MIN_AP || ap > MAX_AP);

                    do
                    {
                        printf(">> Card Defense Points: ");
                        scanf("%d", &dp);
                        if(dp < MIN_DP)
                        {
                            printf(">> Error. Defense Points are below the limit [%d]. Try Again.\n", MIN_DP);
                        }
                        if(dp > MAX_DP)
                        {
                            printf(">> Error. Defense Points exceeds the limit [%d]. Try Again.\n", MAX_DP);
                        }
                    }while(dp < MIN_DP || dp > MAX_DP);

                    // UNA VEZ RECOGIDOS TODOS LOS DATOS, ESTOS SE AÑADEN A UNA NUEVA ESTRUCTURA
                    // QUE POSTERIORMENTE SERÁ AÑADIDA A LA LISTA DE CARTAS
                    newCard = createCard(name, type, lp, ap, dp);
                    addCard(&head, newCard);
                    cardAmount++;
                    printf("\n>> Card saved successfully.");
                }
                else
                {
                    printf(">> You have reached the card limit.\n");
                }
                getchar();
                getchar();
                system("cls");
                break;
            case 3: // VER HISTORIAL
                system("cls");
                printf(">> Game history is not available yet.\n");
                getchar();
                getchar();
                system("cls");
                break;
            case 4: // SALIR DEL MENÚ
                printf("\n>> Closing game...\n");
                break;
            case 99: // revisar lista de cartas
                system("cls");
                printCards(head);
                getchar();
                getchar();
                system("cls");
                break;
            case 98: // revisar valores máximos
                system("cls");
                printf(">> MAX LP: %d, MAX AP: %d, MAX DP: %d\n>> MIN LP: %d, MIN AP: %d, MIN DP: %d\n>> Card Amount: %d\n", MAX_LP, MAX_AP, MAX_DP, MIN_LP, MIN_AP, MIN_DP,cardAmount);
                getchar();
                getchar();
                system("cls");
                break;
            default:
                system("cls");
                break;
        }
    } while(option != 4);

    freeCards(head);

    return 0;
}
