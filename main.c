#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

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

void initDeckArray(Card deck[], int limit)
{
    for(int i = 0; i < limit; i++)
    {
        strcpy(deck[i].name, "");
        strcpy(deck[i].type, "");
        deck[i].LP = 0;
        deck[i].AP = 0;
        deck[i].DP = 0;
    }
}

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

void fillCardPool(int selectedCardsIndex[])
{
    bool saveCard = false;
    int pos = 0;
    while(pos < 30)
    {
        int randomIndex = rand() % cardAmount; // GENERA UN NÚMERO ALEATORIO ENTRE 0 Y EL NÚMERO DE CARTAS EN JUEGO

        for(int i = 0; i < 30; i++)
        {
            if(randomIndex == selectedCardsIndex[i])
            {
                saveCard = false;
                break;
            }
            else
            {
                saveCard = true;
            }
        }

        if(saveCard == true)
        {
            selectedCardsIndex[pos] = randomIndex;
            pos++;
        }
    }
}

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

    printf(">> Player Cards: \n\n");
    for(int g = 0; g < 15; g++)
    {
        printf("%d. %s, %s, %d, %d, %d\n", g,playerCardPool[g].name, playerCardPool[g].type, playerCardPool[g].LP, playerCardPool[g].AP, playerCardPool[g].DP);
    }
    printf("\n");
}

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

    printf("\n>> PC Cards: \n\n");
    for(int l = 0; l < 15; l++)
    {
        printf("%d. %s, %s, %d, %d, %d\n", l,pcCardPool[l].name, pcCardPool[l].type, pcCardPool[l].LP, pcCardPool[l].AP, pcCardPool[l].DP);
    }
    printf("\n");
}

void playerCardSelection(Card playerCardPool[], Card playerHand[])
{
    int c1=0, c2=0, c3=0;
    printf("\n>> Choose 3 cards from the pool by typing the index.\n");

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

    printf("\n>> Done!");
}

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

    printf("\n>> PC cards in hand!");
}



int main()
{
    srand(time(NULL));

    Card *head = NULL;
    Card *newCard;

    char name[50], type[15];
    int option=0, ntype = 0, lp = 0, ap = 0, dp = 0; //p1HandPos = 0, p2HandPos = 0;

    readFile(&head);

    // GUARDAR EN ESTE ARREGLO EL ÍNDICE DE LAS CARTAS, QUE SERÁN IRREPETIBLES
    int selectedCardsIndex[30];
    for(int i = 0; i < 30; i++) {selectedCardsIndex[i]=-1;}

    Card deck[MAX_CARDS];
    initDeckArray(deck, MAX_CARDS);

    Card playerCardPool[15];
    Card pcCardPool[15];

    Card playerHand[15];
    Card pcHand[15];
    initDeckArray(playerHand, 15);
    initDeckArray(pcHand, 15);

    // MENU

    do
    {
        printf("THE CLASH OF THE GUARDIANS\n\n");
        printf("1. Play\n2. Create Card\n3. View History\n4. Exit Game\n\n");
        printf(">> Your option: ");
        scanf("%d", &option);
        switch(option)
        {
            case 1: // JUGAR
                system("cls");
                if(cardAmount >= 30)
                {
                    listToArray(head, deck); // PASO LAS CARTAS DESDE LA LISTA HACIA UN ARREGLO

                    fillCardPool(selectedCardsIndex);

                    getPlayerCards(deck, playerCardPool,selectedCardsIndex);
                    getPCCards(deck, pcCardPool,selectedCardsIndex);

                    playerCardSelection(playerCardPool, playerHand);
                    pcCardSelection(pcCardPool, pcHand);

                    printf("\n\n>> Player hand: \n");
                    for(int i = 0; i < 15; i++)
                    {
                        if(playerHand[i].LP != 0)
                        {
                            printf("%d. %s, %s, %d, %d, %d\n", i,playerHand[i].name, playerHand[i].type, playerHand[i].LP, playerHand[i].AP, playerHand[i].DP);
                        }
                    }

                    printf("\n\n>> PC hand: \n");
                    for(int i = 0; i < 15; i++)
                    {
                        if(pcHand[i].LP != 0)
                        {
                            printf("%d. %s, %s, %d, %d, %d\n", i,pcHand[i].name, pcHand[i].type, pcHand[i].LP, pcHand[i].AP, pcHand[i].DP);
                        }
                    }





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
                if(cardAmount < MAX_CARDS)
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
