#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "autocomplete.h"

// Part 1

static int comp_func(const void *str1, const void *str2)
{
    int temp = strcmp(str1, str2);
    if (temp < 0) return -1;
    if (temp == 0) return 0;
    else return 1;
}

void read_in_terms(struct term **terms, int *pnterms, char *filename)
{
    // each new block is stored in *terms
    FILE *fp = fopen(filename, "r");
    char line[200];

    // count number of terms
    fgets(line, sizeof(line)+1,fp);
    *pnterms = atof(line);

    *terms = (struct term *)malloc(sizeof(struct term) * (*pnterms));
    struct term *pterms = *terms;
    for (int i = 0; i <= *pnterms; i++)
    {
        fscanf(fp, "%lf", &((*terms)->weight));

        fgets(line, sizeof(line), fp);
        line[strcspn(line, "\n")] = 0;
        strcpy((*terms)->term, line+1);
        (*terms)++;
    }
    fclose(fp);


    // sort in alphabetical order
    *terms = pterms;
    qsort(pterms, *pnterms, sizeof(struct term), comp_func);

}



// Part 2

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
    if (nterms == 0)
    {
        return -1;
    }

    int mid = (int) nterms/2;
    char res_mid[strlen(substr)];
    memcpy(res_mid, terms[mid].term, strlen(substr));
    char res_start[strlen(substr)];
    memcpy(res_start, terms[0].term, strlen(substr));
    char res_end[strlen(substr)];
    memcpy(res_end, terms[nterms-1].term, strlen(substr));

    res_mid[strlen(substr)] = '\0';


    if (strcmp(substr, res_start) < 0 || strcmp(substr, res_end) > 0)
    {
        return -1;
    }


    if (strcmp(substr, res_mid) > 0)
    {
        if ((lowest_match(&terms[mid], ceiling(nterms * 0.5), substr)) != -1)
        {
            return ((nterms / 2) + lowest_match(&terms[mid], ceiling(nterms * 0.5), substr));
        }
        else return -1;
    }
    if (strcmp(substr, res_mid) < 0)
    {
        return (lowest_match(terms, (int) (nterms * 0.5), substr));
    }
    if (strcmp(substr, res_mid) == 0)
    {
        if (lowest_match(terms, (int) (nterms * 0.5), substr) != -1)
        {
            return (lowest_match(terms, (int) (nterms * 0.5), substr));
        }
        else return nterms / 2;
    }
    return -1;
}



int highest_match(struct term *terms, int nterms, char *substr)
{
    if (nterms == 0)
    {
        return -1;
    }

    int mid = (int) nterms/2;
    char res_mid[strlen(substr)];
    memcpy(res_mid, terms[mid].term, strlen(substr));
    char res_start[strlen(substr)];
    memcpy(res_start, terms[0].term, strlen(substr));
    char res_end[strlen(substr)];
    memcpy(res_end, terms[nterms-1].term, strlen(substr));

    res_mid[strlen(substr)] = '\0';


    if (strcmp(substr, res_start) < 0 || strcmp(substr, res_end) > 0)
    {
        return -1;
    }

    if (strcmp(substr, res_mid) > 0)
    {
        if (highest_match(&terms[mid], ceiling(nterms*0.5), substr) != -1) // if there is a higher element than mid
        {
            return ((int)(nterms*0.5) + highest_match(&terms[mid], ceiling(nterms*0.5), substr));
        }
        else return -1;
    }

    if (strcmp(substr, res_mid) < 0)
    {
        return (highest_match(terms, (int) (nterms * 0.5), substr));
    }

    if (strcmp(substr, res_mid) == 0)
    {
        if ( (nterms > 1) && highest_match(&terms[mid] + 1,  (ceiling)(nterms*0.5) - 1, substr) != -1)
        {
            return ((int)(nterms*0.5) + 1) + (highest_match(&terms[mid] + 1, (ceiling)(nterms*0.5) - 1 , substr));
        }

        else return (int)(nterms*0.5);
    }

    return -1;
}




//when substr is the last element in sorted terms, highest_match doesnt work hmmm





// Part 3
int sorter(const void *term1, const void *term2)
{
    struct term *term_a = (struct term *)term1;
    struct term *term_b = (struct term *)term2;

    return (term_b->weight - term_a->weight);
}



void autocomplete(struct term **answer, int *n_answer, struct term *terms, int nterms, char *substr)
{
    int first = lowest_match(terms, nterms, substr);
    int last = highest_match(terms, nterms, substr);

    // seg fault here?
    if ((first == last) && (first == -1))
    {
        *n_answer = 0;
        return;
    }

    *n_answer = (last-first)+1;
    *answer = (struct term *)malloc(sizeof(struct term) * (*n_answer));
    struct term *pterms = *answer;

    for (int i = first; i <= last; i++)
    {
        (*answer)->weight = terms[i].weight;
        strcpy((*answer)->term, terms[i].term);
        (*answer)++;
    }

    qsort(pterms, *n_answer, sizeof(struct term),  sorter); // sort the array, answer by weight
    *answer = pterms;

    /*
    for (int k = 0; k < *n_answer; k++)
    {
        printf("%s\n", (*answer)[k].term);
    }
     */
}







