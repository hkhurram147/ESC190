#include <stdio.h>
#include <stdlib.h>
#include "autocomplete.h"


void read_in_terms(struct term **terms, int *pnterms, char *filename)
{
    FILE *fp = fopen(filename, "r");
    
    //count number of terms
    fgets(*pnterms, 5, fp);
    printf("%d\n", *pnterms);


    for(int i = 0; i < cnt; i++) // reads N lines of a text file line-by-line
    {
        char line[200];
        fgets(line, sizeof(line), fp); // read in at most sizeof(line) characters (including '/0') into line

        struct term *new_term = (struct term *)malloc(sizeof(struct term));
        new_term->term = line;
        (*terms)->new_term = line;
        (*terms)++;
    }


    // sort in alphabetical order

}


int lowest_match(struct term *terms, int nterms, char *substr)
{
    int e =
    if (e > terms[len(L)-1] || e < terms[0])
        return -1;

    if (terms[len(L)/2] < e)
        return len(L)/2 + find_lowest(terms[len(L)/2:], e); 

    if (terms[len(L)/2] > e)
        return lowest_match(L[:len(L)/2], e);

    if (terms[len(L)/2] == e)
    {    
        if (lowest_match(terms[:len(L)/2], e) != -1)
            return lowest_match(terms[:len(L)/2], e);

        else
            return len(L)/2;
    }

    return -1;
}



int highest_match(struct term *terms, int nterms, char *substr)
{
    if (e > terms[len(L)-1] || e < L[0])
        return -1;

    if (terms[len(L)/2] < e)
        return len(L)/2 + find_lowest(L[len(L)/2:], e);

    if (terms[len(L)/2] > e)
        return highest_match(L[:len(L)/2], e);

    if (terms[len(L)/2] == e)
    {
        if (highest_match(terms[len(L)/2 + 1:], e) != -1)
            return highest_match(terms[len(L)/2 + 1:], e);
        
        else 
            return len(L)/2;
    }

    return -1;
}




void autocomplete(struct term **answer, int *n_answer, struct term *terms, int nterms, char *substr)
{

}





int main(void)
{

    char *wiki = "wiktionary.txt";
    char *cities = "cities.txt";


}