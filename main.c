#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

typedef struct card
{
    char name[50];
    char type[15];
    int LP;
    int AP;
    int DP;
    struct card *next;
}Card;

#define MAX_CARDS 60
#define MAX_LINE_LENGTH 200

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

    int MAX_LP = 0, MAX_AP = 0, MAX_DP = 0;

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

        //printf("%s, %s, %d, %d, %d\n", newstruct->name,newstruct->type,newstruct->LP,newstruct->AP,newstruct->DP);

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
    Card *tail = NULL;
    Card *newCard;

    int option=0, cardsAmount = 0;
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
                if(cardsAmount >= 30)
                {

                }
                else
                {
                    printf("\n>> You need at least 30 cards to play. [%d/30]\n", cardsAmount);
                }
                getchar();
                getchar();
                system("cls");
                break;
            case 2: // Crear Carta
                system("cls");

                system("cls");
                break;
            case 3: // Ver Historial
                system("cls");

                system("cls");
                break;
            case 4: // Salir del menú de opciones y terminar el programa
                printf("\nClosing game...\n");
                break;
            case 10:
                printCards(head);
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
    freeCards(tail);

    return 0;
}
