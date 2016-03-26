//
//  main.c
//  OPTable
//
//  Created by Jai Verma on 12/03/16.
//  Copyright © 2016 Jai Verma. All rights reserved.
//

//Operator Precedence Table
//assuming all operators are left to right associative

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
//assign precedence to operators
char *assign_precedence(struct production*);
//build operator table
char **build_op_table(char*, int);

int main(int argc, const char * argv[]) {
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
    
    //identify and assign precedence to operators
    //------------------------------------------------------------------------
    char *operators = assign_precedence(head);

    int i, no_of_ops = 0;
    
    printf("*********************\n");
    printf("Operators in decreasing order of precedence:\n");
    for (i = 0; operators[i] != '\0'; i++) {
        printf("%d. %c\n", i+1, operators[i]);
        no_of_ops++;
    }
    printf("*********************\n\n");
    //------------------------------------------------------------------------
    
    //build operator precedence table
    //------------------------------------------------------------------------
    char **precedence_table = build_op_table(operators, no_of_ops);
    
    printf("********Operator Precedence Table********\n");
    for (i = 0; i <= no_of_ops; i++) {
        for (j = 0; j <= no_of_ops; j++) {
            printf("%c ", precedence_table[i][j]);
        }
        printf("\n");
    }
    printf("*********************\n\n");
    //------------------------------------------------------------------------
    
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

char *assign_precedence(struct production *s)
{
    char precedence[20] = {};
    int i = 0;
    while (s != NULL) {
        char *temp = s->prod;
        while (*temp != '\0') {
            if (*temp < 65 || *temp > 90)
            {
                precedence[i] = *temp;
                i++;
            }
            temp++;
        }
        s = s->next;
    }
    printf("No of operators: %d\n", i);
    char *new_precedence = (char*)malloc(i*sizeof(char));
    int j, temp;
    for (j=0; j<i; j++) {
        printf("Enter precedence for %c (1=highest %d=lowest):\n", precedence[j], i);
        scanf("%d", &temp);
        new_precedence[temp-1] = precedence[j];
    }
    //add dollar with least precedence
    new_precedence[j++] = '$';
    //terminating array with \0
    new_precedence[j] = '\0';
    return new_precedence;
}

char **build_op_table(char *o, int n)
{
    n++;
    char **x = (char**)malloc(n*sizeof(char*));
    int i = 0;
    for (; i < n; i++) {
        x[i] = (char*)malloc(n*sizeof(char));
    }
    
    int j;

    //first row and first column hold operators
    for (i = 0; i < n-1; i++) {
        for (j = 0; j < n-1; j++) {
            if (i == 0)
                x[i][j+1] = o[j];
            
            if (j == 0)
                x[i+1][j] = o[i];
        }
    }
    
    //build precedence table
    for (i = 1; i < n; i++)
        for (j = 1; j < n; j++) {
            if (i < j)
                x[i][j] = '>';
            
            else if (i == j && o[i-1] == '$')
                x[i][j] = '-';
            
            else if (i == j)
                x[i][j] = '>';
            
            else
                x[i][j] = '<';
        }
    
    return x;
}
