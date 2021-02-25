#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    FILE *fp = fopen(filename, "r");
    // count number of terms
    fscanf(fp, "%d", pnterms);

    *terms = (struct term *)malloc(sizeof(struct term) * (*pnterms));
    struct term *pterms = *terms;
    for (int i = 0; i <= *pnterms; i++)
    {
        fscanf(fp, "%lf", &((*terms)->weight));
        char line[200];
        fgets(line, sizeof(line), fp);
        line[strcspn(line, "\n")] = 0;
        strcpy((*terms)->term, line+1);
        (*terms)++;
    }
    fclose(fp);

    // sort in alphabetical order
    qsort(pterms, *pnterms, sizeof(struct term), comp_func);
    *terms = pterms;
/*
    for (int i = 0; i < *pnterms; i++){
        printf("%i: %s\n", i, (*terms)[i].term);
    }
*/
}


int ceiling(double num)
{
    double temp = num - (int)num;
    if (temp >= 0.5){
        return (int)num + 1;
    }
    else return (int)num;
}

int lowest_match(struct term *terms, int nterms, char *substr)
{
    /* runtime complexity is (O(log(N))
     BINARY SEARCH implementation
     Function returns the index in terms of the first term in lexical order
    */

    int mid = (int)(nterms / 2);
    char res[strlen(substr)+1];
    memcpy(res, terms[mid].term, strlen(substr));
    //printf("\nRES-low: %s\n", res); ////
    char res_start[strlen(substr)+1];
    memcpy(res_start, terms[0].term, strlen(substr));
    char res_end[strlen(substr)+1];
    memcpy(res_end, terms[nterms-1].term, strlen(substr));

    res[strlen(substr)] = '\0';
    /*
    for (int i = 0; i < nterms; i++){
        printf("%s \n", (terms)[i].term);
    }
    printf("\n\n");
    */
    if (nterms == 0){
        return -1;
    }
    if (strcmp(substr, res_start) < 0 || strcmp(substr, res_end) > 0) {
        return -1;
    }
    if (strcmp(substr, res) > 0) {
        return ((nterms / 2) + lowest_match(&terms[mid], ceiling(nterms*0.5), substr));
    }
    if (strcmp(substr, res) < 0) {
        return (lowest_match(terms, (int)(nterms*0.5), substr));
    }
    if (strcmp(substr, res) == 0)
    {
        if (lowest_match(terms, (int)(nterms*0.5), substr) != -1)
        {
            return (lowest_match(terms, (int)(nterms*0.5), substr));
        } else {
            return nterms/2;
        }
    }
    return -1;
}


int highest_match(struct term *terms, int nterms, char *substr)
{
    int mid = nterms / 2;
    char res[strlen(substr)+1];
    memcpy(res, terms[mid].term, strlen(substr));
    //printf("\nRES-high: %s\n", res);
    char res_start[strlen(substr)+1];
    memcpy(res_start, terms[0].term, strlen(substr));
    char res_end[strlen(substr)+1];
    memcpy(res_end, terms[nterms-1].term, strlen(substr));

    res[strlen(substr)] = '\0';
    /*
    for (int i = 0; i < nterms; i++){
        printf("%s\n", (terms)[i].term);

    printf("\n\n");
    }*/

    if (nterms == 0){
        return -1;
    }
    if (strcmp(substr, res_start) < 0 || strcmp(substr, res_end) > 0) {
        return -1;
    }
    if (strcmp(substr, res) > 0){
        return ((nterms/2) + highest_match(&terms[mid], nterms*0.5, substr));
    }
    if (strcmp(substr, res) < 0){
        return (highest_match(terms, nterms*0.5, substr));
    }
    if (strcmp(substr, res) == 0){
        if (highest_match(&terms[mid] + 1, nterms*0.5 , substr) != -1){
            return ((nterms*0.5) + 1) + (highest_match(&terms[mid] + 1, nterms*0.5 , substr));
        } else {
            return nterms*0.5;
        }
    }
    return -1;
}

// parts 2a and 2b may not work for strings not found ?? should return -1
// everything else works

static int sorter(const void *term1, const void *term2)
{
    struct term *term_a = (struct term *)term1;
    struct term *term_b = (struct term *)term2;

    return (term_b->weight - term_a->weight);
}


void autocomplete(struct term **answer, int *n_answer, struct term *terms, int nterms, char *substr)
{
    // function takes terms sorted in alphabetical order,
    //                      number of terms nterms
    //                      query string substr
    // places answers in answer, (*n_answer) is the number of answers // SORT answers by weight

    int first = lowest_match(terms, nterms, substr);
    int last = highest_match(terms, nterms, substr);

    *n_answer = (last-first)+1;
    *answer = (struct term *)malloc(sizeof(struct term) * (*n_answer));
    struct term *pterms = *answer;

    for (int i = first; i <= last; i++)
    {
        double temp_weight = terms[i].weight;
        char *temp_term = terms[i].term;
        (*answer)->weight = temp_weight;
        strcpy((*answer)->term, temp_term);
        (*answer)++;
    }
    qsort(pterms, *n_answer, sizeof(struct term),  sorter); // sort the array, answer by weight
    *answer = pterms;

    for (int k = 0; k < *n_answer; k++)
    {
        printf("%s %lf\n", (*answer)[k].term, (*answer)[k].weight);
    }

}

int main(void)
{
    struct term *terms;
    int nterms; // changes this value globally
    read_in_terms(&terms, &nterms, "/Users/hassankhurram/Desktop/ESC190/project_1/randomwords.txt");

    //int lowest_ind = lowest_match(terms, nterms, "M");
    //printf("\n%d\n------------\n\n\n", lowest_ind);

    //int highest_ind = highest_match(terms, nterms, "M");
    //printf("\n%d\n", highest_ind);

    struct term *answer;
    int n_answer;

    autocomplete(&answer, &n_answer, terms, nterms, "paj");

    //free allocated blocks here -- not required for the project, but good practice
    free(terms);
    return 0;

}
