#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define max 300
typedef struct ids
{
    char oznacenie[2];
    char cislovanie[4];
    char druh[2];
} IDS;
typedef struct module
{
    char latitude[9];
    char longitude[9];
} MODULE;
typedef struct person
{
    char Typ[3];
    char Hodnota[max];
    char Cas[5];
    char Datum[10];
    struct person *next;
    IDS *ids;
    MODULE *module;
} PERSON;

void nacitaj(PERSON **head);    // done
void vypis(PERSON *head);       // done
void pridaj(PERSON **head);     // done
void zmaz(PERSON **head);       // done
void usporiadaj(PERSON **head); // WIP
void relokacia(PERSON **head);  // done

int main(void)
{
    PERSON *head = NULL;
    PERSON *curr = NULL;
    char c = ' ';
    printf("\nNa Vyber mas tieto funkcie:\n");
    printf(" N - Nacitanie dat\n"
           " V - Vypis dat\n"
           " P - Pridanie dat\n"
           " Z - Zmazanie dat\n"
           " U - Usporiadanie dat\n"
           " R - Relokacia dat\n"
           " K - Koniec\n");
    nacitaj(&head);
    vypis(head);
    usporiadaj(&head);
    vypis(head);
    while (c != 'q')
    {
        printf("Zadaj funckiu: ");
        scanf(" %c", &c);
        printf("\n");
        switch (c)
        {
        case 'n':
        case 'N':
            nacitaj(&head);
            break;
        case 'v':
        case 'V':
            vypis(head);
            break;
        case 'p':
        case 'P':
            pridaj(&head);
            break;
        case 'z':
        case 'Z':
            zmaz(&head);
            break;
        case 'u':
        case 'U':
            usporiadaj(&head);
            break;
        case 'r':
        case 'R':
            relokacia(&head);
            break;
        case 'k':
        case 'K':
            curr = head;
            while (curr != NULL)
            {
                IDS *ids_temp = curr->ids;
                free(ids_temp);
                MODULE *module_temp = curr->module;
                free(module_temp);
                PERSON *temp = curr;
                free(temp); // Free the current element
                curr = curr->next;
            }
            curr = NULL;
            head = NULL;
            return 0;
            break;
        }
    }
    return 0;
}

