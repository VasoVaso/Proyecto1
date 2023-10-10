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

int main()
{
    Card *head = NULL;
    Card *newCard;
    char name[50], type[15];
    int option=0, ntype = 0, lp = 0, ap = 0, dp = 0;
    srand(time(NULL));

    readFile(&head);

    // MENU

    do
    {
        printf("THE CLASH OF THE GUARDIANS\n\n");
        printf("1. Play\n2. Create Card\n3. View History\n4. Exit Game\n\n");
        printf(">> Your option: ");
        scanf("%d", &option);
        switch(option)
        {
            case 1: // Jugar
                system("cls");
                if(cardAmount >= 30)
                {

                }
                else
                {
                    printf("\n>> You need at least 30 cards to play. [%d/30]\n", cardAmount);
                }
                getchar();
                getchar();
                system("cls");
                break;
            case 2: // Crear Carta
                system("cls");
                if(cardAmount < MAX_CARDS)
                {
                    printf(">> Card Name: ");
                    scanf("%s", name);

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
            case 3: // Ver Historial
                system("cls");

                system("cls");
                break;
            case 4: // Salir del menú de opciones y terminar el programa
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
