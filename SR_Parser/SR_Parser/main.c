//
//  main.c
//  SR_Parser
//
//  Created by Jai Verma on 26/02/16.
//  Copyright © 2016 Jai Verma. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct production
{
    char symbol;
    char *prod;
    struct production *next;
};

//return and create a new production
struct production *new_production(char*);
//add production to linked list
struct production *add_production(struct production*, struct production*);
//display production linked list
void display_productions(struct production*);
//shift till end of string
void shift(char[], int*, char[], int *);
//try to reduce string in stack
int try_reduce(char[], int, struct production*);
//replace subsrting
void str_replace(char *, int, char);

int main()
{
    struct production *head = NULL;
    char start_symbol = '\0';
    FILE *fp;
    char *production = NULL;
    size_t len = 0;
    ssize_t read;
    
    //load grammar into linked list
    //------------------------------------------------------------------------
    fp = fopen("/Users/BATMAN/Desktop/grammar.txt", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    int j,counter = 0;
    while ((read = getline(&production, &len, fp)) != -1)
    {
        for (j = 0; production[j] != 0; j++);
        production[j-1] = '\0';
        head = add_production(head, new_production(production));
        if (counter == 0)
            start_symbol = production[0];
        production = NULL;
        counter++;
    }
    
    fclose(fp);
    if (production)
        free(production);
    display_productions(head);
    //------------------------------------------------------------------------
    
    //enter input string to be parsed
    //------------------------------------------------------------------------
    int i;
    char w[20];
    printf("Enter input string\n");
    fgets(w, 20, stdin);
    for (i = 0; w[i]!='\0'; i++);
    w[i-1] = '\0';

    int size_of_input = i;
    //------------------------------------------------------------------------
    
    //don't need to create a stack, stack can just be a string
    char stack[30] = "";
    int input_index, stack_index;
    stack[0] = '$';
    
    printf("********Stack********\n");
    for (input_index = 0, stack_index = 1;;)
    {
        //don't shift once the whole input has been shifted to stack
        if (size_of_input > 0) {
            shift(stack, &stack_index, w, &input_index);
            size_of_input--;
        }

        //print stack
        //--------------------------------------------------------------------
        for (int i = 0; i < stack_index; i++) {
            printf("%c ", stack[i]);
        }
        printf("\n");
        //--------------------------------------------------------------------

        //for debugging
        //printf("%d", size_of_input);
        
        //head is linked list of productions
        int reduction  = try_reduce(stack, stack_index, head);
        if (reduction > 0)
        {
            stack_index -= reduction - 1;
        }
        
        //if reduction doesn't take place after the entire string has been processed
        //then processing is done
        //break and check result
        else if (reduction == -1 && size_of_input == 0)
            break;
        
    }
    printf("*********************");
    
    if (strlen(stack) == 2 && stack[1] == start_symbol)
        printf("\nParsing Successful!\n");
    
    else
        printf("\nParsing failed!\n");
    
    //release linked list from memory
    if (head)
        free(head);
    
    return 0;
}

struct production *new_production(char *string)
{
    struct production *new = (struct production*)malloc(sizeof(struct production));
    new->symbol = string[0];
    new->prod = string + 3;
    new->next = NULL;
    string = NULL;
    return new;
}

struct production *add_production(struct production* start, struct production* new)
{
    if (start == NULL)
        start = new;
    
    else
    {
        struct production *temp = start;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = new;
    }
    return start;
}

void display_productions(struct production *s)
{
    struct production *t = s;
    while (t != NULL) {
        printf("%c->%s\n", t->symbol, t->prod);
        t = t->next;
    }
}

void shift(char stack[], int *stack_index, char w[], int *input_index)
{
    stack[*stack_index] = w[*input_index];
    *stack_index += 1;
    *input_index += 1;
}

int try_reduce(char stack[], int stack_index, struct production* link)
{
    int flag = 0;
    int index = stack_index;
    int j = index - 1;
    char comparison[20] = "";
    while (j >= 1)
    {
        struct production* cur = link;
        strncpy(comparison, stack + j, index - j);
        
        while (cur != NULL) {
            if (strncmp(comparison, cur->prod, index - j + 1) == 0)
            {
                flag = 1;
                str_replace(stack, j, cur->symbol);
                return index - j;
            }
            else
            {
                cur = cur->next;
            }
        }
        j--;
    }
    if (flag == 0) {
        return -1;
    }
    return 0;
}

void str_replace(char *source, int start_index, char replacement)
{
    source[start_index] = replacement;
    source[start_index + 1] = '\0';
}
