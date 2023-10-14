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

void initDeckArray(Card deck[])
{
    for(int i = 0; i < MAX_CARDS; i++)
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
    printf(">> Index: \n\n");
    for(int k = 0; k < 30; k++)
    {
        printf("%d. %d\n", k, selectedCardsIndex[k]);
    }
    printf("\n");
}

void getPlayerCards(Card deck[], Card playerCardPool[],int selectedIndex[])
{
    int pos = 0;
    for(int i = 0; i < 15; i++)
    {
        strcpy(playerCardPool[i].name, deck[selectedIndex[pos]].name);
        printf("%s -- %s\n", playerCardPool[i].name, deck[selectedIndex[pos]].name);
        strcpy(playerCardPool[i].type, deck[selectedIndex[pos]].type);
        playerCardPool[i].LP = deck[selectedIndex[pos]].LP;
        playerCardPool[i].AP = deck[selectedIndex[pos]].AP;
        playerCardPool[i].DP = deck[selectedIndex[pos]].DP;
        pos++;
    }

    printf("\n>> Player Cards: \n\n");
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
        printf("%s -- %s\n", pcCardPool[i].name, deck[selectedIndex[pos]].name);
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
    printf(">> Choose 3 cards from the pool by typing the index.");

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
        printf("\nCard number 2: ");
        scanf("%d", &c2);
        if(c2 < 0 || c2 > 14 || c2 == c1)
        {
            printf(">> Error. Index entered is not valid. Try Again.\n");
        }
    }while(c2 < 0 || c2 > 14 || c2 == c1);

    do
    {
        printf("\nCard number 3: ");
        scanf("%d", &c3);
        if(c3 < 0 || c3 > 14 || c3 == c2 || c3 == c1)
        {
            printf(">> Error. Index entered is not valid. Try Again.\n");
        }
    }while(c3 < 0 || c3 > 14 || c3 == c2 || c3 == c1);

    playerHand[0] = playerCardPool[c1];
    playerHand[1] = playerCardPool[c2];
    playerHand[2] = playerCardPool[c3];
}





int main()
{
    srand(time(NULL));

    Card *head = NULL;
    Card *newCard;

    char name[50], type[15];
    int option=0, ntype = 0, lp = 0, ap = 0, dp = 0, p1HandPos = 0, p2HandPos = 0;

    readFile(&head);

    // GUARDAR EN ESTE ARREGLO EL ÍNDICE DE LAS CARTAS, QUE SERÁN IRREPETIBLES
    int selectedCardsIndex[30];
    for(int i = 0; i < 30; i++) {selectedCardsIndex[i]=-1;}

    Card deck[MAX_CARDS];
    initDeckArray(deck);

    Card playerCardPool[15];
    Card pcCardPool[15];

    Card playerHand[15];
    Card pcHand[15];


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
                    printf("Monos: \n");
                    for(int i = 0; i < cardAmount; i++)
                    {
                        printf("%d. %s, %s, %d, %d, %d\n", i,deck[i].name, deck[i].type, deck[i].LP, deck[i].AP, deck[i].DP);
                    }
                    printf("\n");

                    fillCardPool(selectedCardsIndex);

                    getPlayerCards(deck, playerCardPool,selectedCardsIndex);
                    getPCCards(deck, pcCardPool,selectedCardsIndex);

                    /*
                    printf("\n\n>> Player hand: \n");
                    for(int i = 0; i < 15; i++)
                    {
                        if(playerCardPool != -1)
                        {
                            printf("%d. %s, %s, %d, %d, %d\n", i,playerHand[i].name, playerHand[i].type, playerHand[i].LP, playerHand[i].AP, playerHand[i].DP);
                        }
                    }*/



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