void nacitaj(PERSON **head)
{
    FILE *subor = NULL;
    PERSON *curr = *head;
    char buffer[max];
    int count = 0;

    if (subor == NULL)
    {
        subor = fopen("dataloger_V2.txt", "r");
        if (subor == NULL)
        {
            printf("“Zaznamy neboli nacitane!\n");
            return;
        }
        rewind(subor);
    }
    if (*head != NULL)
    {
        curr = *head;
        while (curr != NULL)
        {
            IDS *ids_temp = curr->ids;
            free(ids_temp);
            MODULE *module_temp = curr->module;
            free(module_temp);
            PERSON *temp = curr;
            curr = curr->next;
            free(temp); // Free the current element
        }
        curr = NULL;
        *head = NULL;
        printf("Clearing all memory\n");
    }

    curr = (*head);
    while (fgets(buffer, sizeof(buffer), subor))
    {
        if ((strlen((const char *)buffer) > 0) && ((buffer[strlen((const char *)buffer) - 1] == '\r') || (buffer[strlen((const char *)buffer) - 1] == '\n'))) // Check if the line contains a '\r' or '\n' character at the end, remove it.
        {
            buffer[strlen((const char *)buffer) - 1] = '\0';
        }
        if (strstr(buffer, "$$$"))
            continue;
        count++;
        if (count % 6 == 1)
        {
            if (curr == NULL) // If the list is empty, allocate memory for the head/first element
            {
                curr = (PERSON *)malloc(sizeof(PERSON));
            }
            else // Otherwise allocate memory for the next element
            {
                curr->next = (PERSON *)malloc(sizeof(PERSON));
                curr = curr->next;
                if (curr == NULL) // If the memory allocation failed, print an error message and return
                {
                    printf("Error while allocating memory\n");
                    return;
                }
            }
            curr->next = NULL;
            if (curr->ids == NULL)
            {
                curr->ids = (IDS *)malloc(sizeof(IDS));
                if (curr->ids == NULL)
                {
                    printf("Error while allocating memory\n");
                    return;
                }
            }
            if ((*head) == NULL) // If the list is empty, set the head to be the current element
            {
                (*head) = curr;
            }
            strncpy(curr->ids->oznacenie, buffer, 1);
            strncpy(curr->ids->cislovanie, buffer + 1, 3);
            strncpy(curr->ids->druh, buffer + 4, 1);
        }
        else if (count % 6 == 2)
        {
            if (curr->module == NULL)
            {
                curr->module = (MODULE *)malloc(sizeof(MODULE));
                if (curr->module == NULL) // If the memory allocation failed, print an error message and return
                {
                    printf("Error while allocating memory\n");
                    return;
                }
            }
            strncpy(curr->module->latitude, buffer, 8);
            strncpy(curr->module->longitude, buffer + 8, 8);
        }
        else if (count % 6 == 3)
        {
            strcpy(curr->Typ, buffer);
        }
        else if (count % 6 == 4)
        {
            strcpy(curr->Hodnota, buffer);
        }
        else if (count % 6 == 5)
        {
            strcpy(curr->Cas, buffer);
        }
        else if (count % 6 == 0)
        {
            strcpy(curr->Datum, buffer);
        }
    }
    curr = (*head);
    if (fclose(subor) == EOF)
    {
        printf("Error while closing file\n");
    }
    printf("Nacitalo sa %d zaznamov\n", count / 6);
}
void vypis(PERSON *head)
{
    PERSON *curr = head;
    IDS *ids = NULL;
    MODULE *module = NULL;
    if (head != NULL)
    {
        for (int i = 0; curr != NULL; i++)
        {
            printf("%d:\n", i + 1);
            printf("ID: %s%s%s   %s  %s\n", curr->ids->oznacenie, curr->ids->cislovanie, curr->ids->druh, curr->Typ, curr->Hodnota);
            printf("Poz: %s   %s\n", curr->module->latitude, curr->module->longitude);
            printf("Dac: %s\t%s\n", curr->Datum, curr->Cas);
            curr = curr->next;
        }
        printf("\n");
    }
}
void pridaj(PERSON **head)
{
    PERSON *curr = (*head);
    PERSON *last = NULL;
    char buffer[max];
    int pos = 0;

    PERSON *new = (PERSON *)malloc(sizeof(PERSON));
    if (new == NULL)
    {
        printf("Error while allocating memory\n");
        return;
    }

    new->ids = (IDS *)malloc(sizeof(IDS));
    if (new->ids == NULL)
    {
        printf("Error while allocating memory\n");
        free(new);
        return;
    }

    new->module = (MODULE *)malloc(sizeof(MODULE));
    if (new->module == NULL)
    {
        printf("Error while allocating memory\n");
        free(new->ids);
        free(new);
        return;
    }

    printf("Zadaj poziciu na ktoru chces pridat zaznam: ");
    scanf("%d", &pos);
    if (pos > 0)
    {
        printf("Zadaj ID: ");
        scanf("%s", buffer);
        if ((isupper(buffer[0]) && isdigit(buffer[1]) && isdigit(buffer[2]) && isdigit(buffer[3]) && islower(buffer[4]) && buffer[5] == '\0'))
        {
            strncpy(new->ids->oznacenie, buffer, 1);
            strncpy(new->ids->cislovanie, buffer + 1, 3);
            strncpy(new->ids->druh, buffer + 4, 1);
            scanf("%s", buffer);
            if ((buffer[0] == '+') && (isdigit(buffer[1])) && (isdigit(buffer[2])) && (isdigit(buffer[3])) && (isdigit(buffer[4])) && (isdigit(buffer[5])) && (isdigit(buffer[6])) && (buffer[7] == '+') && (isdigit(buffer[8])) && (isdigit(buffer[9])) && (isdigit(buffer[10])) && (isdigit(buffer[11])) && (isdigit(buffer[12])) && (isdigit(buffer[13])))
            {
                strncpy(new->module->latitude, buffer, 3);
                strcat(new->module->latitude, ".");
                strncat(new->module->latitude, (buffer + 3), 4);
                strncpy(new->module->longitude, (buffer + 7), 3);
                strcat(new->module->longitude, ".");
                strncat(new->module->longitude, (buffer + 10), 4);
                scanf("%s", buffer);
                if (strstr(buffer, "RM") || strstr(buffer, "RD") || strstr(buffer, "RO") || strstr(buffer, "PI") || strstr(buffer, "PE") || strstr(buffer, "PA"))
                {
                    strncpy(new->Typ, buffer, 2);
                    scanf("%s", new->Hodnota);
                    scanf("%s", buffer);
                    if ((isdigit(buffer[0])) && (isdigit(buffer[1])) && (isdigit(buffer[2])) && (isdigit(buffer[3])))
                    {
                        strncpy(new->Cas, buffer, 4);
                        scanf("%s", buffer);
                        if ((isdigit(buffer[0])) && (isdigit(buffer[1])) && (isdigit(buffer[2])) && (isdigit(buffer[3])) && (isdigit(buffer[4])) && (isdigit(buffer[5])) && (isdigit(buffer[6])) && (isdigit(buffer[7])))
                        {
                            strncpy(new->Datum, buffer, 8);
                            new->next = NULL;
                            if (curr == NULL)
                            {
                                curr = new;
                                (*head) = curr;
                            }
                            else
                            {
                                while (curr != NULL && pos > 1)
                                {
                                    last = curr;
                                    curr = curr->next;
                                    pos--;
                                }

                                last->next = new;
                                new->next = curr;
                            }
                        }
                    }
                }
            }
        }
    }
    else
        return;
}
void zmaz(PERSON **head)
{
    PERSON *curr = (*head);
    PERSON *temp = NULL;
    PERSON *prev = NULL;
    char buffer[max];
    if (curr != NULL)
    {
        printf("Zadaj ID: ");
        scanf("%s", buffer);
        buffer[strcspn(buffer, "\n")] = '\0';

        if ((isupper(buffer[0]) && isdigit(buffer[1]) && isdigit(buffer[2]) && isdigit(buffer[3]) && islower(buffer[4]) && buffer[5] == '\0'))
        {
            char ID[2];
            char Cislovanie[4];
            char Oznacenie[5];
            strncpy(ID, buffer, 1);
            strncpy(Cislovanie, buffer + 1, 3);
            strncpy(Oznacenie, buffer + 4, 1);
            for (int i = 0; curr != NULL; i++)
            {
                if ((strstr(curr->ids->oznacenie, ID)) && (strstr(curr->ids->cislovanie, Cislovanie)) && (strstr(curr->ids->druh, Oznacenie)))
                {
                    if (curr == (*head))
                    {
                        printf("Zaznam pre ID: %s%s%s bol vymazany.\n", curr->ids->oznacenie, curr->ids->cislovanie, curr->ids->druh);
                        temp = curr;
                        (*head) = curr->next;
                        curr = (*head);
                        free(temp);
                    }
                    else
                    {
                        printf("Zaznam pre ID: %s%s%s bol vymazany.\n", curr->ids->oznacenie, curr->ids->cislovanie, curr->ids->druh);
                        temp = curr;
                        if (curr->next != NULL)
                        {
                            prev->next = curr->next;
                            curr = curr->next;
                        }
                        else
                        {
                            prev->next = NULL;
                            curr = NULL;
                        }
                        free(temp);
                    }
                }
                else
                {
                    prev = curr;
                    curr = curr->next;
                }
            }
        }
    }
}
void usporiadaj(PERSON **head)
{
    PERSON *curr = *head;
    PERSON *prev = NULL, *next = NULL, *end = NULL;
    int swapped = 0;
    int count = 0, count_after = 0;

    if ((*head) == NULL)
    {
        return;
    }
    while (curr != NULL)
    {
        count++;
        curr = curr->next;
    }
    curr = *head;
    do
    {
        prev = NULL;
        swapped = 0;
        curr = (*head);
        while (curr->next != end)
        {
            next = curr->next;
            if (strcmp(curr->Datum, next->Datum) < 0 || (strcmp(curr->Datum, next->Datum) == 0 && strcmp(curr->Cas, next->Cas) < 0))
            {
                if (prev == NULL)
                {
                    curr->next = next->next;
                    next->next = curr;
                    (*head) = next;
                    swapped = 1;
                }
                else
                {
                    curr->next = next->next;
                    next->next = curr;
                    prev->next = next;
                    swapped = 1;
                }
                prev = next;
            }
            else
            {
                prev = curr;
                curr = curr->next;
            }
        }
        end = curr;
    } while (swapped);

    curr = *head;
    while (curr != NULL)
    {
        count_after++;
        curr = curr->next;
    }
    if (count == count_after)
    {
        printf("Spajany zoznam bol usporiadany.\n");
    }
    else if (count != count_after)
    {
        printf("Chyba usporiadania.\n");
    }
}
void relokacia(PERSON **head)
{
    PERSON *curr = (*head);
    PERSON *second_curr = (*head);
    PERSON *prev = NULL;
    PERSON *second_prev = NULL;
    PERSON *temp = NULL;
    int pos1 = 0, pos2 = 0;
    int count = 0;
    if (*head != NULL)
    {
        while (curr != NULL)
        {
            count++;
            curr = curr->next;
        }
        curr = (*head);
        printf("Zadaj poziciu prveho zaznamu a poziciu druheho zaznamu: ");
        scanf("%d %d", &pos1, &pos2);
        if ((!(pos1 < 1) || !(pos2 < 1)) && ((pos1 <= count) && (pos2 <= count)))
        {
            while (curr != NULL && pos1 > 1) // Find the element before the first element
            {
                prev = curr;       // Save the previous element
                curr = curr->next; // Move to the next element
                pos1--;            // Decrement the position
            }
            while (second_curr != NULL && pos2 > 1) // Find the element before the second element
            {
                second_prev = second_curr;       // Save the previous element
                second_curr = second_curr->next; // Move to the next element
                pos2--;                          // Decrement the position
            }
            if (prev == NULL) // If the first element is the head, set the head to be the second element
            {
                (*head) = second_curr; // Set the head to be the second element
            }
            else
            {
                prev->next = second_curr; // Set the next element of the first element to be the second element
            }
            if (second_prev == NULL) // If the second element is the head, set the head to be the first element
            {
                (*head) = curr; // Set the head to be the first element
            }
            else
            {
                second_prev->next = curr; // Set the next element of the second element to be the first element
            }
            temp = curr->next; // Swap the next elements of the two elements
            curr->next = second_curr->next;
            second_curr->next = temp;
        }
    }
}
