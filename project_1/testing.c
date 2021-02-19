#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//#include "autocomplete.h"

struct term{
    char term[200]; // assume terms are not longer than 200 chars
    double weight;
};

static int comp_func(const void *str1, const void *str2){
    return strcmp(str1, str2);
}

void read_in_terms(struct term **terms, int *pnterms, char *filename)
{
    // each new block is stored in *terms 
    FILE *fp = fopen("cities2.txt", "r");

    // count number of terms
    fscanf(fp, "%d", pnterms);
    printf("%d\n", *pnterms);///////

    *terms = (struct term *)malloc(sizeof(struct term) * (*pnterms));
    struct term *pterms = *terms; 
    for (int i = 0; i <= *pnterms; i++)
    {
        fscanf(fp, "%lf", &((*terms)->weight)); 
        char line[200];
        fgets(line, sizeof(line), fp); 
        strcpy((*terms)->term, line+1);
        (*terms)++;
    }
    fclose(fp);
    
    // sort in alphabetical order
    qsort(pterms, *pnterms, sizeof(struct term), comp_func);
    *terms = pterms;

    for (int i = 0; i < *pnterms; i++)
    {
        printf("%s%f \n\n\n", (*terms)[i].term, (*terms)[i].weight);
    }
}

/*
char *slicing1(const char *str, int start, int end)
{
        char *res = malloc(end - start + 1);
        if (!res)
        {
            return NULL;
        }
        int i;
        for (i = 0; i < (end-start); i++)
        {
            res[i] = str[start+i];
        }
        res[i] = '\0';
        return res;
}
*/



int lowest_match(struct term *terms, int nterms, char *substr)
{
    // must run in O(log(nterms)) time : runtime complexity = (O(log(N))
    // BINARY SEARCH implementation
    // function returns the index in terms of the first term in lexical order that MATCHES the substr
    int mid = nterms / 2; 
    //char *res = slicing(terms[mid].term, 0, strlen(substr));
    char res[strlen(substr)+1];
    memcpy(res, terms[mid].term, strlen(substr));
    
    printf("\nRES: %s\n", res);
    if (nterms == 1){
        return -1;
    }
    if (strcmp(substr, res) > 0)
    {
        return ((nterms/2) + lowest_match(&terms[mid], nterms/2, substr));
    }
    if (strcmp(substr, res) < 0){
        return (lowest_match(terms, nterms/2, substr));
    }
    if (strcmp(substr, res) == 0)
    {
        if (lowest_match(terms, nterms/2, substr) != -1)
        {
            return (lowest_match(terms, nterms/2, substr));
        } else {
            return nterms/2;
        }
    }
    return -1;

}



int highest_match(struct term *terms, int nterms, char *substr)
{
 int mid = nterms / 2; // look at the middle of the array

    int mid = nterms / 2; 
    //char *res = slicing(terms[mid].term, 0, strlen(substr));
    char res[strlen(substr)+1];
    memcpy(res, terms[mid].term, strlen(substr));
    
    if (nterms == 1)
    {
        //return (int)terms; // return the (current elem index) - (index of first elem in terms)
        return -1;
    }

    if (strcmp(substr, res) > 0)
    {
        return ((nterms/2) + highest_match(&terms[mid], nterms/2, substr));
    }
    else if (strcmp(substr, res) < 0)
    {
        return (highest_match(terms, nterms/2, substr));
    }
    else if (strcmp(substr, res) == 0)
    {
        if (highest_match(&terms[mid] + 1, nterms/2, substr) != -1)
        {
            return (highest_match(&terms[mid] + 1, nterms/2, substr));
        } else {
            return nterms/;
        }
    return -1;

}



/*

void autocomplete(struct term **answer, int *n_answer, struct term *terms, int nterms, char *substr)
{
    // function takes terms sorted in alphabetical order,
    //                      number of terms nterms
    //                      query string substr
    // places answers in answer, (*n_answer) is the number of answers
    
    // SORT answers by weight 



}



*/


int main(void)
{
    struct term *terms;
    int nterms; // changes this value globally 
    read_in_terms(&terms, &nterms, "cities2.txt");

    /*
    int lowest_ind = lowest_match(terms, nterms, "Seoul");
    printf("\n\n\n%d\n", lowest_ind);
    */

    int highest_ind = highest_match(terms, nterms, "Tor");
    printf("\n\n\n%d\n", highest_ind);

    struct term *answer;
    int n_answer;
    //autocomplete(&answer, &n_answer, terms, int nterms, "Tor");
    //free allocated blocks here -- not required for the project, but good practice

    free(terms);
    return 0;
}
